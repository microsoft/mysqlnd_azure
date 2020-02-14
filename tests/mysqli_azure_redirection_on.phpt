--TEST--
Azure redirection test for servers when mysqlnd_azure.enableRedirect="on"
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

//Step 2: check connection result when use SSL
$link = mysqli_init();
$ret = @mysqli_real_connect($link, $host, $user, $passwd, $db, $port, NULL, MYSQLI_CLIENT_SSL);

$last_message = "";
if($ret)
    $last_message =  $link->info;

//Server supports redirection
if(strlen($last_message) > 27 && strcmp(substr($last_message, 0, strlen("Location:")), "Location:")==0) {
    if(substr($link->host_info, 0, strlen($host)) == $host)
        echo "[003] fail\n";
    else
        echo "[003] pass\n";
}
else { //Server does not support redirection
    $lastError = error_get_last()["message"];
    if (strpos($lastError, "Connection aborted because redirection is not enabled on the MySQL server or the network package doesn't meet meet redirection protocol.") !== false)
        echo "[003] pass\n";
    else
        echo "[003] fail\n";
}

mysqli_close($link);

//Step 3: check connection result when not use SSL
$link = mysqli_init();
$ret = @mysqli_real_connect($link, $host, $user, $passwd, $db, $port, NULL, NULL);
if ($ret)
{
	printf("[004] When enableRedirect=on, connect without SSL expects failure, got pass\n");
	die();
}

$lastError = error_get_last()["message"];
if (strpos($lastError, "mysqlnd_azure.enableRedirect is on, but SSL option is not set in connection string. Redirection is only possible with SSL.") !== false)
        echo "[004] pass\n";
    else
        echo "[004] fail\n";

echo "Done\n";
?>
--EXPECTF--
on
[003] pass
[004] pass
Done