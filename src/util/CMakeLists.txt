include_directories(
  ${CMAKE_SOURCE_DIR}/include
)

add_library(
  util
  Logger.cpp
  Logger_spdlog.cpp
  memory_usage.cpp
)

target_link_libraries(
  util
  spdlog::spdlog
)
