<?php
require_once("connect.inc");

ini_set("mysqlnd_azure.enableRedirect", "preferred");

echo "step1: redirect enabled, non-persistent connection \n";
{
    echo "mysqlnd_azure.enableRedirect: ", ini_get("mysqlnd_azure.enableRedirect"), "\n";
    $conn = NULL;
    try {
        $conn = new PDO($pdo_dsn, $user, $passwd,
                    array(PDO::MYSQL_ATTR_SSL_CA => $ssl_cert_file, PDO::ATTR_PERSISTENT=>false));
    }
    catch (Exception $e) {
        die ("[001] pdo connection failed. Exception: ".e.getMessage()."\n");
    }

    $host_info = $conn->getAttribute(PDO::ATTR_CONNECTION_STATUS);
    echo $host_info, "\n";

    if(substr($host_info, 0, strlen($host)) == $host)
        echo "1\n";
    else
        echo "0\n";
    $conn = NULL;
}

echo "step2: redirect enabled, persistent connection \n";
{
    echo "mysqlnd_azure.enableRedirect: ", ini_get("mysqlnd_azure.enableRedirect"), "\n";
    $conn = NULL;
    try {
        $conn = new PDO($pdo_dsn, $user, $passwd,
                    array(PDO::MYSQL_ATTR_SSL_CA => $ssl_cert_file, PDO::ATTR_PERSISTENT=>true));
    }
    catch (Exception $e) {
        die ("[002] pdo connection failed. Exception: ".e.getMessage()."\n");
    }

    $host_info = $conn->getAttribute(PDO::ATTR_CONNECTION_STATUS);
    echo $host_info, "\n";

    if(substr($host_info, 0, strlen($host)) == $host)
        echo "1\n";
    else
        echo "0\n";
    $conn = NULL;
}

ini_set("mysqlnd_azure.enableRedirect", "off");

echo "step3: redirect disabled, non-persistent connection \n";
{
    echo "mysqlnd_azure.enableRedirect: ", ini_get("mysqlnd_azure.enableRedirect"), "\n";
    $conn = NULL;
    try {
        $conn = new PDO($pdo_dsn, $user, $passwd,
                    array(PDO::MYSQL_ATTR_SSL_CA => $ssl_cert_file, PDO::ATTR_PERSISTENT=>false));
    }
    catch (Exception $e) {
        die ("[003] pdo connection failed. Exception: ".e.getMessage()."\n");
    }

    $host_info = $conn->getAttribute(PDO::ATTR_CONNECTION_STATUS);
    echo $host_info, "\n";

    if(substr($host_info, 0, strlen($host)) == $host)
        echo "1\n";
    else
        echo "0\n";
    $conn = NULL;
}

echo "step4: redirect disabled, persistent connection \n";
{
    echo "mysqlnd_azure.enableRedirect: ", ini_get("mysqlnd_azure.enableRedirect"), "\n";
    $conn = NULL;
    try {
        $conn = new PDO($pdo_dsn, $user, $passwd,
                    array(PDO::MYSQL_ATTR_SSL_CA => $ssl_cert_file, PDO::ATTR_PERSISTENT=>true));
    }
    catch (Exception $e) {
        die ("[004] pdo connection failed. Exception: ".e.getMessage()."\n");
    }

    $host_info = $conn->getAttribute(PDO::ATTR_CONNECTION_STATUS);
    echo $host_info, "\n";

    if(substr($host_info, 0, strlen($host)) == $host)
        echo "1\n";
    else
        echo "0\n";
    $conn = NULL;
}

?>