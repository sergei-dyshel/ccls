void foo() {
  int x;
  x = 3;
}
/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 17,
      "detailed_name": "",
      "qual_name_offset": 0,
      "short_name": "",
      "kind": 0,
      "declarations": [],
      "bases": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0],
      "uses": []
    }],
  "funcs": [{
      "id": 0,
      "usr": 4259594751088586730,
      "detailed_name": "void foo()",
      "qual_name_offset": 5,
      "short_name": "foo",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "spell": "1:6-1:9|-1|1|2",
      "extent": "1:1-4:2|-1|1|0",
      "bases": [],
      "derived": [],
      "vars": [0],
      "uses": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 14014650769929566957,
      "detailed_name": "int x",
      "qual_name_offset": 4,
      "short_name": "x",
      "declarations": [],
      "spell": "2:7-2:8|0|3|2",
      "extent": "2:3-2:8|0|3|0",
      "type": 0,
      "uses": ["3:3-3:4|0|3|4"],
      "kind": 13,
      "storage": 1
    }]
}
*/