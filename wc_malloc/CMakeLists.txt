# ########## Project setup ##########
PROJECT(wildcode_malloc C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

# WildCode_Malloc Version
SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 0)
SET(VERSION_RELEASE 1)
SET(WILDCODE_MALLOC_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_RELEASE}")

# Find Macros
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

INCLUDE(CMakeDependentOption)
INCLUDE(CheckCCompilerFlag)
INCLUDE(CheckCSourceCompiles)
INCLUDE(CheckIncludeFile)
INCLUDE(TestBigEndian)

# Set a default build type if none was specified
IF (NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    MESSAGE(STATUS "Setting build type to 'Debug' as none was specified.")
    SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    SET_PROPERTY(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
ENDIF ()
SET(CMAKE_CONFIGURATION_TYPES "${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

# Compiler specific settings
IF (CMAKE_COMPILER_IS_GNUCC OR (CMAKE_C_COMPILER_ID MATCHES "Clang"))
    ADD_DEFINITIONS(
            -Wall -W
            -fno-common
    )

    IF (NOT WIN32 AND NOT CYGWIN)
        SET(OLD_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS}")
        SET(CMAKE_REQUIRED_FLAGS "${OLD_REQUIRED_FLAGS} -Werror")
        CHECK_C_SOURCE_COMPILES("int foo(void) __attribute__((visibility(\"default\")));
                                 int main(void) {return 0;}" HAVE_VISIBILITY_DEFAULT)
        IF (HAVE_VISIBILITY_DEFAULT)
            CHECK_C_COMPILER_FLAG(-fvisibility=hidden HAVE_VISIBILITY_HIDDEN)
        ENDIF ()
        SET(CMAKE_REQUIRED_FLAGS "${OLD_REQUIRED_FLAGS}")
    ENDIF ()

    IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
        ADD_DEFINITIONS(-ggdb3 -O0)
    ELSEIF (CMAKE_BUILD_TYPE STREQUAL "Release")
        ADD_DEFINITIONS(-O3)
    ENDIF (CMAKE_BUILD_TYPE STREQUAL "Debug")
ENDIF ()

IF (CMAKE_C_COMPILER_ID MATCHES "SunPro")
    SET(OLD_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS}")
    SET(CMAKE_REQUIRED_FLAGS "${OLD_REQUIRED_FLAGS} -xldscope=hidden")
    # __SUNPRO_C >= 0x590
    #   CHECK_C_SOURCE_COMPILES("int foo(void) __attribute__((visibility(\"default\")));
    #                            int main(void) {return 0;}" HAVE_VISIBILITY_DEFAULT)
    # __SUNPRO_C >= 0x550
    CHECK_C_SOURCE_COMPILES("__global int foo(void);
                             int main(void) {return 0;}" HAVE_LDSCOPE_GLOBAL)
    IF (HAVE_LDSCOPE_GLOBAL)# OR HAVE_VISIBILITY_DEFAULT
        SET(HAVE_LDSCOPE_HIDDEN 1)
    ENDIF ()
    SET(CMAKE_REQUIRED_FLAGS "${OLD_REQUIRED_FLAGS}")
ENDIF ()

CHECK_C_SOURCE_COMPILES("int main(void) {__builtin_expect(0,0); return 0;}" HAVE___BUILTIN_EXPECT)

CHECK_C_SOURCE_COMPILES("static inline int static_foo() {return 0;}
                         int main(void) {return 0;}" HAVE_C_INLINE)
CHECK_C_SOURCE_COMPILES("static __inline__ int static_foo() {return 0;}
                         int main(void) {return 0;}" HAVE_C___INLINE__)
CHECK_C_SOURCE_COMPILES("static __inline int static_foo() {return 0;}
                         int main(void) {return 0;}" HAVE_C___INLINE)

CHECK_INCLUDE_FILE(stdint.h HAVE_STDINT_H)
CHECK_INCLUDE_FILE(inttypes.h HAVE_INTTYPES_H)

TEST_BIG_ENDIAN(WORDS_BIGENDIAN)

# ######### General setup ##########
INCLUDE_DIRECTORIES(BEFORE "${CMAKE_SOURCE_DIR}/include")
FILE(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/include")
INCLUDE_DIRECTORIES(BEFORE "${CMAKE_BINARY_DIR}/include")
IF (NOT HAVE_STDINT_H) # AND NOT HAVE_INTTYPES_H
    INCLUDE_DIRECTORIES(BEFORE "${CMAKE_SOURCE_DIR}/include/stdint")
ENDIF ()

# Setup up our config file
CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/include/config.h.cmake" "${CMAKE_BINARY_DIR}/include/config.h")

ADD_SUBDIRECTORY(src)
