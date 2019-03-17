#include "JsonValue.h"

namespace lightjson {
void JsonValue::push_back(Json) {
  throw std::runtime_error("ONLY JSONARRAY SUPPORTS PUSH_BACK");
}

void JsonValue::insert(std::pair<std::string, lightjson::Json>) {
  throw std::runtime_error("ONLY JSONOBJECT SUPPORTS INSERT");
}

void JsonObject::insert(std::pair<std::string, lightjson::Json> key_val_pair) {
  val_.insert(key_val_pair);
}

void JsonArray::push_back(Json json) { val_.push_back(json); }

void JsonValue::set_type(JsonType type) { type_ = type; }

JsonType JsonValue::type() const { return type_; }

size_t JsonValue::size() {
  std::string msg("size() is not compatible with type ");
  msg += Jsontype2String(type_);
  throw std::runtime_error(msg);
}

bool JsonValue::ToBool() const {
  std::string msg(Jsontype2String(type_));
  msg += " is not bool";
  throw std::runtime_error(msg);
}

double JsonValue::ToNumber() const {
  std::string msg(Jsontype2String(type_));
  msg += " is not number";
  throw std::runtime_error(msg);
}

const std::string &JsonValue::ToString() const {
  std::string msg(Jsontype2String(type_));
  msg += " is not string";
  throw std::runtime_error(msg);
}

Json::j_array JsonValue::ToArray() {
  std::string msg(Jsontype2String(type_));
  msg += " is not array";
  throw std::runtime_error(msg);
}

Json::j_array JsonArray::ToArray() { return val_; }

Json::j_object JsonValue::ToObject() {
  std::string msg(Jsontype2String(type_));
  msg += " is not object";
  throw std::runtime_error(msg);
}

Json::j_object JsonObject::ToObject() { return val_; }

JsonType JsonNull::type() const { return JsonType::kNull; }

JsonType JsonBool::type() const { return JsonType::kBool; }

JsonType JsonNumber::type() const { return JsonType::kNumber; }

JsonType JsonString::type() const { return JsonType::kString; }

JsonType JsonArray::type() const { return JsonType::kArray; }

JsonType JsonObject::type() const { return JsonType::kObject; }

double JsonNumber::ToNumber() const {
  { return val_; }
}

}  // namespace lightjson
