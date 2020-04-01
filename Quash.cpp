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
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>




using namespace std;

vector<int> Quash::pids;
vector<string> Quash::commands;

Quash::Quash()
{
m_path = getenv ("PATH");
m_home =getenv ("HOME");
m_dir = getenv("PWD");
}

Quash::~Quash()
{

}
//.............................................................................
void Quash::run()
{
  cout<<"home: "<<m_home<<endl;
  int num;
  bool exitShell = false;
  bool ispipe = false;
  string input = "";
  while(exitShell == false)
  {
    vector<string> command1;
    vector<string> command2;
    cout<<getenv("PWD");
    cout<<">";
    getline(cin,input);
    if(input == "exit" || input == "quit")
    {
      exitShell = true;
      return;
    }
    vector<string> test = splitArguments(input);
    for(int j=0;j<test.size();j++)
    {
      if(test.at(j)=="|")
      {
        ispipe = true;
        num = j;
        break;
      }
      //command1.push_back(test.at(j));
    }
    if(ispipe)
    {
      for(int j=0;j<test.size();j++)
      {
        if(test.at(j)=="|")
        {
          num = j;
          break;
        }
        command1.push_back(test.at(j));
      }
      for(int i = num+1;i<test.size();i++)
      {
        command2.push_back(test.at(i));
      }
      pipeCommand(command1,command2);
      ispipe = false;
    }
    else if(test[0]=="set")
    {
      setPaths(test[1]);
    }
    else if(test[0]=="cd")
    {
      if(test.size() == 1)
      {
        changeDir(m_home); //if home sending true
      }else
      {
        changeDir(test[1]);
      }
    }else if (test.size() >= 3 && (test[test.size()-2]=="<" || test[test.size()-2]==">"))
    {
      bool readIt = false;
      if(test[test.size()-2]=="<")
      {
        readIt = true;
      }
      redirect(readIt, test);
    }
    else
    {
      launch(test);
    }


  }


}

//.............................................................................
void Quash::launch(vector<string> args)
{
  bool isBack = false;
  bool isJob = false;
  bool ispipe = false;
  //for background
  if(args.at(args.size()-1) == "&")
  {
  args.pop_back();
  isBack = true;
  commands.push_back(args.at(0));
  }

  //for jobs
  if(args.at(0) == "jobs")
  {
    isJob = true;
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
      //pids.push_back(getpid());
      /*for(int i =0;i<pids.size();i++)
      {
        if(pids.at(i)==getpid())
        {
          cout<<"["<<i<<"]"<<" ";
        }
      }*/

      cout<<getpid()<<" running in the background"<<endl;

      execvp(newArgs[0], newArgs);
    }else
    {
      //cout<<endl<<getpid()<<" running in the foreground"<<endl;
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
    if(isJob)
    {
      for(int i = 0;i<pids.size();i++)
      {
        //cout<<"stuff in vector"<<endl;
        if(pids.at(i)>-1)
        {
          int id = i+1;
          cout<<"["<<id<<"]"<<" ";
          cout<<pids.at(i)<<" "<<commands.at(i)<<endl;
          //also print command later
        }
      }

    }else if(isBack)
    {
      pids.push_back(pid);
    }else
    {
      wait(NULL);
    }

  }
}

//.............................................................................
void Quash::childSignalHandler(int sig)
{
int status;
int p = waitpid(-1, &status, WNOHANG);
  if(p>0)
  {
    for(int i =0;i<pids.size();i++)
    {
      cout<<"p is "<<p<<endl;
      if(pids.at(i)==p)
      {
        int id = i+1;
        cout<<"["<<id<<"]"<<" "<<p<<" finished "<<commands.at(i)<<endl;
        pids[i]= -1;
      }
    }
    //cout<<p<<" finished"<<endl;
    //Quash::pids.push_back(1);
  }

}

//.............................................................................
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

//.............................................................................
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

//.............................................................................
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

//.............................................................................
void Quash::redirect(bool readIt, vector<string> test)
{
  vector<string> temp;
  string fileName = test[test.size()-1];
  const char* fileNameStar = const_cast<char*>(fileName.c_str());
  for(int i = 0; i < test.size()-2; i++)
  {
    temp.push_back(test.at(i));
  }
    if(readIt)
    {
      int o = dup(fileno(stdin));
      freopen(fileNameStar,"r",stdin);
      launch(temp);
      dup2(o,fileno(stdin));
      close(o);
      cout<<"done reading"<<endl;
    }else
    {
      int o = dup(fileno(stdout));
      freopen(fileNameStar,"w",stdout);
      launch(temp);
      dup2(o,fileno(stdout));
      close(o);
      cout<<"done writing"<<endl;
    }

}

void Quash::pipeCommand(vector<string> command1,vector<string> command2)
{
  int pipe_fd[2];
  pipe(pipe_fd);
  pid_t p1,p2;

  p1 = fork();
  if ( p1 == 0 )
  {
     /* Redirect output of process into pipe */
     dup2( pipe_fd[1], STDOUT_FILENO );
     close(pipe_fd[0]);
     launch(command1);
     close(pipe_fd[1]);
     exit(0);
     //execvp(newArgs1[0], newArgs1);
   }

   p2 = fork();
  if (p2 == 0) {
     /* Redirect input of process out of pipe */
     dup2(pipe_fd[0], STDIN_FILENO);
     close(pipe_fd[1]);
     launch(command2);
     close(pipe_fd[0]);
     exit(0);
   }
/* Main process */
close(pipe_fd[0]);
close(pipe_fd[1]);
wait(NULL);

}
