#include <iostream>
#include <exception>
#include <memory>
#include <string>
#include "json.h"

using namespace lightjson;
using namespace std;

int main() {

    Json json;
    try {
        json.Parse("{\"name\": \"dongming\", \"age\": 19, \"secondobj\": {\"univ\": \"zju\"}}");
//        json.Parse("[1,2,3,[1,2,true,null]]");
        cout << Jsontype2String(json.type()) << endl;
//        cout << json.ToString() << endl;
        cout << json.size() << endl;
    } catch (std::exception &e) {
        cout << e.what() << endl;
    }

    return 0;
}