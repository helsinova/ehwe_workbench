include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

set(LIBEMBEDDED_SOURCE
	spiram.c
)

add_library(embedded ${LIBEMBEDDED_SOURCE})

