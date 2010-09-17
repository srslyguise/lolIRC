/***************************************************************************
 * Copyleft EDX. http://edx.shellmix.com, edx1010@gmail.com
 *
 * You like this software? Do what you want under the terms of
 * GNU General Public License.
 ***************************************************************************/

#include <lolIRC_connection.hpp>
#include <cstring>

lolIRC::Client::lolIRC_Connection::lolIRC_Connection() : sd(0), port(6667), hostname(""), hent(NULL)
{

}

lolIRC::Client::lolIRC_Connection::lolIRC_Connection(std::string host, uint16_t port) : sd(0), hent(NULL)
{
	setHost(host);
	setPort(port);
}

lolIRC::Client::lolIRC_Connection::~lolIRC_Connection()
{
	if(sd != 0)
		Close();
}

void lolIRC::Client::lolIRC_Connection::setHost(std::string h)
{
	hostname = h;
}

void lolIRC::Client::lolIRC_Connection::setPort(uint16_t p)
{
	port = p;
}

uint32_t lolIRC::Client::lolIRC_Connection::Connect()
{
	std::string error = "";

	try {
		init_addr();
	}
	catch (std::exception& e)
	{
		throw e;
	}

	if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		error = "lolIRC: Connection: Could not create socket, errno = ";
		error.append(strerror(errno));
		throw lolIRC_Exception(error);
	}

	if(connect(sd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)
	{
		error = "lolIRC: Connection: Could not connect, errno = ";
		error.append(strerror(errno));
		sd = 0;
		throw lolIRC_Exception(error);
	}

	return 0;
}

std::string lolIRC::Client::lolIRC_Connection::Read()
{
	char buffer[1024] = {0};
	std::string ret = "";
	std::string error = "lolIRC: Connection: Error while reading from socket";

	if(recv(sd, buffer, 1024, 0) <= 0)
	{
		sd = 0;
		throw lolIRC_Exception(error);
	}

	ret = buffer;

	return ret;
}

uint32_t lolIRC::Client::lolIRC_Connection::Write(std::string s)
{
	std::string error = "lolIRC: Connection: Error while writing on socket";
	uint32_t ret = 0;

	if((ret = send(sd, s.c_str(), s.length(), 0)) == -1)
	{
		error.append(strerror(errno));
		sd = 0;
		throw lolIRC_Exception(error);
	}

	return ret;
}

void lolIRC::Client::lolIRC_Connection::Close()
{
	std::string error = "lolIRC: Connection: Could not close socket, errno = ";

	if(sd != 0)
	{
		if(close(sd) == -1)
		{
			error.append(strerror(errno));
			throw lolIRC_Exception(error);
		}
	}

	sd = 0;
}

lolIRC::Client::lolIRC_Connection::operator bool() const
{
	if(sd != 0)
		return true;
	else
		return false;
}

void lolIRC::Client::lolIRC_Connection::init_addr()
{
	in_addr a;
	if((hent = gethostbyname(hostname.c_str())) == NULL)
		throw lolIRC_Exception("lolIRC: Connection: Could not resolve hostname");

	a.s_addr = *(in_addr_t *) hent->h_addr_list[0];

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(inet_ntoa(a));	
}
