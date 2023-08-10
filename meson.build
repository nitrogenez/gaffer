project(
    'gaffer',
    'cpp',
    version: '0.2.0-alpha',
    meson_version: '>= 1.0',
    license: 'BSD-3-Clause',

    default_options: [
        'werror=false',
        'warning_level=3',
        'prefer_static=true',
        'default_library=static',
        'buildtype=debug',
        'optimization=2',
    ],
)

gaffer = {}

gaffer += {'version': meson.project_version()}
gaffer += {'name': meson.project_name()}
gaffer += {'license': meson.project_license()[0]}
gaffer += {'description': 'A project manager for modern C++'}
gaffer += {
    'build_id': '@0@-@1@-@2@'.format(
        get_option('buildtype'),
        target_machine.system(),
        target_machine.cpu_family(),
    ),
}
gaffer += {'incdirs': include_directories('include')}
gaffer += {
    'sources': files(
        'src/core/logger.cc',
        'src/core/person.cc',
        'src/core/project.cc',
        'src/core/version.cc',
        'src/main.cc',
        'src/util/fmt.cc',
        'src/util/string.cc',
        'src/util/time.cc',
    ),
}
gaffer += {'prefix': get_option('prefix')}
gaffer += {'datadir': get_option('datadir')}

gaffer += {
    'deps': [
        subproject('argparse').get_variable('argparse_dep'),
    ],
}

out_suffix = ''
cpp = meson.get_compiler('cpp')
cpp_std = 'c++2b'

if cpp.version()[0] == '1' and cpp.version()[1] == '8'
    message('Using C++23 standard')
    cpp_std = 'c++23'
endif

if get_option('profile') != 'default'
    message('Building Gaffer with the development profile')
    out_suffix = '-@0@'.format(get_option('profile'))
endif

add_project_arguments(['-std=@0@'.format(cpp_std)], language: 'cpp')

add_project_arguments(
    [
        '-DGAFFER_VERSION="@0@"'.format(gaffer['version']),
        '-DGAFFER_PLATFORM="@0@"'.format(target_machine.system()),
        '-DGAFFER_ARCH="@0@"'.format(target_machine.cpu_family()),
        '-DGAFFER_BUILD_ID="@0@"'.format(gaffer['build_id']),
        '-DGAFFER_DESCRIPTION="@0@"'.format(gaffer['description']),
        '-DGAFFER_LICENSE="@0@"'.format(gaffer['license']),
        '-DGAFFER_PREFIX="@0@"'.format(gaffer['prefix']),
        '-DGAFFER_DATA_DIR="@0@"'.format(gaffer['datadir']),
    ],
    language: 'cpp',
)

gaffer = executable(
    'gaffer@0@'.format(out_suffix),
    sources: gaffer['sources'],
    include_directories: gaffer['incdirs'],
    dependencies: gaffer['deps'],
)

subdir('tests')
