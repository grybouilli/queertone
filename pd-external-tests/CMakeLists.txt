cmake_minimum_required(VERSION 3.26)
project(ccounter)

# ---------------------------------------------- #
# SETTING GLOBAL VARIABLES
include(CMakePrintHelpers)

if(UNIX AND NOT APPLE)
    set(PD_SOURCE_PATH_DEFAULT /usr/local/pd/src)
elseif(UNIX AND APPLE)
    set(PD_SOURCE_PATH_DEFAULT /Applications/Pd-0.54-0.app/Contents/Resources/src)
elseif(WIN32)
    set(PD_SOURCE_PATH_DEFAULT c:/pd/src)
endif()

set(PD_SOURCE_PATH ${PD_SOURCE_PATH_DEFAULT} CACHE PATH "Pd src path")
set(PD_INSTANCE OFF CACHE BOOL "Builds with PDINSTANCE defined")

find_package(Python3 REQUIRED COMPONENTS Interpreter Development NumPy)

if(VERBOSE)
    cmake_print_variables(Python3_INCLUDE_DIRS)
    cmake_print_variables(Python3_NumPy_INCLUDE_DIRS)
endif()
execute_process(
    COMMAND "${Python3_EXECUTABLE}" -c "import platform; print('.pd_' + platform.system().lower())"
    OUTPUT_VARIABLE PD_EXTERNAL_SUFFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

cmake_print_variables(PD_EXTERNAL_SUFFIX)

set(PD_EXTERNALS_PATH "~/Documents/Pd/externals" CACHE PATH "Pd externals path")

# ---------------------------------------------- #
# BUILDING OBJECTS
add_library(objects OBJECT
    ccounter.c)

target_compile_definitions(objects PRIVATE PD)
target_compile_definitions(objects PRIVATE UNIX)

if(PD_INSTANCE)
    target_compile_definitions(objects PRIVATE PDINSTANCE)
endif()

target_compile_options(objects PRIVATE -fPIC -Wall -Wextra -Wshadow -Winline -Wstrict-aliasing -O3 -ffast-math -funroll-loops -fomit-frame-pointer -march=core2 -mfpmath=sse -msse -msse2 -msse3)

target_include_directories(objects PRIVATE ${PD_SOURCE_PATH})

# ---------------------------------------------- #
# BUILDING OBJECTS
add_library(${PROJECT_NAME} SHARED $<TARGET_OBJECTS:objects>)
target_compile_options(${PROJECT_NAME} PRIVATE -rdynamic -shared -fPIC -Wl,-rpath,"\$ORIGIN",--enable-new-dtags)
target_link_libraries(${PROJECT_NAME}
    PRIVATE -lc
    PRIVATE -lm
)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX "${PD_EXTERNAL_SUFFIX}")

if(NOT ${PD_INSTANCE})
    install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}${PD_EXTERNAL_SUFFIX} DESTINATION ${PD_EXTERNALS_PATH}/${PROJECT_NAME})
else()
    install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}${PD_EXTERNAL_SUFFIX} DESTINATION ${PD_EXTERNALS_PATH}instance/${PROJECT_NAME})
endif()