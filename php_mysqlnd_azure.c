/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) The PHP Group                                          |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Authors: Qianqian Bu <qianqian.bu@microsoft.com>                     |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "mysqlnd_azure.h"
#include "php_mysqlnd_azure.h"
#include "utils.h"
#include "ext/mysqlnd/mysqlnd_ext_plugin.h"
#include "ext/standard/info.h"

ZEND_DECLARE_MODULE_GLOBALS(mysqlnd_azure)

/* {{{ OnUpdateEnableRedirect */
static ZEND_INI_MH(OnUpdateEnableRedirect)
{
    if (STRING_EQUALS(new_value,"preferred") 
        || STRING_EQUALS(new_value, "2")) {

        MYSQLND_AZURE_G(enableRedirect) = REDIRECT_PREFERRED;

    } else if (STRING_EQUALS(new_value, "on") 
        || STRING_EQUALS(new_value, "yes") 
        || STRING_EQUALS(new_value, "true") 
        || STRING_EQUALS(new_value, "1")) {

        MYSQLND_AZURE_G(enableRedirect) = REDIRECT_ON;

    } else {

        MYSQLND_AZURE_G(enableRedirect) = REDIRECT_OFF;

    }

    return SUCCESS;

}

static ZEND_INI_MH(OnUpdateEnableLogfile) {
  MYSQLND_AZURE_G(logfilePath) = new_value;
  return SUCCESS;
}

static ZEND_INI_MH(OnUpdateEnableLogLevel) {
  // Loglevel is a PHP_INI_ALL variable,
  // every time it changed will be logged (unless it is 0 now)
  AZURE_LOG_SYS("mysqlnd_azure.logLevel changed: %d -> %d",
      MYSQLND_AZURE_G(logLevel), atoi(ZSTR_VAL(new_value)));

  if (STRING_EQUALS(new_value, "3")) {
    MYSQLND_AZURE_G(logLevel) = 3;
  } else if (STRING_EQUALS(new_value, "2")) {
    MYSQLND_AZURE_G(logLevel) = 2;
  } else if (STRING_EQUALS(new_value, "1")) {
    MYSQLND_AZURE_G(logLevel) = 1;
  } else {
    MYSQLND_AZURE_G(logLevel) = 0;
  }
  return SUCCESS;
}

static ZEND_INI_MH(OnUpdateEnableLogOutput) {
  int tval = atoi(ZSTR_VAL(new_value));
  if (tval > 0 && tval < 8) {
    MYSQLND_AZURE_G(logOutput) = tval;
  } else {
    MYSQLND_AZURE_G(logOutput) = 0;
  }

  return SUCCESS;
}
/* }}} */

/* {{{ PHP_INI */
PHP_INI_BEGIN()
STD_PHP_INI_ENTRY("mysqlnd_azure.enableRedirect", "preferred", PHP_INI_ALL, OnUpdateEnableRedirect, enableRedirect, zend_mysqlnd_azure_globals, mysqlnd_azure_globals)
STD_PHP_INI_ENTRY("mysqlnd_azure.logfilePath", "mysqlnd_azure_runtime.log", PHP_INI_SYSTEM, OnUpdateEnableLogfile, logfilePath, zend_mysqlnd_azure_globals, mysqlnd_azure_globals)
STD_PHP_INI_ENTRY("mysqlnd_azure.logLevel", "0", PHP_INI_ALL, OnUpdateEnableLogLevel, logLevel, zend_mysqlnd_azure_globals, mysqlnd_azure_globals)
STD_PHP_INI_ENTRY("mysqlnd_azure.logOutput", "0", PHP_INI_SYSTEM, OnUpdateEnableLogOutput, logOutput, zend_mysqlnd_azure_globals, mysqlnd_azure_globals)
PHP_INI_END()
/* }}} */

/* {{{ PHP_GINIT_FUNCTION */
static PHP_GINIT_FUNCTION(mysqlnd_azure)
{
#if defined(COMPILE_DL_MYSQLND_AZURE) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif
    mysqlnd_azure_globals->enableRedirect = REDIRECT_PREFERRED;
    mysqlnd_azure_globals->redirectCache = NULL;
    mysqlnd_azure_globals->logLevel = 0;
}
/* }}} */

/* {{{ PHP_GSHUTDOWN_FUNCTION */
static PHP_GSHUTDOWN_FUNCTION(mysqlnd_azure)
{
    if (mysqlnd_azure_globals->redirectCache) {
        zend_hash_destroy(mysqlnd_azure_globals->redirectCache);
        mnd_pefree(mysqlnd_azure_globals->redirectCache, 1);
        mysqlnd_azure_globals->redirectCache = NULL;
    }
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
static PHP_MINIT_FUNCTION(mysqlnd_azure)
{
  /* globals, ini entries, resources, classes */
  REGISTER_INI_ENTRIES();
  /* register mysqlnd plugin */
  mysqlnd_azure_minit_register_hooks();

  mysqlnd_azure_apply_resources();

  return SUCCESS;
}

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
static PHP_MSHUTDOWN_FUNCTION(mysqlnd_azure)
{
    mysqlnd_azure_release_resources();

    UNREGISTER_INI_ENTRIES();

    return SUCCESS;
}

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(mysqlnd_azure)
{

    php_info_print_table_start();
    php_info_print_table_header(2, "mysqlnd_azure", "enableRedirect");
    php_info_print_table_row(2, "enableRedirect", MYSQLND_AZURE_G(enableRedirect) == REDIRECT_OFF ? "off" : (MYSQLND_AZURE_G(enableRedirect) == REDIRECT_ON ? "on" : "preferred"));
    php_info_print_table_row(2, "logfilePath", ZSTR_VAL(MYSQLND_AZURE_G(logfilePath)));
    char tmp[2];
    sprintf(tmp, "%d", MYSQLND_AZURE_G(logLevel));
    php_info_print_table_row(2, "logLevel", tmp);
    sprintf(tmp, "%d", MYSQLND_AZURE_G(logOutput));
    php_info_print_table_row(2, "logOutput", tmp);
    php_info_print_table_end();
}
/* }}} */

static const zend_module_dep mysqlnd_azure_deps[] = {
    ZEND_MOD_REQUIRED("mysqlnd")
    ZEND_MOD_END
};

/* {{{ mysqlnd_azure_module_entry*/
zend_module_entry mysqlnd_azure_module_entry = {
    STANDARD_MODULE_HEADER_EX,
    NULL,
    mysqlnd_azure_deps,
    PHP_MYSQLND_AZURE_NAME,
    NULL,
    PHP_MINIT(mysqlnd_azure),
    PHP_MSHUTDOWN(mysqlnd_azure),
    NULL,
    NULL,
    PHP_MINFO(mysqlnd_azure),
    PHP_MYSQLND_AZURE_VERSION,
    PHP_MODULE_GLOBALS(mysqlnd_azure),
    PHP_GINIT(mysqlnd_azure),
    PHP_GSHUTDOWN(mysqlnd_azure),
    NULL,
    STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_MYSQLND_AZURE
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(mysqlnd_azure)
#endif
