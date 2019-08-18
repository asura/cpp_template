cmake_minimum_required(
  VERSION 3.10.0
)

#######################################
# プロジェクト設定
project(
  CppTemplate
  VERSION 0.1.0
  LANGUAGES CXX
)

# C++規格
set(CMAKE_CXX_STANDARD 17)

# Debug or Release
if(CMAKE_BUILD_TYPE)
  if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  else()
    message(FATAL_ERROR "変数CMAKE_BUILD_TYPEにはDebugかReleaseを指定してください")
  endif()
else()
  message(FATAL_ERROR "変数CMAKE_BUILD_TYPEが定義されていません")
endif()

# ビルドコマンドを出力 (要 CMake 2.8.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# オプション設定
option(ENABLE_CODECOVERAGE "Enable code coverage testing support")

#######################################
# CppCheck対応 (要 CMake 3.10)
find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
if(CMAKE_CXX_CPPCHECK)
  list(
    APPEND CMAKE_CXX_CPPCHECK 
    "––project=complile_commands.json"
    "--enable=warning,performance,portability"
    "-i ${CMAKE_SOURCE_DIR}/vendor"
    "--suppressions-list=${CMAKE_SOURCE_DIR}/CppCheckSuppressions.txt"
  )
endif()

#######################################
# clang-tidyを実行させる (要 CMake 3.6)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;--header-filter=${CMAKE_SOURCE_DIR}/include")

#######################################
# テスト関連
include(CTest)
enable_testing()

if(ENABLE_CODECOVERAGE)
  if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(WARNING "Code coverage results with an optimised (non-Debug) build may be misleading")
  endif()

  if((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"))
    add_definitions( -fprofile-arcs -ftest-coverage )

    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        link_libraries(gcov)
    endif()

    if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        link_directories(
          # 以下はXCodeのバージョンごとに変化する模様
          #/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/9.1.0/lib/darwin
          /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/10.0.1/lib/darwin
        )
        link_libraries(clang_rt.profile_osx)
    endif()

    set(
      CMAKE_EXE_LINKER_FLAGS
      ${CMAKE_EXE_LINKER_FLAGS} --coverage
    )

    add_custom_target(
      coverage_init
      ALL
      COMMAND find ${CMAKE_BINARY_DIR} -name "*.gcda" -exec rm {} +
    )

    add_custom_target(
      coverage
      COMMAND mkdir -p ${CMAKE_BINARY_DIR}/coverage
      COMMAND gcovr -s -r ${CMAKE_SOURCE_DIR} --html --html-details -o ${CMAKE_BINARY_DIR}/coverage/coverage.html src
    )
  endif()
endif()

#######################################
# ビルド対象
add_subdirectory(vendor/catch2)
add_subdirectory(vendor/spdlog)
add_subdirectory(src/util)
add_subdirectory(test)
