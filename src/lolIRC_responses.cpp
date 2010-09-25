/***************************************************************************
 * Copyleft EDX. http://edx.shellmix.com, edx1010@gmail.com
 *
 * You like this software? Do what you want under the terms of
 * GNU General Public License.
 ***************************************************************************/

#include <lolIRC.hpp>
#include <cstdlib>
#include <ctime>
#include <sstream>

//Textual start
//Ping start
void lolIRC::Client::lolIRC_Client::lolIRC_Response_Ping::doIt(lolIRC_Message& m)
{
	std::string message = "";

	message += "PONG";
	message += " ";
	message += m.parameters.front();
	message += "\r\n";

	client.connection.Write(message);
}
//Ping end

//Join start
void lolIRC::Client::lolIRC_Client::lolIRC_Response_Join::doIt(lolIRC_Message& m)
{
	lolIRC_Nick n(client.connection, m.prefix.start); //Init with connection and nickname of joined nick

	for(std::list<lolIRC_Channel>::iterator i = client.channels.begin(); i != client.channels.end(); i++)
	{
		if(!((*i).getChannelName().compare(m.parameters.front())))//If the channel is in the list...
		{
			(*i).nicknames.push_back(n); //Push a nickname in the nicknames list
			(*client.ch)(client, (*i).getChannelName(), USERJOIN, m.prefix.start.c_str());
		}
	}

	//Debugging
	std::string ch = m.parameters.front();
	std::string nk = m.prefix.start;
	DEBUG_PRINT(client[ch.c_str()][nk.c_str()].getUser());
}
//Join end
//Textual end

//Numeric start
//RPLTOPIC start
void lolIRC::Client::lolIRC_Client::lolIRC_Response_RPLTOPIC::doIt(lolIRC_Message& m)
{
	std::list<std::string>::iterator it = m.parameters.begin(); //List of parameters

	if((*it).compare(client.nick))
		DEBUG_PRINT("You're doing it wrong!");

	it++; //Channel name

	for(std::list<lolIRC_Channel>::iterator i = client.channels.begin(); i != client.channels.end(); i++)
	{
		if(!((*i).getChannelName().compare(*it)))
		{
			it++; //Topic
			(*i).topic = (*it);
			(*client.ch)(client, (*i).getChannelName(), CHANTOPIC, (*it).c_str());
		}
	}
}
//RPLTOPIC end

//RPLTOPIC_WHOTIME start
void lolIRC::Client::lolIRC_Client::lolIRC_Response_RPLTOPIC_WHOTIME::doIt(lolIRC_Message& m)
{
	std::list<std::string>::iterator it = m.parameters.begin(); //List of parameters

	if((*it).compare(client.nick))
		DEBUG_PRINT("You're doing it wrong!");

	it++; //Channel name

	for(std::list<lolIRC_Channel>::iterator i = client.channels.begin(); i != client.channels.end(); i++)
	{
		if(!((*i).getChannelName().compare(*it)))
		{
			it++; //Who set the topic
			(*i).topic_setby = (*it);
			
			it++; //Date
			(*i).topic_setby_date = std::atoi(const_cast<char*>((*it).c_str())); //Get POSIX timestamp
			(*client.ch)(client, (*i).getChannelName(), CHANTOPICSETBY, (*i).topic_setby.c_str(), std::ctime(&((*i).topic_setby_date)));
		}
	}
}
//RPLTOPIC_WHOTIME end

//RPLNAMREPLY start
void lolIRC::Client::lolIRC_Client::lolIRC_Response_RPLNAMREPLY::doIt(lolIRC_Message& m)
{
	std::list<std::string>::iterator it = m.parameters.begin(); //List of parameters

	if((*it).compare(client.nick))
		DEBUG_PRINT("You're doing it wrong!");

	it++; // Symbol '='
	it++; //Channel name

	for(std::list<lolIRC_Channel>::iterator i = client.channels.begin(); i != client.channels.end(); i++)
	{
		if(!((*i).getChannelName().compare(*it)))
		{
			it++; //Start of nicknames
			fillNicknames((*i), (*it));
		}
	}

}

void lolIRC::Client::lolIRC_Client::lolIRC_Response_RPLNAMREPLY::fillNicknames(lolIRC_Channel& c, std::string s)
{
	std::istringstream iss(s);
	std::string tmp;

	while(iss)
	{
		iss >> tmp;
		
		if(!(alreadyJoined(c, tmp)))
		{
			lolIRC_Nick tmpnick(client.connection, tmp);
			c.nicknames.push_back(tmpnick);
		}
		tmp = "";
	}
}

