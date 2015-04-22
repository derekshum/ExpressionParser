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
    virtual double ConvertStringToDouble(string aExpression);
    virtual void SearchReplace(std::string& str, const std::string& oldStr, const std::string& newStr);
    virtual double ConvertNameToDouble(string aExpression);

  protected:
  private:

};

#endif // _EXPRESSIONPARSER_HH_
