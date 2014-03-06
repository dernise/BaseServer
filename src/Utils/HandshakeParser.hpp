#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include "Log.hpp"

typedef struct{
    std::string key;
    std::string value;
} HandshakeField;

class HandshakeParser{
public:
    HandshakeParser(std::istream &handshake);
    ~HandshakeParser();
    std::string getValue(std::string key);
    std::string getWebSocketAcceptKey(std::string secKey);
    std::string getHandshakeAnswer(std::string handshakeKey);
private:
    std::string base64_encode(const ::std::string &bindata);
    std::vector<HandshakeField> fields;
    char* base64_encode(const unsigned char *input, int length);
};
