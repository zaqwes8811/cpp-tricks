# project(testlink)
# cmake_minimum_required(VERSION 2.8)

# #link_libraries(dl)

# find_package(Libunwind REQUIRED)


# #SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ldl")
# #SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ldl")

# add_executable(testlink
#     main.cc
#     heap.c
# )

# target_link_libraries(testlink
#         ${CMAKE_DL_LIBS}
#         ${LIBUNWIND_LIBRARIES}
#         )