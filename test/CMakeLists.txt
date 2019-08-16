include_directories(
  ${CMAKE_SOURCE_DIR}/vendor/catch2/single_include/catch2
)

add_executable(
  Tester
  main.cpp
)
target_link_libraries(
  Tester
  Catch2::Catch2
  spdlog::spdlog
)
cotire(Tester)

add_test(
  NAME Tester
  COMMAND Tester -r junit -o ${CMAKE_BINARY_DIR}/test_result_Tester.xml
)
