project('Archipelago', ['c', 'cpp'],
        version : '1.0',
        default_options : ['warning_level=3', 'c_std=c99', 'cpp_std=c++2a']
)

RootDirectory = ''

# main executable sources:
ProjectSources = []

# unit test executable sources:
CameraTestSources = []
CollisionTestSources = []
DataStructuresTestSources = []
ECSTestSources = []
PathfindingTestSources = []
WorldTestSources = []

subdir('sources')

# project dependencies:
C_Compiler      = meson.get_compiler('c')
mingw32_lib     = C_Compiler.find_library('mingw32')
SDL2_lib        = C_Compiler.find_library('SDL2')
SDL2main_lib     = C_Compiler.find_library('SDL2main')
SDL2image_lib    = C_Compiler.find_library('SDL2_image')

# main executable:
executable('Archipelago', ProjectSources,
           include_directories : RootDirectory,
           dependencies : [mingw32_lib, SDL2_lib, SDL2main_lib, SDL2image_lib]
)

# unit test executables:
test(
        'CameraTest',
        executable('CameraTest', CameraTestSources,
           include_directories : RootDirectory,
           dependencies : [mingw32_lib, SDL2_lib, SDL2main_lib, SDL2image_lib]
        )
)

test(
        'CollisionTest',
        executable('CollisionTest', CollisionTestSources,
           include_directories : RootDirectory,
           dependencies : [mingw32_lib, SDL2_lib, SDL2main_lib, SDL2image_lib]
        )
)

test(
        'DataStructuresTest',
        executable('DataStructuresTest', DataStructuresTestSources,
           include_directories : RootDirectory,
           dependencies : [mingw32_lib, SDL2_lib, SDL2main_lib, SDL2image_lib]
        )
)

test(
        'ECSTest',
        executable('ECSTest', ECSTestSources,
           include_directories : RootDirectory,
           dependencies : [mingw32_lib, SDL2_lib, SDL2main_lib, SDL2image_lib]
        )
)

test(
        'PathfindingTest',
        executable('PathfindingTest', PathfindingTestSources,
           include_directories : RootDirectory,
           dependencies : [mingw32_lib, SDL2_lib, SDL2main_lib, SDL2image_lib]
        )
)

test(
        'WorldTest',
        executable('WorldTest', WorldTestSources,
           include_directories : RootDirectory,
           dependencies : [mingw32_lib, SDL2_lib, SDL2main_lib, SDL2image_lib]
        )
)