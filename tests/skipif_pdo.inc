<?php
    if (!extension_loaded('mysqlnd')) {
        die('skip mysqlnd extension not available');
    }

    if (!extension_loaded('mysqlnd_azure')) {
        die('skip mysqlnd_azure extension not available');
    }

    if (!extension_loaded('openssl')) {
        die('skip openssl extension not available');
    }

    if (!extension_loaded('pdo')) {
        die('skip the tests are depended n pdo API. pdo extension not available');
    }

    if (!extension_loaded('pdo_mysql')) {
        die('skip the tests are depended n pdo_mysql API. pdo_mysql extension not available');
    }

    include_once('connect.inc');
    ini_set("mysqlnd_azure.enabled", "on");
    $conn = NULL;
    try {
        $conn = new PDO($pdo_dsn, $user, $passwd, 
                    array(PDO::MYSQL_ATTR_SSL_CA => $ssl_cert_file, PDO::ATTR_PERSISTENT=>false));
    }
    catch (Exception $e) {
        die ("skip pdo connection failed. Exception: ".$e->getMessage());
    }
    $host_info = $conn->getAttribute(PDO::ATTR_CONNECTION_STATUS);

    if(substr($host_info, 0, strlen($host)) == $host) {
        die ("skip host info is same, redirection is not enabled on this server");
    }
    $conn = NULL;

?>
