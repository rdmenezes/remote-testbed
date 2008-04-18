drop table if exists mote;

CREATE TABLE mote (
	`id` int(11) unsigned NOT NULL auto_increment,
	`site_id` int(11) unsigned default NULL,
	`curr_session_id` int(11) unsigned default NULL,
	`priv_session_id` int(11) unsigned default NULL,
	PRIMARY KEY  (`id`),
	CONSTRAINT `fk_mote_site` FOREIGN KEY (`site_id`) REFERENCES `site` (`id`),
	CONSTRAINT `fk_mote_curr_session` FOREIGN KEY (`curr_session_id`) REFERENCES `session` (`id`) ON DELETE SET NULL,
	CONSTRAINT `fk_mote_priv_session` FOREIGN KEY (`priv_session_id`) REFERENCES `session` (`id`) ON DELETE SET NULL
) ENGINE = InnoDB;
