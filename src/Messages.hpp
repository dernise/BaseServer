#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "ByteBuffer.hpp"

class AuthPacket : public ByteBuffer {
public:
    AuthPacket() : ByteBuffer(0), m_opcode(0x00)
    {
    }
    explicit AuthPacket(uint16 opcode, size_t res = 200) : ByteBuffer(res), m_opcode(opcode) { }
    
	AuthPacket(const AuthPacket& packet) : ByteBuffer(packet), m_opcode(packet.m_opcode)
    {
    }
    
	void Initialize(uint16 opcode, size_t newres = 200)
    {
        _storage.reserve(newres);
        m_opcode = opcode;
    }

    ByteBuffer getPacket(){
		ByteBuffer buff;
		headerSize = (uint16)size();
		buff << headerSize;
		buff << GetOpcode();
		buff << (char*)contents();
		return buff;
	}

	uint16 getSize() const { return headerSize; }
    void setSize(uint16 size) { headerSize = size; }
	uint16 GetOpcode() const { return m_opcode; }
    void SetOpcode(uint16 opcode) { m_opcode = opcode; }
protected:
	uint16 headerSize;
	uint16 m_opcode;
};

#endif // CHAT_MESSAGE_HPP

