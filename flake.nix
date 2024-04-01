{
  description = "NACTL";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.11";
    flake-utils.url = "github:numtide/flake-utils";
    kactl = {
      url = "github:kth-competitive-programming/kactl";
      flake = false;
    };
    typix = {
      url = "github:loqusion/typix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs = inputs@{ self, nixpkgs, flake-utils, typix, ... }:
    flake-utils.lib.eachDefaultSystem
      (system:
        let
          pkgs = import nixpkgs { inherit system; };
          inherit (pkgs) lib;

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

            nactl =
              let
                typixLib = typix.lib.${system};
              in
              typixLib.buildTypstProject {
                src = lib.fileset.toSource {
                  root = ./.;
                  fileset = lib.fileset.unions [
                    (lib.fileset.fromSource (typixLib.cleanTypstSource ./.))
                    ./assets
                    ./snippets
                    ./template.cpp
                    ./toolbin
                  ];
                };
                typstSource = "nactl.typ";
                fontPaths = [ "${pkgs.liberation_ttf}/share/fonts/truetype" ];
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
              contest = pkgs.mkShell { packages = contest-packages; };
              combined = pkgs.mkShell { packages = typst-packages ++ contest-packages; };
              default = combined;
            };
        });
}
