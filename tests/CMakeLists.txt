cmake_minimum_required(VERSION 4.2)

add_executable(regtest test.c)

target_link_libraries(regtest PRIVATE colinregistry)

add_test(NAME regtest COMMAND regtest)

set_target_properties(regtest PROPERTIES
  C_STANDARD 23
  C_STANDARD_REQUIRED ON
)
