# Warleague config file
# On Windows : install boost in C:/boost and run CMAKE, should work
# On Debian/Ubuntu : run apt-get install libboost-all-dev libmysqlcppconn-dev and run CMAKE, should work
# On Mac : Run brew install mysql-connector-c++ boost and run CMAKE, should work
# On FreeBSD : Untested

cmake_minimum_required( VERSION 2.8 )  
project( EternalKingdom )  

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

if(WIN32)
  if(PLATFORM MATCHES X86) # 32-bit
    set(DEP_ARCH win32)
    MESSAGE( "32 bits compiler detected" )
  else() # 64-bit
    set(DEP_ARCH win32) # Okey, dk why
    MESSAGE( "64 bits compiler detected" )
  endif()
endif()

# Win32 delivered packages
if(WIN32)
  set(LIBS_DIR ${CMAKE_INSTALL_PREFIX}/bin) #set LIB DIR
  set(MYSQL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/include/mysql)
  set(MYSQL_LIBRARY ${CMAKE_SOURCE_DIR}/dep/lib/mysqlcppconn.lib)
  set(MYSQL_DEBUG_LIBRARY ${CMAKE_SOURCE_DIR}/dep/lib/mysqlcppconn.lib)
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

# ---- Openssl and mysql connector
if(UNIX)
  FIND_PACKAGE(OpenSSL REQUIRED)
  IF(OPENSSL_FOUND)
    INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})
    LIST(APPEND ADDITIONAL_LIBS ${OPENSSL_LIBRARIES})
  ELSE()
    MESSAGE( "OPENSSL NOT FOUND, WILL NOT COMPILE" )
  ENDIF()

  FIND_PACKAGE(MySQLConnectorCPP)
  IF(MYSQLCONNECTORCPP_FOUND)
    INCLUDE_DIRECTORIES(${MYSQLCONNECTORCPP_INCLUDE_DIR})
    LIST(APPEND ADDITIONAL_LIBS ${MYSQLCONNECTORCPP_LIBRARIES})
    MESSAGE(${MYSQLCONNECTORCPP_LIBRARIES})
  ELSE()  
    MESSAGE( "MYSQL CONNECTOR NOT FOUND, WILL NOT COMPILE" )
  ENDIF()
endif()

#-------------------------
# Include our cmake macros
include (Helpers)

add_subdirectory(src)
