/***************************************************************************
 * Copyleft EDX. http://edx.shellmix.com, edx1010@gmail.com
 *
 * You like this software? Do what you want under the terms of
 * GNU General Public License.
 ***************************************************************************/

#ifndef LOLIRC_CONNECTION_H
#define LOLIRC_CONNECTION_H

#include <common.hpp>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace lolIRC {
	namespace Client {

		class lolIRC_Connection //Class to manage the connection
		{
			public:
				lolIRC_Connection();
				lolIRC_Connection(std::string, uint16_t);

				~lolIRC_Connection();

				void setHost(std::string);
				void setPort(uint16_t);

				uint32_t Connect();
				std::string Read();
				uint32_t Write(std::string);
				void Close();

				operator bool() const; //Is connection alive?

			private:
				void init_addr(); //Initialize address
				int sd;
				struct sockaddr_in addr;
				struct hostent * hent;
				std::string hostname;
				uint16_t port;
		};

	}
}

#endif
