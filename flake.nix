{
  description = "NACTL";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.05";
    flake-utils.url = "github:numtide/flake-utils";
    kactl = {
      url = "github:kth-competitive-programming/kactl";
      flake = false;
    };
  };

  outputs = inputs@{ self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      rec {
        packages = rec {
          default = nactl;

          nactl = pkgs.stdenvNoCC.mkDerivation {
            name = "nactl";
            src = ./.;
            nativeBuildInputs = [ pkgs.typst ];
            phases = [ "unpackPhase" "buildPhase" ];
            buildPhase = "typst compile nactl.typ $out";
          };

          kactl = pkgs.stdenvNoCC.mkDerivation {
            name = "kactl";
            src = inputs.kactl;
            phases = [ "unpackPhase" "patchPhase" "buildPhase" "installPhase" ];
            nativeBuildInputs = with pkgs; [
              python310
              (texlive.combine {
                inherit (texlive) scheme-medium enumitem framed tocloft titlesec paralist;
              })
            ];
            patches = [ ./kactl.patch ];
            buildPhase = "make kactl";
            installPhase = "cp kactl.pdf $out";
          };

          nactl-complete = pkgs.runCommand "nactl-complete" { } "${pkgs.poppler_utils}/bin/pdfunite ${nactl} ${kactl} $out";
        };

        devShells.default = pkgs.mkShell {
          inputsFrom = with packages; [ nactl ];
          packages = with pkgs; [
            typst-lsp
            typst-fmt

            clang-tools_16
            clang_16
            fmt
            gdb
            gnumake
          ];
        };
      });
}
