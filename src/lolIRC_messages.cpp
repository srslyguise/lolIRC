#include <lolIRC.hpp>

void lolIRC::Client::lolIRC_Client::join(std::string chan_name, std::string key)
{
	std::string message = "JOIN ";

	message += chan_name;

	if(key != "")
	{
		message += ' ';
		message += key;
	}

	message += "\r\n";
	connection.Write(message);
}

void lolIRC::Client::lolIRC_Client::part(std::string chan_name)
{
	std::string message = "PART ";

	message += chan_name;
	message += "\r\n";
	connection.Write(message);
}
