drop table if exists session_sessionattr;

CREATE TABLE session_sessionattr (
	`id` int(11) unsigned NOT NULL auto_increment,
	`session_id` int(11) unsigned NOT NULL,
	`sessionattr_id` int(11) unsigned NOT NULL,
	PRIMARY KEY  (`id`),
	CONSTRAINT `fk_sessionattr_sessionid` FOREIGN KEY (`session_id`) REFERENCES `session` (`id`) ON DELETE CASCADE,
	CONSTRAINT `fk_sessionattr_sessionattrid` FOREIGN KEY (`sessionattr_id`) REFERENCES `sessionattr` (`id`),
	CONSTRAINT `cs_session_sessionattr_unique`
		UNIQUE INDEX `ndx_session_sessionattr_unique` (`session_id`,`sessionattr_id`)
) ENGINE = InnoDB;
