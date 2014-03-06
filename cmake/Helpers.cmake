macro (init_target NAME)
    set (TARGET_NAME ${NAME})
    message ("** " ${TARGET_NAME})

    # Include our own module path. This makes #include "x.h" 
    # work in project subfolders to include the main directory headers.
    include_directories (${CMAKE_CURRENT_SOURCE_DIR})
endmacro ()
macro (build_executable TARGET_NAME)
    set (TARGET_LIB_TYPE "EXECUTABLE")
    message (STATUS "-- Build Type:")
    message (STATUS "       " ${TARGET_LIB_TYPE})

    add_executable (${TARGET_NAME} ${ARGN})
endmacro ()
macro (link_boost)
    target_link_libraries (${TARGET_NAME} ${Boost_LIBRARIES})
endmacro ()
macro(link_ssl)
    target_link_libraries(${TARGET_NAME} ${OPENSSL_LIBRARIES})
endmacro ()
