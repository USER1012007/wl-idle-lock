project('wl-idle-lock', 'cpp',
  version: '1.0.0',
  default_options: ['cpp_std=cpp17', 'warning_level=2', 'optimization=2']
)

wayland_dep = dependency('wayland-client')
wayland_protocols = dependency('wayland-protocols', version: '>=1.25')
wayland_scanner = find_program('wayland-scanner', required: true)

protocols_dir = join_paths(meson.source_root(), 'src', 'protocols')
xml_proto = join_paths(protocols_dir, 'ext-idle-notify-v1.xml')

# generar archivo .c (private-code) y header (client-header)
ext_idle_c = custom_target('ext-idle-notify-v1-c',
  input: xml_proto,
  output: 'ext-idle-notify-v1-protocol.c',
  command: [ wayland_scanner, 'private-code', '@INPUT@', '@OUTPUT@' ]
)

ext_idle_h = custom_target('ext-idle-notify-v1-h',
  input: xml_proto,
  output: 'ext-idle-notify-v1-client-protocol.h',
  command: [ wayland_scanner, 'client-header', '@INPUT@', '@OUTPUT@' ]
)

executable('wl-idle-lock',
  sources: [
    'src/main.cpp',
    ext_idle_c,
    ext_idle_h
  ],
  include_directories: include_directories('src'),
  dependencies: [ wayland_dep, wayland_protocols ],
  install: true
)
