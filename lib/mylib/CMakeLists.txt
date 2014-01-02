file(GLOB sources *.cpp *.h)

set(MYLIB_STAT_SRCS ${sources})

add_library(mylib STATIC ${MYLIB_STAT_SRCS})
target_link_libraries(mylib curl)
