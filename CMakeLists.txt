cmake_minimum_required(VERSION 3.10)
project(audio_visualizer)

find_package(SDL3 CONFIG REQUIRED)

add_executable(audio_visualizer src/main.c src/mp3.c src/fft.c src/visualizer.c src/complex.c)
target_include_directories(audio_visualizer PRIVATE lib/minimp3)
target_link_libraries(audio_visualizer PRIVATE SDL3::SDL3)

add_custom_command(TARGET audio_visualizer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL3::SDL3>
    $<TARGET_FILE_DIR:audio_visualizer>
)