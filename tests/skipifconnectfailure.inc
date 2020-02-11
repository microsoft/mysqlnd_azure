<?php
    include_once('connect.inc');
    $link = mysqli_init();
    if(!link)
        die("skip mysqli_init failed");

    ini_set("mysqlnd_azure.enableRedirect", "preferred");
    $ret = mysqli_real_connect($link, $host, $user, $passwd, $db, $port, NULL, MYSQLI_CLIENT_SSL);
    if (!$ret)
        die(sprintf("skip Can't connect to MySQL Server with ssl - [%d] %s", mysqli_connect_errno(), mysqli_connect_error()));

    if(!$link->info || substr( $link->info, 0, 18 ) != "Location: mysql://") {
        mysqli_close($link);
        die("skip this mysql server does not support redirection, do not find valid redirection info");
    }
    mysqli_close($link);
?>
