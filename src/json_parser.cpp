#include "json_parser.h"
#include <cctype>
#include <stdexcept>

JsonValue::JsonValue() : _type(Type::Null), _value(nullptr) {}
JsonValue::JsonValue(bool b) : _type(Type::Bool), _value(b) {}
JsonValue::JsonValue(double n) : _type(Type::Number), _value(n) {}
JsonValue::JsonValue(const std::string& s) : _type(Type::String), _value(s) {}
JsonValue::JsonValue(const JsonObject& obj) : _type(Type::Object), _object(obj) {}
JsonValue::JsonValue(const JsonArray& arr) : _type(Type::Array), _array(arr) {}

JsonValue::Type JsonValue::type() const { return _type; }

std::shared_ptr<JsonValue>& JsonValue::operator[](const std::string& key) {
    if (_type != Type::Object) throw std::runtime_error("Not a JSON object");
    return _object[key];
}

std::shared_ptr<JsonValue>& JsonValue::operator[](size_t index) {
    if (_type != Type::Array) throw std::runtime_error("Not a JSON array");
    return _array.at(index);
}

bool JsonValue::as_bool() const { return std::get<bool>(_value); }
double JsonValue::as_number() const { return std::get<double>(_value); }
const std::string& JsonValue::as_string() const { return std::get<std::string>(_value); }
const JsonObject& JsonValue::as_object() const { return _object; }
 JsonArray& JsonValue::as_array()  { return _array; }

namespace {
    class Parser {
    public:
        Parser(const std::string& t) : text(t), pos(0) {}

        std::shared_ptr<JsonValue> parse_value();

    private:
        std::string text;
        size_t pos;

        void skip_ws();
        char peek();
        char get();
        std::string parse_string();
        double parse_number();
        std::shared_ptr<JsonValue> parse_object();
        std::shared_ptr<JsonValue> parse_array();
    };

    void Parser::skip_ws() {
        while (pos < text.size() && std::isspace(text[pos])) pos++;
    }

    char Parser::peek() {
        skip_ws();
        return text[pos];
    }

    char Parser::get() {
        skip_ws();
        return text[pos++];
    }

    std::string Parser::parse_string() {
        if (get() != '"') throw std::runtime_error("Expected '\"'");
        std::string out;
        while (text[pos] != '"') {
            if (text[pos] == '\\') {
                pos++;
                if (text[pos] == '"') out += '"';
                else if (text[pos] == 'n') out += '\n';
                else if (text[pos] == 't') out += '\t';
                else out += text[pos];
            } else {
                out += text[pos];
            }
            pos++;
        }
        pos++;
        return out;
    }

    double Parser::parse_number() {
        size_t len = 0;
        double val = std::stod(&text[pos], &len);
        pos += len;
        return val;
    }

    std::shared_ptr<JsonValue> Parser::parse_object() {
        get(); // skip '{'
        JsonObject obj;
        skip_ws();
        if (peek() == '}') {
            get();
            return std::make_shared<JsonValue>(obj);
        }
        while (true) {
            std::string key = parse_string();
            if (get() != ':') throw std::runtime_error("Expected ':'");
            auto value = parse_value();
            obj[key] = value;
            char c = get();
            if (c == '}') break;
            if (c != ',') throw std::runtime_error("Expected ',' or '}'");
        }
        return std::make_shared<JsonValue>(obj);
    }

    std::shared_ptr<JsonValue> Parser::parse_array() {
        get(); // skip '['
        JsonArray arr;
        skip_ws();
        if (peek() == ']') {
            get();
            return std::make_shared<JsonValue>(arr);
        }
        while (true) {
            arr.push_back(parse_value());
            char c = get();
            if (c == ']') break;
            if (c != ',') throw std::runtime_error("Expected ',' or ']'");
        }
        return std::make_shared<JsonValue>(arr);
    }

    std::shared_ptr<JsonValue> Parser::parse_value() {
        skip_ws();
        char c = peek();
        if (c == '"') return std::make_shared<JsonValue>(parse_string());
        if (c == '-' || std::isdigit(c)) return std::make_shared<JsonValue>(parse_number());
        if (c == '{') return parse_object();
        if (c == '[') return parse_array();

        if (text.compare(pos, 4, "true") == 0) {
            pos += 4;
            return std::make_shared<JsonValue>(true);
        }
        if (text.compare(pos, 5, "false") == 0) {
            pos += 5;
            return std::make_shared<JsonValue>(false);
        }
        if (text.compare(pos, 4, "null") == 0) {
            pos += 4;
            return std::make_shared<JsonValue>();
        }

        throw std::runtime_error("Invalid JSON");
    }
}

std::shared_ptr<JsonValue> JsonValue::parse(const std::string& text) {
    Parser p(text);
    return p.parse_value();
}
