#include "External/json/json.h"

#include <iostream>
#include <fstream>
#include <string>

#include "JsonBrowser.h"


void LoadJsonBrowser(char * json_filename)
{
  std::ifstream json_stream(json_filename);
  if(json_stream.is_open()){
    std::cout << "Loading Json::Value from " << json_filename << std::endl;
    Json::Value value;
    json_stream >> value;
    JsonBrowser browser(&value);
    browser.browse();
  }
  else
    std::cout << json_filename << " failed to open.";
}

int main(int argc, char * argv[])
{
  if(argc != 2)
    std::cout << "\033[31;1mJson filename needed as argument\033[0m" << std::endl;
  else
    LoadJsonBrowser(argv[1]);
}
