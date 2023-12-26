#include <stdexcept>

#include "nodeInterface.hpp"

using nlohmann::json;

// Interface parsing helper functions
void checkFieldExists(const json &json_obj, std::string fieldName)
{
    if(!json_obj.contains(fieldName))
    {
        throw std::runtime_error("checkFieldExists: Field " + fieldName + " does not exist");
    }
}

void checkFieldIsArray(const json &json_obj, std::string fieldName)
{
    if(!json_obj[fieldName].is_array())
    {
        throw std::runtime_error("checkFieldIsArray: Field " + fieldName + " is not an array");
    }
}

void checkFieldIsString(const json &json_obj, std::string fieldName)
{
    if(!json_obj[fieldName].is_string())
    {
        throw std::runtime_error("checkFieldIsString: Field " + fieldName + " is not string");
    }
}

void checkFieldIsInteger(const json &json_obj, std::string fieldName)
{
    if(!json_obj[fieldName].is_number_integer())
    {
        throw std::runtime_error("checkFieldIsInteger: Field " + fieldName + " is not integer");
    }
}

void checkFieldIsFloat(const json &json_obj, std::string fieldName)
{
    if(!json_obj[fieldName].is_number_float())
    {
        throw std::runtime_error("checkFieldIsFloat: Field " + fieldName + " is not float");
    }
}

