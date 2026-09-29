#include <stdexcept>
#include <string>
#include <vector>

#include "json.h"
#include "parser.h"


std::map<std::string, Value> Json::parse_json(std::string json) {
    Parser parser(json);
    return parser.parse();
}

std::string Json::dumps() {
    std::string json = std::string("");
    json.push_back('{');

    int index = 0;
    for (auto& [key, value] : *this) {
        json.push_back('"');
        json += key;
        json.push_back('"');

        json.push_back(':');

        json += value.dump_value();

        if (index < this->size() - 1) {
            json.push_back(',');
        }

        index++;
    }

    json.push_back('}');
    return json;
}

std::string Value::dump_object() {
    auto obj = std::get<std::map<std::string, Value>>(value);

    std::string json = std::string("");
    json.push_back('{');

    int index = 0;
    for (auto& [key, val] : obj) {
        json.push_back('"');
        json += key;
        json.push_back('"');
        json.push_back(':');

        json += val.dump_value();

        if (index < obj.size() -  1) {
            json.push_back(','); 
        }

        index++;
    }

    json.push_back('}');

    return json;
}

std::string Value::dump_array() {
    std::vector<Value> array = std::get<std::vector<Value>>(value);

    std::string json = std::string("");
    json.push_back('[');

    int index = 0;
    for (Value val : array) {
        json += val.dump_value(); 

        if (index < array.size() - 1) {
            json.push_back(',');
        }

        index++;
    }

    json.push_back(']');

    return json;
}

std::string Value::dump_value() {
    std::string json = std::string("");

    switch (type) {
        case ValueType::String:
            json += "\"" + std::get<std::string>(value) + "\"";
            break;
        case ValueType::Float:
            json += std::to_string(std::get<float>(value));
            break;
        case ValueType::Bool:
            json += std::get<bool>(value) ? "true" : "false";
            break;
        case ValueType::Null:
            json += "null";
            break;
        case ValueType::Array:
            json += dump_array();
            break;
        case ValueType::Object:
            json += dump_object();
            break;
        default:
            throw std::runtime_error("Invalid value type.");
    }

    return json;
}

ValueType Value::get_type() {
    return type;
}
