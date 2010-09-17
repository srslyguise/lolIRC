/***************************************************************************
 * Copyleft EDX. http://edx.shellmix.com, edx1010@gmail.com
 *
 * You like this software? Do what you want under the terms of
 * GNU General Public License.
 ***************************************************************************/

#ifndef LOLIRC_EXCEPTION_H
#define LOLIRC_EXCEPTION_H

#include <exception>
#include <string>

namespace lolIRC {
	
	class lolIRC_Exception : public std::exception //Redefinition of class std::exception
	{
		public:
			lolIRC_Exception(std::string s) { err = s; }
			~lolIRC_Exception() throw() {}

			virtual const char * what() const throw()
			{
				return err.c_str();
			}

		private:
			std::string err;
	};

}

#endif
