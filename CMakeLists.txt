cmake_minimum_required(VERSION 3.10)
project(
    Flatland 
    VERSION 0.1 
    DESCRIPTION "A 2D game engine"
    LANGUAGES CXX)

# set debug mode
#SET( CMAKE_BUILD_TYPE Debug ... FORCE )

# defining flatland target and giving sources
add_library(flatland SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/flatland.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/renderable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/signal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/sprite.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/window.cpp
)

add_library(Flatland::flatland ALIAS flatland)

target_compile_options(flatland 
    PRIVATE 
        $<$<CXX_COMPILER_ID:GNU>:
            -fdiagnostics-color
            -pedantic -Wall -Wextra -Wcast-qual -Wcast-align
            -Wpointer-arith -Winit-self -Wshadow -Wswitch-enum 
            -Wredundant-decls -Wfloat-equal -Wundef -Wvla 
            -Wconversion -Wstrict-aliasing
        >
        
        $<$<CXX_COMPILER_ID:MSVC>: /W4>
)

# setting up c++ standard 17
target_compile_features(flatland
    PRIVATE
        cxx_std_17
)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

# find dependencies
find_package(MM 0.1 REQUIRED
                HINTS $<$<CMAKE_SYSTEM:Linux>:/usr/local/lib/)

find_package(WSDL2 0.1 REQUIRED
                HINTS $<$<CMAKE_SYSTEM:Linux>:/usr/local/lib/)

# external libraries ( SDL? )
target_link_libraries(flatland
    INTERFACE
        MM::MM
        WSDL2::WSDL2
)

# include directories
target_include_directories(flatland
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/engine/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/engine
)

target_compile_definitions(flatland
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

#################
# Test section
#################

enable_testing()

# task_test
add_executable(task_test test/task_test.cpp)

target_link_libraries(task_test
    PRIVATE
        Flatland::flatland
)

target_compile_features(task_test
    PRIVATE
        cxx_std_17
)

target_compile_definitions(task_test
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

add_test(
    NAME task_test
    CONFIGURATIONS Debug
    WORKING_DIRECTORY test
    COMMAND task_test
)

# signal_test
add_executable(signal_test test/signal_test.cpp)

target_link_libraries(signal_test
    PRIVATE
        Flatland::flatland
)

target_compile_features(signal_test
    PRIVATE
        cxx_std_17
)

target_compile_definitions(signal_test
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

add_test(
    NAME signal_test
    CONFIGURATIONS Debug
    WORKING_DIRECTORY test
    COMMAND signal_test
)


# window_test
add_executable(window_test test/window_test.cpp)

target_link_libraries(window_test
    PRIVATE
        Flatland::flatland
)

target_compile_features(window_test
    PRIVATE
        cxx_std_17
)

target_compile_definitions(window_test
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

add_test(
    NAME window_test
    CONFIGURATIONS Debug
    WORKING_DIRECTORY test
    COMMAND window_test
)

# sprite_test
add_executable(sprite_test test/sprite_test.cpp)

target_link_libraries(sprite_test
    PRIVATE
        Flatland::flatland
)

target_compile_features(sprite_test
    PRIVATE
        cxx_std_17
)

target_compile_definitions(sprite_test
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

add_test(
    NAME sprite_test
    CONFIGURATIONS Debug
    WORKING_DIRECTORY test
    COMMAND sprite_test
)

# entities_test 
add_executable(entities_test test/entities_test.cpp)

target_link_libraries(entities_test
    PRIVATE
        Flatland::flatland
)

target_compile_features(entities_test
    PRIVATE
        cxx_std_17
)

target_compile_definitions(entities_test
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

add_test(
    NAME entities_test
    CONFIGURATIONS Debug
    WORKING_DIRECTORY test
    COMMAND entities_test 
)

# flatland_test 
add_executable(flatland_test test/flatland_test.cpp)

target_link_libraries(flatland_test
    PRIVATE
        Flatland::flatland
)

target_compile_features(flatland_test
    PRIVATE
        cxx_std_17
)

target_compile_definitions(flatland_test
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

add_test(
    NAME flatland_test
    CONFIGURATIONS Debug
    WORKING_DIRECTORY test
    COMMAND flatland_test 
)

# manage git submodule mm
#find_path(MM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/libmm/include)

#if ((NOT MM_INCLUDE_DIR) OR (NOT EXISTS ${MM_INCLUDE_DIR}))
#
#    message("Unable to find library mm")
#
#    # clone submodule mm
#    execute_process(COMMAND git submodule update --init -- lib/libmm
#                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
#
#    set(MM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/libmm/include
#        CACHE PATH "MM include directory")
#endif()
#
## manage git submodule wsdl2
#find_path(WSDL2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/libwsdl2/include)
#
#if ((NOT WSDL2_INCLUDE_DIR) OR (NOT EXISTS ${WSDL2_INCLUDE_DIR}))
#
#    message("Unable to find library sdl2")
#
#    # clone submodule wsdl2
#    execute_process(COMMAND git submodule update --init -- lib/libwsdl2
#                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
#
#    set(WSDL2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/libwsdl2/include
#        CACHE PATH "WSDL2 include directory")
#endif()

###################
# Install section
###################

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Flatland)

install(TARGETS flatland
    EXPORT flatland-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set_target_properties(flatland PROPERTIES EXPORT_NAME Flatland)

# install public headers
install(
    FILES  
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/include/flatland.hpp  
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/include/layer.hpp  
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/include/renderbase.hpp  
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/include/serial.hpp  
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/include/bound.hpp  
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/include/sprite.hpp  
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/include/types.hpp  
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/include/window.hpp
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}/flatland
)

install(
    DIRECTORY 
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/include/core
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}/flatland
)

# export targets
install(EXPORT flatland-targets
    FILE
        FlatlandTargets.cmake
    NAMESPACE
        Flatland::
    DESTINATION
        ${INSTALL_CONFIGDIR}
)

# create ConfigVersion.cmake
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/FlatlandConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/cmake/FlatlandConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/FlatlandConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

# install config, configversion
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/FlatlandConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/FlatlandConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

############################
# export from the build tree
export(EXPORT flatland-targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/FlatlandTargets.cmake
    NAMESPACE Flatland::
)

export(PACKAGE Flatland)
