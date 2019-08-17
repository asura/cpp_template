include_directories(
  ${CMAKE_SOURCE_DIR}/vendor/catch2/single_include/catch2
  ${CMAKE_SOURCE_DIR}/include
)

add_executable(
  Tester
  main.cpp
  util/memory_usage_test.cpp
)
target_link_libraries(
  Tester
  Catch2::Catch2
  spdlog::spdlog
  util
)
cotire(Tester)

add_test(
  NAME Tester
  COMMAND Tester -r junit -o ${CMAKE_BINARY_DIR}/test_result_Tester.xml
)
