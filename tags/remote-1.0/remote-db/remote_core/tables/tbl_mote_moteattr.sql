drop table if exists mote_moteattr;

CREATE TABLE mote_moteattr (
	`id` int(11) unsigned NOT NULL auto_increment,
	`mote_id` int(11) unsigned NOT NULL,
	`moteattr_id` int(11) unsigned NOT NULL,
	PRIMARY KEY  (`id`),
	CONSTRAINT `fk_moteattr_moteid` FOREIGN KEY (`mote_id`) REFERENCES `mote` (`id`) ON DELETE CASCADE,
	CONSTRAINT `fk_moteattr_moteattrid` FOREIGN KEY (`moteattr_id`) REFERENCES `moteattr` (`id`),
	CONSTRAINT `cs_mote_moteattr_unique`
		UNIQUE INDEX `ndx_mote_moteattr_unique` (`mote_id`,`moteattr_id`)
) ENGINE = InnoDB;
