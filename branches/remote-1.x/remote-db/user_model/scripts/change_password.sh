#!/bin/sh

. "$(dirname "$0")/credentials.sh"

echo "Enter login of user:"
read LOGIN
echo "Enter new password of user:"
read PASS

echo "update user set password = md5('$PASS') where login = '$LOGIN';"
$MYSQL $DBNAME -e "update user set password = md5('$PASS') where login = '$LOGIN';"

# FIXME: This seem to no longer be needed. --jonas
# . admin_scripts/user_grants.sh
