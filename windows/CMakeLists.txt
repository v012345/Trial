add_definitions(

    # -DUNICODE
)
add_executable(Tool
    WIN32
    Tool.cpp
)
target_link_libraries(Tool PRIVATE
    d3d9
)