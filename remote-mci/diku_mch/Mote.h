#ifndef _MOTE_H_
#define _MOTE_H_

#include "SerialControl.h"
#include "Configuration.h"

namespace remote { namespace diku_mch {

class Mote : public SerialControl
{
	public:
		/** Create a new mote. Just after creation, isValid and isNew will always return true.
		**/
		Mote( uint64_t p_macAddress,
		      std::string& p_path,
		      uint16_t p_ttyNum );

		bool isValid();

		void invalidate();

		void validate(std::string& p_path, uint16_t p_ttyNum);

		uint64_t getMac();
		uint16_t getTty();
		const std::string& getPath();

	private:
		uint64_t macAddress;
		uint16_t ttyNum;
		std::string path;
		bool isvalid;
};

}}

#endif
