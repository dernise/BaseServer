#include "Opcodes.hpp"
#include "../Player/AuthSession.hpp"

OpcodeHandler const Opcodes::emptyHandler =
{
    "<none>",
    STATUS_UNHANDLED,
    &AuthSession::handleNull
};

void Opcodes::BuildOpcodeList(){
    StoreOpcode(NULL_UNHANDLED, "NULL_UNHANDLED", STATUS_UNHANDLED, &AuthSession::handleNull); //0x00
    StoreOpcode(CTS_LOGIN_CHALLENGE, "CTS_LOGIN_CHALLENGE", STATUS_NOT_AUTHED, &AuthSession::handleLoginChallenge);                     /// 0x01
    StoreOpcode(STC_LOGIN_ANSWER, "STC_LOGIN_ANSWER", STATUS_NOT_AUTHED, &AuthSession::handleNull);                     /// 0x02
    StoreOpcode(CTS_REGISTER_MESSAGE, "CTS_REGISTER_MESSAGE", STATUS_NOT_AUTHED, &AuthSession::handleRegisterChallenge); /// 0x03
    StoreOpcode(STC_REGISTER_ANSWER, "STC_REGISTER_ANSWER", STATUS_NOT_AUTHED, &AuthSession::handleNull); //0x04
    StoreOpcode(CTS_MESSAGE, "CTS_MESSAGE", STATUS_LOGGEDIN, &AuthSession::handleMessage); /// 0x05
    StoreOpcode(STC_MESSAGE, "STC_MESSAGE", STATUS_LOGGEDIN, &AuthSession::handleNull); //0x06
}