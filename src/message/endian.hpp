#pragma once

#include <stdint.h>
#include <cstring>

namespace Utilities
{
enum Endianness
{
    LittleEndian,
    BigEndian
};

class Endian
{
private:
    Endian() {}
    ~Endian() {}

public:
    // Returns instance of singleton
    static Endian &Instance()
    {
        static Endian instance;
        return instance;
    }

    // Returns endianness of the system
    Endianness getSystemEndianness() const
    {
        static uint32_t uint32_t_one = 1U;
        return *reinterpret_cast<uint8_t *>(&uint32_t_one) == 1 ? LittleEndian : BigEndian;
    }

    // Returns true if system is big endian
    bool isSystemBigEndian() const { return getSystemEndianness() == BigEndian; }

    // Returns true if system is little endian
    bool isSystemLittleEndian() const { return getSystemEndianness() == LittleEndian; }

    template <typename T>
    T swapEndianness(const T input) const
    {
        size_t objectSize = sizeof(T);

        union data
        {
            T       object;
            uint8_t bytes[sizeof(T)];
        };

        data in;
        in.object = input;

        data out;
        for(size_t i = 0; i < objectSize; i++)
            out.bytes[i] = in.bytes[objectSize - i - 1];

        return out.object;
    }

    template <typename T>
    T toBigEndian(const T input) const
    {
        if(isSystemBigEndian())
            return input;
        else
            return swapEndianness(input);
    }

    template <typename T>
    T toLittleEndian(const T input) const
    {
        if(isSystemLittleEndian())
            return input;
        else
            return swapEndianness(input);
    }

    template <typename T>
    void writeWithEndianness(const T value, uint8_t *ptr, const Endianness endian) const
    {
        T writeVal = value;
        if(endian != getSystemEndianness())
            writeVal = swapEndianness(writeVal);

        memcpy(ptr, &writeVal, sizeof(writeVal));
    }

    template <typename T>
    void readWithEndianness(T &out, const uint8_t *ptr, const Endianness endian) const
    {
        T readVal = 0;
        memcpy(&readVal, ptr, sizeof(readVal));
        if(endian != getSystemEndianness())
            readVal = swapEndianness(readVal);
        out = readVal;
    }
};
} // namespace Utilities