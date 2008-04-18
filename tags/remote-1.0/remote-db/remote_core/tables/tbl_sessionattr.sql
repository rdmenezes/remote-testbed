drop table if exists sessionattr;

CREATE TABLE sessionattr (
	`id` int(11) unsigned NOT NULL auto_increment,
	`sessionattrtype_id` int(11) unsigned NOT NULL,
	`val` text(500) default '',
	PRIMARY KEY  (`id`),
	CONSTRAINT `fk_sessionattr_type` FOREIGN KEY (`sessionattrtype_id`) REFERENCES `sessionattrtype` (`id`)
) ENGINE = InnoDB;
