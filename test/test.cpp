#include <lolIRC.hpp>
#include <iostream>
#include <exception>
#include <cstdarg>
#include <sstream>

using namespace lolIRC::Client;

uint32_t sh(lolIRC_Client&, lolIRC_Client::lolIRC_Server_Response, ...);
uint32_t ch(lolIRC_Client&, std::string, lolIRC_Client::lolIRC_Channel_Response, ...);
uint32_t mh(lolIRC_Client&, std::string, std::string);

int main()
{
	lolIRC_Client client("127.0.0.1", 6667);

	try
	{
		client.setServerHandler(sh);
		client.setChannelHandler(ch);
		client.setMsgHandler(mh);

		client.setNick("lolz");
		client.setUser("lolz");

		client.Connect();
		client.joinChannel("#lolling", "lollingkey");
		client["#lolling"].setTopic("LOL");
		client["#lolling"].Send("LULZ");
		
		while(client)
		{
		}

		client.Close();
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << std::endl;
		client.Close();
	}

	return 0;
}

uint32_t sh(lolIRC_Client& c, lolIRC_Client::lolIRC_Server_Response r, ...)
{
	va_list ap;

	va_start(ap, r);

	switch(r)
	{
		case lolIRC_Client::ERR_NEEDMOREPARAMS:
			std::cout << "Error, " << va_arg(ap, char *) << ": Not enough parameters" << std::endl;
		break;
	}

	va_end(ap);
}

uint32_t ch(lolIRC_Client& c, std::string name, lolIRC_Client::lolIRC_Channel_Response r, ...)
{
	va_list ap;
	std::stringstream ss;
	std::string tmp;

	va_start(ap, r);

	switch(r)
	{
		case lolIRC_Client::CHANTOPIC:
			std::cout << name << " -> Topic: " << va_arg(ap, char *) << std::endl;
		break;

		case lolIRC_Client::CHANTOPICSETBY:
			std::cout << name << " -> Topic set by: " << va_arg(ap, char *);
			std::cout << ", date: " << va_arg(ap, char *);
		break;

		case lolIRC_Client::USERLIST:
			ss << va_arg(ap, char *);
			std::cout << name << " -> User List: ";

			while(ss)
			{
				ss >> tmp;
				std::cout << tmp << " ";
				tmp = "";
			}
			std::cout << std::endl;
		break;

		case lolIRC_Client::USERJOIN:
			std::cout << name << " -> " << va_arg(ap, char *) << " has joined" << std::endl;
		break;
	}

	va_end(ap);
}

uint32_t mh(lolIRC_Client& c, std::string nick, std::string msg)
{
}
