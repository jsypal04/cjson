#pragma once

#include <cstddef>
#include <map>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

enum class ValueType {
    String,
    Float,
    Bool,
    Null,
    Array,
    Object
};

class Value {
    public:
        Value() {
            value = nullptr; 
        }

        template <typename T>
        Value(T val) {
            value = val;

            if constexpr (std::is_same_v<T, std::string>) {
                type = ValueType::String;
            } else if constexpr (std::is_same_v<T, float>) {
                type = ValueType::Float;
            } else if constexpr (std::is_same_v<T, bool>) {
                type = ValueType::Bool;
            } else if constexpr (std::is_same_v<T, std::nullptr_t>) {
                type = ValueType::Null;
            } else if constexpr (std::is_same_v<T, std::vector<Value>>) {
                type = ValueType::Array;
            } else if constexpr (std::is_same_v<T, std::map<std::string, Value>>) {
                type = ValueType::Object;
            }
        }

        template <typename T>
        T get() {
            return std::get<T>(value);
        }

        template <typename T>
        void insert(std::string key, T value) {
            if (type != ValueType::Object) {
                throw std::runtime_error("Can only insert key value pair into an object.");
            }

            Value val(value);

            auto& obj = std::get<std::map<std::string, Value>>(this->value);
            obj.emplace(key, val);
        }

        template <typename T>
        void insert(T value) {
            if (type != ValueType::Array) {
                throw std::runtime_error("Can only insert values into an array.");
            }

            Value val(value);
            auto& array = std::get<std::vector<Value>>(this->value);
            array.push_back(val);
        }

        ValueType get_type();

        std::string dump_object();
        std::string dump_array();
        std::string dump_value();

    private:
        ValueType type;
        std::variant<
            std::string, 
            float, 
            bool, 
            std::nullptr_t,
            std::vector<Value>, 
            std::map<std::string, Value>
        > value;
};

class Json : public std::map<std::string, Value> {
    private:
        static std::map<std::string, Value> parse_json(std::string json);

    public:
        Json() = default;
        Json(std::string json) : std::map<std::string, Value>(parse_json(json)) {}

        std::string dumps();
};



