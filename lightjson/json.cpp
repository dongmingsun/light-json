#include "json.h"
#include "JsonValue.h"

namespace lightjson {
JsonType Json::type() const { return value_->type(); }

size_t Json::size() { return value_->size(); }

Json::Json() : value_(nullptr) {}

Json::~Json() = default;

bool Json::ToBool() const { return value_->ToBool(); }

double Json::ToNumber() const { return value_->ToNumber(); }

const std::string &Json::ToString() const { return value_->ToString(); }

const Json::j_array Json::ToArray() const { return value_->ToArray(); }

const Json::j_object Json::ToObject() const { return value_->ToObject(); }

Json::Json(const lightjson::Json &rhs) {
  switch (rhs.type()) {
    case JsonType::kUndefined:
      value_ = nullptr;
    case JsonType::kNull:
      value_ = std::make_unique<JsonNull>(nullptr);
      break;
    case JsonType::kBool:
      value_ = std::make_unique<JsonBool>(rhs.ToBool());
      break;
    case JsonType::kNumber:
      value_ = std::make_unique<JsonNumber>(rhs.ToNumber());
      break;
    case JsonType::kString:
      value_ = std::make_unique<JsonString>(rhs.ToString());
      break;
    case JsonType::kArray:
      value_ = std::make_unique<JsonArray>(rhs.ToArray());
      break;
    case JsonType::kObject:
      value_ = std::make_unique<JsonObject>(rhs.ToObject());
      break;
  }
}

void Json::ParseWhitespace() {
  size_t position(0);
  while (feedin_string[position] == ' ' || feedin_string[position] == '\t' ||
         feedin_string[position] == '\n' || feedin_string[position] == '\r')
    ++position;
  feedin_string.erase(0, position);
}

void Json::ParseLiteral(const std::string &literal) {
  if (feedin_string.compare(0, literal.size(), literal)) {
    throw std::invalid_argument("INVALID VALUE");
  }

  switch (literal[0]) {
    case 't':
      value_ = std::make_unique<JsonBool>(true);
      break;
    case 'f':
      value_ = std::make_unique<JsonBool>(false);
      break;
    default:
      value_ = std::make_unique<JsonNull>(nullptr);
  }

  feedin_string.erase(0, literal.length());
}

void Json::ParseValue() {
  if (feedin_string.empty()) throw std::invalid_argument("EXPECT VALUE");
  switch (feedin_string[0]) {
    case 'n':
      return ParseLiteral("null");
    case 't':
      return ParseLiteral("true");
    case 'f':
      return ParseLiteral("false");
    case '0' ... '9':
    case '-':
      return ParseNumber();
    case '\"':
      return ParseString();
    case '[':
      return ParseArray();
    case '{':
      return ParseObject();
    default:
      throw std::invalid_argument("INVALID FEED-IN JSON");
  }
}

void Json::Parse(std::string content) {
  feedin_string = content;
  ParseWhitespace();
  ParseValue();
  ParseWhitespace();
  if (!feedin_string.empty()) {
    throw std::invalid_argument("ROOT NON SINGULAR");
  }
}

void Json::ParseNumber() {
  // Entrance premise: string content start with a digit or a minus
  size_t idx;
  double tmp_number = std::stod(feedin_string, &idx);
  value_ = std::make_unique<JsonNumber>(tmp_number);

  if (feedin_string[idx - 1] == '.')
    throw std::invalid_argument("INVALID ARGUMENT");
  feedin_string.erase(0, idx);
}

void Json::ParseString() {
  std::string tmp_string;
  for (size_t i = 1; i < feedin_string.length(); ++i) {
    switch (feedin_string[i]) {
      case '\"':
        value_ = std::make_unique<JsonString>(tmp_string);
        feedin_string.erase(0, i + 1);
        return;
      case '\\':  // escapes
        ++i;
        switch (feedin_string[i]) {
          case '\"':
            tmp_string.push_back('\"');
            break;
          case '\\':
            tmp_string.push_back('\\');
            break;
          case '/':
            tmp_string.push_back('/');
            break;
          case 'b':
            tmp_string.push_back('\b');
            break;
          case 'f':
            tmp_string.push_back('\f');
            break;
          case 'n':
            tmp_string.push_back('\n');
            break;
          case 't':
            tmp_string.push_back('\t');
            break;
          case 'r':
            tmp_string.push_back('\r');
            break;
        }
        break;
      case 0x00 ... 0x1F:
        throw std::invalid_argument(
            "INVALID ASCII CHAR, SHOULD BE GREATER THAN 0X1F");
      default:
        tmp_string += feedin_string[i];
        break;
    }
  }
  throw std::invalid_argument("MISSING DOUBLE QUOTATION MARK");
}

std::string Json::ParseKey() {
  std::string key;
  for (size_t i = 1; i < feedin_string.length(); ++i) {
    switch (feedin_string[i]) {
      case '\"':
        feedin_string.erase(0, i + 1);
        return key;
      case '\\':  // escapes
        ++i;
        switch (feedin_string[i]) {
          case '\"':
            key.push_back('\"');
            break;
          case '\\':
            key.push_back('\\');
            break;
          case '/':
            key.push_back('/');
            break;
          case 'b':
            key.push_back('\b');
            break;
          case 'f':
            key.push_back('\f');
            break;
          case 'n':
            key.push_back('\n');
            break;
          case 't':
            key.push_back('\t');
            break;
          case 'r':
            key.push_back('\r');
            break;
        }
        break;
      case 0x00 ... 0x1F:
        throw std::invalid_argument(
            "INVALID ASCII CHAR, SHOULD BE GREATER THAN 0X1F");
      default:
        key += feedin_string[i];
        break;
    }
  }
  throw std::invalid_argument("KEY MISSING DOUBLE QUOTATION MARK");
}

void Json::ParseArray() {
  feedin_string.erase(0, 1);
  value_ = std::make_unique<JsonArray>();
  while (!feedin_string.empty()) {
    ParseWhitespace();
    if (feedin_string[0] == ']') {
      feedin_string.erase(0, 1);
      return;
    }
    Json json;
    json.feedin_string = std::move(this->feedin_string);
    json.ParseValue();
    value_->push_back(json);
    feedin_string = std::move(json.feedin_string);
    ParseWhitespace();
    if (!feedin_string.empty()) {
      switch (feedin_string[0]) {
        case ',':
          feedin_string.erase(0, 1);
          continue;
        case ']':
          feedin_string.erase(0, 1);
          return;
        default:
          throw std::invalid_argument("MISS COMMA OR RIGHT SQUARE BRACKET");
      }
    }
  }
}

void Json::ParseObject() {
  feedin_string.erase(0, 1);
  value_ = std::make_unique<JsonObject>();
  while (!feedin_string.empty()) {
    ParseWhitespace();
    if (feedin_string[0] == '}') {
      feedin_string.erase(0, 1);
      return;
    }
    auto key = ParseKey();
    ParseWhitespace();
    if (feedin_string.empty() || feedin_string[0] != ':') {
      throw std::invalid_argument("MISS COLON AFTER KEY");
    }
    feedin_string.erase(0, 1);
    Json json;
    json.feedin_string = std::move(this->feedin_string);
    json.ParseWhitespace();
    json.ParseValue();  // Parse value

    value_->insert(std::make_pair(key, json));
    feedin_string = std::move(json.feedin_string);
    ParseWhitespace();
    if (!feedin_string.empty()) {
      switch (feedin_string[0]) {
        case ',':
          feedin_string.erase(0, 1);
          continue;
        case '}':
          feedin_string.erase(0, 1);
          return;
        default:
          throw std::invalid_argument("MISS COMMA OR RIGHT SQUARE BRACE");
      }
    }
  }
}

bool Json::IsNull() const { return type() == JsonType::kNull; }

bool Json::IsBool() const { return type() == JsonType::kBool; }

bool Json::IsNumber() const { return type() == JsonType::kNumber; }

bool Json::IsString() const { return type() == JsonType::kString; }

bool Json::IsArray() const { return type() == JsonType::kArray; }

bool Json::IsObject() const { return type() == JsonType::kObject; }

std::string Jsontype2String(JsonType type) {
  switch (type) {
    case JsonType::kUndefined:
      return "undefined";
    case JsonType::kNull:
      return "null";
    case JsonType::kBool:
      return "bool";
    case JsonType::kNumber:
      return "number";
    case JsonType::kString:
      return "string";
    case JsonType::kArray:
      return "array";
    case JsonType::kObject:
      return "object";
  }
}
}  // namespace lightjson