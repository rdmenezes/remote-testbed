#ifndef _MOTE_H_
#define _MOTE_H_

#include "SerialControl.h"

namespace remote { namespace diku_mch {

class Mote : public SerialControl
{
	public:
		/** Create a new mote.
		 * Just after creation, isValid will always return true.
		 */
		Mote(std::string& mac, std::string& path, std::string& tty);

		bool isValid();

		void invalidate();

		void validate(std::string& path, std::string& tty);

		const std::string& getMac();
		const std::string& getPath();

	private:
		std::string mac;
		std::string path;
		bool isvalid;
};

}}

#endif
