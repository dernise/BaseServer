#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "Types.hpp"
#include "Log.hpp"

class AuthPacket{
public:
    AuthPacket();
    void setOpCode(int popCode){ opCode = popCode; }
    
    boost::archive::binary_oarchive getDataStream(){
        boost::iostreams::back_insert_device<std::string> inserter(data);
        boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
        boost::archive::binary_oarchive oa(s);
        return oa
    }
    
    void constructPacket(){
        
    }
private:
    uint16 length;
    uint16 opCode;
    char data[1024];
    char packet[1024];
};


#endif // CHAT_MESSAGE_HPP


