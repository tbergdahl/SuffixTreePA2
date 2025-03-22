# create static lib
add_library(ParseFasta STATIC
    src/parse.cpp
)

# include dirs for parent module
target_include_directories(ParseFasta PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)