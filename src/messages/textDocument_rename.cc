/* Copyright 2017-2018 ccls Authors

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "message_handler.h"
#include "pipeline.hh"
#include "query_utils.h"
#include "log.hh"
using namespace ccls;

namespace {
MethodType kMethodType = "textDocument/rename";

lsWorkspaceEdit BuildWorkspaceEdit(DB *db, WorkingFiles *working_files,
                                   SymbolRef sym, const std::string &new_text) {
  std::unordered_map<int, lsTextDocumentEdit> path_to_edit;

  EachOccurrence(db, sym, true, [&](Use use) {
    std::optional<lsLocation> ls_location =
        GetLsLocation(db, working_files, use);
    if (!ls_location)
      return;

    int file_id = use.file_id;
    if (path_to_edit.find(file_id) == path_to_edit.end()) {
      path_to_edit[file_id] = lsTextDocumentEdit();

      QueryFile &file = db->files[file_id];
      if (!file.def)
        return;

      const std::string &path = file.def->path;
      LOG_S(INFO) << "Will rename in " << path;
      path_to_edit[file_id].textDocument.uri = lsDocumentUri::FromPath(path);

      WorkingFile *working_file = working_files->GetFileByFilename(path);
      if (working_file)
        path_to_edit[file_id].textDocument.version = working_file->version;
    }

    lsTextEdit edit;
    edit.range = ls_location->range;
    edit.newText = new_text;

    // vscode complains if we submit overlapping text edits.
    auto &edits = path_to_edit[file_id].edits;
    if (std::find(edits.begin(), edits.end(), edit) == edits.end())
      edits.push_back(edit);
  });

  lsWorkspaceEdit edit;
  for (const auto &changes : path_to_edit)
    edit.documentChanges.push_back(changes.second);
  return edit;
}

struct In_TextDocumentRename : public RequestMessage {
  MethodType GetMethodType() const override { return kMethodType; }
  struct Params {
    // The document to format.
    lsTextDocumentIdentifier textDocument;

    // The position at which this request was sent.
    lsPosition position;

    // The new name of the symbol. If the given name is not valid the
    // request must return a [ResponseError](#ResponseError) with an
    // appropriate message set.
    std::string newName;
  };
  Params params;
};
MAKE_REFLECT_STRUCT(In_TextDocumentRename::Params, textDocument, position,
                    newName);
MAKE_REFLECT_STRUCT(In_TextDocumentRename, id, params);
REGISTER_IN_MESSAGE(In_TextDocumentRename);

struct Handler_TextDocumentRename : BaseMessageHandler<In_TextDocumentRename> {
  MethodType GetMethodType() const override { return kMethodType; }
  void Run(In_TextDocumentRename *request) override {
    int file_id;
    QueryFile *file;
    if (!FindFileOrFail(db, project, request->id,
                        request->params.textDocument.uri.GetPath(), &file,
                        &file_id))
      return;

    WorkingFile *wfile = working_files->GetFileByFilename(file->def->path);
    lsWorkspaceEdit result;
    for (SymbolRef sym :
         FindSymbolsAtLocation(wfile, file, request->params.position)) {
      result =
          BuildWorkspaceEdit(db, working_files, sym, request->params.newName);
      break;
    }

    pipeline::Reply(request->id, result);
  }
};
REGISTER_MESSAGE_HANDLER(Handler_TextDocumentRename);
} // namespace
