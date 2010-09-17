/***************************************************************************
 * Copyleft EDX. http://edx.shellmix.com, edx1010@gmail.com
 *
 * You like this software? Do what you want under the terms of
 * GNU General Public License.
 ***************************************************************************/

#include <lolIRC_parser.hpp>
#include <cstring>
#include <sstream>
#include <cctype>

lolIRC::Client::lolIRC_Parser::lolIRC_Parser() : str("") //Unuseful constructor :(
{
}

lolIRC::Client::lolIRC_Parser::lolIRC_Parser(std::string s)
{
	setParseString(s);
}

lolIRC::Client::lolIRC_Parser::~lolIRC_Parser()
{
}

uint32_t lolIRC::Client::lolIRC_Parser::setParseString(std::string s)
{
	str += s;
}

std::list<lolIRC_Message> lolIRC::Client::lolIRC_Parser::Parse()
{
	std::istringstream iss(str); //All the message
	std::string response; //Single message

	std::string prefix, command;
	std::string arguments;
	std::list<lolIRC_Message> message;
	lolIRC_Message tmp; //Temporary struct

	std::string truncated = ""; //Truncated messages

	char buf[512] = {0}; //Buffer needed to extract a single message from the complete message

	while(iss)
	{
		iss.getline(buf, 512, '\n'); //Get the first response in the message
		DEBUG_PRINT(buf);

		DEBUG_PRINT((int)buf[strlen(buf) - 1]);

		response = buf;
		std::istringstream iss2(response);

		arguments = "";

		if(response == "")
			break;

		//For now, this part is useless
		/*
		if(buf[strlen(buf)-1] != '\r')
		{
			DEBUG_PRINT("OMG truncated!!1!1!");
			truncated = buf;
			break;
		}
		*/

		if(response[0] == ':')
			iss2 >> prefix;

		iss2 >> command;

		do
		{
			std::string sub;
			iss2 >> sub;
			arguments += sub + ' ';
		}while(iss2); //Fill the arguments

		initMessage(tmp); //Initalize the temporary struct

		try
		{
			fillPrefix(prefix, tmp);
			fillCommand(command, tmp);
			fillArguments(arguments, tmp);
		}
		catch(std::exception& e)
		{
			throw dynamic_cast<lolIRC_Exception&>(e);
		}

		message.push_back(tmp);
		memset(buf, 0, 512);
		truncated = "";
	}
	
	if(truncated == "")
		str = "";
	else
		str = truncated;

	DEBUG_PRINT(str);

	return message;
}

void lolIRC::Client::lolIRC_Parser::initMessage(lolIRC_Message& m)
{
	m.prefix.start = "";
	m.prefix.user = "";
	m.prefix.host = "";

	m.command.s_command = "";
	m.command.n_command = 0;

	m.parameters.clear();
}

void lolIRC::Client::lolIRC_Parser::fillPrefix(std::string s, lolIRC_Message& m)
{
	if(s[0] != ':') //First character MUST be a colon
		return;

	if(s.find('!') != s.npos) //If there is a '!'...
		m.prefix.start = s.substr(1, (s.find('!') - 1)); //Prefix is from start to '!'
	else if(s.find('@') != s.npos) //else if there is a '@'...
		m.prefix.start = s.substr(1, (s.find('@') - 1)); //Prefix is from start to '@'
	else //The prefix is complete
		m.prefix.start = s.substr(1);

	if(s.find('!') != s.npos) //If there is a '!'...
	{
		if(s.find('@') != s.npos) //...and a '@'...
		{
			m.prefix.user = s.substr((s.find('!') + 1), ((s.find('@')) - (s.find('!')) - 1)); //The user is from '!' to '@'

		m.prefix.host = s.substr((s.find('@') + 1)); //The host is from '@' to the end
		}
		else //The User is from '!' to end
			m.prefix.user = s.substr(s.find('!') + 1);
	}

	if(s.find('@') != s.npos) //If there is a '@'...
		m.prefix.host = s.substr(s.find('@') + 1); //The host is from '@' to the end

	DEBUG_PRINT(m.prefix.start);
	DEBUG_PRINT(m.prefix.user);
	DEBUG_PRINT(m.prefix.host);

	try //Check if nick and user are valid
	{
		isValidNick(m.prefix.start);
		isValidUser(m.prefix.user);
	}
	catch(std::exception& e)
	{
		throw dynamic_cast<lolIRC_Exception&>(e);
	}
}

void lolIRC::Client::lolIRC_Parser::fillCommand(std::string s, lolIRC_Message& m)
{
	std::istringstream iss(s);

	if(isdigit(s[0]) && isdigit(s[1]) && isdigit(s[2]))
		iss >> m.command.n_command; //Command is in numeric form
	else
		iss >> m.command.s_command; //String form

	DEBUG_PRINT(m.command.n_command);
	DEBUG_PRINT(m.command.s_command);
}

uint32_t lolIRC::Client::lolIRC_Parser::isValidNick(std::string s)
{
	size_t f = s.find_first_not_of("0123456789-[]\\`^{}ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"); //Valid characters

	if(!isalpha(s[0])) //First character MUST be an alphabetic character
		throw lolIRC_Exception("lolIRC: Parser: Invalid nick");

	if(f != s.npos)
		throw lolIRC_Exception("lolIRC: Parser: Invalid characters in nick");
	return 0;
}

uint32_t lolIRC::Client::lolIRC_Parser::isValidUser(std::string s)
{
	size_t f = s.find_first_of(" \x0\r\n"); //Invalid characters

	if(f != s.npos)
		throw lolIRC_Exception("lolIRC: Parser: Invalid characters in user");
	return 0;
}

void lolIRC::Client::lolIRC_Parser::fillArguments(std::string s, lolIRC_Message& m)
{
	std::istringstream iss(s);
	std::string tmp = "";
	std::string last = "";

	do
	{
		iss >> tmp;

		if(tmp == "")
			break;

		if(tmp[0] != ':')
			m.parameters.push_back(tmp); //The parameter doesn't contain spaces
		else
		{
			do
			{
				last += tmp + ' ';
				iss >> tmp;
			}while(iss);

			last = last.substr(1); //Remove the ':'

			m.parameters.push_back(last);
		}

		tmp = "";

	}while(iss);

	for(std::list<std::string>::iterator i = m.parameters.begin(); i != m.parameters.end(); i++)
	{
		DEBUG_PRINT(*i);

		try //Every parameter must be valid
		{
			isValidParameter(*i);
		}
		catch(std::exception& e)
		{
			throw dynamic_cast<lolIRC_Exception&>(e);
		}
	}
}

uint32_t lolIRC::Client::lolIRC_Parser::isValidParameter(std::string s)
{
	size_t f = s.find_first_of("\x0\r\n"); //Invalid characters

	if(f != s.npos)
		throw lolIRC_Exception("lolIRC: Parser: Invalid characters in parameter");
	return 0;
}
