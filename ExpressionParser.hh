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
      //check if the string contains any letters, if not, should be safe to cast to double
      //ignore the letter e; if it's by itself, it's likely an exponent
      if(aExpression.find_first_of("abcdfghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")==string::npos)return atof(aExpression.c_str());

      if(aExpression== "one")return 1.0;
      else if(aExpression=="two")return 2.0;
      else if(aExpression=="three") return 3.0;
      else return 0.;

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
