aux_source_directory(./src SRC)
aux_source_directory(./src/subcommands SRC_COMMANDS)

add_executable(sorter ${SRC} ${SRC_COMMANDS})
target_link_libraries(sorter PRIVATE compile_options)

target_link_libraries(sorter PRIVATE tapes CLI11::CLI11)
