{
  description = "wl-idle-lock — Wayland idle locker (C++ version)";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in {
        packages.default = pkgs.stdenv.mkDerivation {
          pname = "wl-idle-lock";
          version = "1.0.0";

          src = ./.;
          unpackPhase = ":";

          nativeBuildInputs = with pkgs; [
            meson
            ninja
            pkg-config
            wayland-scanner
            gcc
          ];

          buildInputs = with pkgs; [
            wayland
            wayland-protocols
          ];

          buildPhase = ''
            echo "==> Meson setup"
            meson setup build --buildtype=release --prefix=$out
            echo "==> ninja build"
            ninja -C build
          '';

          installPhase = ''
            echo "==> instalando binario en $out/bin"
            mkdir -p $out/bin
            cp build/wl-idle-lock $out/bin/
          '';

          meta = with pkgs.lib; {
            description = "Wayland idle locker using ext-idle-notify-v1 (C++ rewrite)";
            license = licenses.mit;
            platforms = platforms.linux;
          };
        };

        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            meson ninja wayland wayland-protocols wayland-scanner pkg-config gcc gdb
          ];
          shellHook = ''
            echo "wl-idle-lock devShell ready"
            echo "Usa: meson setup build && meson compile -C build"
          '';
        };

        apps.default = {
          type = "app";
          program = "${self.packages.${system}.default}/bin/wl-idle-lock";
        };
      });
}
