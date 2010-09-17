/***************************************************************************
 * Copyleft EDX. http://edx.shellmix.com, edx1010@gmail.com
 *
 * You like this software? Do what you want under the terms of
 * GNU General Public License.
 ***************************************************************************/

#include <lolIRC_channel.hpp>

lolIRC::Client::lolIRC_Channel::~lolIRC_Channel()
{
}

lolIRC::Client::lolIRC_Channel::lolIRC_Channel(lolIRC_Connection& c) : name(""), nick(""), connection(c)
{
}

lolIRC::Client::lolIRC_Channel::lolIRC_Channel(lolIRC_Connection& c, std::string n_name, std::string n_nick) : connection(c)
{
	this->name = n_name;
	this->nick = n_nick;
}

uint32_t lolIRC::Client::lolIRC_Channel::setChannelName(std::string n)
{
	this->name = n;
}

std::string lolIRC::Client::lolIRC_Channel::getChannelName() const
{
	return this->name;
}

uint32_t lolIRC::Client::lolIRC_Channel::setNick(std::string n)
{
	this->nick = n;
}

std::string lolIRC::Client::lolIRC_Channel::getNick() const
{
	return this->nick;
}

void lolIRC::Client::lolIRC_Channel::Invite(std::string username)
{
	std::string message = "INVITE ";

	if(username != "")
		message += username;
	else
		return;

	message += ' ';
	message += name;
	message += "\r\n";

	connection.Write(message);
}

void lolIRC::Client::lolIRC_Channel::ModeChan(std::string mode, std::string option)
{
}

std::list<std::string> lolIRC::Client::lolIRC_Channel::Names()
{
	std::list<std::string> names;

	for(std::list<lolIRC_Nick>::iterator i = nicknames.begin(); i != nicknames.end(); i++)
		names.push_back((*i).getUser());

	return names;
}

std::string lolIRC::Client::lolIRC_Channel::getTopic() const
{
	return this->topic;
}

void lolIRC::Client::lolIRC_Channel::setTopic(std::string title)
{
	std::string message = "TOPIC ";

	message += this->name;
	message += " :";
	message += title;
	message += "\r\n";

	connection.Write(message);

}

std::string lolIRC::Client::lolIRC_Channel::getTopicSetBy() const
{
	return this->topic_setby;
}

std::string lolIRC::Client::lolIRC_Channel::getTopicSetDate() const
{
	return ctime(&this->topic_setby_date);
}

void lolIRC::Client::lolIRC_Channel::Send(std::string str)
{
	std::string message = "PRIVMSG ";
	
	message += this->name;
	message += " :";
	message += str;
	message += "\r\n";

	connection.Write(message);
}

lolIRC::Client::lolIRC_Nick& lolIRC::Client::lolIRC_Channel::operator[](const char * nickname)
{
	for(std::list<lolIRC_Nick>::iterator i = this->nicknames.begin(); i != this->nicknames.end(); i++)
		if(!((*i).getUser().compare(nickname)))
			return *i;

	throw lolIRC_Exception("lolIRC: lolIRC_Channel: Invalid nickname");
}
