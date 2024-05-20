aux_source_directory(./src SRC)
aux_source_directory(./src/util SRC_UTIL)

add_library(tapes ${SRC} ${SRC_UTIL})
target_link_libraries(tapes PRIVATE compile_options)

target_include_directories(tapes PUBLIC ./include)
