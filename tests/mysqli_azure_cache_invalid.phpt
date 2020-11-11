--TEST--
Azure redirection test for servers whether ssl_ca will be kept if use cache failed
--INI--
mysqlnd_azure.enableRedirect="on"
--SKIPIF--
<?php
require_once('skipif.inc');
?>
--FILE--
<?php
require_once("connect.inc");

//Step 1: check option setting
$tmp = ini_get("mysqlnd_azure.enableRedirect");
echo $tmp."\n";
if (strcmp($tmp, "on")!=0)
{
	printf("[001] mysqlnd_azure.enableRedirect not set to on, got '%s'\n", $tmp);
    die();
}

//Step 1: first connection use SSL
$link = mysqli_init();
$ret = @mysqli_real_connect($link, $host, $user, $passwd, $db, $port, NULL, MYSQLI_CLIENT_SSL);
if (!$ret || !is_object($link))
{
	printf("[002] Cannot connect to the server using host=%s, user=%s, passwd=***, dbname=%s, port=%s, socket=%s\n  - [%d] %s with ssl\n",
			$host, $user, $db, $port, $socket, mysqli_connect_errno(), mysqli_connect_error());
	die();
}
echo $link->host_info."\n";
$link->close();


//Step 2: Second connection use SSL with invalid ca option. Should use cache and failed.
$link = mysqli_init();
mysqli_ssl_set($link, null,null,"A_Invalid_ca_ath.pem",null, null);
$ret = mysqli_real_connect($link, $host, $user, $passwd, $db, $port, NULL, MYSQLI_CLIENT_SSL);
if ($ret)
{
	printf("[003] Connection with invalid ca info should failed\n");
}

echo "Done\n";
?>
--EXPECTF--
on
%s

Warning: failed loading cafile stream: `A_Invalid_ca_ath.pem' in %s

Warning: mysqli_real_connect(): Cannot connect to MySQL by using SSL in %s

Warning: mysqli_real_connect(): [2002]  (trying to connect via (null)) in %s

Warning: failed loading cafile stream: `A_Invalid_ca_ath.pem' in %s

Warning: mysqli_real_connect(): Cannot connect to MySQL by using SSL in %s

Warning: mysqli_real_connect(): [2002]  (trying to connect via (null)) in %s

Warning: mysqli_real_connect(): (HY000/2002):  in %s
Done