/*  NOTE: The reservation system is not yet complete. Currently, reservations are 
	permanent until they are deleted. Also, a constraint keeps motes from being 
	reserved more than once. This constraint will be removed once the reservation
	system is complete.
*/

drop table if exists mote_reservation;

CREATE TABLE mote_reservation (
	`id` int(11) unsigned NOT NULL auto_increment,
	`reservation_id` int(11) unsigned NOT NULL,
	`mote_id` int(11) unsigned NOT NULL,
	PRIMARY KEY  (`id`),
	CONSTRAINT `fk_mote_rsv_rsv` FOREIGN KEY (`reservation_id`) REFERENCES `reservation` (`id`) ON DELETE CASCADE,
	CONSTRAINT `fk_mote_rsv_mote` FOREIGN KEY (`mote_id`) REFERENCES `mote` (`id`) ON DELETE CASCADE,
	CONSTRAINT `cs_mote_rsv_unique`
		UNIQUE INDEX `ndx_mote_rsv_unique` (`reservation_id`,`mote_id`),
	CONSTRAINT `cs_mote_rsv_unique_temp`
		UNIQUE INDEX `ndx_mote_rsv_unique_temp` (`mote_id`)
) ENGINE = InnoDB;
