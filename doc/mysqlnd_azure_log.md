# Diagnostic Log Configuration

Log down some runtime information from mysqlnd\_azure.

## Configuration
Following is the detailed list of the related configuration option and usage to enable logging. 

### mysqlnd\_azure.logOutput
Use this property to set the destination path for your logs. For all the file paths mentioned below, except Apache and php-fpm's system error log, it need to be configured with right user permission, e.g. www-data for Apache. For relative file path, when working in cli, the father path is the current working directoy, when working with Apache, the father path is /.
We provide multiple options to store your logs.

Name |mysqlnd\_azure.logOutput
:----- | :------
Description | Destination for log
Type | Enumeration
Accepted Value | [ 0\| 1\| 2 \| 4 ]
Default | 0 (No log)
Dynamic | No


Value | Behavior | Sample log
:----- | :------ | :----- 
0 | Like logLevel control, it will turn off the log. No log will be print out. | 
1 | Print logs using PHP error log API. <br/> - PHP CLI: Print to console. <br/> - Works with Apache/Nginx: Will be discussed in seperate section below. |  Warning: mysqli_real_connect(): [2020-07-20 14:09:03] [MYSQLND_AZURE] [ERROR] CLIENT_SSL is not set when mysqlnd_azure.enableRedirect is ON in mysqli_basic.php on line 6
2 | Print logs to a pre-defined file by mysqlnd\_azure's config mysqlnd\_azure.logfilePath=xx.log. | [MYSQLND_AZURE] [ERROR] CLIENT_SSL is not set when mysqlnd_azure.enableRedirect is ON
4 | Print logs using stderr. <br/> - PHP CLI: Print to console. <br/> - Works with Apache/Nginx: Will be discussed in seperate section below. | [MYSQLND_AZURE] [ERROR] CLIENT_SSL is not set when mysqlnd_azure.enableRedirect is ON


Note: Nginx with PHP7.2 also works similarly as the below behavior.

#### Detailed behavior for logOutput=1 

