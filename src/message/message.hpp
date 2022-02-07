#pragma once

#include <cstdint>
#include <vector>

#include "endian.hpp"

class Message
{
    /* Message format:
     *
     * | Field          | Size    | Type     |
     * |----------------|---------|----------|
     * | Message Len    | 4 bytes | uint32_t |
     * | Source ID      | 4 bytes | uint32_t |
     * | Destination ID | 4 bytes | uint32_t |
     * | Payload        | n bytes | raw data |
     * | CRC            | 4 bytes | uint32_t |
     *
     * Bytes are little endian
     */

private:
    static constexpr uint32_t messageLenIndex    = 0;
    static constexpr size_t   messageLenBytesNum = 4;

    static constexpr uint32_t messageSourceIdIndex    = 4;
    static constexpr size_t   messageSourceIdBytesNum = 4;

    static constexpr uint32_t messageDestinationIdIndex    = 8;
    static constexpr size_t   messageDestinationIdBytesNum = 4;

    static constexpr uint32_t messagePayloadIndex = 12;
    static constexpr size_t   messageCrcBytesNum  = 4;

    bool                 isValid;
    std::vector<uint8_t> data;

    // Reset message data
    void reset();

    // Read a uint32_t from specified byte index in message raw data considering endianness
    uint32_t readUint32(const uint32_t index) const;

public:
    static constexpr size_t                overheadLen       = messagePayloadIndex + messageCrcBytesNum;
    static constexpr size_t                maxPayloadLen     = 1024 * 32; // Max 32KB payload size
    static constexpr size_t                maxMessageLen     = maxPayloadLen + overheadLen;
    static constexpr Utilities::Endianness messageEndianness = Utilities::Endianness::LittleEndian;

    // Copy constructor
    Message(const Message &msg)
    {
        isValid = msg.isValid;
        data    = msg.data;
    }

    // Constructor with encode
    Message(const uint32_t sourceId, const uint32_t destinationId, const uint8_t *payload, const size_t payload_len);

    // Constructor with decide
    Message(const uint8_t *bytes, const size_t bytesLen);

    // Encode payload data to message format to send
    void
        encode(const uint32_t sourceId, const uint32_t destinationId, const uint8_t *payload, const size_t payload_len);

    // Decode received raw data to message format
    void decode(const uint8_t *bytes, const size_t bytesLen);

    // Get source ID of the message
    uint32_t getSourceId() const;

    // Get destination ID of the message
    uint32_t getDestinationId() const;

    // Get the length of the payload
    size_t getPayloadLen() const;

    // Get pointer to payload
    const uint8_t *getPayloadPointer() const;

    // Get the total length of raw message frame
    size_t getMessageLen() const;

    // Get pointer to raw message frame
    const uint8_t *getMessagePointer() const;
};
