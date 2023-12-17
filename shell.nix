with import <nixpkgs> {};
llvmPackages_17.stdenv.mkDerivation {
  name = "ccls";

  nativeBuildInputs = [ cmake llvmPackages_17.llvm.dev llvmPackages_17.llvm.lib];
  buildInputs = with llvmPackages_17; [ libclang llvm rapidjson ];

  clang = llvmPackages_17.clang;
  shell = runtimeShell;
}
