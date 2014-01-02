set(CMAKE_CXX_FLAGS "-fpermissive -p -g")
set(CMAKE_BUILD_TYPE Debug)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#source file
file(GLOB sources *.cpp *.h)
set(YYCURL_STAT_SRCS  ${sources})
MESSAGE(status " This is the srcs:" ${sources})

#add mylib
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib)

#add others
include_directories(/usr/include)
link_directories(/usr/local/lib)

add_executable(yycurl ${YYCURL_STAT_SRCS})

target_link_libraries(yycurl mylib)