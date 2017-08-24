#pragma once

#ifdef _WIN64

#ifdef _DEBUG
#pragma comment(lib, "jsoncpp/lib64/lib_json_x64D.lib")
#else
#pragma comment(lib, "jsoncpp/lib64/lib_json_x64.lib")
#endif

#elif _WIN32

#ifdef _DEBUG
#pragma comment(lib, "jsoncpp/lib/lib_jsonD.lib")
#else
#pragma comment(lib, "jsoncpp/lib/lib_json.lib")
#endif

#endif


#include "include/json/json.h"


