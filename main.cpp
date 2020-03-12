#include <string>
#include <iostream>
#include "Quash.h"

using namespace std;


int main(int argc, char **argv, char **envp)
{
  cout<<"Hello"<<endl;
  char* pPath;
  pPath = getenv ("PATH");
  if (pPath!=NULL)
      std::cout<<"The current path is"<<pPath<<"\n";
  Quash theQuash;
  theQuash.run();

  return 0;
}
