project('wl-idle-lock', 'c',
  version: '0.1.0',
  default_options: ['warning_level=3', 'c_std=c11'])

wayland = dependency('wayland-client')
wayland_protos = dependency('wayland-protocols', version: '>=1.31')
wayland_scanner = find_program('wayland-scanner')

protocols_dir = join_paths(meson.current_source_dir(), 'src/protocols')

# Generar archivos del protocolo Wayland
ext_idle_client_c = custom_target('ext-idle-protocol-c',
  input: join_paths(protocols_dir, 'ext-idle-notify-v1.xml'),
  output: 'ext-idle-notify-v1-protocol.c',
  command: [wayland_scanner, 'private-code', '@INPUT@', '@OUTPUT@'])

ext_idle_client_h = custom_target('ext-idle-protocol-h',
  input: join_paths(protocols_dir, 'ext-idle-notify-v1.xml'),
  output: 'ext-idle-notify-v1-client-protocol.h',
  command: [wayland_scanner, 'client-header', '@INPUT@', '@OUTPUT@'])

# Ejecutable principal
executable('wl-idle-lock',
  sources: [
    'src/main.c',
    ext_idle_client_c,
    ext_idle_client_h
  ],
  include_directories: include_directories('src'),
  dependencies: [wayland])
