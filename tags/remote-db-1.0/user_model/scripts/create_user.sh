#!/bin/sh

. "$(dirname "$0")/credentials.sh"

echo "Enter login of new user:"
read LOGIN
echo "Enter password of new user:"
read PASS
echo "Enter email address of new user:"
read EMAIL
echo "Enter full name of new user:"
read NAME

echo "insert into user(login,password,name,email) values ('$LOGIN',md5('$PASS'),'$NAME','$EMAIL');"
$MYSQL $DBNAME -e "insert into user(login,password,name,email) values ('$LOGIN',md5('$PASS'),'$NAME','$EMAIL');"

