#include "Quash.h"
#include <string>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>


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
    const char* c = input.c_str();
    if(input == "exit")
    {
      exitShell = true;
    }
    launch(c);

  }


}

void Quash::launch(const char* args)
{

  pid_t pid, wpid;
  int status;
  //char const * args[] = {"./Exercise1",NULL};
  pid = fork();
  if (pid == 0)
  {
    // Child process
    execvp(args,NULL);
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
