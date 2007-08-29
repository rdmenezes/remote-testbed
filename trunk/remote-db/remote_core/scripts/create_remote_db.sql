create database REMOTE;
GRANT ALL PRIVILEGES ON REMOTE.* TO 'remote_admin'@'localhost'
IDENTIFIED BY 'remote' WITH GRANT OPTION;
GRANT ALL PRIVILEGES ON *.* TO 'remote_admin'@'localhost'
IDENTIFIED BY 'remote' WITH GRANT OPTION;
GRANT CREATE ON *.* TO 'remote_admin'@'localhost'
IDENTIFIED BY 'remote';


GRANT ALL PRIVILEGES ON *.* TO 'remote_admin'@'%'
IDENTIFIED BY 'remote' WITH GRANT OPTION;
GRANT ALL PRIVILEGES ON REMOTE.* TO 'remote_admin'@'%'
IDENTIFIED BY 'remote' WITH GRANT OPTION;
GRANT CREATE ON *.* TO 'remote_admin'@'%'
IDENTIFIED BY 'remote';