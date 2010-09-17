/***************************************************************************
 * Copyleft EDX. http://edx.shellmix.com, edx1010@gmail.com
 *
 * You like this software? Do what you want under the terms of
 * GNU General Public License.
 ***************************************************************************/

#ifndef LOLIRC_H
#define LOLIRC_H

#include <common.hpp>
#include <lolIRC_channel.hpp>
#include <lolIRC_parser.hpp>
#include <list>
#include <pthread.h>

#define DEFINE_RESPONSE_TEXTUAL(X) public: X(lolIRC_Client& c, std::string n) : lolIRC_Response_Textual(c, n) {} virtual void doIt(lolIRC_Message&);

#define DEFINE_RESPONSE_NUMERIC(X) public: X(lolIRC_Client& c, uint16_t n) : lolIRC_Response_Numeric(c, n) {} virtual void doIt(lolIRC_Message&);

namespace lolIRC {
	namespace Client {

		class lolIRC_Client //Main library class
		{
			public:
				typedef enum lolIRC_Channel_Response{
					USERJOIN = 1,
					USERQUIT,
					USERMODE,
					USERMSG,
					USERNICK,
					USERKICK,
					USERLIST,
					CHANTOPIC = 20,
					CHANTOPICSETBY,
					CHANMODE
				}lolIRC_Channel_Response; //All kind of possible responses for the channel
				typedef enum lolIRC_Server_Response{
					USERINVITE,
					MOTD
				}lolIRC_Server_Response; //All kind of possible responses from the server

				typedef uint32_t (*lolIRC_ServerHandler)(lolIRC_Client&, lolIRC_Server_Response, ...); //Handler for server messages
				typedef uint32_t (*lolIRC_ChannelHandler)(lolIRC_Client&, std::string, lolIRC_Channel_Response, ...); //Handler for channel messages
				typedef uint32_t (*lolIRC_MsgHandler)(lolIRC_Client&, std::string, std::string); //Handler for private messages

				//Constructor and Destructors
				lolIRC_Client(); //Base Constructor
				lolIRC_Client(std::string, uint16_t); //Constructor with server name and port

				~lolIRC_Client(); //Base Destructor

				//Base Class Functions
				uint32_t setServerName(std::string);
				uint32_t setServerPort(uint16_t);
				std::string getServerName() const;
				uint16_t getServerPort() const;

				uint32_t setServerHandler(lolIRC_ServerHandler);
				uint32_t setChannelHandler(lolIRC_ChannelHandler);
				uint32_t setMsgHandler(lolIRC_MsgHandler);

				uint32_t setNick(std::string);
				uint32_t setUser(std::string);
				std::string getNick() const;
				std::string getUser() const;

				//Main Functions
				uint32_t Connect();
				void Close();

				//Overloaded operators
				lolIRC_Channel& operator[](const char *); //Get channel with lolIRC_Client["channel"]
				operator bool() const; //True if still connected

				//Channels functions
				uint32_t joinChannel(std::string, std::string); //Join a channel, parameters are channel name and the optional key
				void closeChannel(std::string);

			private:
				class lolIRC_Response;

				pthread_t r_thread; //Thread descriptor

				lolIRC_ServerHandler sh;
				lolIRC_ChannelHandler ch;
				lolIRC_MsgHandler mh;

				lolIRC_Connection connection;
				std::list<lolIRC_Channel> channels; //List of channels joined (max 10)
				std::list<lolIRC_Response*> responses; //List of responses(numeric and textual)

				std::string server_name;
				uint16_t port;

				std::string nick;
				std::string user;

				class lolIRC_Response //Abstract Class for generic responses
				{
					public:
						lolIRC_Response(lolIRC_Client&);
						virtual void doIt(lolIRC_Message&) = 0;
					protected:
						lolIRC_Client& client;
				};

				class lolIRC_Response_Numeric : public lolIRC_Client::lolIRC_Response //Abstract class for numeric responses
				{
					public:
						lolIRC_Response_Numeric(lolIRC_Client&, uint16_t);
						uint16_t getCode() const;
						virtual void doIt(lolIRC_Message&) = 0;
					protected:
						uint16_t code;
				};

				class lolIRC_Response_Textual : public lolIRC_Client::lolIRC_Response //Abstract class for textual responses
				{
					public:
						lolIRC_Response_Textual(lolIRC_Client&, std::string);
						std::string getName() const;
						virtual void doIt(lolIRC_Message&) = 0;
					protected:
						std::string name;
				};

				//Responses Start

				class lolIRC_Response_Ping : public lolIRC_Client::lolIRC_Response_Textual //Ping Response
				{
					DEFINE_RESPONSE_TEXTUAL(lolIRC_Response_Ping);
				};

				class lolIRC_Response_Join : public lolIRC_Client::lolIRC_Response_Textual //Join response
				{
					DEFINE_RESPONSE_TEXTUAL(lolIRC_Response_Join);
				};

				class lolIRC_Response_RPLTOPIC : public lolIRC_Client::lolIRC_Response_Numeric //Topic reponse after join
				{
					DEFINE_RESPONSE_NUMERIC(lolIRC_Response_RPLTOPIC);
				};

				class lolIRC_Response_RPLTOPIC_WHOTIME : public lolIRC_Client::lolIRC_Response_Numeric //Who set the topic, received after join(not standard but used)
				{
					DEFINE_RESPONSE_NUMERIC(lolIRC_Response_RPLTOPIC_WHOTIME);
				};

				class lolIRC_Response_RPLNAMREPLY : public lolIRC_Client::lolIRC_Response_Numeric //Users in the channel after join
				{
					DEFINE_RESPONSE_NUMERIC(lolIRC_Response_RPLNAMREPLY);
					private:
						void fillNicknames(lolIRC_Channel&, std::string);
						bool alreadyJoined(lolIRC_Channel&, std::string&);
				};

				class lolIRC_Response_RPLENDOFNAMES : public lolIRC_Client::lolIRC_Response_Numeric //Response after RPLNAMREPLY
				{
					DEFINE_RESPONSE_NUMERIC(lolIRC_Response_RPLENDOFNAMES);
					private:
						void callHandler(lolIRC_Channel&);
				};

				//Responses End

				void* threadHandler(void *);
				void initResponses(); //Init the list of responses
				void freeResponses(); //Free the dynamic memory of responses list
				std::string upper(std::string); //Return an uppercase string

				//Messages start
				void join(std::string, std::string);
				void part(std::string);
				//Messages end
		};

	}
}

#endif
