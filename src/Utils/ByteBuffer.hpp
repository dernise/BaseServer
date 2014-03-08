#ifndef _BYTEBUFFER_H
#define _BYTEBUFFER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "EndianConverter.hpp"
#include "Log.hpp"

class ByteBufferException
{
    public:
        ByteBufferException(bool _add, size_t _pos, size_t _esize, size_t _size)
            : add(_add), pos(_pos), esize(_esize), size(_size)
        {
            //PrintPosError();
        }

        //void PrintPosError() const;
    private:
        bool add;
        size_t pos;
        size_t esize;
        size_t size;
};

template<class T>
struct Unused
{
    Unused() {}
};

class ByteBuffer
{
    public:
        const static size_t DEFAULT_SIZE = 0x1000;

        // constructor
        ByteBuffer(): _rpos(0), _wpos(0)
        {
            _storage.reserve(DEFAULT_SIZE);
        }

        // constructor
        ByteBuffer(size_t res): _rpos(0), _wpos(0)
        {
            _storage.reserve(res);
        }

        // copy constructor
        ByteBuffer(const ByteBuffer& buf): _rpos(buf._rpos), _wpos(buf._wpos), _storage(buf._storage) { }

		ByteBuffer& operator<<(uint8 value)
        {
            append<uint8>(value);
            return *this;
        }

        ByteBuffer& operator<<(uint16 value)
        {
            append<uint16>(value);
            return *this;
        }

        ByteBuffer& operator<<(uint32 value)
        {
            append<uint32>(value);
            return *this;
        }
    
        ByteBuffer& operator<<(uint64 value)
        {
            append<uint64>(value);
            return *this;
        }

        // signed as in 2e complement
        ByteBuffer& operator<<(int8 value)
        {
            append<int8>(value);
            return *this;
        }

        ByteBuffer& operator<<(int16 value)
        {
            append<int16>(value);
            return *this;
        }

        ByteBuffer& operator<<(int32 value)
        {
            append<int32>(value);
            return *this;
        }

        // floating points
        ByteBuffer& operator<<(float value)
        {
            append<float>(value);
            return *this;
        }

        ByteBuffer& operator<<(double value)
        {
            append<double>(value);
            return *this;
        }

        ByteBuffer& operator<<(const std::string& value)
        {
            append((uint8 const*)value.c_str(), value.length());
            append((uint8)0);
            return *this;
        }

        ByteBuffer& operator<<(const char* str)
        {
            append((uint8 const*)str, str ? strlen(str) : 0);
            append((uint8)0);
            return *this;
        }

        ByteBuffer& operator>>(bool& value)
        {
            value = read<char>() > 0 ? true : false;
            return *this;
        }

        ByteBuffer& operator>>(uint8& value)
        {
            value = read<uint8>();
            return *this;
        }

        ByteBuffer& operator>>(uint16& value)
        {
            value = read<uint16>();
            return *this;
        }

        ByteBuffer& operator>>(uint32& value)
        {
            value = read<uint32>();
            return *this;
        }
    
        ByteBuffer& operator>>(uint64& value)
        {
            value = read<uint64>();
            return *this;
        }

        // signed as in 2e complement
        ByteBuffer& operator>>(int8& value)
        {
            value = read<int8>();
            return *this;
        }

        ByteBuffer& operator>>(int16& value)
        {
            value = read<int16>();
            return *this;
        }

        ByteBuffer& operator>>(int32& value)
        {
            value = read<int32>();
            return *this;
        }

        ByteBuffer& operator>>(float& value)
        {
            value = read<float>();
            return *this;
        }

        ByteBuffer& operator>>(double& value)
        {
            value = read<double>();
            return *this;
        }

        ByteBuffer& operator>>(std::string& value)
        {
            value.clear();
            while (rpos() < size())                         // prevent crash at wrong string format in packet
            {
                char c = read<char>();
                if (c == 0)
                    break;
                value += c;
            }
            return *this;
        }

        template<class T>
        ByteBuffer& operator>>(Unused<T> const&)
        {
            read_skip<T>();
            return *this;
        }


        uint8 operator[](size_t pos) const
        {
            return read<uint8>(pos);
        }

        size_t rpos() const { return _rpos; }

        size_t rpos(size_t rpos_)
        {
            _rpos = rpos_;
            return _rpos;
        }

        size_t wpos() const { return _wpos; }

        size_t wpos(size_t wpos_)
        {
            _wpos = wpos_;
            return _wpos;
        }

        template<typename T>
        void read_skip() { read_skip(sizeof(T)); }

        void read_skip(size_t skip)
        {
            if (_rpos + skip > size())
                throw ByteBufferException(false, _rpos, skip, size());
            _rpos += skip;
        }

        template <typename T> T read()
        {
            T r = read<T>(_rpos);
            _rpos += sizeof(T);
            return r;
        }

        template <typename T> T read(size_t pos) const
        {
            if (pos + sizeof(T) > size())
                throw ByteBufferException(false, pos, sizeof(T), size());
            T val = *((T const*)&_storage[pos]);
            EndianConvert(val);
            return val;
        }

        void read(uint8* dest, size_t len)
        {
            if (_rpos  + len > size())
                throw ByteBufferException(false, _rpos, len, size());
            memcpy(dest, &_storage[_rpos], len);
            _rpos += len;
        }

        const uint8* contents() const { return &_storage[0]; }

        size_t size() const { return _storage.size(); }
        bool empty() const { return _storage.empty(); }

        void resize(size_t newsize)
        {
            _storage.resize(newsize);
            _rpos = 0;
            _wpos = size();
        }

        void reserve(size_t ressize)
        {
            if (ressize > size())
                _storage.reserve(ressize);
        }

        void append(const std::string& str)
        {
            append((uint8 const*)str.c_str(), str.size() + 1);
        }

        void append(const char* src, size_t cnt)
        {
            return append((const uint8*)src, cnt);
        }

        template<class T> void append(const T* src, size_t cnt)
        {
            return append((const uint8*)src, cnt * sizeof(T));
        }

        void append(const uint8* src, size_t cnt)
        {
            if (!cnt)
                return;

            if (_storage.size() < _wpos + cnt)
                _storage.resize(_wpos + cnt);
            memcpy(&_storage[_wpos], src, cnt);
            _wpos += cnt;
        }

        void append(const ByteBuffer& buffer)
        {
            if (buffer.wpos())
                append(buffer.contents(), buffer.wpos());
        }

    	void appendData(const char* header, uint32 length){
	        if (_storage.size() < _wpos + length)
                _storage.resize(_wpos + length);
            memcpy(&_storage[_wpos], header, length);
            _wpos += length;
		}

        void put(size_t pos, const uint8* src, size_t cnt)
        {
            if (pos + cnt > size())
                throw ByteBufferException(true, pos, cnt, size());
            memcpy(&_storage[pos], src, cnt);
        }

	private:
        template <typename T> void append(T value)
        {
            EndianConvert(value);
            append((uint8*)&value, sizeof(value));
        }
    protected:
        size_t _rpos, _wpos;
        std::vector<uint8> _storage;
};

#endif