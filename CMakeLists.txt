cmake_minimum_required(
  VERSION 3.0.0 FATAL_ERROR
)

project(
  CppTemplate
  LANGUAGES CXX
)

set(
  PROJECT_VERSION 0.1.0
)

OPTION( ENABLE_CODECOVERAGE "Enable code coverage testing support" )

if ( ENABLE_CODECOVERAGE )
    if ( NOT CMAKE_BUILD_TYPE STREQUAL "Debug" )
        message( WARNING "Code coverage results with an optimised (non-Debug) build may be misleading" )
    endif ( NOT CMAKE_BUILD_TYPE STREQUAL "Debug" )

    if ( NOT DEFINED CODECOV_OUTPUTFILE )
        set( CODECOV_OUTPUTFILE cmake_coverage.output )
    endif ( NOT DEFINED CODECOV_OUTPUTFILE )

    if ( NOT DEFINED CODECOV_HTMLOUTPUTDIR )
        set( CODECOV_HTMLOUTPUTDIR coverage_results )
    endif ( NOT DEFINED CODECOV_HTMLOUTPUTDIR )

    if ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"))
        find_program( CODECOV_GCOV gcov )
        find_program( CODECOV_LCOV lcov )
        find_program( CODECOV_GENHTML genhtml )
        add_definitions( -fprofile-arcs -ftest-coverage )
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            link_libraries( gcov )
        endif()
        if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
            link_directories(
              # 以下はXCodeのバージョンごとに変化する模様
              /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/9.1.0/lib/darwin
            )
            link_libraries(
              clang_rt.profile_osx
            )
        endif()
        set(
          CMAKE_EXE_LINKER_FLAGS
          ${CMAKE_EXE_LINKER_FLAGS} --coverage
        )
        add_custom_target(
          coverage_init
          ALL
          COMMAND find ${CMAKE_BINARY_DIR} -name "*.gcda" | xargs rm
        )
        add_custom_target(
          coverage
          gcovr -s -r ${CMAKE_SOURCE_DIR} --html --html-details -o ${CMAKE_BINARY_DIR}/coverage.html src
        )
    endif ()
endif (ENABLE_CODECOVERAGE )

enable_testing()

add_subdirectory(test)
