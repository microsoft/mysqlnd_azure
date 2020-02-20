# Troubleshooting

## Possible FAQs
1. How to ensure my application uses redirection with this extension to connect to Azure Database for MySQL?

Answer:  Set mysqlnd_azure.enableRedirect=on   The option value "on" will enforce redirection and return error if the configuration is wrong, e.g. the connection is not configured with SSL.

2. How to verify the connection is currently using redirection?

Answer:  For mysqli interface, check the "host_info" property of the connection object. For PDO interface, check the "PDO::ATTR_CONNECTION_STATUS" attribute.
If the connection is using redirection, the content will be different from the host info you used to connect. Otherwise, it should contains the same value.

## PHP related crash problem with this extension

If there is PHP related crash problem with this extension, you can follow following steps for initial troubleshooting:

1. Check the configuration of option mysqlnd_azure.enableRedirect.

 Usually, the related config for PHP served for a web service is under /etc/php/x.x/mods-available/,  E.g. /etc/php/7.2/mods-available/.
 
If it's on/preferred now, switch it to off first to see whether the problem still exist. If the problem still exists, then try to uninstall the plugin to check the situation.

2. If the problem disappeared after the plugin is uninstalled, then please contact us, and helping provide related web server log will be highly appreciated:

## Web server log
E.g. on Ubuntu, the related error log paths  usually are: 
     
     - Apache:      /var/log/apache2/error.log
     - Nginx:       /var/log/nginx/error.log
     - php-fpm:     /var/log/php7.2-fpm.log (for 7.2)

Please check the log, and provide the related content when crash happened.
