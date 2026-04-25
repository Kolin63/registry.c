cmake_minimum_required(VERSION 4.2)

add_executable(regtest regtest.c)
add_executable(strcmptest strcmptest.c)
add_executable(regstrcmptest regstrcmptest.c)

target_link_libraries(regtest PRIVATE colinregistry)
target_link_libraries(regstrcmptest PRIVATE colinregistry)

add_test(NAME regtest COMMAND regtest)
add_test(NAME strcmptest COMMAND strcmptest)
add_test(NAME regstrcmptest COMMAND regstrcmptest)

set_target_properties(regtest PROPERTIES
  C_STANDARD 99
  C_STANDARD_REQUIRED ON
)
set_target_properties(strcmptest PROPERTIES
  C_STANDARD 99
  C_STANDARD_REQUIRED ON
)
set_target_properties(regstrcmptest PROPERTIES
  C_STANDARD 99
  C_STANDARD_REQUIRED ON
)
