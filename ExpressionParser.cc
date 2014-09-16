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

double ExpressionParser::Evaluate(string aExpression)
{
  //clean up string:
  SearchReplace(aExpression,"++","+");
  SearchReplace(aExpression,"+-","-");
  SearchReplace(aExpression,"--","+");
  //if there are no operators, convert the string to a double. Slightly complicated because the string could start
  //with a + or - to indicate sign. The string can also contain a + or - from an exponent, eng 2e-2
  //need to find the case where there is only 1 + or - and its related to an exponential

  if(aExpression.find('*')==string::npos && aExpression.find('/')==string::npos && aExpression.find('(')==string::npos &&
    aExpression.find(')')==string::npos){

    if(aExpression.find('-',1)==string::npos){
      std::cout<<aExpression.c_str()<<" has no negative signs"<<std::endl;
      if(aExpression.find('+',1)==string::npos)return ConvertStringToDouble(aExpression);
      if(std::count(aExpression.begin()+1, aExpression.end(),'+')==1){
        size_t find=aExpression.find('+',1);
        if(aExpression[find-1]=='e'){
          if(isdigit(aExpression[find-2]) && isdigit(aExpression[find+1]))return ConvertStringToDouble(aExpression);
        }
      }
    }
    if(aExpression.find('+',1)==string::npos){
      std::cout<<aExpression.c_str()<<" has no positive signs and "<< std::count(aExpression.begin()+1, aExpression.end(),'-')
       <<" negative signs at position "<<aExpression.find('-',1) <<std::endl;
      if(aExpression.find('-',1)==string::npos)return ConvertStringToDouble(aExpression);
      if(std::count(aExpression.begin()+1, aExpression.end(),'-')==1){
        size_t find=aExpression.find('-',1);
        if(aExpression[find-1]=='e'){
          if(isdigit(aExpression[find-2]) && isdigit(aExpression[find+1]))return ConvertStringToDouble(aExpression);
        }
      }
    }
  }

  //step 1: Evaluate expression in parentheses
  //Look for closing parenthesis, walk back to open parenthesis and evaluate that expression
  string theExpression=aExpression;
  size_t slength=theExpression.size();
  for(size_t i=0;i<slength;i++){
    if(theExpression[i]==')'){
      size_t j=i-1;
      while(j>0 && theExpression[j-1]!='(')j--;
      string parExpr=theExpression.substr(j,i-j);

      stringstream parDouble;
      double pdouble=Evaluate(parExpr);
      parDouble<<pdouble;

      theExpression.replace(j-1,i-j+2,parDouble.str());
      return Evaluate(theExpression);
    }
  }

  //step 2: Look for multiplications and divisions, and evaluate them

  for(size_t i=0;i<slength;i++){
    if(theExpression[i]=='*' || theExpression[i]=='/'){
      size_t j1=i;
      while(j1>0 && theExpression[j1]!='+' && theExpression[j1]!='-')j1--;
      //check that it didn't find an exponent
      if(j1>2 && theExpression[j1-1]=='e' && isdigit(theExpression[j1-2])){
        j1--;
        while(j1>0 && theExpression[j1]!='+' && theExpression[j1]!='-')j1--;
      }
      double term1=Evaluate(theExpression.substr(j1,i-j1));

      size_t j2=i+1;
      while(j2<theExpression.size()-1 && theExpression[j2]!='+' && theExpression[j2]!='-')j2++;
      double term2=Evaluate(theExpression.substr(i+1,j2-i+1));
      double multdiv=0;
      if(theExpression[i]=='*')multdiv=term1*term2;
      else multdiv=term1/term2;

      stringstream parDouble;
      parDouble<<multdiv;

      theExpression.replace(j1,j2-j1+2,string("+")+parDouble.str());

      return Evaluate(theExpression);
    }
  }

  //step 3: Look for additions and subtractions and evaluate them
  double result=0.;
  slength=theExpression.size();
  //cout<<"Summing: "<<theExpression.c_str()<<endl;
  for(size_t i=1;i<slength;i++){//start at 1 in case the first term is an overall sign
    if(theExpression[i]=='+' || theExpression[i]=='-'){
      result=Evaluate(theExpression.substr(0,i))+Evaluate(theExpression.substr(i));
      break;
    }
  }

  return result;
}
