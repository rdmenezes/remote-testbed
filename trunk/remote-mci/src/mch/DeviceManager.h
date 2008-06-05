#ifndef REMOTE_MCH_DEVICEMANAGER_H
#define REMOTE_MCH_DEVICEMANAGER_H

#include "remote.h"
#include "mch/Mote.h"

namespace remote { namespace mch {

typedef std::map<std::string, Mote *> motemap_t;

/** MCH Mote device manager.
 *
 * The device manager maintains a database of known motes and their
 * status. The database consists of three mote maps: one containing
 * newly discovered motes, one with motes that are no longer available,
 * and a map containing all currently valid and operational motes.
 * The database can be updated using #refresh().
 */
class DeviceManager
{
public:
	motemap_t motes;	/**< The current valid motes. */
	motemap_t newMotes;	/**< Motes that were just attached. */
	motemap_t lostMotes;	/**< Motes that were just detached. */

	/** Refresh the mote device database.
	 *
	 * Update the registered devices to match the existing system using a
	 * "mark and sweep" approach. First all motes are marked invalid
	 * after which the mote device hierarchy is iterated to revalidate all
	 * available mote. Finally, motes that are still invalid are moved to
	 * the #lostMotes.
	 *
	 * @param devicePath	The path to the mote devices.
	 */
	void refresh(std::string devicePath);

private:
	/** Traverse and update all available motes.
	 *
	 * Traverses all mote device directories under the given
	 * device path and updates the mote.
	 *
	 * @param devicePath	The path to the mote devices.
	 */
	void readMoteDevices(std::string devicePath);

	/** Update mote information.
	 *
	 * If the mote already exists it is validated using
	 * #mch::Mote::validate else a new mote is created
	 * and placed in both #motes and #newMotes.
	 *
	 * @param mac		The MAC address of the mote.
	 * @param directory	The device directory of the mote.
	 */
	void updateMote(std::string mac, std::string directory);
};

}}

#endif
