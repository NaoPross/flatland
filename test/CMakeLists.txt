enable_testing()

# task_test
add_executable(task_test task_test.cpp)

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
add_executable(signal_test signal_test.cpp)

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
add_executable(window_test window_test.cpp)

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
add_executable(sprite_test sprite_test.cpp)

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

# scene_test
add_executable(scene_test scene_test.cpp)

target_link_libraries(scene_test
    PRIVATE
        Flatland::flatland
)

target_compile_features(scene_test
    PRIVATE
        cxx_std_17
)

target_compile_definitions(scene_test
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

add_test(
    NAME scene_test
    CONFIGURATIONS Debug
    WORKING_DIRECTORY test
    COMMAND scene_test
)

# flatland_test 
add_executable(flatland_test flatland_test.cpp)

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

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res
    DESTINATION ${PROJECT_BINARY_DIR}/test
)
