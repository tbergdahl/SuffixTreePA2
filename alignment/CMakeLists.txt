# create static lib
add_library(alignment STATIC
    src/align.cpp
    src/align_impl.cpp
)

# include dirs for parent module
target_include_directories(alignment PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

