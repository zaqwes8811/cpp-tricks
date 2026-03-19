#find_package(iceoryx REQUIRED iceoryx_utils)
#find_package(tl-expected REQUIRED)

# Doesn't work for clang -Wl,--wrap=malloc
set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions -Wall -Werror  ${CMAKE_CXX_FLAGS}")

add_executable(SafeUt expected_test.cpp)

#get_target_property(TL_EXPECTED tl::expected INTERFACE_INCLUDE_DIRECTORIES)
#get_target_property(ICEORYX_UTILS iceoryx::iceoryx_utils INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(SafeUt PUBLIC ${TL_EXPECTED} ${ICEORYX_UTILS})