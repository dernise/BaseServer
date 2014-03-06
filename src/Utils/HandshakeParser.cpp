#include "HandshakeParser.hpp"

using namespace boost::algorithm;

HandshakeParser::HandshakeParser(std::istream &handshake){
    while(!handshake.eof())
    {
        std::string line;
        std::vector<std::string> lineValues;
        int lineValuesSize = 0;
        
        getline(handshake, line);
        split(lineValues, line, is_any_of(": "), token_compress_on);
        
        lineValuesSize = lineValues.size();
        
        if(lineValuesSize > 1)
        {
            lineValues[lineValuesSize-1].erase(lineValues[lineValuesSize-1].size() - 1); // Okey, let's remove the \r

            HandshakeField field;
            field.key = lineValues[0];
            field.value = lineValues[1]; //If the length is > 2, it's the Host key, well, we don't care about the port in this one no need to parse more
            
            fields.push_back(field);
        }
    }
}

HandshakeParser::~HandshakeParser(){
}

std::string HandshakeParser::getValue(std::string key){
    for(int i=0; i<fields.size(); i++){
        if(fields[i].key == key){
            return fields[i].value;
        }
    }
    return "unknown";
}

std::string HandshakeParser::getWebSocketAcceptKey(std::string secKey){
    secKey += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    unsigned char* sha_str = SHA1(reinterpret_cast<const unsigned char*>(secKey.c_str()), secKey.length(), nullptr);
    
    std::string finalKey = base64_encode(reinterpret_cast<const unsigned char*>(sha_str), 20);
    return finalKey;
}

std::string HandshakeParser::getHandshakeAnswer(std::string handshakeKey){
    std::ostringstream oss;
    oss << "HTTP/1.1 101 Switching Protocols\r\n";
    oss << "Upgrade: websocket\r\n";
    oss << "Connection: Upgrade\r\n";
    oss << "Sec-WebSocket-Accept: " << handshakeKey << "\r\n";
    oss << "\r\n";
    
    return oss.str();
}


char *HandshakeParser::base64_encode(const unsigned char *input, int length) {
    BIO *bmem, *b64;
    BUF_MEM *bptr;
    
    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, input, length);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);
    
    char *buff = (char *)malloc(bptr->length);
    memcpy(buff, bptr->data, bptr->length-1);
    buff[bptr->length-1] = 0;
    
    BIO_free_all(b64);
    
    return buff;
}