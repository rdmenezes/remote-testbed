#!/bin/sh

. "$(dirname "$0")/credentials.sh"

echo "Enter login of user to drop:"
read LOGIN

echo "delete from user where login='$LOGIN';"
$MYSQL $DBNAME -e "delete from user where login='$LOGIN';"
