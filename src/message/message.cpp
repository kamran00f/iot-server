#include <cstring>
#include <stdexcept>

#include "message.hpp"
#include "crc.hpp"

Message::Message(const uint32_t sourceId,
                 const uint32_t destinationId,
                 const uint8_t *payload,
                 const size_t   payload_len)
{
    encode(sourceId, destinationId, payload, payload_len);
}

Message::Message(const uint8_t *bytes, const size_t bytesLen)
{
    decode(bytes, bytesLen);
}

void Message::reset()
{
    data.clear();
    isValid = false;
}

uint32_t Message::readUint32(const uint32_t index) const
{
    uint32_t retVal = 0;
    if(index + sizeof(retVal) > data.size())
    {
        // Invalid index, data.size()
        std::string excmsg = "Invalid index in Message::readUint32 -> index=" + std::to_string(index) +
                             ", data.size()=" + std::to_string(data.size());
        throw std::runtime_error(excmsg);
    }
    Utilities::Endian::Instance().readWithEndianness(retVal, data.data() + index, messageEndianness);
    return retVal;
}

void Message::encode(const uint32_t sourceId,
                     const uint32_t destinationId,
                     const uint8_t *payload,
                     const size_t   payload_len)
{
    auto writeFieldRaw = [&](const uint32_t index, const uint8_t *input_ptr, const size_t len) {
        if(index + len > data.size())
        {
            // Invalid index, len, data.size() combination
            std::string excmsg = "Invalid arguments in writeFieldRaw -> index=" + std::to_string(index) +
                                 ", len=" + std::to_string(len) + ", data.size()=" + std::to_string(data.size());
            throw std::runtime_error(excmsg);
        }

        memcpy(data.data() + index, input_ptr, len);
    };

    auto writeUint32 = [&](const uint32_t index, const uint32_t val) {
        if(index + sizeof(val) > data.size())
        {
            // Invalid index, data.size()
            std::string excmsg = "Invalid index in writeUint32 -> index=" + std::to_string(index) +
                                 ", data.size()=" + std::to_string(data.size());
            throw std::runtime_error(excmsg);
        }
        Utilities::Endian::Instance().writeWithEndianness(val, data.data() + index, messageEndianness);
    };

    reset();
    size_t messageLen = payload_len + overheadLen;
    data.resize(messageLen);
    writeUint32(messageLenIndex, messageLen);
    writeUint32(messageSourceIdIndex, sourceId);
    writeUint32(messageDestinationIdIndex, destinationId);

    if(payload_len > 0)
    {
        writeFieldRaw(messagePayloadIndex, payload, payload_len);
    }

    uint32_t crc32 = Utilities::crc32_instance.calculate(data.data(), messageLen - messageCrcBytesNum);
    writeUint32(messageLen - messageCrcBytesNum, crc32);
    isValid = true;
}

void Message::decode(const uint8_t *bytes, const size_t bytesLen)
{
    if(bytesLen < overheadLen || bytesLen > maxMessageLen)
    {
        isValid = false;
        throw std::runtime_error("Invalid bytesLen in Message::decode");
    }

    reset();
    data.resize(bytesLen);
    memcpy(data.data(), bytes, bytesLen);

    uint32_t receivedCrc   = readUint32(bytesLen - messageCrcBytesNum);
    uint32_t calculatedCrc = Utilities::crc32_instance.calculate(bytes, bytesLen - messageCrcBytesNum);
    if(calculatedCrc != receivedCrc)
    {
        isValid = false;
        throw std::runtime_error("Invalid CRC in Message::decode");
    }

    isValid = true;
}

uint32_t Message::getSourceId() const
{
    if(!isValid)
    {
        // Invalid message
        throw std::runtime_error("getSourceId() called for invalid message");
    }

    return readUint32(messageSourceIdIndex);
}

uint32_t Message::getDestinationId() const
{
    if(!isValid)
    {
        // Invalid message
        throw std::runtime_error("getDestinationId() called for invalid message");
    }

    return readUint32(messageDestinationIdIndex);
}

size_t Message::getPayloadLen() const
{
    if(!isValid)
    {
        // Invalid message
        throw std::runtime_error("getPayloadLen() called for invalid message");
    }

    return data.size() - overheadLen;
}

const uint8_t *Message::getPayloadPointer() const
{
    if(!isValid || data.size() <= overheadLen)
        return nullptr;
    else
        return data.data() + messagePayloadIndex;
}

size_t Message::getMessageLen() const
{
    if(!isValid)
        return 0;
    else
        return data.size();
}

const uint8_t *Message::getMessagePointer() const
{
    if(!isValid)
        return nullptr;
    else
        return data.data();
}