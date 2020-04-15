<?php
if (!extension_loaded('mysqlnd')) {
    die('skip mysqlnd extension not available');
}

if (!extension_loaded('openssl')) {
    die('skip openssl extension not available');
}

if (!extension_loaded('mysqli')) {
    die('skip the tests are depended n mysqli API. mysqli extension not available');
}
?>
