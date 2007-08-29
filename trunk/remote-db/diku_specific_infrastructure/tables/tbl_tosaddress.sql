drop table if exists tosaddress;

CREATE TABLE tosaddress (
	`tosaddress` int(11) unsigned NOT NULL auto_increment,
	`mote_id` int(11) unsigned NOT NULL,
	PRIMARY KEY  (`tosaddress`),
	CONSTRAINT `fk_tosaddress_mote` FOREIGN KEY (`mote_id`) REFERENCES `mote` (`id`) ON DELETE CASCADE,
	CONSTRAINT `cs_tos_mote_unique`	UNIQUE INDEX `ndx_tos_mote_unique` (`mote_id`)
) ENGINE = InnoDB;
