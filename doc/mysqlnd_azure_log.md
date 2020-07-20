# Diagnostic Log Configuration

Log down some runtime information from mysqlnd\_azure.

## Configuration

### Log destination supported (mysqlnd\_azure.logOutput)

**Notice**
For all the file paths mentioned below, except Apache and php-fpm's system error log, it need to be configured with right user permission, e.g. www-data for Apache. For relative file path, when working in cli, the father path is the current working directoy, when working with Apache, the father path is /.

We provide multiple options for log destination you may want to choose. 

Name |mysqlnd\_azure.logOutput
:----- | :------
Comment | Destination for log
Type | Enumeration
Accepted Value | [ 0\| 1\| 2 \| 4 ]
Default | 0 (No log)
Dynamic | No


Value | Behavior | Sample log
:----- | :------ | :----- 
0 | Like logLevel control, it will turn off the log. No log will be print out | 
1 | Print logs using PHP error log API. <br/> PHP CLI: Print to console <br/> Work scenario with Apache/Nginx: Will be discussed in seperated section below |  Warning: mysqli_real_connect(): [2020-07-20 14:09:03] [MYSQLND_AZURE] [ERROR] CLIENT_SSL is not set when mysqlnd_azure.enableRedirect is ON in mysqli_basic.php on line 6
2 | Print logs to a pre-defined file by mysqlnd\_azure's config mysqlnd\_azure.logfilePath=xx.log | [MYSQLND_AZURE] [ERROR] CLIENT_SSL is not set when mysqlnd_azure.enableRedirect is ON
4 | Print logs using stderr. <br/> PHP CLI: Print to console <br/> Work scenario with Apache/Nginx: Will be discussed in seperated section below | [MYSQLND_AZURE] [ERROR] CLIENT_SSL is not set when mysqlnd_azure.enableRedirect is ON

#### Detailed behavior for logOutput1 and logOutput4
- **For mod-php and Apache (take php7.2 as example)**

Config file | Config options | Usage
:----- | :------ | :----- 
/etc/php/7.2/apache2/php.ini | -error_log <br/> -log_error <br/> -display_errors | This file is Apache mod_php's config file s <br/> -log_error: logOutput1 is only available if this is on. <br/> error_log: where logOutput1 goes into. <br/> --1.	error_log=syslog:  logOutput1 is not available with this option. <br/> --2.	error_log=A_file_path. It will take priority of apache’s error log setting. <br/> display_errors: Whether print logOutput1 to a client web page.
/etc/apache2/sites-available/000-default.conf | ErrorLog | This file is Apache's error log path for seperated virtual instance. <br/> If error_log is not set in php.ini above, logOutput1 will go here.
/etc/apache2/apache2.conf | ErrorLog | This file is Apache’s global error log path. <br/> logOutput4 goes here. <br/> If ErrorLog is not set for each virtual host, and error_log is not set in php.ini, then logOutput4 also goes here.
 
 - **For php-fpm and Apache (take php7.2 as example)**
 
 Config file | Config options | Usage
:----- | :------ | :----- 
/etc/php/7.2/fpm/php-fpm.conf | error_log	| This Php-fpm’s system error log, has content like php-fpm is restarted or crashed. <br/> -- logOutput4 will go here if catch_workers_output = yes in www.conf. <br/> logOutput1 will also go here if catch_workers_output = yes in www.conf && log_errors=ON && error_log is not set in php.ini and www.conf below.
/etc/php/7.2/fpm/php.ini | - error_log <br/> - log_error <br/> - display_errors	| - error_log: The destination logOutput1 goes to. <br/> - log_error: same with mod_php. <br/> - display_errors: same with mod_php.
/etc/php/7.2/fpm/pool.d/www.conf	| - catch_workers_output <br/> - php_admin_value[error_log] <br/> - php_admin_flag[log_errors] - php_flag[display_errors] | - catch_workers_output: <br/> Whether catch logOutput4 and logOutput1. <br/> -- php_admin_value[error_log]: <br/> -- php_admin_flag[log_errors]: <br> -- php_flag[display_errors]: <br/> Same as that in php.ini, but the definitions here take priority.
/etc/apache2/sites-available/000-default.conf	| ErrorLog	| logOutput1 will go here if the error_log is not set in php.ini and www.conf. <br/> Please notice that the logs for single request is in single line since fastcgi treat one request’s log as single line)
/etc/apache2/apache2.conf	| ErrorLog	| Apache’s global error log path. <br/> logOutput1 will go here if the error_log is not set in php.ini and www.conf and ErrorLog is not set in each virtual instance


 ### mysqlnd\_azure.logfilePath

Name | mysqlnd\_azure.logfilePath
:----- | :------
Comment | Filename that you want your log writes to, if mysqlnd\_azure.logOutput=2
Type | String
Accepted Value | String length <= 255, legal filename string
Default | mysqlnd\_azure\_runtime.log
Dynamic | NO
Note | 1. This variable is not dynamic, only can set in php.ini, not possible by `ini_set('mysqlnd_azure.logLevel', '$VAL'); ` <br> 2. Filename length cannot exceed the file system's restriction(255), otherwise system will use the default filename and throw a warning "Given logfile name too long, redirected to default: mysqlnd_azure_runtime.log"

 ### mysqlnd\_azure.logLevel
Name | mysqlnd\_azure.logLevel
:----- | :------
Comment | The verbose level of log we generate.
Type | Enumeration
Accepted Value | [ 0 \| 1 \| 2 \| 3 ]
Default | 0 (OFF)
Dynamic | Yes

### Loglevel supported
we support 3 level of logs for user:
- [DEBUG]:
  1. source code path(inside which function);
  2. connection information(except password);
  3. verbose running status (e.g. customer given redirected server information directly... )
- [INFO]:
  1. connection fallback when mysqlnd\_azure.enableRedirection=preferred with details (cannot get redirection
      info from OK packet/ try connect using redirect information failed , ...);
  2. cache operations
- [ERROR]:
  1. any operation may cause customer see connection break (e.x. original conn/ redirected\_conn
    established failed, ... and connection fall back when mysqlnd\_azure.enableRedirection = ON)

> [SYSTM] maked log rows for Azure mysqlnd Log Module itself, e.g. invalid file length:
> ```
> 2020-03-27 09:09:05 [SYSTM] Given logfile name too long, redirected to default: mysqlnd_azure_runtime.log
> ```

> note: OOM not included this version.

#### Level value relationship to level logged
- 0: No log written
- 1: [SYSTM] + [ERROR]
- 2: [SYSTM] + [ERROR] + [INFO]
- 3: [SYSTM] + [ERROR] + [INFO] + [DEBUG]


## Usage Example
> You can add to section [mysqlnd\_azure] in file `php.ini`

```
[mysqlnd_azure]
mysqlnd_azure.enableRedirect = on
mysqlnd_azure.logfilePath = "test.log"
mysqlnd_azure.logLevel = 3
mysqlnd_azure.logOutput = 2
```

### phpinfo() / php -i

After successfully configured, try `php -i`, there will be var:val pairs listed below
mysqlnd\_azure section, like:

```
mysqlnd_azure

mysqlnd_azure => enableRedirect
enableRedirect => on
logfilePath => test.log
logLevel => 3
logOutput => 2
```
