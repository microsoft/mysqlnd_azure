--TEST--
Azure redirection option test for mysqlnd_azure.enableRedirect
--INI--
mysqlnd.collect_statistics="1"
mysqlnd.collect_memory_statistics="1"
--SKIPIF--
<?php
require_once('skipif.inc');
?>
--FILE--
<?php
require_once("connect.inc");

function ini_set_test($value) {
    global $host, $user, $passwd, $db, $port;

    ini_set("mysqlnd_azure.enableRedirect", $value);
    echo ini_get("mysqlnd_azure.enableRedirect"), "\n";
    $link = mysqli_init();
    $ret = @mysqli_real_connect($link, $host, $user, $passwd, $db, $port, NULL, NULL);
    if(!$ret)
        echo error_get_last()["message"], "\n";
    
    if($ret) {
        if(substr($link->host_info, 0, strlen($host)) == $host)
            echo "equal\n";
        else
            echo "not equal\n";
        $link->close();
    }
    
    $link = mysqli_init();
    $ret = @mysqli_real_connect($link, $host, $user, $passwd, $db, $port, NULL, MYSQLI_CLIENT_SSL);
    $last_message = "";
    if($ret)
        $last_message =  $link->info;

    //Server supports redirection
    if(strlen($last_message) > 27 && strcmp(substr($last_message, 0, strlen("Location:")), "Location:")==0) {
        $config = ini_get("mysqlnd_azure.enableRedirect");
        if(substr($link->host_info, 0, strlen($host)) == $host && (strcasecmp($config, "preferred")==0 || strcasecmp($config, "2")==0))
            echo "FAIL\n";
        else
            echo "PASS\n";
    } 
    else if($ret && is_object($link)) {
        if(substr($link->host_info, 0, strlen($host)) == $host)
            echo "PASS\n";
        else
            echo "FAIL\n";
    }
    else if(!$ret) { //Server does not support redirection and redirect on
        $lastError = error_get_last()["message"];
        if (strpos($lastError, "Connection aborted because redirection is not enabled on the MySQL server or the network package doesn't meet meet redirection protocol.") !== false)
            echo "PASS\n";
        else
            echo "FAIL\n";
    }
    if($ret) $link->close();
}

ini_set_test("on");
ini_set_test("On");
ini_set_test("yes");
ini_set_test("Yes");
ini_set_test("true");
ini_set_test("True");
ini_set_test("1");
ini_set_test(1);

ini_set_test("preferred");
ini_set_test("Preferred");
ini_set_test("2");
ini_set_test(2);

ini_set_test("off");
ini_set_test("Off");
ini_set_test("0");
ini_set_test(0);
ini_set_test("otherValue");

echo "Done\n";
?>
--EXPECTF--
on
mysqli_real_connect(): (HY000/2054): mysqlnd_azure.enableRedirect is on, but SSL option is not set in connection string. Redirection is only possible with SSL.
PASS
On
mysqli_real_connect(): (HY000/2054): mysqlnd_azure.enableRedirect is on, but SSL option is not set in connection string. Redirection is only possible with SSL.
PASS
yes
mysqli_real_connect(): (HY000/2054): mysqlnd_azure.enableRedirect is on, but SSL option is not set in connection string. Redirection is only possible with SSL.
PASS
Yes
mysqli_real_connect(): (HY000/2054): mysqlnd_azure.enableRedirect is on, but SSL option is not set in connection string. Redirection is only possible with SSL.
PASS
true
mysqli_real_connect(): (HY000/2054): mysqlnd_azure.enableRedirect is on, but SSL option is not set in connection string. Redirection is only possible with SSL.
PASS
True
mysqli_real_connect(): (HY000/2054): mysqlnd_azure.enableRedirect is on, but SSL option is not set in connection string. Redirection is only possible with SSL.
PASS
1
mysqli_real_connect(): (HY000/2054): mysqlnd_azure.enableRedirect is on, but SSL option is not set in connection string. Redirection is only possible with SSL.
PASS
1
mysqli_real_connect(): (HY000/2054): mysqlnd_azure.enableRedirect is on, but SSL option is not set in connection string. Redirection is only possible with SSL.
PASS
preferred
equal
PASS
Preferred
equal
PASS
2
equal
PASS
2
equal
PASS
off
equal
PASS
Off
equal
PASS
0
equal
PASS
0
equal
PASS
otherValue
equal
PASS
Done