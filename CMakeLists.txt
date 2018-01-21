cmake_minimum_required(VERSION 3.9)
project(pong)

set(CMAKE_CXX_STANDARD 17)

add_custom_target(CopyMedia
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/media ${CMAKE_BINARY_DIR}/media
)

include_directories(libs/spdlog-0.16.3/include)
include_directories(libs/glad/include)
include_directories(libs/stb/include)

find_package(Boost 1.66.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

if(MINGW)
    include_directories(libs/mingw-std-threads)
    include_directories(libs/sdl2/include)

    set(SDL2_LIBRARY "${CMAKE_SOURCE_DIR}/libs/sdl2/win/SDL2.dll" "${CMAKE_SOURCE_DIR}/libs/sdl2/win/SDL2main.lib")

    add_custom_target(CopyBinaries
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/libs/sdl2/win/SDL2.dll ${CMAKE_BINARY_DIR}
    )

    set(CUSTOM_SOURCE_FILES "utils/win_timer.cpp" gfx/common.h libs/stb/include/stb_image.h)
endif()

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

add_executable(pong main.cpp utils/log.h utils/common.h utils/file.h window.h window.cpp glad.c gfx/shader.h gfx/shader.cpp game.h utils/timer.h ${CUSTOM_SOURCE_FILES} game.cpp)
target_link_libraries(pong ${SDL2_LIBRARY})

add_dependencies(pong CopyBinaries CopyMedia)