#include <lolIRC.hpp>
#include <iostream>
#include <exception>
#include <cstdarg>
#include <sstream>
#include <list>

using namespace lolIRC::Client;

uint32_t sh(lolIRC_Client&, lolIRC_Client::lolIRC_Server_Response, ...);
uint32_t ch(lolIRC_Client&, std::string, lolIRC_Client::lolIRC_Channel_Response, ...);
uint32_t mh(lolIRC_Client&, std::string, std::string);

int main(int argc, char ** argv)
{
	lolIRC_Client client(argv[1], 6667);

	try
	{
		client.setServerHandler(sh);
		client.setChannelHandler(ch);
		client.setMsgHandler(mh);

		client.setNick("lolzlolzlalz");
		client.setUser("lolzlolzlalz");
		client.setRealName("lolz lulz");

		client.Connect();
		/*client.joinChannel("#lolling", "lollingkey");
		client["#lolling"].setTopic("LOL");
		client["#lolling"] << "LULZ" << "LOLZ"; */
		
		while(client)
		{
			sleep(1);
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
		case lolIRC_Client::ERR_BADCHANNELKEY:
			std::cout << va_arg(ap, char *) << " -> Bad channel key" << std::endl;
		break;
	}

	va_end(ap);
}

uint32_t ch(lolIRC_Client& c, std::string name, lolIRC_Client::lolIRC_Channel_Response r, ...)
{
	va_list ap;
	std::stringstream ss;
	std::string tmp;
	std::list<std::string> names = c[name.c_str()].Names();

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
			std::cout << "Users on channel " << name << ":" << std::endl;
			for(std::list<std::string>::iterator i = names.begin(); i != names.end(); i++)
				std::cout << *i << std::endl;
		break;

		case lolIRC_Client::USERPART:
			std::cout << name << " -> " << va_arg(ap, char *) << " has left: ";
			std::cout << va_arg(ap, char *) << std::endl;
			std::cout << "Users on channel " << name << ":" << std::endl;
			for(std::list<std::string>::iterator i = names.begin(); i != names.end(); i++)
				std::cout << *i << std::endl;
		break;
	}

	va_end(ap);
}

uint32_t mh(lolIRC_Client& c, std::string nick, std::string msg)
{
}
