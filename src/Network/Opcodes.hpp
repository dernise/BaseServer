#ifndef _OPCODES_H
#define _OPCODES_H

#include <iostream>
#include <map>
#include "Messages.hpp"
#include "../Utils/ByteBuffer.hpp"

class AuthSession;

enum SessionStatus
{
    STATUS_NOT_AUTHED = 1,                                      ///< Player not authenticated
    STATUS_LOGGEDIN = 2,                                        /// Player logged in
    STATUS_UNHANDLED                                        ///< We don' handle this opcode yet
};

enum WebsocketOpcode
{
    TEXT_MESSAGE = 0x81,
	BINARY_MESSAGE = 0x82 //Message opCode
};

enum OpcodeList{
    NULL_UNHANDLED = 0,
    CTS_LOGIN_CHALLENGE = 1,
    STC_LOGIN_ANSWER = 2,
    CTS_REGISTER_MESSAGE = 3,
    STC_REGISTER_ANSWER = 4,
	CTS_MESSAGE = 5,
	STC_MESSAGE = 6,
    END_OPCODE
};

struct OpcodeHandler
{
    char const* name;
    SessionStatus status;
    void (AuthSession::*handler)(AuthMessage& recvPacket);
};

typedef std::map<uint8, OpcodeHandler> OpcodeMap;

class Opcodes
{
public:
    static Opcodes& getInstance()
    {
        static Opcodes instance;
        return instance;
    }
    
    void BuildOpcodeList();
    void StoreOpcode(uint8 Opcode, char const* name, SessionStatus status, void (AuthSession::*handler)(AuthMessage& recvPacket))
    {
        OpcodeHandler& ref = mOpcodeMap[Opcode];
        ref.name = name;
        ref.status = status;
        ref.handler = handler;
    }
    
    /// Lookup opcode
    inline OpcodeHandler const* LookupOpcode(uint8 id) const
    {
        OpcodeMap::const_iterator itr = mOpcodeMap.find(id);
        if (itr != mOpcodeMap.end())
        { return &itr->second; }
        return NULL;
    }

    inline OpcodeHandler const& operator[](uint8 id) const
    {
        OpcodeMap::const_iterator itr = mOpcodeMap.find(id);
        if (itr != mOpcodeMap.end())
        { return itr->second; }
        return emptyHandler;
    }
    
    static OpcodeHandler const emptyHandler;
    
    OpcodeMap mOpcodeMap;
private:
    ~Opcodes() { }
    Opcodes(){
        BuildOpcodeList();
    }
};

#define sOpcodeTable Opcodes::getInstance()

#endif