bool lolIRC::Client::lolIRC_Client::lolIRC_Response_RPLNAMREPLY::alreadyJoined(lolIRC_Channel& c, std::string& nick)
{
	for(std::list<lolIRC_Nick>::iterator i = c.nicknames.begin(); i != c.nicknames.end(); i++)
		if(!((*i).getUser().compare(nick)))
			return true;
	return false;
}
//RPLNAMREPLY end

//RPLENDOFNAMES start
void lolIRC::Client::lolIRC_Client::lolIRC_Response_RPLENDOFNAMES::doIt(lolIRC_Message& m)
{
	std::list<std::string>::iterator it = m.parameters.begin(); //List of parameters

	if((*it).compare(client.nick))
		DEBUG_PRINT("You're doing it wrong!");

	it++; //Channel name

	for(std::list<lolIRC_Channel>::iterator i = client.channels.begin(); i != client.channels.end(); i++)
	{
		if(!((*i).getChannelName().compare(*it)))
			callHandler(*i);
	}
}

void lolIRC::Client::lolIRC_Client::lolIRC_Response_RPLENDOFNAMES::callHandler(lolIRC_Channel& c)
{
	std::string us; //The string contains the nicknames separated by a ' '

	for(std::list<lolIRC_Nick>::iterator i = c.nicknames.begin(); i != c.nicknames.end(); i++)
		us += (((*i).getUser()) + ' ');

	DEBUG_PRINT(us);

	(*client.ch)(client, c.getChannelName(), USERLIST, us.c_str());
}
//RPLENDOFNAMES end
//Numeric end

//Errors start

void lolIRC::Client::lolIRC_Client::lolIRC_Response_ERR_NEEDMOREPARAMS::doIt(lolIRC_Message& m)
{
	std::list<std::string>::iterator it = m.parameters.begin(); //List of parameters

	it++; //Command Name
	DEBUG_PRINT("ERR_NEEDMOREPARAMS -> " << (*it) << " -> You're doing it very wrong!!1!");
}

void lolIRC::Client::lolIRC_Client::lolIRC_Response_ERR_BANNEDFROMCHAN::doIt(lolIRC_Message& m)
{
	std::list<std::string>::iterator it = m.parameters.begin(); //List of parameters

	it++; //Channel name

	for(std::list<lolIRC_Channel>::iterator i = client.channels.begin(); i != client.channels.end(); i++)
	{
		if(!((*i).getChannelName().compare(*it)))
		{
			DEBUG_PRINT("ERR_BANNEDFROMCHAN -> Erasing channel -> " << (*i).getChannelName());

			(*client.sh)(client, ERR_BANNEDFROMCHAN, (*it).c_str());
			client.channels.erase(i);
			return;
		}
	}
}

void lolIRC::Client::lolIRC_Client::lolIRC_Response_ERR_INVITEONLYCHAN::doIt(lolIRC_Message& m)
{
	std::list<std::string>::iterator it = m.parameters.begin(); //List of parameters

	it++; //Channel name

	for(std::list<lolIRC_Channel>::iterator i = client.channels.begin(); i != client.channels.end(); i++)
	{
		if(!((*i).getChannelName().compare(*it)))
		{
			DEBUG_PRINT("ERR_INVITEONLYCHAN -> Erasing channel -> " << (*i).getChannelName());

			(*client.sh)(client, ERR_INVITEONLYCHAN, (*it).c_str());
			client.channels.erase(i);
			return;
		}
	}
}

void lolIRC::Client::lolIRC_Client::lolIRC_Response_ERR_BADCHANNELKEY::doIt(lolIRC_Message& m)
{
	std::list<std::string>::iterator it = m.parameters.begin(); //List of parameters

	it++; //Channel name

	for(std::list<lolIRC_Channel>::iterator i = client.channels.begin(); i != client.channels.end(); i++)
	{
		if(!((*i).getChannelName().compare(*it)))
		{
			DEBUG_PRINT("ERR_BADCHANNELKEY -> Erasing channel -> " << (*i).getChannelName());

			(*client.sh)(client, ERR_BADCHANNELKEY, (*it).c_str());
			client.channels.erase(i);
			return;
		}
	}
}

