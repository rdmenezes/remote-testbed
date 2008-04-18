/*  NOTE: The reservation system is not yet complete. Currently, reservations are 
	permanent until they are deleted - begin and end times are not used.
	Also, a constraint keeps projects from having more than one reservation.
	This constraint will be removed once the reservation system is complete.
*/

drop table if exists reservation;

CREATE TABLE reservation (
	`id` int(11) unsigned NOT NULL auto_increment,
	`project_id` int(11) unsigned NOT NULL,
	`begin` datetime NOT NULL,
	`end` datetime NOT NULL,
	PRIMARY KEY  (`id`),
	CONSTRAINT `fk_rsv_project` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`),
	CONSTRAINT `cs_rsv_project_unique_temp`
		UNIQUE INDEX `ndx_rsv_project_unique_temp` (`project_id`)
) ENGINE = InnoDB;
