{
  description = "wl-idle-lock — simple Wayland idle locker using ext-idle-notify-v1";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in {
        # ========================================
        # Package: wl-idle-lock
        # ========================================
        packages.default = pkgs.stdenv.mkDerivation {
          pname = "wl-idle-lock";
          version = "1.0.0";

          # Raíz del proyecto, meson.build debe estar aquí
          src = ./.;
          unpackPhase = ":";

          nativeBuildInputs = with pkgs; [
            meson
            ninja
            pkg-config
            wayland-scanner
          ];

          buildInputs = with pkgs; [
            wayland
            wayland-protocols
          ];

          # Fases de build explícitas para Git tree sucio
          buildPhase = ''
            meson setup build --buildtype=release --prefix=$out
            ninja -C build
          '';

          meta = with pkgs.lib; {
            description = "Wayland idle locker using ext-idle-notify-v1";
            homepage = "https://github.com/emilio/wl-idle-lock";
            license = licenses.mit;
            platforms = platforms.linux;
          };
        };

        # ========================================
        # DevShell para desarrollo
        # ========================================
        devShells.default = pkgs.mkShell {
          buildInputs = [
            pkgs.meson
            pkgs.ninja
            pkgs.wayland
            pkgs.wayland-protocols
            pkgs.wayland-scanner
            pkgs.pkg-config
            pkgs.clang
            pkgs.gdb
          ];

          shellHook = ''
            echo "wl-idle-lock devShell listo"
            echo "Ejecuta: meson setup build && meson compile -C build"
          '';
        };

        # ========================================
        # App ejecutable directo con nix run
        # ========================================
        apps.default = flake-utils.lib.mkApp {
          drv = self.packages.${system}.default;
          name = "wl-idle-lock";
        };
      });
}
