#ifndef _MOTE_H_
#define _MOTE_H_

#include "Session.h"
#include "MoteControlInfrastructure.h"
#include "MsgPayload.h"

namespace remote { namespace mcs {
	
class Host;
class Session;
class Mote;

typedef std::map<dbkey_t,Mote*> motemapbykey_t;

/** The mote class acts as a global mote pool. Any client requests for mote
	control are handled by this class. Also, MoteControlInfrastructure instances must
	report their available motes to this class. Finally, the mote class
	maintains high level information about mote-site relations.
	Mote objects know their indexes in the global mote pool as well as the
	responsible	MoteControlInfrastructure object along with the custom address
	(MCIAddress) that uniquely identifies the mote within the owning
	MoteControlInfrastructure object. Motes are globally uniquely identified using their
	primary key ID from the database. The MoteControlInfrastructure object is responsible
	for resolving the global ID of any mote	before creating a Mote object. The
	ID is connected to the actual mote rather than the site the mote is
	attached to. Therefore, the MCIAddress and the MoteControlInfrastructure may
	become invalid if the mote is hotplugged. Whenever this happens, the 
	MoteControlInfrastructure losing the mote must delete the mote.
	The MoteControlInfrastructure getting the mote must look up the global ID
	in the database and create the Mote object anew.
**/
class Mote
{
	public:
		/** Request client control of the mote with the specified id.
		 * \param p_mote_id Database key of the mote
		 * \param p_session Pointer to the session object requesting control
		 * \param p_mote Pointer-pointer to receive a pointer for the mote object
		 * \returns 
		**/
		static result_t getById( dbkey_t p_mote_id,		                     
		                         Session* p_session,
		                         Mote** p_mote );
		
		/** Reset all nonstatic information about all motes in the database. **/
		static void resetDb();
		
		/** Look up the mote with specified id in the database, register the
		 * 	site on the mote.
		 * \param p_mote_id Database key of the mote
		 * \param p_site_id Database key of the site
		 * \param p_infrastructure Control infrastructure object owning the mote
		 * \param p_mciAddress Control infrastructure address of the mote
		**/
		Mote( dbkey_t p_mote_id,
		      dbkey_t p_site_id,
		      MoteControlInfrastructure& p_infrastructure,
		      MCIAddress& p_mciAddress );
		
		/** Create a new mote in the database with specified site_id.
		 * \param p_site_id Database key of the site
		 * \param p_infrastructure Control infrastructure object owning the mote
		 * \param p_mciAddress Control infrastructure address of the mote
		 *  **/
		Mote( dbkey_t p_site_id,
		      MoteControlInfrastructure& p_infrastructure,
			  MCIAddress& p_mciAddress );
		
		/** Delete the mote object
		 * \param silent If true, exceptions are caught and logged silently.
		 * */
		void destroy(bool silent = false);
		
		/** Sets a session object for the mote.
		 * \param p_session Pointer to the session to set.
		**/
		bool setSession( Session* p_session );
		
		/** Removes the mote from the current session, if any.
		 * \param notify If true, the session should notify its client that the mote is no longer
		 * controlled.
		**/
		void dropSession(bool notify = true);

		/** Send message to the mote.
		 * \param request Messsage to send
		**/
		void request( MsgPayload& request );
		
		/** Receive message from the mote
		 * \param confirm Message to receive
		**/
		void confirm( MsgPayload& confirm );
		
		/** Register a custom attribute on the mote in the database.
		 *  The attribute type must pre-exist in the database or else
		 *  an exception is thrown. If an attribute
		 *  with the given type and value does not exist, it is created.
		 *  If the mote allready has an attribute of the given type, this
		 *  attribute is removed from the mote.
		 * \param type Name of the attribute type
		 * \param value Attribute value to assign
		 **/
		void setAttribute(std::string type, std::string value);

		/** Query existence of attribute. */
		std::string getAttribute(std::string type);

		/**	 Mote Control Infrastructure address. **/
		MCIAddress& mciAddress;
		/** Database key of the mote **/
		dbkey_t mote_id;
		/** Database ket of the current deployment site of the mote. **/
		dbkey_t site_id;

	protected:
		/** Mote object destructor **/
		virtual ~Mote();
		/** ensures no dangling mote pointers
		 * \param mote 
		**/		
		static void registerMote(Mote* mote); // 
		/** Keyed map of all current Mote objects **/
		static motemapbykey_t mote;
		/** The current owner of this mote **/
		MoteControlInfrastructure& mci; 
		/** The session currently in control of this mote **/
		Session* session;					
};

}}

#endif
