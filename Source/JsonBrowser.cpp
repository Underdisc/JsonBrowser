/*****************************************************************************/
/*!
\file JsonBrowser.cpp
\author Connor Deakin
\par E-mail: connortdeakin\@gmail.com
\par Project: JsonBrowser
\date 5/16/2017
\brief
  Contains the implementation for the JsonBrowser.
*/
/*****************************************************************************/

#include <iostream>
#include <stdio.h>

#include "JsonBrowser.h"

JsonBrowser::JsonBrowser(Json::Value * json_value):
m_JsonValueStack(), m_Path()
{
  m_JsonValueStack.push(json_value);
  m_Path.push_back("B");
}

void JsonBrowser::browse()
{
  bool browsing = true;
  while(browsing){
    clearTerminal();
    printHeader();
    std::cout << std::endl;
    Json::ValueType value_type = m_JsonValueStack.top()->type();
    switch (value_type) {
      case Json::objectValue: browsing = exploreObjectValue(); break;
      case Json::arrayValue:  browsing = exploreArrayValue(); break;
      case Json::stringValue:
      case Json::intValue:
      case Json::realValue:   browsing = showValue(value_type); break;
      default: upLevel(); break;
    }
    if(m_JsonValueStack.empty())
      browsing = false;
  }
  clearTerminal();
  std::cout << "\033[31;1mENDING BROWSING SESSION\033[0m" << std::endl;
}

bool JsonBrowser::exploreObjectValue()
{
  printGeneralSelections();
  std::vector<std::string> member_names = m_JsonValueStack.top()->getMemberNames();
  unsigned num_members = member_names.size();
  std::cout << "\033[1m";
  for(unsigned i = 0; i < num_members; ++i){
    std::cout << (i + 2) << " | " << member_names[i] << std::endl;
  }
  std::cout << "\033[0m";
  unsigned selected = getSelection(num_members + 1);
  if(selected == 0) upLevel();
  else if(selected == 1) return false;
  else
    downLevel(member_names[selected - 2]);
  return true;
}

bool JsonBrowser::exploreArrayValue()
{
  printGeneralSelections();
  Json::Value * top = m_JsonValueStack.top();
  unsigned range = top->size();
  if(range <= 0)
    std::cout << "\033[1mNo arrayValue members\033[0m" << std::endl;
  else
    std::cout << "\033[1mIndicies: 2 - " << range + 1 << "\033[0m" << std::endl;
  unsigned selected = getSelection(range + 1);
  if(selected == 0) upLevel();
  else if(selected == 1) return false;
  else downLevel(selected - 2);
  return true;
}

bool JsonBrowser::showValue(Json::ValueType value_type)
{
  printGeneralSelections();
  std::cout << "\033[1m\033[33;1mValue: ";
  switch (value_type) {
    case Json::stringValue:  std::cout << m_JsonValueStack.top()->asString(); break;
    case Json::intValue:     std::cout << m_JsonValueStack.top()->asInt();    break;
    case Json::uintValue:    std::cout << m_JsonValueStack.top()->asUInt();   break;
    case Json::realValue:    std::cout << m_JsonValueStack.top()->asDouble(); break;
    case Json::booleanValue: std::cout << m_JsonValueStack.top()->asBool();   break;
    default: std::cout << "NULL";                                             break;
  }
  std::cout << "\033[0m" << std::endl;
  unsigned selected = getSelection(1);
  if(selected == 0) upLevel();
  else return false;
  return true;
}

void JsonBrowser::upLevel()
{
  m_JsonValueStack.pop();
  m_Path.pop_back();
}

void JsonBrowser::downLevel(const std::string & value)
{
  Json::Value * top_value = m_JsonValueStack.top();
  m_JsonValueStack.push(&((*top_value)[value]));
  m_Path.push_back(value);
}

void JsonBrowser::downLevel(unsigned index)
{
  Json::Value * top = m_JsonValueStack.top();
  m_JsonValueStack.push(&((*top)[index]));
  std::string directory = std::to_string(index);
  m_Path.push_back(directory);
}

unsigned JsonBrowser::getSelection(unsigned max) const
{
  std::cout << "\033[32;1mEntry: ";
  unsigned selected;
  scanf("%u", &selected);
  while(selected > max){
    std::cout << "\033[31;1mNot an option - Reselect" << std::endl;
    std::cout << "\033[32;1mEntry: ";
    scanf("%u", &selected);
  }
  std::cout << "\033[0m";
  return selected;
}

void JsonBrowser::printGeneralSelections() const
{
  std::cout << "0 | UP LEVEL";
  if(m_JsonValueStack.empty())
    std::cout << "(ENDS BROWSING AT ~B)";
  std::cout << std::endl;
  std::cout << "1 | STOP BROWSING" << std::endl;
}

void JsonBrowser::printHeader() const
{
  std::cout << "\033[32;1m";
  printPath();
  std::cout << "\033[36;1m";
  std::cout << " - ";
  printValueType();
  std::cout << "\033[0m";
}

void JsonBrowser::printPath() const
{
  std::cout << "~";
  unsigned path_size = m_Path.size();
  for(unsigned i = 0; i < path_size; ++i){
    std::cout << m_Path[i] << "/";
  }
}

void JsonBrowser::printValueType() const
{
  Json::ValueType value_type = m_JsonValueStack.top()->type();
  switch (value_type) {
    case Json::nullValue:    std::cout << "nullValue";    break;
    case Json::intValue:     std::cout << "intValue";     break;
    case Json::uintValue:    std::cout << "uintValue";    break;
    case Json::realValue:    std::cout << "realValue";    break;
    case Json::stringValue:  std::cout << "stringValue";  break;
    case Json::booleanValue: std::cout << "booleanValue"; break;
    case Json::arrayValue:   std::cout << "arrayValue";   break;
    case Json::objectValue:  std::cout << "objectValue";  break;
  }
}

void JsonBrowser::clearTerminal() const
{
  std::cout << "\033[2J\033[1;1H";
}
