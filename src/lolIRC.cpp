/***************************************************************************
 * Copyleft EDX. http://edx.shellmix.com, edx1010@gmail.com
 *
 * You like this software? Do what you want under the terms of
 * GNU General Public License.
 ***************************************************************************/

#include <lolIRC.hpp>
#include <cstring>
#include <cctype>

//lolIRC_Client Start

lolIRC::Client::lolIRC_Client::lolIRC_Client() : sh(NULL), ch(NULL), mh(NULL), server_name(""), nick(""), user(""), port(6667), r_thread(0)
{
}

lolIRC::Client::lolIRC_Client::lolIRC_Client(std::string s, uint16_t port = 6667): sh(NULL), ch(NULL), mh(NULL), nick(""), user(""), r_thread(0)
{
	server_name = s;
	this->port = port;
}

lolIRC::Client::lolIRC_Client::~lolIRC_Client()
{
	this->Close();
}

uint32_t lolIRC::Client::lolIRC_Client::setServerName(std::string s)
{
	server_name = s;
}

std::string lolIRC::Client::lolIRC_Client::getServerName() const
{
	return server_name;
}

uint32_t lolIRC::Client::lolIRC_Client::setServerPort(uint16_t p)
{
	port = p;
}

uint16_t lolIRC::Client::lolIRC_Client::getServerPort() const
{
	return port;
}

uint32_t lolIRC::Client::lolIRC_Client::setServerHandler(lolIRC_ServerHandler p)
{
	sh = p;
}

uint32_t lolIRC::Client::lolIRC_Client::setChannelHandler(lolIRC_ChannelHandler p)
{
	ch = p;
}

uint32_t lolIRC::Client::lolIRC_Client::setMsgHandler(lolIRC_MsgHandler p)
{
	mh = p;
}

uint32_t lolIRC::Client::lolIRC_Client::setNick(std::string n)
{
	nick = n;
}

std::string lolIRC::Client::lolIRC_Client::getNick() const
{
	return nick;
}

uint32_t lolIRC::Client::lolIRC_Client::setUser(std::string u)
{
	user = u;
}

std::string lolIRC::Client::lolIRC_Client::getUser() const
{
	return user;
}

uint32_t lolIRC::Client::lolIRC_Client::Connect()
{
	uint32_t ret = 0;

	if(!((sh != NULL) && (ch != NULL) && (mh != NULL)))
		throw lolIRC_Exception("lolIRC: lolIRC_Client: You must set the response handlers before connect"); //Everything MUST be set!

	if(!((nick != "") && (user != "")))
		throw lolIRC_Exception("lolIRC: lolIRC_Client: You must set nick and user before connect"); //Everything MUST be set!

	connection.setHost(server_name);
	connection.setPort(port);

	try
	{
		connection.Connect();
	}
	catch(std::exception& e)
	{
		throw dynamic_cast<lolIRC_Exception&>(e);
	}

	if((ret = pthread_create(&r_thread, NULL, (void* (*)(void*))&lolIRC_Client::threadHandler, (void*)this)) != 0)
	{
		r_thread = 0;
		this->Close();
		throw lolIRC_Exception("lolIRC: lolIRC_Client: Unable to create thread");
	}

	initResponses(); //Init the response list
}

void lolIRC::Client::lolIRC_Client::Close()
{
	connection.Close();

	sh = NULL;
	ch = NULL;
	mh = NULL;

	freeResponses(); //Free the dynamic memory

	r_thread = 0;
}

lolIRC::Client::lolIRC_Channel& lolIRC::Client::lolIRC_Client::operator[](const char * chan_name)
{
	for(std::list<lolIRC_Channel>::iterator i = channels.begin(); i != channels.end(); i++) //Check all the channels...
		if(!((*i).getChannelName().compare(chan_name)))//If the channel exist...
			return *i; //Returns the found channel

	throw lolIRC_Exception("lolIRC: lolIRC_Client: Invalid channel name"); //Invalid channel
}

lolIRC::Client::lolIRC_Client::operator bool() const //Check if the connection is alive
{
	if(connection)
		return true;
	else
		return false;
}

uint32_t lolIRC::Client::lolIRC_Client::joinChannel(std::string chan_name, std::string key)
{
	lolIRC_Channel c(connection, chan_name, this->nick);

	for(std::list<lolIRC_Channel>::iterator i = channels.begin(); i != channels.end(); i++) //Check all the channels...
		if(!((*i).getChannelName().compare(chan_name)))//If already joined...
			throw lolIRC_Exception("lolIRC: lolIRC_Client: Already joined to channel");

	channels.push_back(c);
	join(chan_name, key);
}

void lolIRC::Client::lolIRC_Client::closeChannel(std::string chan_name)
{
	for(std::list<lolIRC_Channel>::iterator i = channels.begin(); i != channels.end(); i++) //Check all the channels...
		if(!((*i).getChannelName().compare(chan_name))) //If the channel is in the list
		{
			channels.erase(i); //Delete it
			part(chan_name);
			return;
		}

	throw lolIRC_Exception("lolIRC: lolIRC_Client: Channel name not found");
}

//lolIRC_Client End
//lolRC_Client::lolIRC_Response Start

lolIRC::Client::lolIRC_Client::lolIRC_Response::lolIRC_Response(lolIRC_Client& c) : client(c)
{
}

