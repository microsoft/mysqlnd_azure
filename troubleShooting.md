# Trouble shooting

If you encounter PHP related crash problem with this extension, you can follow following steps for inital trouble shooting:

1. Check the config mysqlnd_azure.enableRedirect option's choice. If it is off now, big change is that the problem is not caused by this extension. Otherwise, switch it to off to see whether the problem still exist.

2. If the problem only persistents when the option is not with value off, then please contact with us for detailed investigation. Before this, helping prepare the following information will be highly appreciated:

## Web server log
E.g. on Ubuntu, the related error log paths  usually are: 
     - Apache: /var/log/apache2/error.log
     - Nginx: /var/log/nginx/error.log
     - php-fpm:  /var/log/php7.2-fpm.log (for 7.2)

Please check the log, and provide the related content when crash happened.

## Crash dump file

**Note:** Before config to get the dump file, you may need to disable apport or abrtd service/demon, since it will prevent creation of core dump files.

### dump file for php-fpm (Linux)
You may refer to the following link to get a php-fpm dump file:
https://ma.ttias.be/generate-php-core-dumps-segfaults-php-fpm/
Following are the detailed steps:

1. Choose a directory for the dump file, e.g. /var/coredumps. Then set it in /proc/sys/kernel/core_pattern
```bash
sudo bash -c 'echo "/var/coredumps/core-%e.%p" > /proc/sys/kernel/core_pattern'
```
Then the dump file will be under /var/coredumps, with name like core-php-fpm7.2.pid

2. Create the directory, and make sure it is writable for the dump, e.g. 
```bash
mkdir /var/coredumps
chown www-data: /var/coredumps
chmod 777 /var/coredumps
```
3. Set rlimit_core = unlimited in php-fpm pool config file, e.g.
/etc/php/7.2/fpm/pool.d/www.conf
```conf
rlimit_core = unlimited
process.dumpable = yes
```
4. Restart php-fpm service, e.g. 
```bash
service php7.2-fpm restart
```
5. Check whether the setup works:
```bash
     ps auxf | grep php-fpm | grep -v grep  #to get a child process id
     kill  -SIGSEGV a_child_process_id
     ls /var/coredumps #check file listed
```

### Dump file for Apache with mod_php (Linux)
You may refer to the following link to get a Apache dump file:
https://support.plesk.com/hc/en-us/articles/213366549-How-to-enable-core-dumps-for-Apache-and-trace-Apache-segmentation-fault-on-a-Linux-server

Following are the detailed steps (take apache2 on Ubuntu as example):
1. Follow the same step with php-fpm above to set up the dump file directory.
2. 
```bash 
systemctl edit apache2
```
and add the following lines:
```
[Service]
LimitCORE=infinity
```
3. Open the /etc/init.d/apache2 file and locate the do_start() section. Add the following line in this section:
```
ulimit -c unlimited
```
4. Specify a CoreDumpDirectory location in /etc/apache2/apache2.conf :
```
CoreDumpDirectory /var/coredumps
```
5. Restart Apache service 
```
service apache2 restart
```

6. Check whether the setup works:
```
     ps auxf | grep apache2  | grep -v grep  #to get a child process id
     kill  -SIGSEGV a_child_process_id
     ls /var/coredumps
```


### Dump file for Apache with mod_php (Windows)
You may refer to the following link to get a Apache dump file:
https://bugs.php.net/bugs-generating-backtrace-win32.php

Following are the detailed steps:
1. Download the corresponding version of debug pack for php-src (https://windows.php.net/download/) and mysqlnd_azure (https://pecl.php.net/package/mysqlnd_azure
 from the DLL link, the pdb file is under same package)
2. Extract the package and put them in same location, e.g. D:/php-debug-pack
3. Download/install the Windows Debug Diagnostic Tool from e.g. https://www.microsoft.com/en-us/download/details.aspx?id=58210
4. Open the DebugDiag Collection
5. Select the Tools menu and click on "Options and settings". The first tab contains the path to the symbols files, add the "debug pack folder" just set
6. We will use the wizard, click the "Add Rule..." button at bottom and choose "Crash" as the rule type.
Click Next and choose "A specific process", chose the process you want to monitor and activate the rule. After a crash happened, a dump will be created 
under the spedicifed rule folder.
