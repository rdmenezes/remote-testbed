drop table if exists `project`;

CREATE TABLE `project` (
	`id` int(11) unsigned NOT NULL auto_increment,
	`name` varchar(255) NOT NULL,
	PRIMARY KEY  (`id`),
	CONSTRAINT `cs_project_name_unique`
		UNIQUE INDEX `ndx_project_name_unique` (`name`)
) ENGINE = InnoDB;