//lolIRC_Client::lolIRC_Response End
//lolIRC_Client::lolIRC_Response_Numeric Start

lolIRC::Client::lolIRC_Client::lolIRC_Response_Numeric::lolIRC_Response_Numeric(lolIRC_Client& c, uint16_t n_code) : lolIRC_Response(c), code(n_code)
{
}

uint16_t lolIRC::Client::lolIRC_Client::lolIRC_Response_Numeric::getCode() const
{
	return code;
}

//lolIRC_Client::lolIRC_Response_Numeric End
//lolIRC_Client::lolIRC_Response_Textual Start

lolIRC::Client::lolIRC_Client::lolIRC_Response_Textual::lolIRC_Response_Textual(lolIRC_Client& c, std::string n) : lolIRC_Response(c), name(n)
{
}

std::string lolIRC::Client::lolIRC_Client::lolIRC_Response_Textual::getName() const
{
	return name;
}

//lolIRC_Client::lolIRC_Response_Textual End
//lolIRC_Client::Other Start

void* lolIRC::Client::lolIRC_Client::threadHandler(void * arg)
{
	std::string message;
	lolIRC_Parser p;
	std::list<lolIRC_Message> m;
	std::string uppercase_response = "";

	while(1) //While not canceled
	{

	try
	{
		try
		{
			message = connection.Read(); //Read a message
		}
		catch(std::exception& e)
		{
			throw dynamic_cast<lolIRC_Exception&>(e);
		}
		p.setParseString(message);

		m = p.Parse(); //Get the message list

		for(std::list<lolIRC_Message>::iterator I = m.begin(); I != m.end(); I++)
		{
			for(std::list<lolIRC_Response*>::iterator i = responses.begin(); i != responses.end(); i++) //Check all the responses
			{
				lolIRC_Response_Textual* rt = dynamic_cast<lolIRC_Response_Textual*>(*i); //Downcast from lolIRC_Response to lolIRC_Response_Textual
				lolIRC_Response_Numeric* rn = dynamic_cast<lolIRC_Response_Numeric*>(*i); //Downcast from lolIRC_Response to lolIRC_Response_Numeric

				if(rt) //If textual response downcast had success...
				{
					uppercase_response = upper((*I).command.s_command); //Transform the response command in uppercase

					if(!(rt->getName().compare(uppercase_response)))
					{
						DEBUG_PRINT("Calling -> " << rt->getName());
						rt->doIt(*I);//Call the virtual method on Abstract class lolIRC_Response_Textual
					}
				}
				else //numeric response
				{
					if(rn->getCode() == (*I).command.n_command)
					{
						DEBUG_PRINT("Calling -> " << rn->getCode());
						rn->doIt(*I);//Call the virtual method on Abstract class lolIRC_Response_Numeric
					}
				}
			}
		}
		
	}
	catch(std::exception& e)
	{
		throw dynamic_cast<lolIRC_Exception&>(e);
	}

	}
}

void lolIRC::Client::lolIRC_Client::initResponses()
{
	//To add a reponse just add a line like below with specific class and response type
	
	//Textual
	responses.push_back(new lolIRC_Response_Ping(*this, "PING"));
	responses.push_back(new lolIRC_Response_Join(*this, "JOIN"));

	//Numeric
	responses.push_back(new lolIRC_Response_RPLTOPIC(*this, 332));
	responses.push_back(new lolIRC_Response_RPLTOPIC_WHOTIME(*this, 333)); //Not standard, defined by ircu
	responses.push_back(new lolIRC_Response_RPLNAMREPLY(*this, 353));
	responses.push_back(new lolIRC_Response_RPLENDOFNAMES(*this, 366));

	//Errors
	responses.push_back(new lolIRC_Response_ERR_NEEDMOREPARAMS(*this, 461));
	responses.push_back(new lolIRC_Response_ERR_BANNEDFROMCHAN(*this, 474));
	responses.push_back(new lolIRC_Response_ERR_INVITEONLYCHAN(*this, 473));
	responses.push_back(new lolIRC_Response_ERR_BADCHANNELKEY(*this, 475));
	responses.push_back(new lolIRC_Response_ERR_CHANNELISFULL(*this, 471));
	responses.push_back(new lolIRC_Response_ERR_BADCHANMASK(*this, 476));
	responses.push_back(new lolIRC_Response_ERR_NOSUCHCHANNEL(*this, 403));
	responses.push_back(new lolIRC_Response_ERR_TOOMANYCHANNELS(*this, 405));
	responses.push_back(new lolIRC_Response_ERR_TOOMANYTARGETS(*this, 407));
	responses.push_back(new lolIRC_Response_ERR_UNAVAILRESOURCE(*this, 437));
	responses.push_back(new lolIRC_Response_ERR_NOTONCHANNEL(*this, 442));
}

void lolIRC::Client::lolIRC_Client::freeResponses() //Free the dynamic memory
{
	for(std::list<lolIRC_Response*>::iterator i = responses.begin(); i != responses.end(); i++)
		delete *i;
}

std::string lolIRC::Client::lolIRC_Client::upper(std::string s)
{
	for(std::string::iterator i = s.begin(); i != s.end(); i++)
		(*i) = std::toupper(*i);

	return s;
}

//lolIRC_Client::Other End
