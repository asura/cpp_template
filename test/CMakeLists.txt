include_directories(
  ${CMAKE_SOURCE_DIR}/vendor
  ${CMAKE_SOURCE_DIR}/include
)

add_executable(
  Tester
  main.cpp
)

add_test(
  NAME Tester
  COMMAND Tester -r junit -o ${CMAKE_BINARY_DIR}/test_result.xml
)
