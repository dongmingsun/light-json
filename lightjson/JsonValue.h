#pragma once

#include <exception>
#include "json.h"

namespace lightjson {
class JsonValue {
 public:
  JsonValue() : type_(JsonType::kUndefined) {}

  virtual ~JsonValue() = default;  // Effective C++: term 7

  virtual JsonType type() const = 0;

  virtual size_t size();

  virtual bool ToBool() const;

  virtual double ToNumber() const;

  virtual const std::string &ToString() const;

  virtual Json::j_array ToArray();

  virtual Json::j_object ToObject();

  virtual void push_back(Json);

  virtual void insert(std::pair<std::string, Json>);

  void set_type(JsonType);

 protected:
  JsonType type_;
};

template <typename T>
class Value : public JsonValue {
 public:
  Value() = default;

  explicit Value(T val) : val_(val){};

  JsonType type() const override;

 protected:
  T val_;
};

template <typename T>
JsonType Value<T>::type() const {
  return type_;
}

class JsonNull : public Value<std::nullptr_t> {
 public:
  JsonNull(std::nullptr_t) : Value(nullptr) { set_type(JsonType::kNull); };

  JsonType type() const override;
};

class JsonBool : public Value<bool> {
 public:
  explicit JsonBool(bool val) : Value(val) { set_type(JsonType::kBool); };

  JsonType type() const override;

  bool ToBool() const override { return val_; }
};

class JsonNumber : public Value<double> {
 public:
  explicit JsonNumber(double val) : Value(val) { set_type(JsonType::kNumber); };

  JsonType type() const override;

  double ToNumber() const override;
};

class JsonString : public Value<std::string> {
 public:
  explicit JsonString(std::string val) : Value(val) {
    set_type(JsonType::kString);
  };

  JsonType type() const override;

  const std::string &ToString() const override { return val_; }
};

class JsonArray : public Value<Json::j_array> {
 public:
  JsonArray() { set_type(JsonType::kArray); };

  explicit JsonArray(const Json::j_array &val) : Value(val) {
    set_type(JsonType::kArray);
  };

  JsonType type() const override;

  void push_back(Json) override;

  size_t size() override { return val_.size(); }

  Json::j_array ToArray() override;
};

class JsonObject : public Value<Json::j_object> {
 public:
  JsonObject() { set_type(JsonType::kObject); };

  explicit JsonObject(const Json::j_object &val) : Value(val) {
    set_type(JsonType::kObject);
  };

  JsonType type() const override;

  size_t size() override { return val_.size(); }

  void insert(std::pair<std::string, lightjson::Json>) override;

  Json::j_object ToObject() override;
};

}  // namespace lightjson
