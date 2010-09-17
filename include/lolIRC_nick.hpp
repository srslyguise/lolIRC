/***************************************************************************
 * Copyleft EDX. http://edx.shellmix.com, edx1010@gmail.com
 *
 * You like this software? Do what you want under the terms of
 * GNU General Public License.
 ***************************************************************************/

#ifndef LOLIRC_NICK_H
#define LOLIRC_NICK_H

#include <common.hpp>
#include <lolIRC_connection.hpp>

namespace lolIRC {
	namespace Client {

		class lolIRC_Nick //Object that defines a nick
		{
			public:
				lolIRC_Nick(lolIRC_Connection&);
				lolIRC_Nick(lolIRC_Connection&, std::string);

				~lolIRC_Nick();

				void Kick(std::string); //Kick the current user
				void Mode(std::string); //Set mode fo user

				std::string getUser() const;

			private:

				std::string name;
				lolIRC_Connection& connection;
		};		

	}
}

#endif
