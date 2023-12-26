
# Node interface (version 0.0)

Node interface is the highest level of the interface structure to interact with a node. Node interface consists of the following fields:
* interfaceVersion : (float)
* nodeType : (string)
* nodeName : (string)
* nodeDescription : (string)
* nodeVerson : (float)
* interfaces : (interface array)

## interfaceVersion : float
A float number indicating the supported version of node interface. The integer part of this number specifies major version number and fraction part specifies minor version. Major versions are not necessarily backward compatible, but minor versions are backward compatible.

## nodeType: string
Type of the node. An arbitrary string that can be used by to query nodes of the same type

## nodeName : string
Name of the node that is shown to the user, can be used to distinguish nodes in user friendly manner.

## nodeDescription : string
Short description about the node.

## nodeVersion:
Version of the node. An arbitrary number to be able to trace the version of a node.

## interfaces : interface array
Array of interfaces that the node provides. Must be defined as an array, even if the node supports only one interface.

<br>

---

<br>

# interface

Each interface defines a number of fields that can be used to interact with a node. Each node can define any number of interfaces, each for one aspect of its functionality. For example, a node that is connected to a motion detector, a light switch and a camera, can provide 3 interfaces. One for each connected sensor/actuator to separate different aspects of its functionality. This way, the motion detector interface can be used by another node that opens a door, without any knowledge of the camera and light switch that are also connected to that node. Interface consists of the following fields:
* index : integer
* name : string
* description : string
* interfaceFields : interfaceField array

## index : integer
Index of the interface. This number is zero based and should be incremented by one per interface. This index is used for selecting the interface by other nodes.

## name : string
Name of the interface. This name will be shown in the user interface. A descriptive name helps the user to realise what is the functionality of the node and what actions are supported.

## description : string
Short description of the interface.

## fields : field array
An array of fields in the current interface. Must be defined as an array, even if the interface has only one field.

<br>

---

<br>

# fields
Each field can be used to interact with the node. All fields have index, name and description and a type. Type defines the field type which can be one of the following:
* data
* trigger
* function

The rest of the items inside field, depend on the field type.

<br>

## data field
A data field has the following items in addition to standard field items:
* readAllowed (Boolean)
* writeAllowed (Boolean)
* arguments (argument array)

For each field, readAllowed or writeAllowed or both must be set to true. Arguments is an array of arguments which is discussed later. A data field must have more than 0 arguments.

## trigger field
A trigger field has the following items in addition to standard field items:
* input (Boolean)
* output (Boolean)
* arguments (argument array)
For each field, either input or output must be set to true (not both at the same time). The arguments array is optional.

## function field
A function field is similar to a function call in programming languages. In addition to standard field items, function fields have an inputArguments and outputArguments fields which are both optional. When a function field is invoked, input arguments are provided by the caller of the function, and in response, the node provides outputArguments to the caller.

## argument
Argument item is used by all data fields. In addition to standard index, name and description fields, it contains the following fields:
* dataType (string) : one of the following:
  * "Integer"
  * "Double"
  * "string"
  * "Byte"
* len (Integer) : optional, must be > 0, mutually exclusive with variableLength
* variableLength (Boolean) : optional, mutually exclusive with len)

If an argument does not specify "len" and "variableLength" fields, a fixed length of 1 for that argument is used.
The only valid value for variableLength is true.

<br>

---

<br>

# Interface overview
```
Node interface
{
    interfaceVersion (float)
    nodeType (string)
    nodeName (string)
    nodeDescription (string)
    nodeVersion (float)
    interfaces (array)
    [
        index, name, description
        fields (array)
        [
            index, name, description
            fieldType: "data", "trigger" or "function"
            { type == data:
                readAllowed
                writeAllowed
                arguments (array)
                [
                    index, name, description
                    dataType: "Integer", "Double", "string" or "Byte"
                    {
                        nothing (fixed len = 1) or
                        len = integer > 0 or
                        variableLength = true
                    }
                ]
            },
            { type == trigger:
                input
                output
                arguments (array)
                [
                    Same as data field
                ]
            },
            { type == function:
                input arguments (array)
                [
                    Same as data field
                ]
                output arguments (array)
                [
                    Same as data field
                ]
            },
        ]
    ]
}
```