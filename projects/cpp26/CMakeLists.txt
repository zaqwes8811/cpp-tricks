set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cpp26
        ${CMAKE_SOURCE_DIR}/projects/all_tests.cc
        base_test.cc
        )

# Modules is questionable thing
#   https://vector-of-bool.github.io/2019/01/27/modules-doa.html
#   https://medium.com/@nerudaj/are-c-modules-there-yet-77cde050afce
target_sources(cpp26
  PUBLIC
    FILE_SET all_my_modules TYPE CXX_MODULES FILES
    demo.cppm
)

target_link_libraries(cpp26
        ${GTEST_BOTH_LIBRARIES}
        pthread dl m rt
        )