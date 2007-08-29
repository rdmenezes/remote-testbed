USER=remote_admin
echo "remote_admin password:"
read PASS
DBNAME=REMOTE
HOST=amigos33.distlab.diku.dk
MYSQL="mysql -u$USER -p$PASS -h$HOST"
export USER
export PASS
export DBNAME
export HOST
export MYSQL