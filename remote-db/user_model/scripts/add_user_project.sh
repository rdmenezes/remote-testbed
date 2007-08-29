#!/bin/sh

. credentials.sh

echo "Enter login of user:"
read LOGIN
echo "Enter name of project:"
read PROJECT

echo "insert into user_project(user_id,project_id) select (select id from user where login='$LOGIN'),(select id from project where name='$PROJECT')"
echo "grant select,insert,delete,update on $PROJECT.* to '$LOGIN'@'%'"
$MYSQL $DBNAME -e "insert into user_project(user_id,project_id) select (select id from user where login='$LOGIN'),(select id from project where name='$PROJECT');" \
-e "grant select,insert,delete,update on $PROJECT.* to '$LOGIN'@'%';"