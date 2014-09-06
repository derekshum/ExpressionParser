#ifndef _EXPRESSIONPARSER_HH_
#define _EXPRESSIONPARSER_HH_

#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;

class ExpressionParser
{
  public:
    ExpressionParser();
    virtual ~ExpressionParser();
    virtual double Evaluate(string aExpression);
    virtual double ConvertStringToDouble(string aExpression){
      return atof(aExpression.c_str());
    }
    virtual void SearchReplace(std::string& str, const std::string& oldStr, const std::string& newStr)
    {
      size_t pos = 0;
      while((pos = str.find(oldStr, pos)) != std::string::npos)
      {
        str.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
      }
    }
  protected:
  private:

};

#endif // _EXPRESSIONPARSER_HH_
