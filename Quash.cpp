#include "Quash.h"
using namespace std;

Quash::Quash()
{

}

Quash::~Quash()
{

}

void Quash::run()
{
  bool exitShell = false;
  string input = "jsjd";
  while(exitShell == false)
  {
    cout<<">";
    cin>>input;
      if(input == "exit")
      {
        exitShell = true;
      }
  }


}
