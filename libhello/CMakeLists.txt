#libHello

add_library(Hello
"../src/hello.cpp"
)

target_include_directories(Hello PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)

add_executable(HelloTests "../src/hello.tests.cpp")
target_include_directories(HelloTests PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(HelloTests 
PRIVATE gtest_main Hello)
gtest_discover_tests(HelloTests)