This option acts similar as php user script level error_log (https://www.php.net/manual/en/function.error-log.php), with following differences:
1. It has script file name and line name in format (Check the sample log above).
2. It can be configured to log to client web page.
3. The option value syslog of error_log config does not work with this option.

- **For mod-php and Apache (using PHP 7.2 as an example)**

Destination | Config file and options | Turn on switch
:----- | :------ | :-----
A file path specifed in config files with different priorities. | Following files and options control where the log logs to, **prioity is from highest to lowerest**: <br/> (1) **error_log** in /etc/php/7.2/apache2/php.ini <br/><br/> (2) **ErrorLog** in /etc/apache2/sites-available/000-default.conf (This file is Apache's error log path for seperated virtual instance if you define it). <br/><br/> (3) **ErrorLog** in /etc/apache2/apache2.conf (This file is Apache’s global error log path. If ErrorLog is not set for each virtual host, and error_log is not set in php.ini, then log goes here). | **log_errors=on** in /etc/php/7.2/apache2/php.ini
Client web page | **display_errors=on** in /etc/php/7.2/apache2/php.ini |  (1) **log_errors=on** in /etc/php/7.2/apache2/php.ini <br/><br/> **display_errors=on** in /etc/php/7.2/apache2/php.ini. <br/><br/> Please notice this destination is not exclusive to a file path, you may have both.

 
 - **For PHP-FPM and Apache (using PHP 7.2 as an example)**
 
 Destination | Config file and options | Turn on switch
:----- | :------ | :----- 
A file path specifed in config files with different priorities | Following files and options control where the log logs to, **prioity is from highest to lowerest**: <br/> (1) **php_admin_value[error_log]** in /etc/php/7.2/fpm/pool.d/www.conf <br/><br/> (2) **error_log** in /etc/php/7.2/fpm/php.ini <br/><br/> (3) **ErrorLog** in /etc/apache2/sites-available/000-default.conf <br/>Please notice that the logs for single request is in single line since fastcgi treat one request’s log as single line. <br/><br/> (4) **ErrorLog** in /etc/apache2/apache2.conf | (1) **catch_workers_output = yes** in  /etc/php/7.2/fpm/pool.d/www.conf. <br/><br/> (2) Following files and options control the another switch, prioity is from highest to lowerest:<br/> (2-1) **php_admin_flag[log_errors]** in /etc/php/7.2/fpm/pool.d/www.conf <br/> (2-2) **log_errors** in /etc/php/7.2/fpm/php.ini
Client web page | **display_errors=on** in /etc/php/7.2/apache2/php.ini or in /etc/php/7.2/fpm/pool.d/www.conf. | (1) **log_errors** in /etc/php/7.2/fpm/php.ini. <br/> <br/> (2) display_errors=on, **prioity is from highest to lowerest**: <br/> (2-1) **display_errors=on** in /etc/php/7.2/fpm/pool.d/www.conf. <br/> (2-2) **display_errors=on** in /etc/php/7.2/apache2/php.ini. <br/> <br/>Please notice this destination is not exclusive to a file path, you may have both.

#### Detailed behavior for logOutput=4

This option values will use stderr, when intergated with web server, it will be redirected to corrensponding error log files.

- **For mod-php and Apache (using PHP 7.2 as an example)**

Destination | Config file and options | Turn on switch
:----- | :------ | :-----
A file path specifed in Apache's config | **ErrorLog** in /etc/apache2/apache2.conf | It behaviors same with Apache's error log.
 
 - **For PHP-FPM and Apache (using PHP 7.2 as an example)**
 
 Destination | Config file and options | Turn on switch
:----- | :------ | :----- 
A file path specifed in PHP-FPM's config | **error_log** in /etc/php/7.2/fpm/php-fpm.conf | **catch_workers_output = yes** in  /etc/php/7.2/fpm/pool.d/www.conf


 ### mysqlnd\_azure.logfilePath

Name | mysqlnd\_azure.logfilePath
:----- | :------
Description | Filename that you want your log writes to, if mysqlnd\_azure.logOutput=2
Type | String
Accepted Value | String length <= 255, legal filename string
Default | mysqlnd\_azure\_runtime.log .<br>Length cannot exceed the file system's restriction (255). Otherwise, system will use the default file name and throw a warning "Given logfile name too long, redirected to default: mysqlnd_azure_runtime.log
Dynamic | NO
Note | Filename length cannot exceed the file system's restriction(255), otherwise system will use the default filename and throw a warning "Given logfile name too long, redirected to default: mysqlnd_azure_runtime.log"

### mysqlnd\_azure.logLevel
Name | mysqlnd\_azure.logLevel
:----- | :------
Description | The verbose level of log we generate.
Type | Enumeration
Accepted Value | [ 0 \| 1 \| 2 \| 3 ]
Default | 0 (OFF)
Dynamic | Yes
Note | This variable is dynamic, you may use user script API `ini_set('mysqlnd_azure.logLevel', '$VAL'); ` to check log with different verbose level.

#### Loglevel supported
The mysqlnd_azure extension supports following log levels:
- [DEBUG]:
  1. source code path (inside which function);
  2. connection information (except password);
  3. verbose running status (e.g. customer given redirected server information directly... )
- [INFO]:
  1. connection fallback when mysqlnd\_azure.enableRedirection=preferred with details (cannot get redirection
      info from OK packet/ try connect using redirect information failed , ...);
  2. cache operations
- [ERROR]:
  1. any operation may cause customer see connection break (e.x. original conn/ redirected\_conn
    established failed, ... and connection fall back when mysqlnd\_azure.enableRedirection = ON)

- [SYSTM]:
Make log rows for Azure mysqlnd Log Module itself, e.g. invalid file length:
> ```
> 2020-03-27 09:09:05 [SYSTM] Given logfile name too long, redirected to default: mysqlnd_azure_runtime.log
> ```

> note: out of memory errors are not covered by the log.

#### Level value relationship to level logged
- 0: No log written
- 1: [SYSTM] + [ERROR]
- 2: [SYSTM] + [ERROR] + [INFO]
- 3: [SYSTM] + [ERROR] + [INFO] + [DEBUG]


## Usage Example
> You can add to section [mysqlnd\_azure] in file `php.ini` as follows:

```
[mysqlnd_azure]
mysqlnd_azure.enableRedirect = on
mysqlnd_azure.logfilePath = "/tmp/test.log"
mysqlnd_azure.logLevel = 3
mysqlnd_azure.logOutput = 2
```

Please notice you should create the file /tmp/test.log ahead and configure it with right permission for the user.

After successfully configured, try `php -i` in command line or `phpinfo();` in script, there will be var:val pairs listed below
mysqlnd\_azure section, like:

```
mysqlnd_azure

mysqlnd_azure => enableRedirect
enableRedirect => on
logfilePath => /tmp/test.log
logLevel => 3
logOutput => 2
```
