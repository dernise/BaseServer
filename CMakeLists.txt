cmake_minimum_required( VERSION 2.8 )  
project( EternalKingdom )  

# ---- Get CPU information
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    MESSAGE( "64 bits compiler detected" )
    SET( EX_PLATFORM 64 )
    SET( EX_PLATFORM_NAME "x64" )
else( CMAKE_SIZEOF_VOID_P EQUAL 8 ) 
    MESSAGE( "32 bits compiler detected" )
    SET( EX_PLATFORM 32 )
    SET( EX_PLATFORM_NAME "x86" )
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

# ---- Get boost
FIND_PACKAGE(Boost COMPONENTS system regex REQUIRED)
IF (Boost_FOUND)
    link_directories( ${Boost_LIBRARY_DIRS} )
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

# ---- Openssl
include(cmake/FindOpenSSL.cmake)
FIND_PACKAGE(OpenSSL REQUIRED)
IF(OPENSSL_FOUND)
  INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})
  LIST(APPEND ADDITIONAL_LIBS ${OPENSSL_LIBRARIES})
ELSE()
  MESSAGE( "OPENSSL NOT FOUND, WILL NOT COMPILE" )
ENDIF()

#-----------------------------------------------------------------------------
# We provide a few modules for our own needs to find and configure uncommon
# software on which we depend.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Include our cmake macros
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include (Helpers)

#add_subdirectory(dep)
add_subdirectory(src)
