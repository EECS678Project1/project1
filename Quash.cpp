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
m_dir = getenv("PWD");
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
    if(test[0]=="set")
    {
      setPaths(test[1]);
    }else if(test[0]=="cd")
    {
      if(test.size() == 1)
      {
        changeDir(m_home);
      }else
      {
        changeDir(test[1]);
      }

    }else
    {
      launch(test);
    }


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
  int e = putenv(const_cast<char*>(mPath.c_str()));
  if(e)
  {
    std::cout<<"\nDidn't work";
  }else
  {
    cout<<"it works"<<endl;
    cout<<getenv("PATH")<<endl;
    cout<<getenv("HOME")<<endl;

  }
}

void Quash::changeDir(string mdir)
{
  char* dir_name = const_cast<char*>(mdir.c_str());
  if(chdir(dir_name))
  {
    cout<<getenv("PWD");
  }else
  {
    cout<<"\nError";
  }
}
