<?php
    function convert_username_format($user) {
        #Azure  The server name you tried cannot be found. Please use the correct name and retry
        $newUser = $user;
        if (strpos($user, "@") !== false) {
            $userName = explode("@", $user)[0];
            $hostName = explode("@", $user)[1];
            $newUser = $hostName."%".$userName;
        } 
        
        return $newUser;
    }
    
    function get_real_username($user) {
        $realUser = $user;
        if (strpos($user, "@") !== false) {
            $realUser = explode("@", $user)[0];
        } else if (strpos($user, "%") != false) {
            $realUser = explode("%", $user)[1];
        }
        return $realUser;
    }
    
    function get_connectionId($link) {
        if(!$res=$link->query("select connection_id() as thread_id;")) {
            printf("[000] [%d] %s\n", mysqli_errno($link), mysqli_error($link));
            $thread_id=0;
        }
        else {
            $tmp = mysqli_fetch_assoc($res);
            $thread_id = $tmp["thread_id"];
            mysqli_free_result($res);
        }
        return intval($thread_id);
    }
?>