project('Archipelago', ['c', 'cpp'],
        version : '1.0',
        default_options : ['warning_level=3', 'c_std=c99']
)

RootDirectory = ''

ProjectSources = []

subdir('sources')

C_Compiler      = meson.get_compiler('c')
mingw32_lib     = C_Compiler.find_library('mingw32')
SDL2_lib        = C_Compiler.find_library('SDL2')
SDL2main_lib     = C_Compiler.find_library('SDL2main')
SDL2image_lib    = C_Compiler.find_library('SDL2_image')

executable('Archipelago', ProjectSources,
           include_directories : RootDirectory,
           dependencies : [mingw32_lib, SDL2_lib, SDL2main_lib, SDL2image_lib]
)