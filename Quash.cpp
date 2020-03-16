#include "Quash.h"
#include <string>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>


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
    getline(cin,input);
    if(input == "exit")
    {
      exitShell = true;
      return;
    }
    string* const test = splitArguments(input);
    for(int i=0;i<5;i++)
    {
      cout<<test[i];
    }


    launch(test);

  }


}

void Quash::launch(string* const args)
{
//const char* c = args->c_str();
cout<<"in launch"<<endl;
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0)
  {
    // Child process
    cout<<args[0]<<endl;
    cout<<args[1]<<endl;
    char** const newArgs = new char*[args->size()];
    for(int i=0;i<args->size();i++)
    {
      const newArgs[i] = args[i].c_str();
    }
    execvp(newArgs[0], newArgs);
  }
  else if (pid < 0)
  {
    // Error forking
    std::cout<<"\nError";
  }
  else
  {
      wait(NULL);

  }
}

string* Quash::splitArguments(string line)
{
  cout<<"here"<<endl;
  cout<<line.length()<<endl;
  vector<string> myVector;
  string argument = "";
  for(int i=0;i<line.length();i++)
  {
    cout<<"hi"<<endl;
    if(line[i] != ' ')
    {
      argument = argument + line[i];
    }else
    {
      myVector.push_back(argument);
      argument = "";
    }
  }
  myVector.push_back(argument);
  string* args = new string[myVector.size()];
  cout<<myVector.at(0)<<endl;
  for(int i=0;i<myVector.size();i++)
  {
    args[i] = myVector.at(i);
    cout<<args[i]<<endl;
  }
  return args;
}
