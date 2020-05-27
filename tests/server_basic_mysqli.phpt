--TEST--
Test redirection in web server with baisc functionality - mysqli
--SKIPIF--
<?php 
    require_once('skipif_server.inc');
    require_once('skipif.inc');
    require_once('skipif_mysqli.inc');
?>
--CONFLICTS--
server
--FILE--
<?php
include 'server.inc';
$host = cli_server_start();

// start testing
echo "*** Testing mysqli in web server: basic functionality ***\n";

$url = "http://"."{$host}/server_basic_mysqli_testcase.php";

$fp = fopen($url, 'r');
if(!$fp) {
    echo "[000] request url failed \n";
    die();
}
while (!feof($fp)) {
    echo fgets($fp, 4096);
}

fclose($fp);
?>
===DONE===
--EXPECTF--
*** Testing mysqli in web server: basic functionality ***
step1: redirect enabled, non-persistent connection 
mysqlnd_azure.enableRedirect: preferred
%s
Location: mysql://%s
0
step2: redirect enabled, persistent connection 
mysqlnd_azure.enableRedirect: preferred
%s
Location: mysql://%s
0
step3: redirect disabled, non-persistent connection 
mysqlnd_azure.enableRedirect: off
%s
Location: mysql://%s
1
step4: redirect disabled, persistent connection 
mysqlnd_azure.enableRedirect: off
%s

0
step5: redirect enforced, non-ssl connection 
mysqlnd_azure.enableRedirect: on
mysqli_real_connect(): (HY000/2054): mysqlnd_azure.enableRedirect is on, but SSL option is not set in connection string. Redirection is only possible with SSL.
step6: redirect enforced, ssl connection 
mysqlnd_azure.enableRedirect: on
%s
Location: mysql://%s
0
===DONE===