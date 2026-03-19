
add_executable(train_test
        ${CMAKE_SOURCE_DIR}/projects/all_tests.cc
        ccup_amazon_test.cc
        ccup_apple_test.cc
        ccup_facebook_test.cc
        ccup_google_test.cc
        ccup_nvidia_test.cc
        ccup_unknown_test.cc
        #
        crack_code2_test.cc
        crack_code_gr_trees_test.cc
        crack_code_lists.cc
        #
        oj_bst_test.cc
        oj_spec_test.cc
        oj_test.cc
        )


target_link_libraries(train_test
        ${GTEST_BOTH_LIBRARIES}
        pthread dl m rt
        )

####################################################################

find_package(benchmark REQUIRED)
include_directories(${benchmark_INCLUDE_DIRS})
link_libraries(${benchmark_LIBRARIES} pthread)

add_executable(train_bench
        ${CMAKE_SOURCE_DIR}/projects/all_benches.cpp
        )


target_link_libraries(train_bench
        ${GTEST_BOTH_LIBRARIES}
        pthread dl m rt
        )

add_executable(train
        ${CMAKE_SOURCE_DIR}/projects/all_tests.cc
        my_recyclable_list_test.cc
        )


target_link_libraries(train
        ${GTEST_BOTH_LIBRARIES}
        pthread dl m rt
        )
