add_executable(example1 example1.c)

target_link_libraries(example1 PRIVATE colinregistry)

set_target_properties(example1 PROPERTIES
  C_STANDARD 99
  C_STANDARD_REQUIRED ON
)
