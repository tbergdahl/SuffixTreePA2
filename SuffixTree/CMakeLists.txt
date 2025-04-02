# create static lib
add_library(suffixtree STATIC
    src/tree_pub.cpp
    src/tree_priv.cpp
)

# include dirs
target_include_directories(suffixtree PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)