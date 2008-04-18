drop table if exists user;

CREATE TABLE user (
	`id` int(11) unsigned NOT NULL auto_increment,
	`login` varchar(255) NOT NULL,
	`password` varchar(41) NOT NULL,
	`name` varchar(255) NOT NULL,
	`email` varchar(255) NOT NULL,
	PRIMARY KEY  (`id`),
	CONSTRAINT `cs_user_login_unique`
		UNIQUE INDEX `ndx_user_login_unique` (`login`)
) ENGINE = InnoDB;
