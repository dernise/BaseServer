file (GLOB NETWORK Network/*.cpp Network/*.hpp)
file (GLOB UTILS Utils/*.cpp Utils/*.hpp)
file (GLOB PLAYER Player/*.cpp Player/*.hpp)

init_target (kingdom_server)

build_executable (${TARGET_NAME} ${NETWORK} ${UTILS} ${PLAYER} main.cpp)

SOURCE_GROUP("Network" FILES ${NETWORK})
SOURCE_GROUP("Utils" FILES ${UTILS})
SOURCE_GROUP("Player" FILES ${PLAYER})

link_boost()
link_ssl()
