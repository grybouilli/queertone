cmake_minimum_required(VERSION 3.14) 
project(my_external)           

# personal variables
set(SRC_DIR ".")
set(HDR_DIR "hdr")

# -----------------------------------------------------------------------
# sources into objects

add_library(objects OBJECT ${SRC_DIR}/*.cpp)

# object flext compile options
target_compile_options(objects PRIVATE -DFLEXT_USE_CMEM -DNDEBUG -DFLEXT_SYS=2 -DPD -ffast-math -O3 -mtune=native -fPIC -fvisibility-inlines-hidden -pthread)

# include directories from flext build command line
target_include_directories(objects PRIVATE /usr/local/pd/src)
target_include_directories(objects PRIVATE /usr/local/include/stk )
target_include_directories(objects PRIVATE /usr/local/include/flext)

# -----------------------------------------------------------------------
# objects into binary

add_library(${PROJECT_NAME} SHARED
    $<TARGET_OBJECTS:objects>
)

# lib flext compile options
target_compile_options(${PROJECT_NAME} PRIVATE -Wl,-S)

target_link_libraries(${PROJECT_NAME} 
                        PRIVATE -L/usr/local/pd/bin 
                        PRIVATE -L/usr/local/lib
                        PRIVATE -L/usr/lib
                        PRIVATE -lpthread
                        PRIVATE -lflext-pd_s)


set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

# setting pd extension suffix
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".pd_linux")

# stripping file to make it lighter 
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND strip --strip-unneeded build/${PROJECT_NAME}.pd_linux
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Stripping file..."
)

# copying file to Pd externals directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND mkdir -p ~/Documents/Pd/externals/${PROJECT_NAME} && cp build/${PROJECT_NAME}.pd_linux ~/Documents/Pd/externals/${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Copying file to Pd externals directory..."
)