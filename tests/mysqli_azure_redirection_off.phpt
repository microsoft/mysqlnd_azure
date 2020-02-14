--TEST--
Azure redirection test for servers when mysqlnd_azure.enableRedirect="off"
--INI--
mysqlnd_azure.enableRedirect="off"
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
if (strcmp($tmp, "off")!=0)
{
	printf("[001] mysqlnd_azure.enableRedirect not set to off, got '%s'\n", $tmp);
    die();
}

//Step 2: check connection result when use SSL
$link = mysqli_init();
$ret = @mysqli_real_connect($link, $host, $user, $passwd, $db, $port, NULL, MYSQLI_CLIENT_SSL);
if (!$ret || !is_object($link))
{
	printf("[002] Cannot connect to the server using host=%s, user=%s, passwd=***, dbname=%s, port=%s, socket=%s\n  - [%d] %s with ssl\n",
			$host, $user, $db, $port, $socket, mysqli_connect_errno(), mysqli_connect_error());
	die();
}
echo $link->host_info."\n";
$last_message =  $link->info;

//Server supports redirection
if(strlen($last_message) > 27 && strcmp(substr($last_message, 0, strlen("Location:")), "Location:")==0) {
    if(substr($link->host_info, 0, strlen($host)) == $host)
        echo "[003] pass\n";
    else
        echo "[003] fail\n";
}
else { //Server does not support redirection, use the proxy connection
    if(substr($link->host_info, 0, strlen($host)) == $host)
        echo "[003] pass\n";
    else
        echo "[003] fail\n";
}

mysqli_close($link);

//Step 3: check connection result when not use SSL
$link = mysqli_init();
$ret = @mysqli_real_connect($link, $host, $user, $passwd, $db, $port, NULL, NULL);
if (!$ret || !is_object($link))
{
	printf("[004] Cannot connect to the server using host=%s, user=%s, passwd=***, dbname=%s, port=%s, socket=%s\n  - [%d] %s without ssl\n",
			$host, $user, $db, $port, $socket, mysqli_connect_errno(), mysqli_connect_error());
	die();
}

if(substr($link->host_info, 0, strlen($host)) == $host)
    echo "[004] pass\n";
else
    echo "[004] fail\n";

echo "Done\n";
?>
--EXPECTF--
off
%s
[003] pass
[004] pass
Done