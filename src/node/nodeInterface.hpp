#pragma once
#include <string>
#include <vector>

class Base
{
public:
    Base() = delete;

    int         getIndex() const { return index; }
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }

protected:
    Base(int index, const std::string &name, const std::string &description) :
        index(index), name(name), description(description)
    {
    }
    ~Base() = default;

private:
    int         index;
    std::string name;
    std::string description;
};

class Argument : public Base
{
public:
    enum DataType
    {
        Integer,
        Double,
        String,
        Byte,
    };

    Argument() = delete;
    Argument(int index, const std::string &name, const std::string &description, DataType dataType) :
        Base(index, name, description), dataType(dataType)
    {
    }
    ~Argument() = default;

    static DataType getDataTypeFromString(const std::string &dataType);

private:
    DataType dataType;
};

class ArgumentFixedLength : public Argument
{
public:
    ArgumentFixedLength() = delete;
    ArgumentFixedLength(int                index,
                        const std::string &name,
                        const std::string &description,
                        DataType           dataType,
                        int                len) :
        Argument(index, name, description, dataType), len(len)
    {
        if(len <= 0)
        {
            throw std::runtime_error("ArgumentFixedLength: len in invalid, len = " + std::to_string(len));
        }
    }
    ~ArgumentFixedLength() = default;

    int getLen() const { return len; }

private:
    int len;
};

class ArgumentVariablesLength : public Argument
{
public:
    ArgumentVariablesLength() = delete;
    ArgumentVariablesLength(int index, const std::string &name, const std::string &description, DataType dataType) :
        Argument(index, name, description, dataType)
    {
    }
    ~ArgumentVariablesLength() = default;
};

class Field : public Base
{
public:
    using Arguments = std::vector<Argument>;

    enum FieldType
    {
        Data,
        Trigger,
        Function,
    };

    Field()  = delete;
    ~Field() = default;

    FieldType getFieldType() const { return fieldType; }

    static FieldType getFieldTypeFromString(const std::string &dataType);

protected:
    Field(int index, const std::string &name, const std::string &description, FieldType fieldType) :
        Base(index, name, description), fieldType(fieldType)
    {
    }

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
              const Arguments &  arguments) :
        Field(index, name, description, FieldType::Data),
        readAllowed(readAllowed),
        writeAllowed(writeAllowed),
        arguments(arguments)
    {
        if(!(readAllowed || writeAllowed))
        {
            throw std::runtime_error("DataField: readAllowed or/and writeAllowed must be set to true");
        }

        if(arguments.size() <= 0)
        {
            throw std::runtime_error("DataField: arguments.size is invalid, arguments.size = " +
                                     std::to_string(arguments.size()));
        }
    }
    ~DataField() = default;

    bool      isReadAllowed() const { return readAllowed; }
    bool      isWriteAllowed() const { return writeAllowed; }
    Arguments getArguments() const { return arguments; }

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
                 bool               readAllowed,
                 bool               writeAllowed,
                 const Arguments &  arguments) :
        Field(index, name, description, FieldType::Data),
        readAllowed(readAllowed),
        writeAllowed(writeAllowed),
        arguments(arguments)
    {
        if(arguments.size() <= 0)
        {
            throw std::runtime_error("TriggerField: arguments.size is invalid, arguments.size = " +
                                     std::to_string(arguments.size()));
        }
        if(readAllowed && writeAllowed)
        {
            throw std::runtime_error("TriggerField: Trigger cannot be both readable and writable");
        }
        if(!(readAllowed || writeAllowed))
        {
            throw std::runtime_error("TriggerField: Trigger cannot be neither readable nor writable");
        }
    }
    ~TriggerField() = default;

    bool      isReadAllowed() const { return readAllowed; }
    bool      isWriteAllowed() const { return writeAllowed; }
    Arguments getArguments() const { return arguments; }

private:
    bool      readAllowed;
    bool      writeAllowed;
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
                  const Arguments &  outputArguments) :
        Field(index, name, description, FieldType::Data),
        inputArguments(inputArguments),
        outputArguments(outputArguments)
    {
    }
    ~FunctionField() = default;

    Arguments getInputArguments() const { return inputArguments; }
    Arguments getOutputArguments() const { return outputArguments; }

private:
    Arguments inputArguments;
    Arguments outputArguments;
};

class Interface : public Base
{
public:
    Interface() = delete;
    Interface(const int                 index,
              const std::string &       name,
              const std::string &       description,
              const std::vector<Field> &fields) :
        Base(index, name, description), fields(fields)
    {
    }
    ~Interface() = default;
    std::vector<Field> getFileds() const { return fields; }

private:
    std::vector<Field> fields;
};

class NodeInterface
{
public:
    NodeInterface()  = default;
    ~NodeInterface() = default;

    NodeInterface(float                  interfaceVersion,
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

    float                  getInterfaceVersion() const { return interfaceVersion; }
    std::string            getNodeType() const { return nodeType; }
    std::string            getNodeName() const { return nodeName; }
    std::string            getNodeDescription() const { return nodeDescription; }
    float                  getNodeVersion() const { return nodeVersion; }
    std::vector<Interface> getInterfaces() const { return interfaces; }

private:
    float                  interfaceVersion;
    std::string            nodeType;
    std::string            nodeName;
    std::string            nodeDescription;
    float                  nodeVersion;
    std::vector<Interface> interfaces;
};
