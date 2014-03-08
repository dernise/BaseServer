#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../Utils/ByteBuffer.hpp"

class WebSocketMessage : public ByteBuffer {
public:
    WebSocketMessage() : ByteBuffer(0), m_opcode(0x82)
    {
    }
    explicit WebSocketMessage(uint8 opcode, size_t res = 200) : ByteBuffer(res), m_opcode(opcode) { }
    
	WebSocketMessage(const WebSocketMessage& packet) : ByteBuffer(packet), m_opcode(packet.m_opcode)
    {
    }
    
	void Initialize(uint8 opcode, size_t newres = 200)
    {
        _storage.reserve(newres);
        m_opcode = opcode;
    }

	uint8* getMask() const { return m_mask; }
    void setMask(uint8* mask) { m_mask = mask; }
	uint64 getSize() const { return m_headerSize; }
    void setSize(uint64 size) { m_headerSize = size; }
	uint16 GetOpcode() const { return m_opcode; }
    void SetOpcode(uint8 opcode) { m_opcode = opcode; }
    int GetMaskIndex() const { return m_maskIndex; }
    void SetMaskIndex(int maskIndex) { m_maskIndex = maskIndex; }
protected:
    int m_maskIndex;
    uint8* m_mask;
	uint64 m_headerSize;
	uint8 m_opcode;
};

class AuthMessage : public ByteBuffer {
public:
    AuthMessage(){ }
    ~AuthMessage() { }
    
    void deserialize(const char* packet, uint16 length){
        appendData(packet, length);
        (*this) >> opcode_;
        length_ = length - 4;
    }
    
    uint8 getOpcode(){ return opcode_; }
    void set_opcode_(uint8 opcode){ opcode_ = opcode; }
    void set_length_(uint8 length){ length_ = length; }
protected:    
    uint16 length_;
    uint8 opcode_;
};

#endif // CHAT_MESSAGE_HPP

