SET(wildcode_malloc_executable_SRCS
        ${GETOPT}
        ${SYS_OTHER}
        main.c
        wc_malloc.c
        )

SET(wildcode_malloc_executable_HDRS
        ../include/wc_malloc.h
        )

ADD_EXECUTABLE(wildcode_malloc
    ${wildcode_malloc_executable_SRCS}
    ${wildcode_malloc_executable_HDRS}
    )

# set our target paths
SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

INSTALL(TARGETS wildcode_malloc DESTINATION bin)

SET(WARNINGS "/Wall") # Since windows can only disable specific warnings, not enable them

SET_TARGET_PROPERTIES(wildcode_malloc PROPERTIES COMPILE_FLAGS "${WARNINGS} ${MT_BUILD}")
