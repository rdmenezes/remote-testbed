drop table if exists path;

CREATE TABLE path (
	`id` int(11) unsigned NOT NULL auto_increment,
	`host_id` int(11) unsigned NOT NULL,
	`path` varchar(255) NOT NULL,
	`site_id` int(11) unsigned NOT NULL default 1,
	PRIMARY KEY  (`id`),
	CONSTRAINT `fk_path_host` FOREIGN KEY (`host_id`) REFERENCES `host` (`id`) ON DELETE CASCADE,
	CONSTRAINT `fk_path_site` FOREIGN KEY (`site_id`) REFERENCES `site` (`id`)
) ENGINE = InnoDB;
