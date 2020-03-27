// This is a comment
/*
Comment
*/

#include <iostream>

int main(int argc, char* argv[])
{
std::cout<<"args:"<<argc<<std::endl;
	if(argc<2)
	{
	std::cout<<"Not right"<<std::endl;
	}else
	{
	std::cout<<argv[1];
	std::cout<<"Hello World"<<std::endl;
	std::cout<<"Hello World\n";
	}
}
