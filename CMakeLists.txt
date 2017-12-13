include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/interfaces/include")

option(STDLIB_TARGET
    "If real target has a working stdlib" ON)

option(HAS_PRINTF
    "If has printf and it works" ON)

## Test/parts of the embedded program
option(WORKBENCH_TEST_CS
	"Toggle CS once" OFF)

option(WORKBENCH_STATUS_CHECK
	"Simple register acces of ADXL345" OFF)

option(WORKBENCH_LOOP_MEASURE
	"Sample data and output continously to printf" OFF)

	set(LIBEMBEDDED_SOURCE
	adxl345_spi.c
)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/embedded_config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/embedded_config.h"
  )

add_library(embedded ${LIBEMBEDDED_SOURCE})

