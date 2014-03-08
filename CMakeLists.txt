cmake_minimum_required( VERSION 2.8 )  
project( EternalKingdom )  

if(WIN32)
  if(PLATFORM MATCHES X86) # 32-bit
    set(DEP_ARCH win32)
    MESSAGE( "32 bits compiler detected" )
  else() # 64-bit
    set(DEP_ARCH x64)
    MESSAGE( "64 bits compiler detected" )
  endif()
endif()

# Win32 delivered packages
if(WIN32)
  set(LIBS_DIR ${CMAKE_INSTALL_PREFIX}/bin) #set LIB DIR
  set(MYSQL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/include/mysql)
  set(MYSQL_LIBRARY ${CMAKE_SOURCE_DIR}/dep/lib/${DEP_ARCH}_release/libmySQL.lib)
  set(MYSQL_DEBUG_LIBRARY ${CMAKE_SOURCE_DIR}/dep/lib/${DEP_ARCH}_debug/libmySQL.lib)
  set(OPENSSL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/include)
  set(OPENSSL_LIBRARIES ${CMAKE_SOURCE_DIR}/dep/lib/${DEP_ARCH}_release/libeay32.lib)
  set(OPENSSL_DEBUG_LIBRARIES ${CMAKE_SOURCE_DIR}/dep/lib/${DEP_ARCH}_debug/libeay32.lib)
endif()

# ---- Get boost
FIND_PACKAGE(Boost COMPONENTS system regex REQUIRED)
IF (Boost_FOUND)
    link_directories( ${Boost_LIBRARY_DIRS} )
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

# ---- Openssl
if(UNIX)
  include(cmake/FindOpenSSL.cmake)
  FIND_PACKAGE(OpenSSL REQUIRED)
  IF(OPENSSL_FOUND)
    INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})
    LIST(APPEND ADDITIONAL_LIBS ${OPENSSL_LIBRARIES})
  ELSE()
    MESSAGE( "OPENSSL NOT FOUND, WILL NOT COMPILE" )
  ENDIF()
endif()

#-----------------------------------------------------------------------------
# We provide a few modules for our own needs to find and configure uncommon
# software on which we depend.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Include our cmake macros
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include (Helpers)

add_subdirectory(src)
