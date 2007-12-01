#ifndef REMOTE_MCH_MOTE_H
#define REMOTE_MCH_MOTE_H

#include "libutil/File.h"
#include "SerialControl.h"

namespace remote { namespace diku_mch {

/** MCH Mote
 *
 * This class is used for keeping track of information from the mote
 * device directory and whether a mote is valid. A mote object is
 * created for each mote attached to a mote host.
 *
 * All the lowlevel control of the serial connection to the mote is
 * done via the subclassed #SerialControl class.
 */
class Mote : public SerialControl
{
public:
	/** Create a new mote.
	 *
	 * A newly created will automatically be validated using
	 * #validate().
	 *
	 * @param mac		The mote's MAC address.
	 * @param directory	The mote device directory.
	 */
	Mote(std::string& mac, std::string& directory);

	/** Is the mote valid?
	 *
	 * @return	True if the mote is valid.
	 */
	bool isValid();

	/** Mark the mote invalid. */
	void invalidate();

	/** Validate mote.
	 *
	 * When validating a mote, information from files in the mote
	 * device directory are reread in order to check if the mote is
	 * still operational.
	 */
	void validate();

	/** Get MAC address.
	 *
	 * @return	The MAC address.
	 */
	const std::string& getMac();

	/** Get the physical device path.
	 *
	 * The physical device path contains information about where on
	 * the mote host the mote is attached. This includes information
	 * such as which PCI bus it uses, which USB interface etc, for
	 * example: "/devices/pci0000:00/0000:00:1d.0/usb1/1-1".
	 *
	 * This information is read from the "path" file in the mote
	 * device directory.
	 *
	 * @return	The physical device path.
	 */
	const std::string& getDevicePath();

	/** Get platform name.
	 *
	 * Contains the name of the mote platform, for example "MicaZ"
	 * or "dig528-2".
	 *
	 * This information is read from the "platform" file in the mote
	 * device directory.
	 *
	 * @return	The name of the mote's platform.
	 */
	const std::string& getPlatform();

	/** Get mote programmer path.
	 *
	 * When programming the mote this path should be used.
	 *
	 * The programmer path is derived "programmer" file in the mote
	 * device directory. It is a symlink pointing to a binary or
	 * script on the system capable of programming the mote.
	 *
	 * @return	The path to the mote programmer binary.
	 */
	std::string getProgrammerPath();

	/** Get path to flash image file.
	 *
	 * This path can be used to temporarily save flash images when a
	 * mote is being reprogrammed.
	 *
	 * @return	Path to flash image file.
	 */
	std::string getImagePath();

private:
	std::string mac;	/**< MAC address. */
	std::string directory;	/**< Device directory path. */
	std::string path;	/**< Physical device path. */
	std::string platform;	/**< Platform name. */
	bool isvalid;		/**< Valid flag. */
};

}}

#endif
