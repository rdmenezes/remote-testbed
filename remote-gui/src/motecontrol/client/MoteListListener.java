package diku.distlab.motecontrolclientlib.client;

public interface MoteListListener {
	void addedMote(MoteList moteList, Mote mote);
	void removedMote(MoteList moteList, Mote mote);
	void cleared(MoteList moteList);
}
