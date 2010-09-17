/***************************************************************************
 * Copyleft EDX. http://edx.shellmix.com, edx1010@gmail.com
 *
 * You like this software? Do what you want under the terms of
 * GNU General Public License.
 ***************************************************************************/

/* Header for common definitions */
#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <string>
#include <cerrno>
#include "lolIRC_exception.hpp"

#define DEBUG

#ifdef DEBUG
#include <iostream>
#define DEBUG_PRINT(X) std::cout << __FILE__ << ' ' << __LINE__ << ' ' << X << std::endl;
#else
#define DEBUG_PRINT(X) ""
#endif

#endif
