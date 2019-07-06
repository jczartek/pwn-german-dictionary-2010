
gnome = import('gnome')

resources = gnome.compile_resources(
  'pwn-german-dictionary-2010-resources',
  'pwn-german-dictionary-2010.gresource.xml',
  c_name: 'pwn',
)

pwn_german_dictionary_2010_sources = [
  'pwn-german-dictionary-2010.c',
  'pwn-german-dictionary-2010-plugin.c',
  'pwn-german-dictionary-2010-prefs.c',
  'pwn-german-dictionary-2010-window.c',
  resources[0],
  resources[1],
]

pwn_german_dictionary_2010_headers = [
]

cc = meson.get_compiler('c')

pwn_german_dictionary_2010_deps = [
  dependency('gio-2.0', version: '>= 2.50'),
  dependency('gtk+-3.0', version: '>= 3.24'),
  dependency('libgydict-1.0'),
  dependency('libpeas-1.0', version: '>= 1.22.0'),
  dependency('libdazzle-1.0', version: '>= 3.26.0'),
  cc.find_library('z', required: true),
]

pwn_german_dictionary_2010_lib = shared_library('pwn-german-dictionary-2010',
  pwn_german_dictionary_2010_sources,
  dependencies: pwn_german_dictionary_2010_deps,
  install: true,
  install_dir: get_option('prefix'),
)

install_data(
  'pwn-german-dictionary-2010.plugin',
  install_dir: get_option('prefix')
)


