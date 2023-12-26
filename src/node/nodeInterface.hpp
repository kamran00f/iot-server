#pragma once
#include <string>
#include <vector>

#include "json/single_include/nlohmann/json.hpp"

class InterfaceBase
{
public:
    int         getIndex() const { return index; }
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }

    virtual std::string toJsonString(int indent_level, int tab_size) = 0;

protected:
    InterfaceBase()  = default;
    ~InterfaceBase() = default;

    int         index;
    std::string name;
    std::string description;
};

class Argument : public InterfaceBase
{
public:
    enum DataType
    {
        Integer,
        Double,
        String,
        Byte,
    };

    DataType getDataType() { return dataType; }

    static DataType getDataTypeFromString(const std::string &dataType);
    static DataType getDataTypeFromJson(const nlohmann::json &json_obj);
    static bool     isFixedLengthFromJson(const nlohmann::json &json_obj);
    static bool     isVariableLengthFromJson(const nlohmann::json &json_obj);

protected:
    Argument()  = default;
    ~Argument() = default;

    DataType dataType;
};

class ArgumentFixedLength : public Argument
{
public:
    ArgumentFixedLength() = delete;
    ArgumentFixedLength(const nlohmann::json &json_obj);
    ArgumentFixedLength(int index, const std::string &name, const std::string &description, DataType dataType, int len);
    ArgumentFixedLength(int                index,
                        const std::string &name,
                        const std::string &description,
                        std::string        dataType,
                        int                len);
    ~ArgumentFixedLength() = default;

    int         getLen() const { return len; }
    std::string toJsonString(int indent_level, int tab_size);

private:
    int len;
};

class ArgumentVariableLength : public Argument
{
public:
    ArgumentVariableLength() = delete;
    ArgumentVariableLength(const nlohmann::json &json_obj);
    ArgumentVariableLength(int index, const std::string &name, const std::string &description, DataType dataType);
    ArgumentVariableLength(int index, const std::string &name, const std::string &description, std::string dataType);
    ~ArgumentVariableLength() = default;
    std::string toJsonString(int indent_level, int tab_size);
};

class Field : public InterfaceBase
{
public:
    using Arguments = std::vector<Argument>;

    enum FieldType
    {
        Data,
        Trigger,
        Function,
    };

    FieldType getFieldType() const { return fieldType; }

    static FieldType getFieldTypeFromString(const std::string &dataType);
    static FieldType getFieldTypeFromJson(const nlohmann::json &json_obj);

protected:
    Field()  = default;
    ~Field() = default;

    FieldType fieldType;
};

class DataField : public Field
{
public:
    DataField() = delete;
    DataField(int                index,
              const std::string &name,
              const std::string &description,
              bool               readAllowed,
              bool               writeAllowed,
              const Arguments &  arguments);
    DataField(const nlohmann::json &json_obj);
    ~DataField() = default;

    bool        isReadAllowed() const { return readAllowed; }
    bool        isWriteAllowed() const { return writeAllowed; }
    Arguments   getArguments() const { return arguments; }
    std::string toJsonString(int indent_level, int tab_size);

private:
    bool      readAllowed;
    bool      writeAllowed;
    Arguments arguments;
};

class TriggerField : public Field
{
public:
    TriggerField() = delete;
    TriggerField(int                index,
                 const std::string &name,
                 const std::string &description,
                 bool               input,
                 bool               output,
                 const Arguments &  arguments);
    TriggerField(const nlohmann::json &json_obj);
    ~TriggerField() = default;

    bool        isReadAllowed() const { return input; }
    bool        isWriteAllowed() const { return output; }
    Arguments   getArguments() const { return arguments; }
    std::string toJsonString(int indent_level, int tab_size);

private:
    bool      input;
    bool      output;
    Arguments arguments;
};

class FunctionField : public Field
{
public:
    FunctionField() = delete;
    FunctionField(int                index,
                  const std::string &name,
                  const std::string &description,
                  const Arguments &  inputArguments,
                  const Arguments &  outputArguments);
    FunctionField(const nlohmann::json &json_obj);
    ~FunctionField() = default;

    Arguments   getInputArguments() const { return inputArguments; }
    Arguments   getOutputArguments() const { return outputArguments; }
    std::string toJsonString(int indent_level, int tab_size);

private:
    Arguments inputArguments;
    Arguments outputArguments;
};

class Interface : public InterfaceBase
{
public:
    Interface() = delete;
    Interface(const int                index,
              const std::string &      name,
              const std::string &      description,
              const std::vector<Field> fields);
    Interface(const nlohmann::json &json_obj);
    ~Interface() = default;

    std::vector<Field> getFileds() const { return fields; }
    std::string        toJsonString(int indent_level, int tab_size);

private:
    std::vector<Field> fields;
};

class NodeInterface
{
public:
    NodeInterface() = default;
    NodeInterface(float                  interfaceVersion,
                  std::string            nodeType,
                  std::string            nodeName,
                  std::string            nodeDescription,
                  float                  nodeVersion,
                  std::vector<Interface> interfaces);
    NodeInterface(const nlohmann::json &json_obj);
    NodeInterface(std::string nodeInterfaceJsonString);
    ~NodeInterface() = default;

    float                  getInterfaceVersion() const { return interfaceVersion; }
    std::string            getNodeType() const { return nodeType; }
    std::string            getNodeName() const { return nodeName; }
    std::string            getNodeDescription() const { return nodeDescription; }
    float                  getNodeVersion() const { return nodeVersion; }
    std::vector<Interface> getInterfaces() const { return interfaces; }
    std::string            toJsonString(int tab_size);

private:
    float                  interfaceVersion;
    std::string            nodeType;
    std::string            nodeName;
    std::string            nodeDescription;
    float                  nodeVersion;
    std::vector<Interface> interfaces;
};
