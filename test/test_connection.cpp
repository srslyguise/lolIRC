#include <lolIRC_connection.hpp>
#include <string>
#include <iostream>
#include <exception>
#include <cstdlib>

using namespace lolIRC::Client;

int main(int argc, char ** argv)
{
	lolIRC_Connection c(argv[1], atoi(argv[2]));
	std::string message("LOOOL\n");

	try
	{
		c.Connect();

		c.Write(message);
		std::cout << c.Read() << std::endl;

		c.Close();
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
