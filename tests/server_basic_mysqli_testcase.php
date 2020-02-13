<?php
require_once("connect.inc");

ini_set("mysqlnd_azure.enableRedirect", "preferred");

echo "step1: redirect enabled, non-persistent connection \n";
{
    echo "mysqlnd_azure.enableRedirect: ", ini_get("mysqlnd_azure.enableRedirect"), "\n";
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
}

echo "step2: redirect enabled, persistent connection \n";
{
    echo "mysqlnd_azure.enableRedirect: ", ini_get("mysqlnd_azure.enableRedirect"), "\n";
    $link = mysqli_init();
    $ret = mysqli_real_connect($link, "p:".$host, $user, $passwd, $db, $port, NULL, MYSQLI_CLIENT_SSL);
    if (!$ret || !is_object($link))
    {
        printf("[002] Cannot connect to the server using host=%s, user=%s, passwd=***, dbname=%s, port=%s, socket=%s\n  - [%d] %s with ssl",
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
}

ini_set("mysqlnd_azure.enableRedirect", "off");

echo "step3: redirect disabled, non-persistent connection \n";
{
    echo "mysqlnd_azure.enableRedirect: ", ini_get("mysqlnd_azure.enableRedirect"), "\n";
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
}

echo "step4: redirect disabled, persistent connection \n";
{
    echo "mysqlnd_azure.enableRedirect: ", ini_get("mysqlnd_azure.enableRedirect"), "\n";
    $link = mysqli_init();
    $ret = mysqli_real_connect($link, "p:".$host, $user, $passwd, $db, $port, NULL, MYSQLI_CLIENT_SSL);
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
}

ini_set("mysqlnd_azure.enableRedirect", "on");

echo "step5: redirect enforced, non-ssl connection \n";
{
    echo "mysqlnd_azure.enableRedirect: ", ini_get("mysqlnd_azure.enableRedirect"), "\n";
    $link = mysqli_init();
    $ret = mysqli_real_connect($link, $host, $user, $passwd, $db, $port, NULL, NULL);
    if ($ret || is_object($link))
    {
        printf("[001] when enableRedirect=on, Connection without SSL should fail.\n");
        mysqli_close($link);
        die();
    }
    echo error_get_last()["message"];
}

echo "step6: redirect enforced, ssl connection \n";
{
    echo "mysqlnd_azure.enableRedirect: ", ini_get("mysqlnd_azure.enableRedirect"), "\n";
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
}

?>