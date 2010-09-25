/***************************************************************************
 * Copyleft EDX. http://edx.shellmix.com, edx1010@gmail.com
 *
 * You like this software? Do what you want under the terms of
 * GNU General Public License.
 ***************************************************************************/

#include <lolIRC.hpp>

void lolIRC::Client::lolIRC_Client::Join(std::string chan_name, std::string key)
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

void lolIRC::Client::lolIRC_Client::Part(std::string chan_name)
{
	std::string message = "PART ";

	message += chan_name;
	message += "\r\n";
	connection.Write(message);
}

void lolIRC::Client::lolIRC_Client::Nick(std::string n)
{
	std::string message = "NICK ";

	message += n;
	message += "\r\n";
	connection.Write(message);
}

void lolIRC::Client::lolIRC_Client::User(std::string u, std::string rn, std::string mode)
{
	std::string message = "USER ";

	message += u;
	message += ' ';
	message += mode;
	message += ' ';
	message += u;
	message += " :";
	message += rn;
	message += "\r\n";

	connection.Write(message);
}