void lolIRC::Client::lolIRC_Client::lolIRC_Response_ERR_CHANNELISFULL::doIt(lolIRC_Message& m)
{
	std::list<std::string>::iterator it = m.parameters.begin(); //List of parameters

	it++; //Channel name

	for(std::list<lolIRC_Channel>::iterator i = client.channels.begin(); i != client.channels.end(); i++)
	{
		if(!((*i).getChannelName().compare(*it)))
		{
			DEBUG_PRINT("ERR_CHANNELISFULL -> Erasing channel -> " << (*i).getChannelName());

			(*client.sh)(client, ERR_CHANNELISFULL, (*it).c_str());
			client.channels.erase(i);
			return;
		}
	}
}

void lolIRC::Client::lolIRC_Client::lolIRC_Response_ERR_BADCHANMASK::doIt(lolIRC_Message& m)
{
	std::list<std::string>::iterator it = m.parameters.begin(); //List of parameters

	it++; //Channel name

	for(std::list<lolIRC_Channel>::iterator i = client.channels.begin(); i != client.channels.end(); i++)
	{
		if(!((*i).getChannelName().compare(*it)))
		{
			DEBUG_PRINT("ERR_BADCHANMASK -> Erasing channel -> " << (*i).getChannelName());

			(*client.sh)(client, ERR_BADCHANMASK, (*it).c_str());
			client.channels.erase(i);
			return;
		}
	}
}

void lolIRC::Client::lolIRC_Client::lolIRC_Response_ERR_NOSUCHCHANNEL::doIt(lolIRC_Message& m)
{
	std::list<std::string>::iterator it = m.parameters.begin(); //List of parameters

	it++; //Channel name

	for(std::list<lolIRC_Channel>::iterator i = client.channels.begin(); i != client.channels.end(); i++)
	{
		if(!((*i).getChannelName().compare(*it)))
		{
			DEBUG_PRINT("ERR_NOSUCHCHANNEL -> Erasing channel -> " << (*i).getChannelName());

			(*client.sh)(client, ERR_NOSUCHCHANNEL, (*it).c_str());
			client.channels.erase(i);
			return;
		}
	}
}

void lolIRC::Client::lolIRC_Client::lolIRC_Response_ERR_TOOMANYCHANNELS::doIt(lolIRC_Message& m)
{
	std::list<std::string>::iterator it = m.parameters.begin(); //List of parameters

	it++; //Channel name

	for(std::list<lolIRC_Channel>::iterator i = client.channels.begin(); i != client.channels.end(); i++)
	{
		if(!((*i).getChannelName().compare(*it)))
		{
			DEBUG_PRINT("ERR_TOOMANYCHANNELS -> Erasing channel -> " << (*i).getChannelName());

			(*client.sh)(client, ERR_TOOMANYCHANNELS, (*it).c_str());
			client.channels.erase(i);
			return;
		}
	}
}

void lolIRC::Client::lolIRC_Client::lolIRC_Response_ERR_TOOMANYTARGETS::doIt(lolIRC_Message& m)
{
	std::list<std::string>::iterator it = m.parameters.begin(); //List of parameters

	DEBUG_PRINT("ERR_TOOMANYTARGETS -> You're doing it very wrong!!1!");
	it++; //Target

	(*client.sh)(client, ERR_TOOMANYTARGETS, (*it).c_str());
}

void lolIRC::Client::lolIRC_Client::lolIRC_Response_ERR_UNAVAILRESOURCE::doIt(lolIRC_Message& m)
{
	std::list<std::string>::iterator it = m.parameters.begin(); //List of parameters

	it++; //Channel/nick name

	for(std::list<lolIRC_Channel>::iterator i = client.channels.begin(); i != client.channels.end(); i++)
	{
		if(!((*i).getChannelName().compare(*it)))
		{
			DEBUG_PRINT("ERR_UNAVAILRESOURCE -> Erasing channel -> " << (*i).getChannelName());

			(*client.sh)(client, ERR_UNAVAILRESOURCE, (*it).c_str());
			client.channels.erase(i);
			return;
		}
	}
}

void lolIRC::Client::lolIRC_Client::lolIRC_Response_ERR_NOTONCHANNEL::doIt(lolIRC_Message& m)
{
	std::list<std::string>::iterator it = m.parameters.begin(); //List of parameters

	it++; //Channel name

	DEBUG_PRINT("ERR_NOTONCHANNEL -> " << (*it) << " -> You're doing it very wrong!!1!");
}

//Errors end
