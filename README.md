# light-json
## Introduction

A simple JSON library written in C++ following the [tutorial in C by miloyip](https://github.com/miloyip/json-tutorial) and referring a lot from  [the C++ version by Yuan-Hang](https://github.com/Yuan-Hang/Json)

This project is mainly for self-practicing in C++ language and might not be rigorous in either grammer or C++ programming paradigm. It only implement part of the parsing functionality for the moment.

The source code for the lib is stored in dir `lightjson`

## Usage Example

Suppose `document` is a raw json string

```c++
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include "json.h"

using namespace lightjson;
using namespace std;

int main() {
  // document = ...
  Json json;
  try {
    json.Parse(document);
  } catch (std::exception &e) {
    cout << e.what() << endl;
  }
  return 0;
}
```

## TODO

- [ ] re-write the copy constructor for `class Json`
- [ ] `overload` operator[] for `JsonArray` and `JsonObject`
- [ ] add unit tests
- [ ] add serializing function
- [ ] add formatting function
