add_executable( EternalKingdom main.cpp 
				AuthServer.cpp 
				AuthServer.hpp
				AuthSession.hpp
				Messages.hpp
				Log.cpp
				Log.hpp
				Types.hpp
				Players.cpp
				Players.hpp
                ByteBuffer.hpp
				ByteBuffer.cpp
				Opcodes.hpp
				Opcodes.cpp
				EndianConverter.hpp
)  

target_link_libraries(EternalKingdom ${Boost_LIBRARIES})

