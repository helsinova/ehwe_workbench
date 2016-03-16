include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/interfaces/include")

option(STDLIB_TARGET
    "If real target has a working stdlib" ON)

option(HAS_PRINTF
    "If has printf and it works" ON)

set(LIBEMBEDDED_SOURCE
	spiram.c
)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/embedded_config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/embedded_config.h"
  )

add_library(embedded ${LIBEMBEDDED_SOURCE})

