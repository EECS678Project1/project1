#include <string>
#include <iostream>

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
void launch(const char* args);

};
#endif
