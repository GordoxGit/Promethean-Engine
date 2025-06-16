find_package(GTest CONFIG REQUIRED)
add_executable(dummy_test test_dummy.cpp)
target_link_libraries(dummy_test PRIVATE GTest::gtest_main)
add_test(NAME dummy_test COMMAND dummy_test)
