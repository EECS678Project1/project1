#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>

using namespace std;

#ifndef QUASH_H
#define QUASH_H

class Quash
{
private:
string m_path;
string m_home;
string m_dir;
static Quash instance;
public:
Quash();
~Quash();
void run();//add path
void launch(vector<string> args);
vector<string> splitArguments(string line);
void setPaths(string mPath);
void changeDir(string mdir);
static void childSignalHandler(int signum);
static vector<int> pids;
static vector<string> commands;
void redirect(bool readIt, vector<string> test);
void pipeCommand(vector<string> command1,vector<string> command2);

};
#endif
