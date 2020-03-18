#include <string>
#include <iostream>
#include <vector>

using namespace std;

#ifndef QUASH_H
#define QUASH_H

class Quash
{
private:

public:
Quash();
~Quash();
void run();//add path
void launch(vector<string> args);
vector<string> splitArguments(string line);

};
#endif
