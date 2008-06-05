#ifndef REMOTE_ERROR_H
#define REMOTE_ERROR_H

#include <exception>
#include <stdio.h>

namespace remote { 

class error : public std::exception
{
	public:
		error(std::string message)
		{
			description = message;
		}

		error(int error, std::string message)
		{
			description = message + ": " + strerror(error);
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
