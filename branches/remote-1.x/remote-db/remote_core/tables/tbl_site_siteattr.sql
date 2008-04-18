drop table if exists site_siteattr;

CREATE TABLE site_siteattr (
	`id` int(11) unsigned NOT NULL auto_increment,
	`site_id` int(11) unsigned NOT NULL,
	`siteattr_id` int(11) unsigned NOT NULL,
	PRIMARY KEY  (`id`),
	CONSTRAINT `fk_siteattr_siteid` FOREIGN KEY (`site_id`) REFERENCES `site` (`id`) ON DELETE CASCADE,
	CONSTRAINT `fk_siteattr_siteattrid` FOREIGN KEY (`siteattr_id`) REFERENCES `siteattr` (`id`),
	CONSTRAINT `cs_site_siteattr_unique`
		UNIQUE INDEX `ndx_site_siteattr_unique` (`site_id`,`siteattr_id`)
) ENGINE = InnoDB;
