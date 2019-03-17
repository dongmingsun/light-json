#pragma once

#include <exception>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace lightjson {
enum class JsonType : int {
  kUndefined,
  kNull,
  kBool,
  kNumber,
  kString,
  kArray,
  kObject
};

std::string Jsontype2String(JsonType type);

class JsonValue;

class Json {
 public:
  using j_array = std::vector<Json>;
  using j_object = std::unordered_map<std::string, Json>;

  Json();

  Json(const Json &);

  ~Json();

  // Check the type of the object
  JsonType type() const;

  // Obtain the size of arrays or objects
  size_t size();

  // Core function, parse a Json string
  void Parse(std::string);

  // TODO Check if the object is of a specific type

  bool IsNull() const;

  bool IsBool() const;

  bool IsNumber() const;

  bool IsString() const;

  bool IsArray() const;

  bool IsObject() const;

  // Convert the object to the underneath json value
  bool ToBool() const;

  double ToNumber() const;

  const std::string &ToString() const;

  const j_array ToArray() const;

  const j_object ToObject() const;

  // TODO overload operator[] for arrays and objects

 private:
  std::string feedin_string;

  std::unique_ptr<JsonValue> value_;

  // ParseXXX: auxiliary functions to process the string
  void ParseWhitespace();

  void ParseValue();

  void ParseLiteral(const std::string &);

  void ParseNumber();

  void ParseString();

  void ParseArray();

  void ParseObject();

  std::string ParseKey();
};
}  // namespace lightjson
