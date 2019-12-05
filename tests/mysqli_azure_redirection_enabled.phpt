--TEST--
Azure redirection test for servers when mysqlnd_azure.enableRedirect
--INI--
mysqlnd_azure.enableRedirect=1
--SKIPIF--
<?php
require_once('skipif.inc');
require_once('skipifconnectfailure.inc');
?>
--FILE--
<?php
require_once("connect.inc");

if (($tmp = ini_get("mysqlnd_azure.enableRedirect")) != true)
		printf("[001] mysqlnd_azure.enableRedirect not set to true, got '%s'\n", $tmp);

$link = mysqli_init();
$ret = mysqli_real_connect($link, $host, $user, $passwd, $db, $port, NULL, MYSQLI_CLIENT_SSL);
if (!$ret || !is_object($link))
{
	printf("[001] Cannot connect to the server using host=%s, user=%s, passwd=***, dbname=%s, port=%s, socket=%s\n  - [%d] %s with ssl",
			$host, $user, $db, $port, $socket, mysqli_connect_errno(), mysqli_connect_error());
	die();
}
echo $link->host_info."\n";
echo $link->info."\n";
if(substr($link->host_info, 0, strlen($host)) == $host)
	echo "1\n";
else
	echo "0\n";
mysqli_close($link);
echo "Done\n";
?>
--EXPECTF--
%s
Location: mysql://%s:%d/user=%s@%s
0
Done
