#!/bin/sh

. credentials.sh

echo "Enter name of project to drop: "
read PROJECT

echo "delete from project where name='$PROJECT';"

$MYSQL $DBNAME -e "delete from project where name='$PROJECT';"
