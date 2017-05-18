#include "External/json/json.h"

#include <stack>
#include <string>
#include <vector>

class JsonBrowser
{
public:
  JsonBrowser(Json::Value * json_value);
  void browse();

private:
  bool exploreObjectValue();
  bool exploreArrayValue();
  bool showValue(Json::ValueType value_type);
  void upLevel();
  void downLevel(const std::string & value);
  void downLevel(unsigned index);
  unsigned getSelection(unsigned max) const;
  void printGeneralSelections() const;
  void printHeader() const;
  void printPath() const;
  void printValueType() const;
  void clearTerminal() const;
  std::stack<Json::Value *> m_JsonValueStack;
  std::vector<std::string> m_Path;
};
