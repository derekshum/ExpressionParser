#include <iostream>
#include "ExpressionParser.hh"

using namespace std;

int main()
{
  ExpressionParser expression;

  //double answer=expression.Evaluate("three*(2e1*(1+(-2*3)-(4*-5)/2.0))");
  double answer=expression.Evaluate("one+3*1e-2+5");

  cout<<"The answer evaluates to: "<<answer<<endl;
  return 0;
}
