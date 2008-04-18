drop table if exists siteattr;

CREATE TABLE siteattr (
	`id` int(11) unsigned NOT NULL auto_increment,
	`siteattrtype_id` int(11) unsigned NOT NULL,
	`val` text(500) default '',
	PRIMARY KEY  (`id`),
	CONSTRAINT `fk_siteattr_type` FOREIGN KEY (`siteattrtype_id`) REFERENCES `siteattrtype` (`id`)
) ENGINE = InnoDB;
