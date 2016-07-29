////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Machine Learning Library (EMLL)
//  File:     Serialization.tcc (utilities)
//  Authors:  Chuck Jacobs
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>

namespace utilities
{
    //
    // Serializer class
    //

    // TODO: split out scalar vs. array here (or is it fundamental vs. ISerializable?)
    template <typename ValueType>
    void Serializer::Serialize(ValueType&& value)
    {
        Serialize("", value);
    }

    template <typename ValueType, IsNotVector<ValueType> concept>
    void Serializer::Serialize(const char* name, ValueType&& value)
    {
        SerializeValue(name, value);
    }

    // template <typename ValueType, IsVector<ValueType> concept>
    // void Serializer::Serialize(const char* name, ValueType&& value)
    // {
    //     std::cout << "Vector" << std::endl;
    //     SerializeValue(name, value);
    //

    template <typename ValueType, IsFundamental<ValueType> concept>
    void Serializer::Serialize(const char* name, const std::vector<ValueType>& array)
    {
        SerializeArrayValue(name, array);
    }

    // Vector of serializable objects
    template <typename ValueType, IsSerializable<ValueType> concept>
    void Serializer::Serialize(const char* name, const std::vector<ValueType>& array)
    {
        std::vector<const utilities::ISerializable*> tmpArray;
        for (const auto& item : array)
        {
            tmpArray.push_back(&item);
        }
        SerializeArrayValue(name, tmpArray);
    }

    // Vector of serializable pointers
    template <typename ValueType, IsSerializable<ValueType> concept>
    void Serializer::Serialize(const char* name, const std::vector<const ValueType*>& array)
    {
        std::vector<const utilities::ISerializable*> tmpArray;
        for (const auto& item : array)
        {
            tmpArray.push_back(item);
        }
        SerializeArrayValue(name, tmpArray);
    }

    //
    // Deserialization
    //

    template <typename ValueType>
    void Serializer::Deserialize(ValueType&& value)
    {
        Deserialize("", value);
    }
}
