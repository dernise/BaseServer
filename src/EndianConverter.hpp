#include "Types.hpp"

namespace EndianConverter
{
    template<size_t T>
    inline void convert(char* val)
    {
        std::swap(*val, *(val + T - 1));
        convert < T - 2 > (val + 1);
    }

    template<> inline void convert<0>(char*) {}
    template<> inline void convert<1>(char*) {}             // ignore central byte

    template<typename T>
    inline void apply(T* val)
    {
        convert<sizeof(T)>((char*)(val));
    }
}

template<typename T> inline void EndianConvert(T& val) { EndianConverter::apply<T>(&val); }
template<typename T> void EndianConvert(T*);         // will generate link error
inline void EndianConvert(uint8&) { }
inline void EndianConvert(int8&)  { }
