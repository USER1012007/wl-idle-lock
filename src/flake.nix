{
  description = "C flake to run a wayland based daemon";

  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";

  outputs =
    { self, nixpkgs }:
    let
      systems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];
      forEachSystem =
        f:
        nixpkgs.lib.genAttrs systems (
          system:
          f {
            pkgs = import nixpkgs { inherit system; };
          }
        );
    in
    {
      packages = forEachSystem (
        { pkgs }:
        {
          default = pkgs.stdenv.mkDerivation {
            pname = "main";
            version = "1.0";

            src = ./.;
            buildInputs = [
              pkgs.wayland
              pkgs.wayland-protocols
              pkgs.meson
            ];
            buildPhase = ''
              meson setup build
              meson compile -C build
            '';
            installPhase = ''
              mkdir -p $out/bin
              cp main $out/bin/
            '';
          };
        }
      );

      apps = forEachSystem (
        { pkgs }:
        {
          default = {
            type = "app";
            program = "${self.packages.${pkgs.system}.default}/bin/main";
          };
        }
      );
    };
}
