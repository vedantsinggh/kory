#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <memory>

class JsonValue;

using JsonObject = std::unordered_map<std::string, std::shared_ptr<JsonValue>>;
using JsonArray = std::vector<std::shared_ptr<JsonValue>>;
using JsonPrimitive = std::variant<std::nullptr_t, bool, double, std::string>;

class JsonValue {
public:
    enum class Type { Null, Bool, Number, String, Object, Array };

    JsonValue();
    JsonValue(bool);
    JsonValue(double);
    JsonValue(const std::string&);
    JsonValue(const JsonObject&);
    JsonValue(const JsonArray&);

    Type type() const;

    std::shared_ptr<JsonValue>& operator[](const std::string&);
    std::shared_ptr<JsonValue>& operator[](size_t);

    bool as_bool() const;
    double as_number() const;
    const std::string& as_string() const;
    const JsonObject& as_object() const;
    const JsonArray& as_array() const;

    static std::shared_ptr<JsonValue> parse(const std::string& text);

private:
    Type _type;
    JsonPrimitive _value;
    JsonObject _object;
    JsonArray _array;
};
