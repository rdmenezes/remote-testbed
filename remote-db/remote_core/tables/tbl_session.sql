drop table if exists `session`;

CREATE TABLE `session` (
	`id` int(11) unsigned NOT NULL auto_increment,
	`auth` boolean NOT NULL default 0,
	PRIMARY KEY  (`id`)
) ENGINE = InnoDB;
