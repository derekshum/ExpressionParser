#include <iostream>
#include "ExpressionParser.hh"

using namespace std;

int main()
{
  ExpressionParser expression;

  //double answer=expression.Evaluate("three*(2e1*(1+(-2*3)-(4*-5)/2.0))");
  double answer=expression.Evaluate("-2e-3*2+8*3/2e2+5-2*18");

  cout<<"The answer evaluates to: "<<answer<<endl;
  return 0;
}
