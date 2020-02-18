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
    ${CMAKE_CURRENT_SOURCE_DIR}/flatland.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bounded.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sprite.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AABB.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/event.cpp
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

# target_link_libraries(flatland
#     PRIVATE "$<$<CXX_COMPILER_ID:GNU>:-fsanitize=address -fno-omit-frame-pointer>"
# )

# setting up c++ standard 17
target_compile_features(flatland
    PRIVATE
        cxx_std_17
)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

# find dependencies
find_package(MM 0.1 REQUIRED
                HINTS $<$<CMAKE_SYSTEM:Linux>:/usr/local/lib/>)

find_package(WSDL2 0.1 REQUIRED
                HINTS $<$<CMAKE_SYSTEM:Linux>:/usr/local/lib/>)

find_package(sol2 3.0 REQUIRED
                HINTS $<$<CMAKE_SYSTEM:Linux>:/usr/local/lib/>)

# external libraries ( SDL? )
target_link_libraries(flatland
    PUBLIC
        MM::MM
        WSDL2::WSDL2
)

# include directories
target_include_directories(flatland
    PUBLIC
        $<INSTALL_INTERFACE:include/>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/
)

message("Include directories:")
message($<INSTALL_INTERFACE:include/>)
message($<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
message(${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(flatland
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

#################
# Test section
#################
add_subdirectory(test)


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
        # core
        #${CMAKE_CURRENT_SOURCE_DIR}/include/flatland/core/priority.hpp
        #${CMAKE_CURRENT_SOURCE_DIR}/include/flatland/core/signal.hpp
        #${CMAKE_CURRENT_SOURCE_DIR}/include/flatland/core/task.hpp
        # traits
        #${CMAKE_CURRENT_SOURCE_DIR}/include/flatland/trait/bounded.hpp
        #${CMAKE_CURRENT_SOURCE_DIR}/include/flatland/trait/positioned.hpp
        #${CMAKE_CURRENT_SOURCE_DIR}/include/flatland/trait/renderable.hpp
        # others
        ${CMAKE_CURRENT_SOURCE_DIR}/include/flatland/debug.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/flatland/flatland.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/flatland/scene.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/flatland/sprite.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/flatland/widget.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/flatland/AABB.hpp
        #${CMAKE_CURRENT_SOURCE_DIR}/include/flatland/event.hpp

    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}/flatland
)

install(
    DIRECTORY 
        ${CMAKE_CURRENT_SOURCE_DIR}/include/flatland/core
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}/flatland
)

install(
    DIRECTORY 
        ${CMAKE_CURRENT_SOURCE_DIR}/include/flatland/trait
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
