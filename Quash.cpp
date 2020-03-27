#include "Quash.h"
#include <string.h>
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
  cout<<"home: "<<m_home<<endl;
  bool exitShell = false;
  string input = "";
  while(exitShell == false)
  {
    cout<<getenv("PWD");
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
        changeDir(m_home); //if home sending true
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
  bool isBack = false;
  if(args.at(args.size()-1) == "&")
  {
  cout<<"it is &"<<endl;
  args.pop_back();
  isBack = true;
  }

char** newArgs = new char*[args.size()+1];

  pid_t pid, wpid;



signal(SIGCHLD, Quash::childSignalHandler);

  pid = fork();
  if (pid == 0)
  {
    // Child process
    for(int i=0;i<args.size();i++)
    {
      newArgs[i] = const_cast<char*>(args[i].c_str());
    }
    newArgs[args.size()] = NULL;

    if(isBack)
    {
      cout<<endl<<getpid()<<" running in the background"<<endl;
        execvp(newArgs[0], newArgs);
    }else
    {
      cout<<endl<<getpid()<<" running in the foreground"<<endl;
        execvp(newArgs[0], newArgs);
    }

  }
  else if (pid < 0)
  {
    // Error forking
    std::cout<<"\nError";
  }
  else
  {
    //Parent process
    if(!isBack)
    {
      wait(NULL);
    }


  }
}

void Quash::childSignalHandler(int sig)
{
int status;
cout<<"got it"<<endl;
int p = waitpid(-1, &status, WNOHANG);
cout<<p<<endl;
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
    const char* name_of_dir = (mdir.c_str());
    if(chdir(name_of_dir)==0)
    {
      char* dir = getcwd(NULL, 1024);
      setenv("PWD",dir,1);
    }else
    {
      cout<<"No such file or directory"<<endl;
    }

}
