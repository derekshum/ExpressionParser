#include <iostream>
#include "ExpressionParser.hh"

using namespace std;

int main()
{
  ExpressionParser expression;

  double answer=expression.Evaluate("2*(1+(-2*3)-(4*-5)/2.0)");
  cout<<"The answer evaluates to: "<<answer<<endl;
  return 0;
}
