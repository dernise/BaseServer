#ifndef _OPCODES_H
#define _OPCODES_H

enum SessionStatus
{
    STATUS_NOT_AUTHED = 0,                                      ///< Player not authenticated
    STATUS_LOGGEDIN,                                        /// Player logged in
    STATUS_UNHANDLED                                        ///< We don' handle this opcode yet
};

enum OpcodesList
{
	CTS_WELCOME = 0x1AF
};


struct OpcodeHandler
{
    char const* name;
    SessionStatus status;
    void (AuthSession::*handler)(authPacket& recvPacket);
};



#endif