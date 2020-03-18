#include "Quash.h"
#include <string>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include<cstdlib>
#include<stdlib.h>


using namespace std;

Quash::Quash()
{
m_path = getenv ("PATH");
m_home =getenv ("HOME");
}

Quash::~Quash()
{

}

void Quash::run()
{
  cout<<m_home<<endl;
  bool exitShell = false;
  string input = "";
  while(exitShell == false)
  {
    cout<<">";
    getline(cin,input);
    if(input == "exit" || input == "quit")
    {
      exitShell = true;
      return;
    }
    vector<string> test = splitArguments(input);
    if(test[0]=="export")
    {
      if(test[1] == "PATH")
      {
        setPaths(test[2]);
      }
      else
      {
        setHomes(test[2]);
      }
    }
    launch(test);

  }


}

void Quash::launch(vector<string> args)
{
char** newArgs = new char*[args.size()+1];

  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0)
  {
    // Child process
    for(int i=0;i<args.size();i++)
    {
      newArgs[i] = const_cast<char*>(args[i].c_str());
    }
    newArgs[args.size()] = NULL;

      execvp(newArgs[0], newArgs);

  }
  else if (pid < 0)
  {
    // Error forking
    std::cout<<"\nError";
  }
  else
  {
    //Parent process
      wait(NULL);

  }
}

vector<string> Quash::splitArguments(string line)
{
  vector<string> myVector;
  string argument = "";
  for(int i=0;i<line.length();i++)
  {
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
  return myVector;
}

void Quash::setPaths(string mPath)
{
  int e = _putenv_s("PATH",mPath.c_str());
  if(e)
  {
    std::cout<<"\nDidn't work";
  }
}

void Quash::setHomes(string mHome)
{
return;
}
