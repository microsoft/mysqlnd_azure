# PHP mysqlnd redirection extension mysqlnd_azure
The source code here is a PHP extension implemented using mysqlnd plugin API (https://www.php.net/manual/en/mysqlnd.plugin.php), which provides redirection feature support.  The extension is also available on PECL website at  https://pecl.php.net/package/mysqlnd_azure.

## Important notice for prerequisites to use redirection
- There is a limitation for Azure DB for MySQL where redirection is only possible when the connection is configured with SSL and only works with TLS 1.2 with a FIPS approved cipher for redirection.
- On the server side, redirection also needs to be enabled. You can enable redirection by updating the **redirect_enabled** server parameter using the [Azure portal](https://docs.microsoft.com/azure/mysql/howto-server-parameters) or [Azure CLI](https://docs.microsoft.com/azure/mysql/howto-configure-server-parameters-using-cli). You can also use "show global variables like 'redirect_enabled'" to check the current redirection enable status on server side.

## Option Usage
**Before 1.1.0**, the option is with name **mysqlnd_azure.enabled**. Valid values are on/off, and the option "on" supports fallback logic. The detailed usage of the option enableRedirect is as follows:

(Version before 1.1.0. Config name: **mysqlnd_azure.enabled**. Valid value: on/off. Default value: off)
<table>
<tr>
<td>off(0)</td>     
<td> - It will not use redirection. </td>
</tr>
<tr>
<td>on(1)</td>
<td>  - It will use redirection if possible (Connection is with SSL and Server supports/needs redirection).</br>
      - If connection does not use SSL, or server does not support redirection, or redirected connection fails to connect for any non-fatal reason while the proxy connection is still a valid one, it will fallback to the first proxy connection.
</td> 
</tr>
</table>

**Since 1.1.0**, the logic changes as follows:
- The option mysqlnd_azure.enabled is renamed to **mysqlnd_azure.enableRedirect**, option "on" enforces redirection, and there is a new option value "preferred" provided which becomes the default option.
- The detailed usage of the option enableRedirect is as follows:

(Version >= 1.1.0. Config name: **mysqlnd_azure.enableRedirect**. Valid value: on/off/preferred. Default value: preferred)
<table>
<tr>
<td>off(0)</td>
<td> - Redirection will not be used.</td>
</tr>
<tr>
<td>on(1)</td>
<td>  -  If connection does not use SSL on the driver side, no connection will be made. The following error will be returned:
	<i>"mysqlnd_azure.enableRedirect is on, but SSL option is not set in connection string. Redirection is only possible with SSL."</i></br>
      - If SSL is used on the driver side, but redirection is not supported on the server, the first connection is aborted and the following error is returned: <i>"Connection aborted because redirection is not enabled on the MySQL server or the network package doesn't meet redirection protocol."</i></br>
      - If the MySQL server supports redirection, but the redirected connection failed for any reason, also abort the first proxy connection. Return the error of the redirected connection.
</td> 
</tr>
<tr>
<td>
preferred(2)
</td>
<td>  - It will use redirection if possible.</br>
      - If the connection does not use SSL on the driver side, the server does not support redirection, or the redirected connection fails to connect for any non-fatal reason while the proxy connection is still a valid one, it will fall back to the first proxy connection
</td> 
</tr>
</table>

## Name and Extension Version
Extension name: **mysqlnd_azure**

Required PHP min version: PHP7.2.15+ and PHP7.3.2+.

Valid version:
The latest version is **1.1.1**. Please check [package.xml](/package.xml) for changelog details. Following are some special notice for certain versions:

- 1.1.1  Enable runtime log check support. Fix crash problem when mysqlnd.collect_memory_statistics=1.
- 1.1.0  Rename option mysqlnd_azure.enabled to mysqlnd_azure.enableRedirect, and add a new option value "preferred". When enableRedirect is "on", it will enforce redirection. Please check [package.xml](/package.xml) for changelog details.
- 1.0.2  Limitation: crash when working with PDO interface with flag PDO::ATTR_PERSISTENT=>false
- 1.0.1  Limitation: cannot work with 7.2.23+ and 7.3.10+
- 1.0.0  Limitation: cannot install with pecl on linux; cannot work with 7.2.23+ and 7.3.10+

Following is a brief guide of how to install using pecl or build and test the extension from source. 

## Step to install using PECL
The PECL link is available at  https://pecl.php.net/package/mysqlnd_azure.
Following steps assume that php and php-mysql have already been normally installed on each platform.
#### Setup
###### Linux
Example to install required tools for php7.3 on Ubuntu:
- sudo apt-get install php-pear
- sudo apt-get install php7.3-dev

Example to install required tools for php7.3 on Redhat 7.4:
- sudo yum --enablerepo=epel,remi-php73 install php-devel
- sudo yum --enablerepo=epel,remi-php73 install php-pear
- //If you cannot find the package, there may be a need to renable related repository, e.g. enable REMI,EPEL using following command
- rpm -Uvh https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
- rpm -Uvh http://rpms.famillecollet.com/enterprise/remi-release-7.rpm

#### Install
###### Linux
sudo pecl install mysqlnd_azure 

For beta version like 1.0.3RC, please specify the version number, e.g. *pecl install mysqlnd_azure-1.0.3RC*

###### Windows
Download the corresponding dll package from the DLL link on https://pecl.php.net/package/mysqlnd_azure. Extract the zip file, find the dll file named with *php_mysqlnd_azure.dll*, and put it under extension_dir (Follow the configure step below to get the value).

You may use following command to check the thread safety setting of the php:
- php -i | findstr "Thread"

If you cannnot determine the x86/64 version of the php, you may use following script to figure out:
```php
switch(PHP_INT_SIZE) {
    case 4:
        echo '32-bit version of PHP', "\n";
        break;
    case 8:
        echo '64-bit version of PHP', "\n";
        break;
    default:
        echo 'PHP_INT_SIZE is ' . PHP_INT_SIZE, "\n";
}
```
#### Configure
The configuration step is same with that of build from source. Check content below for Linux or Windows platform.


## Step to build on Linux
#### Setup
* Install phpize for build using following command
  - Ubuntu: apt-get install php7.x-dev  
  //use the version that corresponds to your PHP version, i.e. if you use PHP7.3, use php7.3-dev here

  - Redhat: yum install php7x-php-devel  
  	//e.g. php72-php-devel. after install if you still cannot find phpize or php-config, you need to link phpize and php-config in order to make it work correctly:
	- ln -s /opt/remi/php72/root/bin/phpize /usr/bin/phpize
	- ln -s /opt/remi/php72/root/bin/php-config /usr/bin/php-config
	- //If you cannot find the package, there may be a need to renable related repository, e.g. enable REMI,EPEL using following command
   	- rpm -Uvh https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
	- rpm -Uvh http://rpms.famillecollet.com/enterprise/remi-release-7.rpm
	
#### Compile
* Chose your folder to hold the source code, e.g. mysqlnd_azure, and cd to it
* git clone https://github.com/microsoft/mysqlnd_azure .
* git checkout to corresponding branch
* run
  - phpize
  - ./configure
  - make 
* After that, you will find a .so file named **mysqlnd_azure.so** under ./modules folder. 

#### Install and configure
Then you can run **make install** to put the .so to your php so library. However this may not add the configuration file for you automatically. So the alternative way is using following steps:
  - run php -i, you will find two field **extension_dir** and  **Scan this dir for additional .ini files**, you can find it using php -i | grep "extension_dir" or grep "dir for additional .ini files".
  - put mysqlnd_azure.so under extension_dir.
  - under directory for additional .ini files, you will find the ini files for the common used modules, e.g. 10-mysqlnd.ini for mysqlnd, 20-mysqli.ini for mysqli. Create a new ini file for mysqlnd_azure here. **Make sure the alphabet order of the name is after that of mysqnld**, since the modules are loaded according to the name order of the ini files. E.g. if mysqlnd ini is with name 10-mysqlnd.ini,then name the ini as 20-mysqlnd-azure.ini. In the ini file, add the following two lines:
      - extension=mysqlnd_azure
      - mysqlnd_azure.enableRedirect = on/off/preferred
      	- **Notice:** since 1.1.0, if this value is set to on, the connection must be configured with SSL, and it requires server support redirection. Otherwise, the connection will fail. Please check the Option Usage section for detailed information.


## Step to build on Windows
#### Set up 
There need more steps to build the extension under Windows, and it willl use the php-sdk-binary-tools developed for help. The detailed step is decribed on https://wiki.php.net/internals/windows/stepbystepbuild_sdk_2. And it is repeated here:
* If compiling PHP 7.2+:
  Install Visual Studio 2017
  If compiling PHP 7.4+:
  Install Visual Studio 2019

* If compiling PHP 7.2+ open either the **“VS2017 x64 Native Tools Command Prompt”** or the **“VS2017 x86 Native Tools Command Prompt”**.
* Fetch the latest stable SDK tag from https://github.com/Microsoft/php-sdk-binary-tools
  The new PHP SDK is required, when building PHP 7.2+
  Read the PHP SDK specific notes on the Github repository page

* Run the phpsdk_buildtree batch script which will create the desired directory structure: 
  - phpsdk_buildtree phpdev
* The phpsdk_buildtree script will create the path according to the currently VC++ version used and switch into the newly created directory. 
  cd to C:\php-sdk\phpdev\vX##\x##, where:
    vX## is the compiler version you are using (eq vc14 or vs16)
    x## is your architecture (x86 or x64)
    For example: C:\php-sdk\phpdev\vc15\x64\  for PHP7.2+
* Under the folder, git clone php source code from https://github.com/php/php-src.git  Fecth the related version, e.g. PHP-7.2.20
* cd to php-src directory
* use the PHP SDK tools to fetch the suitable dependencies automatically for a valid branch by calling 
  - phpsdk_deps -u
* Extract mysqlnd_azure code https://github.com/microsoft/mysqlnd_azure to the php-source-code-folder\ext\mysqlnd_azure(a new directory need to create), e.g. to C:\php-sdk\phpdev\vc15\x64\php-src\ext\mysqlnd_azure. Or create another folder with name "pecl" which is in parallel with php source, e.g. C:\php-sdk\phpdev\vc15\x64\pecl, and extract the code there in folder mysqlnd_azure.
After this, the code directory should look like C:\php-sdk\phpdev\vc15\x64\php-src\ext\mysqlnd_azure, or C:\php-sdk\phpdev\vc15\x64\pecl\mysqlnd_azure
  
#### Compile
* Invoke the starter script, for example for Visual Studio 2017 64-bit build, invoke     
  - phpsdk-vc15-x64.bat
* checkout to corresponding branch
* run **buildconf**
* run **configure.bat --disable-all --enable-cli --with-mysqlnd --enable-mysqlnd_azure=shared**
* run **nmake**
* after that, you will find a dll under .\x64\Release_TS\ with name **php_mysqlnd_azure.dll**, this is the target library.

#### Install and configure
* Use php -i | find "extension_dir" to find the extension_dir, copy php_mysqlnd_azure.dll there
* Use php -i | find "Loaded Configuration File" to find the ini file location, modify the ini file with the following extra lines:
    - Under the Dynamic Extensions section add:
    	- extension=mysqlnd_azure
    - Under the Module Settings section add:
    	- [mysqlnd_azure]
    	- mysqlnd_azure.enableRedirect = on/off/preferred
      	- **Notice:** since 1.1.0, if this value is set to on, the connection must be configured with SSL, and it requires server support redirection. Otherwise, the connection will fail. Please check the Option Usage section for detailed information.


## Test
* Currently redirection is only possible when the connection is via ssl, and it need that the redirection feature switch is enabled on server side. Following is a snippet to test connection with redirection:

```php
  echo "mysqlnd_azure.enableRedirect: ", ini_get("mysqlnd_azure.enableRedirect"), "\n";
  $db = mysqli_init();
  //The connection must be configured with SSL for redirection test
  $link = mysqli_real_connect ($db, 'your-hostname-with-redirection-enabled', 'user@host', 'password', "db", 3306, NULL, MYSQLI_CLIENT_SSL);
  if (!$link) {
     die ('Connect error (' . mysqli_connect_errno() . '): ' . mysqli_connect_error() . "\n");
  }
  else {
    echo $db->host_info, "\n"; //if redrection succeed, you will find the host_info differ from your-hostname used to connect
    $res = $db->query('SHOW TABLES;'); //test query with the connection
    print_r ($res);
	$db->close();
  }
```

## Troubleshooting
To troubleshoot issues when using this extension, you may follow the steps described in [troubleshooting.md](/troubleshooting.md) for initial troubleshooting.

### Diagnostic Log for mysqlnd\_azure
[Configuration to get more runtime logs](/mysqlnd_azure_log.md)
