add_executable(ufs_tests
    parsers/test_csv_parser.cpp
)

target_link_libraries(ufs_tests PRIVATE ufs_core Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(ufs_tests)