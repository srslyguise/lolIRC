/***************************************************************************
 * Copyleft EDX. http://edx.shellmix.com, edx1010@gmail.com
 *
 * You like this software? Do what you want under the terms of
 * GNU General Public License.
 ***************************************************************************/

#include <lolIRC_nick.hpp>

lolIRC::Client::lolIRC_Nick::lolIRC_Nick(lolIRC_Connection& c) : connection(c)
{
}

lolIRC::Client::lolIRC_Nick::lolIRC_Nick(lolIRC_Connection& c, std::string n) : connection(c), name(n)
{
}

lolIRC::Client::lolIRC_Nick::~lolIRC_Nick()
{
}

void lolIRC::Client::lolIRC_Nick::Kick(std::string user)
{
}

void lolIRC::Client::lolIRC_Nick::Mode(std::string user)
{
}

std::string lolIRC::Client::lolIRC_Nick::getUser() const
{
	return name;
}
