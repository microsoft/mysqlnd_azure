mysqlnd_azure
=============
NOTE: For PHP-7.2, requires PHP-7.2.15 or later, for PHP-7.3, requires 7.3.2 or later.
mysqlnd_azure is a redirection plugin for the MySQL native driver for PHP. It is a PHP extension that enables redirection logic for mysqlnd driver. The basic idea is to store redirection
information as the last message in the first login request's OK packet. Then use this redirection information to establish the new connection, if connection succeeds, close the original
connection, and use the new one afterward.
+---------------------------+
| CONFIGURING mysqlnd_azure |
+---------------------------+
OPTION DESCRIPTION
------------------ ----------------------------------------------------------
mysqlnd_azure.enabled This option is to control enable or disable mysqlnd_rd. 
If this is set to 0, it will not use redirection.
(Default: 0)

