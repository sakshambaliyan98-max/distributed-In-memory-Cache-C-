#pragma once

#include <string>
#include <vector>
using namespace std;
namespace cache
{

// A parsed request line, e.g. "SET user Saksham" ->
//   name = "SET", args = {"user", "Saksham"}
struct Command
{
    string name;

    vector<string> args;
};

}
