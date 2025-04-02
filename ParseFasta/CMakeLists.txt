# create static lib
add_library(parsefasta STATIC
    src/parse.cpp
)

# include dirs for parent module
target_include_directories(parsefasta PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)