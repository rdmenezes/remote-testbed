drop table if exists site;

CREATE TABLE site (
	`id` int(11) unsigned NOT NULL auto_increment,
	`sitename` varchar(50) NOT NULL,
	`position_id` int(11) unsigned default NULL,
	PRIMARY KEY  (`id`),
	CONSTRAINT `fk_site_pos` FOREIGN KEY (`position_id`) REFERENCES `position` (`id`)
) ENGINE = InnoDB;

insert into site(sitename) values ('?SITE');
