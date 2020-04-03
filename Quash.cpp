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
#include<iterator>
#include<algorithm>




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
  int num;
  int counter = 0;
  bool exitShell = false;
  bool ispipe = false;
  string input = "";
  string previnput = "";
  cout<<getenv("PWD");
  cout<<">";
  while(exitShell == false)
  {
    vector<string> command1;
    vector<string> command2;
    getline(cin,input);
    if(previnput == input)
    {
      counter++;
    }
    else
    {
      counter = 0;
      previnput = input;

    }
    if(counter == 5)
    {
      exitShell = true;
      break;
    }



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
      cout<<getenv("PWD");
      cout<<">";
    }
    else if(test[0]=="cd")
    {
      if(test.size() == 1)
      {
        changeDir(m_home); //if home sending true
        cout<<getenv("PWD");
        cout<<">";
      }else
      {
        changeDir(test[1]);
        cout<<getenv("PWD");
        cout<<">";
      }
    }else if (test.size() >= 3 && (test[test.size()-2]=="<" || test[test.size()-2]==">"))
    {
      bool readIt = false;
      if(test[test.size()-2]=="<")
      {
        readIt = true;
      }
      redirect(readIt, test);
      cout<<getenv("PWD");
      cout<<">";
    }
    else if (test[0]=="jobs")
    {
      printJobs();
      cout<<getenv("PWD");
      cout<<">";
    }
    else if(test[0]=="kill")
    {
      killprocess(test[1],test[2]);
    }
    else
    {
      launch(test);
      cout<<getenv("PWD");
      cout<<">";
    }


  }


}

//.............................................................................
void Quash::launch(vector<string> args)
{
  bool isBack = false;
  bool ispipe = false;
  //for background
  if(args.at(args.size()-1) == "&")
  {
  args.pop_back();
  isBack = true;
  commands.push_back(args.at(0));
  }

  char** newArgs = new char*[args.size()+1];

  pid_t pid, wpid;

  signal(SIGCHLD, Quash::childSignalHandler);
  if(isBack)
  {
    count++;
  }
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

      cout<<endl<<"["<<count<<"] "<<getpid()<<" running in the background"<<endl<<getenv("PWD")<<">"<<endl;
      //cout<<getenv("PWD");
      //cout<<">";

      if (execvp(newArgs[0], newArgs)<0)
      {
        cout<<"\nCant find executable to run in background\n";
      }
    }else
    {
      //cout<<endl<<getpid()<<" running in the foreground"<<endl;
      if (execvp(newArgs[0], newArgs)<0)
      {
        cout<<"\nExecutable not found\n";
      }
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
    if(isBack)
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
      cout<<endl;
      if(pids.at(i)==p)
      {
        int id = i+1;
        cout<<"["<<id<<"]"<<" "<<p<<" finished "<<commands.at(i)<<endl<<getenv("PWD")<<">"<<endl;
        //pids.at(i)= -100;
        pids.at(i) = -100;


      }
    }
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
  //int e = putenv(const_cast<char*>(mPath.c_str()));
  string envName = "";
  string envValue = "";
  bool part1 = true;
  for(int i=0;i<mPath.length();i++)
  {
    if(mPath[i]!='=' && part1 == true)
    {
      envName = envName + mPath[i];
    }else
    {
      part1 = false;
      if(mPath[i]!='=')
      {
        envValue = envValue + mPath[i];
      }

    }
  }

  int e = setenv(const_cast<char*>(envName.c_str()),const_cast<char*>(envValue.c_str()),1);
  if(e)
  {
    std::cout<<"\nDidn't work";
  }else
  {
    cout<<"Environment set correctly"<<endl;
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
      cout<<"Done reading"<<endl;
    }else
    {
      int o = dup(fileno(stdout));
      freopen(fileNameStar,"w",stdout);
      launch(temp);
      dup2(o,fileno(stdout));
      close(o);
      cout<<"Done writing"<<endl;
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
     cout<<getenv("PWD");
     cout<<">";
     exit(0);
   }
/* Main process */
close(pipe_fd[0]);
close(pipe_fd[1]);
wait(NULL);
}


void Quash::printJobs()
{
    for(int i = 0;i<pids.size();i++)
    {
      if(pids.at(i)!=(-100))
      {
        int id = i+1;
        cout<<"["<<id<<"]"<<" ";
        cout<<pids.at(i)<<" "<<commands.at(i)<<endl;
      }
    }

}

void Quash::killprocess(string signum, string jobid)
{
  int pid = 0;
  for(int i=0;i<pids.size();i++)
  {
    if(i==(stoi(jobid)-1))
    {
      pid = pids.at(i);
    }
  }
  kill(pid,SIGKILL);
}
