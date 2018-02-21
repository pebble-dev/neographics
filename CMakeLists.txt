cmake_minimum_required (VERSION 2.8.2)
project (neographics)

###################################################################
# Dependencies
###################################################################

include(ExternalProject)
ExternalProject_Add(
	stb
	GIT_REPOSITORY https://github.com/nothings/stb.git
	GIT_TAG master
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/stb
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)
ExternalProject_Get_Property(stb source_dir)
set(STB_SOURCE_DIR ${source_dir})

###################################################################
# Utilities
###################################################################

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

###################################################################
# neographics
###################################################################

set(sources_neographics
  src/draw_command/draw_command.c
  src/draw_command/draw_command.h
  src/fonts/fonts.c
  src/fonts/fonts.h
  src/path/path.c
  src/path/path.h
  src/primitives/circle.c
  src/primitives/circle.h
  src/primitives/line.c
  src/primitives/line.h
  src/primitives/rect.c
  src/primitives/rect.h
  src/text/text.c
  src/text/text.h
  src/types/color.h
  src/types/colors.h
  src/types/cornermask.h
  src/types/point.h
  src/types/rect.c
  src/types/rect.h
  src/types/size.h
  src/common.c
  src/common.h
  src/context.c
  src/context.h
  src/graphics.h
  src/macros.h
  src/types.h

  contrib/pebble_env/pebble.c
  contrib/pebble_env/pebble.h
)
assign_source_group(${sources_neographics})

add_definitions(-DNGFX_IS_CORE)
include_directories("contrib/pebble_env")
if (NOT WIN32)
    link_libraries(m) # everyone needs a bit of math (except windows)
endif()

add_library(neographics_rect ${sources_neographics} )
target_compile_definitions(neographics_rect PUBLIC PBL_RECT=1)

add_library(neographics_rect_bw ${sources_neographics} )
target_compile_definitions(neographics_rect_bw PUBLIC PBL_RECT=1 PBL_BW=1)

add_library(neographics_round ${sources_neographics} )
target_compile_definitions(neographics_round PUBLIC)

###################################################################
# tests
###################################################################

set(sources_test_neographics
    contrib/test/testrunner.c
    contrib/test/stb_impl.c
)
assign_source_group(${sources_test_neographics})

add_executable(test_neographics_rect ${sources_test_neographics} )
target_link_libraries(test_neographics_rect neographics_rect)
target_compile_definitions(neographics_rect PUBLIC PBL_TYPE=rect PBL_RECT=1)
target_include_directories(neographics_rect PUBLIC "src" ${STB_SOURCE_DIR})
add_dependencies(neographics_rect stb)

add_executable(test_neographics_rect_bw ${sources_test_neographics} )
target_link_libraries(test_neographics_rect_bw neographics_rect_bw)
target_compile_definitions(neographics_rect_bw PUBLIC PBL_TYPE=rect_bw PBL_RECT=1 PBL_BW=1)
target_include_directories(neographics_rect_bw PUBLIC "src" ${STB_SOURCE_DIR})
add_dependencies(neographics_rect_bw stb)

add_executable(test_neographics_round ${sources_test_neographics} )
target_link_libraries(test_neographics_round neographics_round)
target_compile_definitions(neographics_round PUBLIC PBL_TYPE=round)
target_include_directories(neographics_round PUBLIC "src" ${STB_SOURCE_DIR})
add_dependencies(neographics_round stb)
