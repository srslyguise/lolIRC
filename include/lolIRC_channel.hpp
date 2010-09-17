/***************************************************************************
 * Copyleft EDX. http://edx.shellmix.com, edx1010@gmail.com
 *
 * You like this software? Do what you want under the terms of
 * GNU General Public License.
 ***************************************************************************/

#ifndef LOLIRC_CHANNEL_H
#define LOLIRC_CHANNEL_H

#include <common.hpp>
#include <lolIRC_connection.hpp>
#include <lolIRC_nick.hpp>
#include <list>
#include <ctime>

namespace lolIRC {
	namespace Client {

		class lolIRC_Channel //Object that defines a channel
		{
			friend class lolIRC_Client; //Main lolIRC Class MUST get complete acces to this class

			public:

				~lolIRC_Channel();

				void Invite(std::string); //Invite a user to this channel
				void ModeChan(std::string, std::string); //Set mode
				std::list<std::string> Names(); //Get Users
				std::string getTopic() const;//Get Topic
				void setTopic(std::string); //Set topic
				std::string getTopicSetBy() const;
				std::string getTopicSetDate() const;
				void Send(std::string); //Send a message

				lolIRC_Nick& operator[](const char *);//Get single nick with lolIRC_Nick["nickname"]

			private:
				lolIRC_Channel(lolIRC_Connection&);
				lolIRC_Channel(lolIRC_Connection&, std::string, std::string); //Parameters are the main connection, the channel name and the current nick

				uint32_t setChannelName(std::string);
				std::string getChannelName() const;

				uint32_t setNick(std::string);
				std::string getNick() const;


				std::string name;
				std::string nick;
				lolIRC_Connection& connection;

				std::string topic;
				std::string topic_setby;
				time_t topic_setby_date; //POSIX timestamp

				std::list<lolIRC_Nick> nicknames; //List of nicknames in the channel
		};		

	}
}

#endif
