INCLUDE (CheckLibraryExists)
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

set(LIBEMBEDDED_SOURCE
	spiram.c
)

set(LIBEMBEDDED_SOURCE
	${LIBEMBEDDED_SOURCE}
)

add_library(ligmtime ${LIBEMBEDDED_SOURCE})

CHECK_LIBRARY_EXISTS(rt clock_gettime "" HAVE_LIB_RT)
if (HAVE_LIB_RT)
    target_link_libraries(ligmtime rt)
else (HAVE_LIB_RT)
    target_link_libraries(ligmtime)
endif (HAVE_LIB_RT)
