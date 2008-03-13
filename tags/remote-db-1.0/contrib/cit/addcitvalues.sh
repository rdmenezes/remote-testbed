#!/bin/sh

. "$(dirname "$0")/credentials.sh"

#delete tables
$MYSQL $DBNAME -e "delete from site"
$MYSQL $DBNAME -e "delete from position;"
$MYSQL $DBNAME -e "delete from siteattr;"
$MYSQL $DBNAME -e "delete from site_siteattr;"
$MYSQL $DBNAME -e "delete from host;"

#alter id of the tables
$MYSQL $DBNAME -e "alter table position AUTO_INCREMENT=1;"
$MYSQL $DBNAME -e "alter table site AUTO_INCREMENT=1;"
$MYSQL $DBNAME -e "alter table siteattr AUTO_INCREMENT=1;"
$MYSQL $DBNAME -e "alter table site_siteattr AUTO_INCREMENT=1;"
$MYSQL $DBNAME -e "alter table host AUTO_INCREMENT=1;"

#site position - values
$MYSQL $DBNAME -e "insert into position (x,y,z) values(0,0,0);"

#site - values
$MYSQL $DBNAME -e "insert into site (sitename, position_id) values('B294_OFFICE',1);"
$MYSQL $DBNAME -e "insert into site (sitename, position_id) values('B294_CORRIDOR',1);"
#$MYSQL $DBNAME -e "update site set sitename='CAWSCITIE' where id=1;"
#$MYSQL $DBNAME -e "update site set position_id=1 where id=1;"

#site attributes - values
$MYSQL $DBNAME -e "insert into siteattr (siteattrtype_id,val) values(1,'157.190.198.92'),(2,'emnets-gw1');"
$MYSQL $DBNAME -e "insert into siteattr (siteattrtype_id,val) values(1,'157.190.198.93'),(2,'emnets-gw2');"

#site attributes - config
#CAWSCITIE has two attributes (host IP, host DNS)
$MYSQL $DBNAME -e "insert into site_siteattr (site_id,siteattr_id) values(1,1),(1,2);"
$MYSQL $DBNAME -e "insert into site_siteattr (site_id,siteattr_id) values(2,3),(2,4);"

#host - values
$MYSQL $DBNAME -e "insert into host (dnsname,ip) values('emnets-gw1','157.190.198.92'),('emnets-gw2','157.190.198.93')"

