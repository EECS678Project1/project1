#include <string>
#include <iostream>
#include <vector>

using namespace std;

#ifndef QUASH_H
#define QUASH_H

class Quash
{
private:
string m_path;
string m_home;
string m_dir;
public:
Quash();
~Quash();
void run();//add path
void launch(vector<string> args);
vector<string> splitArguments(string line);
void setPaths(string mPath);
void changeDir(string mdir);

};
#endif
