/***************************************************************************
 * Copyleft EDX. http://edx.shellmix.com, edx1010@gmail.com
 *
 * You like this software? Do what you want under the terms of
 * GNU General Public License.
 ***************************************************************************/

#ifndef LOLIRC_PARSER_H
#define LOLIRC_PARSER_H

#include <common.hpp>
#include <list>

typedef struct lolIRC_Message{ //This struct defines a message
	struct prefix {
		std::string start;
		std::string user;
		std::string host;
	}prefix;
	struct command {
		std::string s_command; //Textual command
		uint16_t n_command; //Numeric command
	}command;
	std::list<std::string> parameters; //List of parameters
}lolIRC_Message;

namespace lolIRC {
	namespace Client {
		
		class lolIRC_Parser //This class parses the message and return a list of struct lolIRC_Message
		{
			public:
				lolIRC_Parser();
				lolIRC_Parser(std::string);
				~lolIRC_Parser();

				uint32_t setParseString(std::string);

				std::list<lolIRC_Message> Parse();

			private:
				void initMessage(lolIRC_Message&); //Initialize the struct

				void fillPrefix(std::string, lolIRC_Message&); //Fill the prefix of struct
				void fillCommand(std::string, lolIRC_Message&); //Fill the command of struct
				void fillArguments(std::string, lolIRC_Message&); //Fill the arguments of struct

				uint32_t isValidNick(std::string); //Check if the Nickname is a valid IRC nickname
				uint32_t isValidUser(std::string); //Check if the Username is a valid IRC username
				uint32_t isValidParameter(std::string); //Check if the parameter is a valid IRC parameter

				std::string str;
		};

	}
}

#endif
