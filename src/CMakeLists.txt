#App
add_library(Algos
	huffman.cpp
	util.cpp
	bits.cpp
)
target_include_directories(Algos PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)

add_executable(AlgosTests 
	huffman.tests.cpp
)
target_include_directories(AlgosTests PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)

target_link_libraries(AlgosTests 
  PRIVATE 
  GTest::gtest_main
  Algos
)

add_executable(App
	main.cpp
	compressor.cpp
)
target_link_libraries(App PRIVATE Algos)
target_include_directories(App PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)

add_custom_command(TARGET AlgosTests PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/tests/ ${CMAKE_CURRENT_BINARY_DIR}/tests)

gtest_discover_tests(AlgosTests)




# Now simply link against gtest or gtest_main as needed. Eg