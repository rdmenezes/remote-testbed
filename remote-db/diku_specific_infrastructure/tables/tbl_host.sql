drop table if exists host;

CREATE TABLE host (
	`id` int(11) unsigned NOT NULL auto_increment,
	`dnsname` varchar(255) default '',
	`ip` varchar(46) default '',
	PRIMARY KEY  (`id`)
) ENGINE = InnoDB;
