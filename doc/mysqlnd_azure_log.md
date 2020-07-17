# Diagnostic Log Configuration

Log down some running information from mysqlnd\_azure.

## Configuration

Name |mysqlnd\_azure.logOutput
:----- | :------
Comment | Destination for log
Type | Enumeration
Accepted Value | [ 0\| 1\| 2 \| 4 ]
Default | 0 (No log)
Dynamic | No
Note | 1. "0": Turn off the log. <br> 2. "1": Print logs using PHP doc ref call. <br> 3. "2": Print logs to local file. <br> 4. When set to "2"(File), mysqlnd\_azure.logfilePath becomes effective. <br> 5. "4": Print logs to stderr <br>

__note__: If used with Apache, log to stderr is redirected to system wide error.log, besides, PHP doc ref is redirected
to virtual host's error.log. If you want use both, configured this value to 5 (1 | 4).

Name | mysqlnd\_azure.logfilePath
:----- | :------
Comment | Filename that you want your log writes to.
Type | String
Accepted Value | String length <= 255, legal filename string
Default | mysqlnd\_azure\_runtime.log
Dynamic | NO
Note | 1. This variable was inited at PHP Module Init (MINIT) period, and immutable at runtime. a call of `ini_set('mysqlnd_azure.logLevel', '$VAL'); ` would fail. <br> 2. Filename length cannot exceed the file system's restriction(255), otherwise system will use the default filename and throw a warning "Given logfile name too long, redirected to default: mysqlnd_azure_runtime.log"


Name | mysqlnd\_azure.logLevel
:----- | :------
Comment | The verbose level of log we generate.
Type | Enumeration
Accepted Value | [ 0 \| 1 \| 2 \| 3 ]
Default | 0 (OFF)
Dynamic | Yes
Note | 1. When the current `mysqlnd_azure.logLevel > 0`, changing logLevel at runtime will be logged.

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

> [SYSTM] maked log rows for Azure mysqlnd Log Module itself, when current
> `mysqlnd_azure.logLevel > 0`, any operation related to log system itself will be logged.
> For example, if someone call  `ini_set('mysqlnd_azure.logLevel', '2');` at runtime, there
> may appear a log like
> ```
> 2020-03-27 09:09:05 [SYSTM] mysqlnd_azure.logLevel changed: 3 -> 2
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
mysqlnd_azure.enableRedirect = 2
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
enableRedirect => preferred
logfilePath => santotest.log
logLevel => 3
logOutput => 2
```
