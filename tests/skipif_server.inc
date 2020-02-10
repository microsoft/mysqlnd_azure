<?php
    if(false === getenv('PHP_HTTP_REMOTE_SERVER')) {
        if (php_sapi_name() != "cli") {
                die("skip PHP_HTTP_REMOTE_SERVER env variable is not defined");
        }
    }
