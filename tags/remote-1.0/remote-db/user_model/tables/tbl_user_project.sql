drop table if exists user_project;

CREATE TABLE user_project (
	`id` int(11) unsigned NOT NULL auto_increment,
	`user_id` int(11) unsigned NOT NULL,
	`project_id` int(11) unsigned NOT NULL,
	PRIMARY KEY  (`id`),
	CONSTRAINT `fk_user_prj_usr` FOREIGN KEY (`user_id`) REFERENCES `user` (`id`) ON DELETE CASCADE,
	CONSTRAINT `fk_user_prj_prj` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`) ON DELETE CASCADE,
	CONSTRAINT `cs_user_prj_unique`
		UNIQUE INDEX `ndx_user_prj_unique` (`user_id`,`project_id`)
) ENGINE = InnoDB;
