project('pwn-german-dictionary-2010', 'c',
          version: '0.1.0',
    meson_version: '>= 0.50.0',
  default_options: [ 'warning_level=2',
                     'c_std=gnu11',
                   ],
)


add_project_arguments([
  '-I' + meson.build_root(),
  '-Wno-unused-parameter',
], language: 'c')

subdir('src')

