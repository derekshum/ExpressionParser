#include "ExpressionParser.hh"
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <algorithm>

ExpressionParser::ExpressionParser()
{
  //ctor
}

ExpressionParser::~ExpressionParser()
{
  //dtor
}

double ExpressionParser::Evaluate(string expression)
{
  //clean up string:
  SearchReplace(expression, "++", "+");
  SearchReplace(expression, "+-", "-");
  SearchReplace(expression, "--", "+");
  //cout<<"Evaluating "<<expression.c_str()<<endl;

  //if there are no operators, convert the string to a double. Slightly complicated because the string could start
  //with a + or - to indicate sign. The string can also contain a + or - from an exponent, eg 2e-2
  //need to find the case where there is only 1 + or - and its related to an exponential

  if(expression.find('*') == string::npos &&
     expression.find('/') == string::npos &&
     expression.find('(') == string::npos &&
     expression.find(')') == string::npos)
  {
    if(expression.find('-', 1) == string::npos) {//no minus signs
      if(expression.find('+', 1) == string::npos) return ConvertStringToDouble(expression);
      if(count(expression.begin()+1, expression.end(), '+') == 1) {
        size_t ifind = expression.find('+', 1);
        if(expression[ifind-1] == 'e') {
          if(isdigit(expression[ifind-2]) && isdigit(expression[ifind+1])) return ConvertStringToDouble(expression);
        }
      }
    }
    if(expression.find('+', 1) == string::npos) {//no plus signs
      if(count(expression.begin()+1, expression.end(), '-') == 1) {
        size_t ifind = expression.find('-', 1);
        if(expression[ifind-1] == 'e') {
          if(isdigit(expression[ifind-2]) && isdigit(expression[ifind+1])) return ConvertStringToDouble(expression);
        }
      }
    }
  }

  //step 1: Evaluate expression in parentheses
  //Look for closing parenthesis, walk back to open parenthesis and evaluate that expression
  string theExpression = expression;
  size_t slength = theExpression.size();
  for(size_t i=0; i<slength; i++) {
    if(theExpression[i] == ')') {
      size_t j = i-1;
      while(j>0 && theExpression[j-1] != '(')j--;
      string parExpr = theExpression.substr(j, i-j);

      stringstream parDouble;
      double pdouble = Evaluate(parExpr);
      parDouble << pdouble;
      string temp=theExpression.substr(0,j-1)+parDouble.str()+theExpression.substr(i+1);
      //cout<<"original: "<<theExpression<<endl;
      //cout<<"parexp: "<<parExpr<<" evaluated to "<<pdouble<<endl;
      //cout<<"replaced: "<<temp<<endl;
      //cout<<"i, j "<<i<<" "<<j<<endl;
      //theExpression.replace(j-1, i-j+2, parDouble.str());
      //return Evaluate(theExpression);
      return Evaluate(temp);
    }
  }

  //step 2: Look for multiplications and divisions, and evaluate them

  for(size_t i=0; i<slength; i++) {
    if(theExpression[i] == '*' || theExpression[i] == '/') {
     // cout<<"found a multiplication: "<<theExpression<<endl;

      size_t j1 = i;
      if(i>0)j1--;
      while(j1>0 && theExpression[j1] != '+' && theExpression[j1] != '-') j1--;
      //cout<<"i "<<i<<" j1 "<<j1<<endl;
      //check that it didn't find an exponent
      if(j1>2 && theExpression[j1-1] == 'e' && isdigit(theExpression[j1-2])) {
        j1--;
        while(j1>0 && theExpression[j1] != '+' && theExpression[j1] != '-') j1--;
      }
      //cout<<"first term = "<<theExpression.substr(j1,i-j1)<<" "<<j1<<" "<<i-j1<<endl;
      double term1 = Evaluate(theExpression.substr(j1, i-j1));

      size_t j2 = i+1;
      if(theExpression[j2] == '+' || theExpression[j2] == '-')j2++;//e.g. 2*-2
      if(j2>theExpression.size()-1)j2=theExpression.size()-1;//this will be wrong

      bool foundend=false;
      while(j2<theExpression.size()-1 && !foundend){
        if(theExpression[j2] == '+' || theExpression[j2] == '-'){
          foundend=true;
          if(theExpression[j2-1] =='*' || theExpression[j2-1] =='/'){//to catch, e.g.m a 2*+2
            foundend=false;
          }
          if(j2<theExpression.size()-1){
            if(theExpression[j2-1] =='e'){
              foundend=false;
            }
          }
        }

        if(foundend){
          if(theExpression[j2] == '+' || theExpression[j2] == '-')j2--;
          break;
        }
        j2++;
      }

     // cout<<"second term = "<<theExpression.substr(i+1, j2-i)<<" "<<i+1<<" "<<j2-i<<endl;
      double term2 = Evaluate(theExpression.substr(i+1, j2-i));
      double multdiv = 0;
      if(theExpression[i] == '*') multdiv = term1*term2;
      else{
      	if(term2!=0.)multdiv = term1/term2;
      }

      stringstream parDouble;
      parDouble << multdiv;
      ////!!replace is probably wrong, build a new string?!
      //cout<<"Mulitply before: "<<theExpression<<endl;
      theExpression.replace(j1, j2-j1+1,string("+")+ parDouble.str());
      //cout<<"after replace "<<theExpression<<endl;
     // string temp=theExpression.substr(0,j-1)+parDouble.str()+theExpression.substr(i+1);
      return Evaluate(theExpression);
    }
  }

  //step 3: Look for additions and subtractions and evaluate them
  double result = 0.;
  slength = theExpression.size();
  //cout << "Summing: " << theExpression.c_str() << endl;
  for(size_t i=1; i<slength; i++) { //start at 1 in case the first term is an overall sign
    if(theExpression[i] == '+' || theExpression[i] == '-') {
      //cout<<"adding "<<theExpression.substr(0,i).c_str()<<" and "<<theExpression.substr(i).c_str()<<endl;
      result = Evaluate(theExpression.substr(0, i)) + Evaluate(theExpression.substr(i));
      break;
    }
  }

  return result;
}

double ExpressionParser::ConvertStringToDouble(string expression)
{
  //check if the string contains any letters, if not, should be safe to cast to double
  //ignore the letter e; if it's by itself, it's likely an exponent
  if(expression.find_first_of("abcdfghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")==string::npos)return atof(expression.c_str());
  else{//may need to take out a sign in front of an expression before converting to double
    if(expression.at(0)=='-'){
      return -ConvertNameToDouble(expression.substr(1));
    }
    else if(expression.at(0)=='+'){
      return ConvertNameToDouble(expression.substr(1));
    }
    else return ConvertNameToDouble(expression);
  }
}

double ExpressionParser::ConvertNameToDouble(string aExpression)
{
  if(aExpression== "one")return 1.0;
  else if(aExpression=="two")return 2.0;
  else if(aExpression=="three") return 3.0;
  else return 0.;
}

void ExpressionParser::SearchReplace(std::string& str, const std::string& oldStr, const std::string& newStr)
{
 size_t pos = 0;
 while((pos = str.find(oldStr, pos)) != std::string::npos)
  {
    str.replace(pos, oldStr.length(), newStr);
    pos += newStr.length();
  }
}


