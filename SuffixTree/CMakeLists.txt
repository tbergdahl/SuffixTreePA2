# create static lib
add_library(SuffixTree STATIC
    src/tree.cpp
)

# include dirs
target_include_directories(SuffixTree PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)