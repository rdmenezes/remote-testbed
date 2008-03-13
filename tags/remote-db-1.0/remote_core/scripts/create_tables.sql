set FOREIGN_KEY_CHECKS=0;

source remote_core/tables/tbl_mote.sql;
source remote_core/tables/tbl_moteattrtype.sql;
source remote_core/tables/tbl_moteattr.sql;
source remote_core/tables/tbl_mote_moteattr.sql;
source remote_core/tables/tbl_position.sql;
source remote_core/tables/tbl_site.sql;
source remote_core/tables/tbl_siteattrtype.sql;
source remote_core/tables/tbl_siteattr.sql;
source remote_core/tables/tbl_site_siteattr.sql;
source remote_core/tables/tbl_session.sql;
source remote_core/tables/tbl_sessionattrtype.sql;
source remote_core/tables/tbl_sessionattr.sql;
source remote_core/tables/tbl_session_sessionattr.sql;
source remote_core/tables/tbl_host.sql;
source remote_core/tables/tbl_path.sql;

set FOREIGN_KEY_CHECKS=1;
