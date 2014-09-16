#include <iostream>
#include "ExpressionParser.hh"

using namespace std;

int main()
{
  ExpressionParser expression;

  //double answer=expression.Evaluate("three*(2e1*(1+(-2*3)-(4*-5)/2.0))");
  double answer=expression.Evaluate("-2e-2*-2");

  cout<<"The answer evaluates to: "<<answer<<endl;
  return 0;
}
