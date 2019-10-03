# PHP mysqlnd redirection extension for Azure Database for MySQL (mysqlnd_azure)

mysqlnd_azure is a PHP extension implemented using the [mysqlnd plugin API](https://www.php.net/manual/en/mysqlnd.plugin.php), which provides redirection feature support for the Azure Database for MySQL and MariaDB services.

## Name and Extension Version

- Extension name: `mysqlnd_azure`
- Branch: *mysqlnd-azure-php723-1.0.0* or *tag v1.0.0*.
- Required minimum PHP version: PHP7.2.15+ and PHP7.3.2+

## Build

### Linux

1) Install `phpize` for build using one of the following commands based on your Linux distro

	- Ubuntu: Run `apt-get install php7.3-dev`
	   - In the above command, update the version to correspond with your PHP version. For example, if you use PHP7.3, use `php7.3-dev`.
	- RedHat: Run `yum install php72-php-devel`  
	   - In the above command, update the version to correspond with your PHP version. For example, if you use PHP7,2. use `php72-php-devel`.

2) After installing, if you cannot find `phpize` or `php-config`, you need to link them in order to continue. Use the below commands. Update the version of PHP based on your language version.
    - Run `ln -s /opt/remi/php72/root/bin/phpize /usr/bin/phpize`
    - Run `ln -s /opt/remi/php72/root/bin/php-config /usr/bin/php-config`

3) Choose your folder to hold the source code, e.g. php-mysqlnd_extension, and change directories into it
  - Run `git clone https://github.com/microsoft/mysqlnd_azure`
	- Run `git checkout to branch mysqlnd-azure-php723-1.0.0`
	- Change directories to the `mysqlnd_azure` source code folder
	- Run `phpize`
	- Run `./configure`
	- Run `make`

	After running these commands, you will find a file named `**mysqlnd_azure.so**` under the `./modules` folder.

4) Run `make install` to put the `**mysqlnd_azure.so**` in your PHP so library.

This may not add the configuration file for you automatically. Alternatively, use the following steps:
  - Run `php -i`. You will find two folders **extension_dir** and  **Scan this dir for additional .ini files**, you can find it using `php -i | grep "extension_dir"` or `grep "dir for additional .ini files"`.
  - Put the `mysqlnd_azure.so` in the `extension_dir` folder.
  - Under the directory for `additional .ini files`, you will find the ini files for the commonly used modules (ex. `10-mysqlnd.ini` for mysqlnd, `20-mysqli.ini` for mysqli).
  - Create a new ini file for `mysqlnd_azure` here. **Make sure the alphabetical order of the name is after that of mysqnld**, since the modules are loaded according to the name order of the ini files. (ex. if the `mysqlnd` ini is with named `10-mysqlnd.ini`,then name the ini as `20-mysqlnd-azure.ini`.
  - In the ini file, add the following two lines:
    - `extension=mysqlnd_azure`
    - `mysqlnd_azure.enabled = on`
     - Setting this to OFF disables redirection.

### Windows

#### Set up

To build on Windows, the [php-sdk-binary-tools](https://github.com/Microsoft/php-sdk-binary-tools) are required. Additional documentation for building is available in the [PHP documentation](https://wiki.php.net/internals/windows/stepbystepbuild_sdk_2).

1) Install Visual Studio
    - If compiling PHP 7.2+, install Visual Studio 2017 and open either the **“VS2017 x64 Native Tools Command Prompt”** or the **“VS2017 x86 Native Tools Command Prompt”**.
    - If compiling PHP 7.4+: install Visual Studio 2019

2) Fetch the latest stable SDK tag from https://github.com/Microsoft/php-sdk-binary-tools
    - The new PHP SDK is required, when building PHP 7.2+
    - Read the PHP SDK specific notes on the GitHub repository page

3) Run the **phpsdk_buildtree** batch script which will create the desired directory structure:
  - Run `phpsdk_buildtree phpdev`

4) The *phpsdk_buildtree* script will create the path according to the currently Visual C++ version used and switch into the newly created directory `C:\php-sdk\phpdev\vX##\x##`, where:
    - `vX#`# is the compiler version you are using (ex. vc14 or vs16)
    - `x##` is your architecture (x86 or x64)
    - Example: `C:\php-sdk\phpdev\vc15\x64\` for PHP7.2+

5) Under this folder, clone the PHP source code
    - Run `git clone https://github.com/php/php-src`  
    - Fetch the branch based on the related language version (ex. PHP-7.2.20)

6) Use the PHP SDK tools to fetch the suitable dependencies automatically by calling `phpsdk_deps -u`

7) Extract mysqlnd_azure source code to the ext folder `php-source-code-folder\ext\` or create a "pecl" folder parralel to php-src.
    - Example: `C:\php-sdk\phpdev\vc15\x64\php-src\ext\mysqlnd_azure` or `C:\php-sdk\phpdev\vc15\x64\pecl\mysqlnd_azure`
  
#### Compile

1) Invoke the starter script. For example if using Visual Studio 2017 64-bit to build, invoke `C:\php-sdk\phpsdk-vc15-x64.bat`

2) Run `buildconf`

3) Run `configure.bat --disable-all --enable-cli --with-mysqlnd --enable-mysqlnd_azure=shared`

4) Run `nmake`

5) This creates a DLL under `.\x64\x64\Release_TS\` with the name **php_mysqlnd_azure.dll**. This is the target library.

#### Install

1) Use `php -i` to find the ini to find the extension_dir, copy the **php_mysqlnd_azure.dll** there

2) Find the ini file location, modify the ini file with the following extra lines:
    - `extension=mysqlnd_azure`
    - `mysqlnd_azure.enabled = on`
        - Setting this to OFF disables redirection.

## Test

Currently, redirection is only possible with SSL-enabled connections and the redirection feature switch is enabled on the Azure Database for MySQL server.

The following is a code sample to test connection with redirection:

```php
  echo "mysqlnd_azure.enabled: ", ini_get("mysqlnd_azure.enabled") == true?"On":"Off", "\n";
  $db = mysqli_init();
  $link = mysqli_real_connect ($db, 'your-hostname-with-redirection-enabled', 'user@host', 'password', "db", 3306, NULL, MYSQLI_CLIENT_SSL);
  if (!$link) {
     die ('Connect error (' . mysqli_connect_errno() . '): ' . mysqli_connect_error() . "\n");
  }
  else {
    echo $db->host_info, "\n"; //if redrection succeeds, you will find the host_info differ from your-hostname used to connect
    $res = $db->query('SHOW TABLES;'); //test query with the connection
    print_r ($res);
	$db->close();
  }
```