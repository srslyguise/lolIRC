#include <lolIRC_parser.hpp>
#include <string>
#include <iostream>
#include <exception>
#include <cstdlib>

using namespace lolIRC::Client;

int main(int argc, char ** argv)
{
	std::string lol = argv[1];
	lol += "\r\n";
	lol += argv[2];

	lolIRC_Parser c(lol);

	try
	{
		c.Parse();
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
