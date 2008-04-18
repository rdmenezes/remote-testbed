drop table if exists sessionattrtype;

CREATE TABLE sessionattrtype (
	`id` int(11) unsigned NOT NULL auto_increment,
	`sortseq` int(11) unsigned default 10,
	`name` varchar(10) NOT NULL,
	`description` text(500) default '',
	PRIMARY KEY  (`id`)
) ENGINE = InnoDB;
