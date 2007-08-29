set FOREIGN_KEY_CHECKS=0;

source diku_specific_infrastructure/tables/tbl_host.sql;
source diku_specific_infrastructure/tables/tbl_macaddress.sql;
source diku_specific_infrastructure/tables/tbl_path.sql;
source diku_specific_infrastructure/tables/tbl_tosaddress.sql;

set FOREIGN_KEY_CHECKS=1;
