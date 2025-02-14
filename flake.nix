# SPDX-FileCopyrightText: Olivier Le Doeuff <olivier.ldff@gmail.com>
# SPDX-License-Identifier: MIT
{
  description = "QOlm";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-24.11";
    flake-utils.url = "github:numtide/flake-utils";
    nix-filter.url = "github:numtide/nix-filter";
  };

  outputs =
    { self
    , nixpkgs
    , flake-utils
    , nix-filter
    }:
    flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs {
        inherit system;
      };

      qt = pkgs.qt6;

      nativeBuildInputs = with pkgs; [
        qt.wrapQtAppsHook
        makeWrapper
        gcc
        git
        cmake
        cpm-cmake
        ninja
        pkg-config

        gtest
      ];


      buildInputsQt = with pkgs.qt6; [
        qtbase
        qtdeclarative
      ];

      buildInputs = buildInputsQt;

      shellHook = ''
        # Crazy shell hook to set up Qt environment, from:
        # https://discourse.nixos.org/t/python-qt-woes/11808/12
        setQtEnvironment=$(mktemp --suffix .setQtEnvironment.sh)
        echo "shellHook: setQtEnvironment = $setQtEnvironment"
        makeWrapper "/bin/sh" "$setQtEnvironment" "''${qtWrapperArgs[@]}"
        sed "/^exec/d" -i "$setQtEnvironment"
        source "$setQtEnvironment"
      '';

      devShellHook = pkgs.lib.concatStringsSep "\n" (
        [ shellHook ]
      );

      version = import ./nix/get-project-version.nix { file = ./CMakeLists.txt; prefix = "QOLM"; };
      CPM_USE_LOCAL_PACKAGES = "ON";

      packages = {
        qolm = with pkgs; stdenv.mkDerivation rec {
          inherit version nativeBuildInputs buildInputs;
          inherit CPM_USE_LOCAL_PACKAGES;

          pname = "qolm";
          src = nix-filter {
            root = ./.;
            include = [
              "cmake"
              "examples"
              "include"
              "src"
              "tests"
              ./CMakeLists.txt
            ];
          };

          cmakeFlags = [
            (pkgs.lib.strings.cmakeBool "BUILD_SHARED_LIBS" true)
            (pkgs.lib.strings.cmakeBool "QOLM_ENABLE_TESTS" doCheck)
            (pkgs.lib.strings.cmakeBool "QOLM_ENABLE_EXAMPLES" doCheck)
            (pkgs.lib.strings.cmakeBool "QOLM_USE_LOCAL_CPM_FILE" true)
          ];

          cmakeConfigType = "Release";
          enableParallelBuilding = true;
          # Enable debug output folder to exists and be kept
          separateDebugInfo = true;

          out = [ "out" ];

          buildPhase = ''
            echo "Building qolm version ${version} in ${cmakeConfigType} mode"

            cmake --build . --config ${cmakeConfigType} --target \
              QOlm \
              --parallel $NIX_BUILD_CORES
          '';

          doCheck = pkgs.stdenv.hostPlatform == pkgs.stdenv.buildPlatform;
          checkPhase = pkgs.lib.optionalString doCheck ''
            cmake --build . --config ${cmakeConfigType} --target \
              QOlm_Tests \
              QOlm_TestsQml \
              QOlm_Example \
              QOlm_ExampleQml \
              --parallel $NIX_BUILD_CORES

            echo "Run shell hook"
            ${shellHook}

            export QT_QPA_PLATFORM=offscreen
            echo "Run tests"
            ctest -C "${cmakeConfigType}" --output-on-failure --verbose
          '';

          installPhase = ''
            cmake --install . --config ${cmakeConfigType} --prefix $out
          '';
        };

        default = packages.qolm;

        deadnix = pkgs.runCommand "deadnix" { } ''
          ${pkgs.deadnix}/bin/deadnix --fail ${./.}
          mkdir $out
        '';
      };

      minimalDevBuildInputs = with pkgs; [
        gh
      ];
      fullDevBuildInputs = with pkgs; nativeBuildInputs
        ++ minimalDevBuildInputs
        ++ [
        sccache
        nixpkgs-fmt
        cmake-format
        clang-tools
        nurl
        lazygit
        neovim
      ]
        ++ pkgs.lib.lists.optionals (stdenv.isLinux) [
        gdb
      ];

    in
    {
      inherit packages;

      devShells = {
        minimal = pkgs.mkShell {
          name = "qolm-minimal-shell";

          inherit buildInputs shellHook;
          inherit CPM_USE_LOCAL_PACKAGES;

          nativeBuildInputs = nativeBuildInputs
            ++ minimalDevBuildInputs;
        };

        default = pkgs.mkShell {
          name = "qolm-dev-shell";

          inherit buildInputs;
          inherit CPM_USE_LOCAL_PACKAGES;

          shellHook = devShellHook;
          nativeBuildInputs = fullDevBuildInputs;
        };

        qolm = packages.qolm;
      };

      formatter = pkgs.nixpkgs-fmt;
      checks = {
        inherit (self.packages.${system}) deadnix;
      };
    });
}
