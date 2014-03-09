file (GLOB NETWORK Network/*.cpp Network/*.hpp)
file (GLOB UTILS Utils/*.cpp Utils/*.hpp)
file (GLOB PLAYER Player/*.cpp Player/*.hpp)
file (GLOB DATABASE Database/*.cpp Database/*.hpp)

init_target (kingdom_server)

include_directories(
    ${MYSQL_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
  )

add_executable (${TARGET_NAME} ${NETWORK} ${UTILS} ${PLAYER} ${DATABASE} main.cpp)

if(WIN32)
  target_link_libraries(${TARGET_NAME}
    optimized ${MYSQL_LIBRARY}
    optimized ${OPENSSL_LIBRARIES}
    debug ${MYSQL_DEBUG_LIBRARY}
    debug ${OPENSSL_DEBUG_LIBRARIES}
  )
  if(PLATFORM MATCHES X86)
    target_link_libraries(${TARGET_NAME})
  endif()
endif()

if(UNIX)
    target_link_libraries(${TARGET_NAME} ${OPENSSL_LIBRARIES})
endif()

if(UNIX)
    add_definitions("-Wno-shift-count-overflow -Wno-deprecated-declarations -Wno-format-security")
endif()


SOURCE_GROUP("Network" FILES ${NETWORK})
SOURCE_GROUP("Utils" FILES ${UTILS})
SOURCE_GROUP("Player" FILES ${PLAYER})
SOURCE_GROUP("Database" FILES ${DATABASE})

link_boost()
