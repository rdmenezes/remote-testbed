drop table if exists moteattr;

CREATE TABLE moteattr (
	`id` int(11) unsigned NOT NULL auto_increment,
	`moteattrtype_id` int(11) unsigned NOT NULL,
	`val` text(500) default '',
	PRIMARY KEY  (`id`),
	CONSTRAINT `fk_moteattr_type` FOREIGN KEY (`moteattrtype_id`) REFERENCES `moteattrtype` (`id`)
) ENGINE = InnoDB;
