#!/bin/sh

. credentials.sh

echo "Enter name of new project:"
read PROJECT_NAME

echo "creating $PROJECT_NAME;"
echo "insert into project(name) values ('$PROJECT_NAME');"
$MYSQL $DBNAME -e "insert into project(name) values ('$PROJECT_NAME');"