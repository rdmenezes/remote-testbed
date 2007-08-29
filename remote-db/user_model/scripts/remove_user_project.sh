#!/bin/sh

. admin_scripts/credentials.sh

echo "Enter login of user:"
read LOGIN
echo "Enter name of project:"
read PROJECT

echo "delete from user_project where user_id = (select id from user where login='$LOGIN') and project_id = (select id from project where name='$PROJECT');"
$MYSQL $DBNAME -e "delete from user_project where user_id = (select id from user where login='$LOGIN') and project_id = (select id from project where name='$PROJECT');"
