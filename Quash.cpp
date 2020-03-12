#include "Quash.h"
#include<unistd.h>
#include<stdio.h>
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
    //launch();
      if(input == "exit")
      {
        exitShell = true;
      }
  }


}

void Quash::launch()
{

  pid_t pid, wpid;
  int status;
  //char const * args[] = {"./Exercise1",NULL};
  pid = fork();
  if (pid == 0)
  {
    // Child process
    execvp("./HelloWorld",NULL);
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
