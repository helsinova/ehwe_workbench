INCLUDE (CheckLibraryExists)
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/interfaces/include")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/include")

option(STDLIB_TARGET
    "If real target has a working stdlib" ON)

option(HAS_PRINTF
    "If has printf and it works" ON)

set(LIBEMBEDDED_SOURCE
  main.c
  pm_ina233.c
)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/embedded_config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/embedded_config.h"
  )

add_library(embedded ${LIBEMBEDDED_SOURCE})

CHECK_LIBRARY_EXISTS(m sqrt "" HAVE_LIB_MATH)
if (HAVE_LIB_MATH)
    target_link_libraries(embedded m)
else (HAVE_LIB_MATH)
    target_link_libraries(embedded)
endif (HAVE_LIB_MATH)

