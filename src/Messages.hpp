#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "ByteBuffer.hpp"

class AuthPacket : public ByteBuffer {
public:
	AuthPacket(); 
    AuthPacket(const AuthPacket& packet)              : ByteBuffer(packet), m_opcode(packet.m_opcode)
    {
    }
    
	void Initialize(uint16 opcode, size_t newres = 200)
    {
        _storage.reserve(newres);
        m_opcode = opcode;
    }

	uint16 GetOpcode() const { return m_opcode; }
    void SetOpcode(uint16 opcode) { m_opcode = opcode; }
protected:
	uint16 m_opcode;
};

#endif // CHAT_MESSAGE_HPP

