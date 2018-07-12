cmake_minimum_required(VERSION 3.0)

add_subdirectory("ext/flags")

add_executable(flags_example "flags_example.cpp")
target_link_libraries(flags_example flags)
target_compile_options(flags_example PRIVATE "-Wfatal-errors" "-Werror" "-Wall" "-std=c++17")
