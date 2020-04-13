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
    
?>