void checkFieldIsBoolean(const json &json_obj, std::string fieldName)
{
    if(!json_obj[fieldName].is_boolean())
    {
        throw std::runtime_error("checkFieldIsBool: Field " + fieldName + " is not boolean");
    }
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


ArgumentFixedLength::ArgumentFixedLength(int                index,
                                         const std::string &name,
                                         const std::string &description,
                                         DataType           dataType,
                                         int                len)
{
    InterfaceBase::index       = index;
    InterfaceBase::name        = name;
    InterfaceBase::description = description;
    Argument::dataType         = dataType;

    if(len <= 0)
    {
        throw std::runtime_error("ArgumentFixedLength: len in invalid, len = " + std::to_string(len));
    }
    len = len;
}

ArgumentFixedLength::ArgumentFixedLength(int                index,
                                         const std::string &name,
                                         const std::string &description,
                                         std::string        dataType,
                                         int                len) :
    ArgumentFixedLength(index, name, description, getDataTypeFromString(dataType), len)
{
}

ArgumentFixedLength::ArgumentFixedLength(const json &json_obj)
{
    std::string fieldName = "index";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsInteger(json_obj, fieldName);
    index = json_obj.value(fieldName, -1);

    fieldName = "name";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsString(json_obj, fieldName);
    name = json_obj.value(fieldName, "");

    fieldName = "description";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsString(json_obj, fieldName);
    description = json_obj.value(fieldName, "");

    fieldName = "dataType";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsString(json_obj, fieldName);
    std::string dataTypeStr = json_obj.value(fieldName, "");
    dataType                = Argument::getDataTypeFromString(dataTypeStr);

    fieldName = "len";
    if(json_obj.contains(fieldName))
    {
        checkFieldIsInteger(json_obj, fieldName);
    }
    auto len = json_obj.value(fieldName, 1); // use 1 as default value for field
    if(len < 1)
    {
        throw std::runtime_error("ArgumentFixedLength::ArgumentFixedLength: \"len\" value \"" + std::to_string(len) +
                                 "\" is invalid");
    }
}

std::string ArgumentFixedLength::toJsonString(int indent_level, int tab_size)
{
    return "Not implemented";
}


ArgumentVariableLength::ArgumentVariableLength(int                index,
                                               const std::string &name,
                                               const std::string &description,
                                               DataType           dataType)
{
    InterfaceBase::index       = index;
    InterfaceBase::name        = name;
    InterfaceBase::description = description;
    Argument::dataType         = dataType;
}

ArgumentVariableLength::ArgumentVariableLength(int                index,
                                               const std::string &name,
                                               const std::string &description,
                                               std::string        dataType) :
    ArgumentVariableLength(index, name, description, getDataTypeFromString(dataType))
{
}

ArgumentVariableLength::ArgumentVariableLength(const json &json_obj)
{
    std::string fieldName = "index";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsInteger(json_obj, fieldName);
    index = json_obj.value(fieldName, -1);

    fieldName = "name";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsString(json_obj, fieldName);
    name = json_obj.value(fieldName, "");

    fieldName = "description";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsString(json_obj, fieldName);
    description = json_obj.value(fieldName, "");

    fieldName = "dataType";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsString(json_obj, fieldName);
    std::string dataTypeStr = json_obj.value(fieldName, "");
    dataType                = Argument::getDataTypeFromString(dataTypeStr);

    fieldName = "isVariableLength";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsBoolean(json_obj, fieldName);
    bool isVariableLength = json_obj.value(fieldName, false);

    if(isVariableLength == false)
    {
        throw std::runtime_error("ArgumentVariableLength::ArgumentVariableLength: The only valid value for "
                                 "\"variableLen\" field is \"true\"");
    }
}

std::string ArgumentVariableLength::toJsonString(int indent_level, int tab_size)
{
    return "Not implemented";
}


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


DataField::DataField(int                index,
                     const std::string &name,
                     const std::string &description,
                     bool               readAllowed,
                     bool               writeAllowed,
                     const Arguments &  arguments) :
    readAllowed(readAllowed), writeAllowed(writeAllowed), arguments(arguments)
{
    InterfaceBase::index       = index;
    InterfaceBase::name        = name;
    InterfaceBase::description = description;

    if(!(readAllowed || writeAllowed))
    {
        throw std::runtime_error("DataField: readAllowed or/and writeAllowed must be set to true");
    }

    if(arguments.size() == 0)
    {
        throw std::runtime_error("DataField: arguments.size is invalid, arguments.size = " +
                                 std::to_string(arguments.size()));
    }
}

DataField::DataField(const json &json_obj)
{
    fieldType = FieldType::Data;

    std::string fieldName = "index";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsInteger(json_obj, fieldName);
    index = json_obj.value(fieldName, -1);

    fieldName = "name";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsString(json_obj, fieldName);
    name = json_obj.value(fieldName, "");

    fieldName = "description";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsString(json_obj, fieldName);
    description = json_obj.value(fieldName, "");

    fieldName = "readAllowed";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsBoolean(json_obj, fieldName);
    readAllowed = json_obj.value(fieldName, false);

    fieldName = "writeAllowed";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsBoolean(json_obj, fieldName);
    writeAllowed = json_obj.value(fieldName, false);

    fieldName = "arguments";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsArray(json_obj, fieldName);
    auto argumentsJsonObj = json_obj[fieldName];
    for(auto argument_it = argumentsJsonObj.begin(); argument_it != argumentsJsonObj.end(); argument_it++)
    {
        json argJsonObj    = argument_it.value();
        bool isFixedLen    = Argument::isFixedLengthFromJson(argJsonObj);
        bool isVariableLen = Argument::isVariableLengthFromJson(argJsonObj);
        if(isFixedLen == isVariableLen)
        {
            throw std::runtime_error(
                "DataField::getDataField: Argument type should be either fixed length or variable length!");
        }

        int argIndex;
        if(isFixedLen)
        {
            ArgumentFixedLength arg(argJsonObj);
            argIndex = arg.getIndex();
            arguments.push_back(arg);
        }
        else if(isVariableLen)
        {
            ArgumentVariableLength arg(argJsonObj);
            argIndex = arg.getIndex();
            arguments.push_back(arg);
        }

        int expected_index = static_cast<int>(arguments.size());
        if(argIndex != expected_index)
        {
            throw std::runtime_error("DataField::getDataField: Unexpected Argument index " + std::to_string(argIndex) +
                                     ", expected index is " + std::to_string(expected_index));
        }
    }
}

std::string DataField::toJsonString(int indent_level, int tab_size)
{
    return "Not implemented";
}


TriggerField::TriggerField(int                index,
                           const std::string &name,
                           const std::string &description,
                           bool               input,
                           bool               output,
                           const Arguments &  arguments) :
    input(input), output(output), arguments(arguments)
{
    InterfaceBase::index       = index;
    InterfaceBase::name        = name;
    InterfaceBase::description = description;
    Field::fieldType           = FieldType::Trigger;

    if(input == output)
    {
        throw std::runtime_error("TriggerField::TriggerField: Trigger should be either input or output");
    }

    if(arguments.size() == 0)
    {
        throw std::runtime_error("TriggerField: arguments.size is invalid, arguments.size = " +
                                 std::to_string(arguments.size()));
    }
}

TriggerField &TriggerField::getTriggerField(const json &json_obj)
{
    std::string fieldName = "index";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsInteger(json_obj, fieldName);
    int index = json_obj.value(fieldName, -1);

    fieldName = "name";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsString(json_obj, fieldName);
    std::string name = json_obj.value(fieldName, "");

    fieldName = "description";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsString(json_obj, fieldName);
    std::string description = json_obj.value(fieldName, "");

    fieldName = "input";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsBoolean(json_obj, fieldName);
    bool input = json_obj.value(fieldName, false);

    fieldName = "output";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsBoolean(json_obj, fieldName);
    bool output = json_obj.value(fieldName, false);

    fieldName = "arguments";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsArray(json_obj, fieldName);
    auto                  argumentsJsonObj = json_obj[fieldName];
    std::vector<Argument> argumentsVec;
    for(auto argument_it = argumentsJsonObj.begin(); argument_it != argumentsJsonObj.end(); argument_it++)
    {
        Argument &arg            = Argument::getArgument(argument_it.value());
        int       expected_index = static_cast<int>(argumentsVec.size());
        if(arg.getIndex() != expected_index)
        {
            throw std::runtime_error("TriggerField::getTriggerField: Unexpected Argument index " +
                                     std::to_string(arg.getIndex()) + ", expected index is " +
                                     std::to_string(expected_index));
        }
        argumentsVec.push_back(arg);
    }

    TriggerField triggerField = TriggerField(index, name, description, input, output, argumentsVec);
    return triggerField;
}

std::string TriggerField::toJsonString(int indent_level, int tab_size)
{
    return "Not implemented";
}


FunctionField::FunctionField(int                index,
                             const std::string &name,
                             const std::string &description,
                             const Arguments &  inputArguments,
                             const Arguments &  outputArguments) :
    Field(index, name, description, FieldType::Function),
    inputArguments(inputArguments),
    outputArguments(outputArguments)
{
}

FunctionField::FunctionField(const json &json_obj)
{
    std::string fieldName = "index";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsInteger(json_obj, fieldName);
    int index = json_obj.value(fieldName, -1);

    fieldName = "name";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsString(json_obj, fieldName);
    std::string name = json_obj.value(fieldName, "");

    fieldName = "description";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsString(json_obj, fieldName);
    std::string description = json_obj.value(fieldName, "");

    fieldName = "inputArguments";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsArray(json_obj, fieldName);
    auto                  inputArgumentsJsonObj = json_obj[fieldName];
    std::vector<Argument> inputArgumentsVec;
    for(auto input_argument_it = inputArgumentsJsonObj.begin(); input_argument_it != inputArgumentsJsonObj.end();
        input_argument_it++)
    {
        Argument &arg            = Argument::getArgument(input_argument_it.value());
        int       expected_index = static_cast<int>(inputArgumentsVec.size());
        if(arg.getIndex() != expected_index)
        {
            throw std::runtime_error("FunctionField::getFunctionField: Unexpected Argument index " +
                                     std::to_string(arg.getIndex()) + ", expected index is " +
                                     std::to_string(expected_index));
        }
        inputArgumentsVec.push_back(arg);
    }

    fieldName = "outputArguments";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsArray(json_obj, fieldName);
    auto                  outputArgumentsJsonObj = json_obj[fieldName];
    std::vector<Argument> outputArgumentsVec;
    for(auto output_argument_it = outputArgumentsJsonObj.begin(); output_argument_it != outputArgumentsJsonObj.end();
        output_argument_it++)
    {
        Argument &arg            = Argument::getArgument(output_argument_it.value());
        int       expected_index = static_cast<int>(outputArgumentsVec.size());
        if(arg.getIndex() != expected_index)
        {
            throw std::runtime_error("Interface::Interface: Unexpected Argument index " +
                                     std::to_string(arg.getIndex()) + ", expected index is " +
                                     std::to_string(expected_index));
        }
        outputArgumentsVec.push_back(arg);
    }

    FunctionField functionField = FunctionField(index, name, description, inputArgumentsVec, outputArgumentsVec);
    return functionField;
}

std::string FunctionField::toJsonString(int indent_level, int tab_size)
{
    return "Not implemented";
}


Interface::Interface(const int                index,
                     const std::string &      name,
                     const std::string &      description,
                     const std::vector<Field> fields) :
    InterfaceBase(index, name, description), fields(fields)
{
}

Interface::Interface(const json &json_obj)
{
    std::string fieldName = "index";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsInteger(json_obj, fieldName);
    int index = json_obj.value(fieldName, -1);

    fieldName = "name";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsString(json_obj, fieldName);
    std::string name = json_obj.value(fieldName, "");

    fieldName = "description";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsString(json_obj, fieldName);
    std::string description = json_obj.value(fieldName, "");

    fieldName = "fields";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsArray(json_obj, fieldName);
    auto FieldsJsonObj = json_obj[fieldName];

    std::vector<Field> fieldsVec;
    for(auto field_it = FieldsJsonObj.begin(); field_it != FieldsJsonObj.end(); field_it++)
    {
        Field &field          = Field::getField(field_it.value());
        int    expected_index = static_cast<int>(fieldsVec.size());
        if(field.getIndex() != expected_index)
        {
            throw std::runtime_error("Interface::getInterface: Unexpected Field index " +
                                     std::to_string(field.getIndex()) + ", expected index is " +
                                     std::to_string(expected_index));
        }

        fieldsVec.push_back(field);
    }

    Interface interface = Interface(index, name, description, fieldsVec);
    return interface;
}

std::string Interface::toJsonString(int index_level, int tab_size)
{
    return "Not implemented";
}


NodeInterface::NodeInterface(float                  interfaceVersion,
                             std::string            nodeType,
                             std::string            nodeName,
                             std::string            nodeDescription,
                             float                  nodeVersion,
                             std::vector<Interface> interfaces) :
    interfaceVersion(interfaceVersion),
    nodeType(nodeType),
    nodeName(nodeName),
    nodeDescription(nodeDescription),
    nodeVersion(nodeVersion),
    interfaces(interfaces)
{
}

NodeInterface::NodeInterface(const json &json_obj)
{
    std::string fieldName = "interfaceVersion";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsFloat(json_obj, fieldName);
    auto interfaceVersion = json_obj.value(fieldName, 0.0);

    fieldName = "nodeType";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsString(json_obj, fieldName);
    auto nodeType = json_obj.value(fieldName, "");

    fieldName = "nodeName";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsString(json_obj, fieldName);
    auto nodeName = json_obj.value(fieldName, "");

    fieldName = "nodeDescription";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsString(json_obj, fieldName);
    auto nodeDescription = json_obj.value(fieldName, "");

    fieldName = "nodeVersion";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsFloat(json_obj, fieldName);
    auto nodeVersion = json_obj.value(fieldName, 0.0);

    fieldName = "interfaces";
    checkFieldExists(json_obj, fieldName);
    checkFieldIsArray(json_obj, fieldName);
    auto                   interfacesJsonObj = json_obj[fieldName];
    std::vector<Interface> interfaceVect;
    for(auto interface_it = interfacesJsonObj.begin(); interface_it != interfacesJsonObj.end(); interface_it++)
    {
        Interface interface      = Interface::getInterface(interface_it.value());
        int       expected_index = static_cast<int>(interfaceVect.size());
        if(interface.getIndex() != expected_index)
        {
            throw std::runtime_error("NodeInterface::NodeInterface: Unexpected Interface index " +
                                     std::to_string(interface.getIndex()) + ", expected index is " +
                                     std::to_string(expected_index));
        }
        interfaceVect.push_back(interface);
    }

    NodeInterface(interfaceVersion, nodeType, nodeName, nodeDescription, nodeVersion, interfaceVect);
}

NodeInterface::NodeInterface(std::string nodeInterfaceJsonString)
{
    const json &json_obj = json::parse(nodeInterfaceJsonString);
    NodeInterface(json_obj);
}

std::string toJsonString(int tab_size)
{
    return "Not implemented";
}
