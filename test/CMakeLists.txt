include_directories(
  ${CMAKE_SOURCE_DIR}/vendor/catch2/single_include/catch2
)

add_executable(
  Tester
  main.cpp
)
cotire(Tester)

add_test(
  NAME Tester
  COMMAND Tester -r junit -o ${CMAKE_BINARY_DIR}/test_result_Tester.xml
)
