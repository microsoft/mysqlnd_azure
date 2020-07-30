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
Accepted Value | [ 0\| 1\| 2 ]
Default | 0 (No log)
Dynamic | No


Value | Behavior | Sample log
:----- | :------ | :----- 
0 | Like logLevel control, it will turn off the log. No log will be print out. | 
1 | Print logs using stderr. <br/> - PHP CLI: Print to console. <br/> - Works with Apache/Nginx: Will be discussed in seperate section below. | [2020-07-29 10:45:16] [MYSQLND_AZURE] [ERROR] CLIENT_SSL is not set when mysqlnd_azure.enableRedirect is ON
2 | Print logs to a pre-defined file set by mysqlnd\_azure's config mysqlnd\_azure.logfilePath=xx.log. | [2020-07-29 10:46:16] [MYSQLND_AZURE] [ERROR] CLIENT_SSL is not set when mysqlnd_azure.enableRedirect is ON

Note: Nginx with PHP-FPM works similarly as the below corrensponding behavior.

#### Detailed behavior for logOutput=1 when works with Apache

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
Description | Filename log writes to when mysqlnd\_azure.logOutput=2
Type | String
Accepted Value | A legal filename string. String length <= 255. If the file path contains non-exist sub-directories or work with Web server, you should create it beforehand, and configure it with right user permission, e.g. www-data for Apache. Relative path is not recommended.
Default | NULL
Dynamic | NO

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

> note: out of memory errors are not covered by the log.

#### Level value relationship to level logged
- 0: No log written
- 1: [ERROR]
- 2: [ERROR] + [INFO]
- 3: [ERROR] + [INFO] + [DEBUG]


## Usage Example
> You can add to section [mysqlnd\_azure] in file `php.ini` as follows, which uses logOutput=2 (logs to file logfilePath) and sets logLevel to most verbose level 3:

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
