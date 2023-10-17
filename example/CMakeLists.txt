
add_executable(Book WIN32
    "book.cpp"
    "../Games/src/WindowCreator/WindowCreator.cpp"
)
target_include_directories(Book PRIVATE
    "../Games/src"
    "../Games/include"
)

target_compile_definitions(Book PRIVATE
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

target_link_libraries(Book
    GameLib
)

# add_custom_command(TARGET Book POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E copy
# "${CMAKE_SOURCE_DIR}/log/stageData.txt"
# ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)