#include <stdexcept>

#include "nodeInterface.hpp"

Field::FieldType Field::getFieldTypeFromString(const std::string &fieldType)
{
    if(fieldType == "data")
        return FieldType::Data;
    else if(fieldType == "trigger")
        return FieldType::Trigger;
    else if(fieldType == "function")
        return FieldType::Function;
    else
        throw std::runtime_error("Field::getFieldTypeFromString: Unknown field type string: " + fieldType);
}

Argument::DataType Argument::getDataTypeFromString(const std::string &dataType)
{
    if(dataType == "integer")
        return DataType::Integer;
    else if(dataType == "double")
        return DataType::Double;
    else if(dataType == "string")
        return DataType::String;
    else if(dataType == "byte")
        return DataType::Byte;
    else
        throw std::runtime_error("Field::getDataTypeFromString: Unknown data type string: " + dataType);
}