set(NAME tetris-main)

file(GLOB_RECURSE source_files
    "*.cpp"
)
add_executable(${NAME} ${source_files})

target_include_directories(${NAME} PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/"
)
target_link_libraries(${NAME} PRIVATE
    sfml-system sfml-graphics sfml-window sfml-audio
    ImGui-SFML::ImGui-SFML
)

target_compile_features(${NAME} PRIVATE cxx_std_20)

target_compile_definitions(${NAME} PRIVATE SOURCE_PATH="${CMAKE_SOURCE_DIR}/")