project(
  'gach',
  'cpp',
  version: '1.0.0',
  default_options: [
    'cpp_std=c++17',
    'warning_level=2',
    'optimization=3',
    'b_lto=true',
    'b_ndebug=if-release'
  ],
  meson_version: '>=1.1'
)

gtk_version = get_option('gtk_version')

if gtk_version == 'auto'
  gtk_dep = dependency('gtk+-2.0', required: false)
  if not gtk_dep.found()
    gtk_dep = dependency('gtk+-3.0', required: true)
  endif
else
  gtk_dep = dependency(gtk_version, required: true)
endif

cairo_dep = dependency('cairo', required: true)
sqlite_dep = dependency('sqlite3', required: true)

sources = files(
  'src/domain/phonics_syllable.cpp',
  'src/domain/flow_fsm.cpp',
  'src/application/story_interactor.cpp',
  'src/adapters/db/sqlite_story_repository.cpp',
  'src/adapters/ui/cairo_renderer.cpp',
  'src/adapters/ui/ui_layout.cpp',
  'src/adapters/ui/gtk_touch_adapter.cpp',
  'src/infrastructure/kindle_power.cpp',
  'src/infrastructure/app_config.cpp',
  'src/main.cpp'
)

executable(
  'gach',
  sources,
  include_directories: include_directories('src'),
  dependencies: [gtk_dep, cairo_dep, sqlite_dep],
  cpp_args: ['-static-libstdc++', '-static-libgcc'],
  link_args: ['-static-libstdc++', '-static-libgcc'],
  install: true
)
