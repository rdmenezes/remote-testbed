#ifndef REMOTE_ERROR_H
#define REMOTE_ERROR_H

#include <exception>
#include <stdio.h>
#include <stdarg.h>

namespace remote { 

class error : public std::exception
{
	public:
		error(std::string message)
		{
			description = message;
		}

		error(int errno, std::string message)
		{
			description = message + ": " + strerror(errno);
		}

		~error() throw ()
		{
		}

		const char *what() const throw ()
		{
			return description.c_str();
		}

	private:
		std::string description;
};

}

#endif
