drop table if exists position;

CREATE TABLE position (
	`id` int(11) unsigned NOT NULL auto_increment,
	`x` float NOT NULL,
	`y` float NOT NULL,
	`z` float NOT NULL,
	PRIMARY KEY  (`id`)
) ENGINE = InnoDB;
