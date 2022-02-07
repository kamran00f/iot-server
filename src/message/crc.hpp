#pragma once

#include <cstdint>

namespace Utilities
{
template <typename T>
class CRC
{
private:
    static constexpr uint32_t lookup_table_elements_count = 256;
    static constexpr uint32_t WIDTH                       = sizeof(T) * 8;
    static constexpr uint32_t MSB_BIT_INDEX               = WIDTH - 8;

    T    _polynomial;
    T    _initial_value;
    T    _final_xor_value;
    bool _reflect_input;
    bool _reflect_output;

    T _table[lookup_table_elements_count];

    uint8_t reflect_byte(const uint8_t byte) const
    {
        uint8_t reflected = 0;
        for(uint8_t i = 0; i < 8; i++)
        {
            if((byte & (1 << i)) != 0)
            {
                reflected |= (uint8_t)(1 << (7 - i));
            }
        }

        return reflected;
    }

    void reflect(T &crc) const
    {
        T reflected = 0;
        for(uint32_t i = 0; i < WIDTH; i++)
        {
            if((crc & (1 << i)) != 0)
            {
                reflected |= 1 << (WIDTH - 1 - i);
            }
        }
        crc = reflected;
    }

    void calculate_table()
    {
        for(uint32_t divident = 0; divident < lookup_table_elements_count; divident++)
        {
            T curByte = (T)(divident << MSB_BIT_INDEX); /* move divident byte into MSB of T CRC */
            for(uint8_t bit = 0; bit < 8; bit++)
            {
                uint64_t last_bit_mask = ((uint64_t)1) << (WIDTH - 1);
                if((curByte & last_bit_mask) != 0)
                {
                    curByte <<= 1;
                    curByte ^= _polynomial;
                }
                else
                {
                    curByte <<= 1;
                }
            }
            _table[divident] = curByte;
        }
    }

public:
    CRC(const T    polynomial,
        const T    initial_value,
        const T    final_xor_value,
        const bool reflect_input,
        const bool reflect_output)
    {
        _polynomial      = polynomial;
        _initial_value   = initial_value;
        _final_xor_value = final_xor_value;
        _reflect_input   = reflect_input;
        _reflect_output  = reflect_output;

        calculate_table();
    }

    // Initiates a CRC calculation chain
    T init() const { return _initial_value; }

    // Updates given CRC calculation chain
    void update(T &crc, const uint8_t *data, const uint32_t data_len) const
    {
        for(uint32_t i = 0; i < data_len; i++)
        {
            uint8_t byte = data[i];
            if(_reflect_input)
            {
                byte = reflect_byte(byte);
            }

            /* calculate position in table */
            uint8_t pos = uint8_t((crc ^ (byte << MSB_BIT_INDEX)) >> MSB_BIT_INDEX);
            crc         = (T)((crc << 8) ^ (T)(_table[pos]));
        }
    }

    // Finished CRC calculation chain
    void finish(T &crc) const
    {
        if(_reflect_output)
        {
            reflect(crc);
        }
        crc ^= _final_xor_value;
    }

    // Calculate CRC from bytes
    T calculate(const uint8_t *data, const uint32_t data_len) const
    {
        T crc = init();
        update(crc, data, data_len);
        finish(crc);
        return crc;
    }
};

// A CRC32 calculator instance with default configurations
static CRC<uint32_t> crc32_instance(0x4C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, true, true);

} // namespace Utilities