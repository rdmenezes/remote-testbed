drop table if exists macaddress;

CREATE TABLE macaddress (
	`macaddress` bigint unsigned not NULL,
	`mote_id` int(11) unsigned NOT NULL,
	PRIMARY KEY  (`macaddress`),
	CONSTRAINT `fk_macaddress_mote` FOREIGN KEY (`mote_id`) REFERENCES `mote` (`id`) ON DELETE CASCADE,
	CONSTRAINT `cs_mac_mote_unique`
		UNIQUE INDEX `ndx_mac_mote_unique` (`mote_id`)
) ENGINE = InnoDB;
