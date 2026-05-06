cmake_minimum_required(VERSION 3.21)
project(sleep-blocker LANGUAGES CXX)

# ---------------------------
# Global config
# ---------------------------
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)
include(FetchContent)

# ---------------------------
# Backend option
# ---------------------------
option(USE_SDL "Use SDL backend instead of GLFW" OFF)

# ---------------------------
# Dependencies
# ---------------------------
if(USE_SDL)
  set(SDL_SHARED OFF CACHE BOOL "" FORCE)
  set(SDL_STATIC ON CACHE BOOL "" FORCE)
  set(SDL_X11_XSCRNSAVER OFF CACHE BOOL "" FORCE)
  set(SDL_X11_XTEST OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(
    sdl3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.8
  )
else()
  set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 7b6aead # 3.4 release
  )
endif()

# ImGui
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.92.7
)

# stb
FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG 5736b15
)

# GoogleTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)

if(USE_SDL)
  FetchContent_MakeAvailable(sdl3 imgui stb googletest)
else()
  FetchContent_MakeAvailable(glfw imgui stb googletest)
endif()

# OpenGL
find_package(OpenGL REQUIRED)

# Threads (used on Linux)
find_package(Threads REQUIRED)

# stb (header-only)
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})

# ---------------------------
# Window backend
# ---------------------------
add_library(window_backend INTERFACE)

if(USE_SDL)
  target_link_libraries(window_backend INTERFACE SDL3::SDL3-static)
  target_compile_definitions(window_backend INTERFACE USE_SDL_BACKEND)
else()
  target_link_libraries(window_backend INTERFACE glfw)
  target_compile_definitions(window_backend INTERFACE USE_GLFW_BACKEND)
endif()

# ---------------------------
# ImGui target
# ---------------------------
add_library(imgui_lib STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
  $<IF:$<BOOL:${USE_SDL}>,
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp,
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp>
)
target_include_directories(imgui_lib PUBLIC
  ${imgui_SOURCE_DIR}
  ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui_lib PUBLIC
  window_backend
  OpenGL::GL
)

set(WINDOW_SRC_DIR ${CMAKE_SOURCE_DIR}/src/window)

# ---------------------------
# Main executable
# ---------------------------
add_executable(sleep-blocker
  src/main.cpp
  $<IF:$<BOOL:${USE_SDL}>,
  ${WINDOW_SRC_DIR}/app_sdl_window.cpp,
  ${WINDOW_SRC_DIR}/app_glfw_window.cpp>
)

target_include_directories(sleep-blocker PRIVATE
  ${CMAKE_SOURCE_DIR}/include/core
  ${CMAKE_SOURCE_DIR}/include/window
)
target_link_libraries(sleep-blocker PRIVATE
  imgui_lib
  stb
  $<$<PLATFORM_ID:Linux>:${CMAKE_DL_LIBS} Threads::Threads>
)

# ---------------------------
# Tests
# ---------------------------
# enable_testing()
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# add_executable(unit_tests
# tests/test.cpp
# )
# target_include_directories(unit_tests PRIVATE
# ${CMAKE_SOURCE_DIR}/include
# )
# target_link_libraries(unit_tests PRIVATE
# imgui_lib
# GTest::gtest_main
# )
# include(GoogleTest)
# gtest_discover_tests(unit_tests)
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  add_custom_target(copy_compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/build/compile_commands.json
    COMMENT "Copying compile_commands.json to project root"
  )
endif()