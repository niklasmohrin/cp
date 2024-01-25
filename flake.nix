{
  description = "NACTL";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.11";
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
        lib = pkgs.lib;
        pdfPageCount = file: lib.fileContents (pkgs.runCommand "pdfpages" { } "${pkgs.pdftk}/bin/pdftk ${file} dump_data | grep NumberOfPages | sed 's/[^0-9]*//' > $out");
        kactlDerivation = initialPage: pkgs.stdenvNoCC.mkDerivation {
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
          buildPhase = ''
            echo '\setcounter{page}{ ${toString initialPage} }' > initial-page.tex
            make kactl
          '';
          installPhase = "cp kactl.pdf $out";
        };

      in
      rec {
        packages = rec {
          default = nactl;

          nactl = pkgs.stdenvNoCC.mkDerivation {
            name = "nactl";
            src = ./.;
            nativeBuildInputs = [ pkgs.typst ];
            phases = [ "unpackPhase" "buildPhase" ];
            buildPhase = "typst compile --format pdf nactl.typ $out";
          };

          kactl = kactlDerivation 1;
          nactl-complete = pkgs.runCommand "nactl-complete" { } "${pkgs.poppler_utils}/bin/pdfunite ${nactl} ${kactlDerivation (lib.strings.toInt (pdfPageCount nactl) + 1)} $out";
        };

        devShells =
          let
            typst-packages = with pkgs; [ typst-lsp typst-fmt ];
            contest-packages = with pkgs; [
              clang-tools_16
              clang_16
              fmt
              gdb
              gnumake

              python310
              pypy310
            ];

          in
          rec {
            nactl = pkgs.mkShell { inputsFrom = [ packages.nactl ]; packages = typst-packages; };
            cpp = pkgs.mkShell { packages = contest-packages; };
            combined = pkgs.mkShell { packages = typst-packages ++ contest-packages; };
            default = combined;
          };
      });
}
