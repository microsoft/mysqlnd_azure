/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 2006-2018 The PHP Group                                |
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
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "mysqlnd_azure.h"
#include "php_mysqlnd_azure.h"
#include "ext/mysqlnd/mysqlnd_ext_plugin.h"

ZEND_DECLARE_MODULE_GLOBALS(mysqlnd_azure)


/* {{{ OnUpdateEnableRedirect */
static ZEND_INI_MH(OnUpdateEnableRedirect)
{
    if ((ZSTR_LEN(new_value) == 9 && strcasecmp("preferred", ZSTR_VAL(new_value)) == 0)
        || (ZSTR_LEN(new_value) == 1 && strcasecmp("2", ZSTR_VAL(new_value)) == 0)) {

        MYSQLND_AZURE_G(enableRedirect) = REDIRECT_PREFERRED;

    } else if ((ZSTR_LEN(new_value) == 2 && strcasecmp("on", ZSTR_VAL(new_value)) == 0)
        || (ZSTR_LEN(new_value) == 3 && strcasecmp("yes", ZSTR_VAL(new_value)) == 0)
        || (ZSTR_LEN(new_value) == 4 && strcasecmp("true", ZSTR_VAL(new_value)) == 0)
        || (ZSTR_LEN(new_value) == 1 && strcasecmp("1", ZSTR_VAL(new_value)) == 0)) {

        MYSQLND_AZURE_G(enableRedirect) = REDIRECT_ON;

    } else {

        MYSQLND_AZURE_G(enableRedirect) = REDIRECT_OFF;

    }

	return SUCCESS;

}
/* }}} */


/* {{{ PHP_INI */
PHP_INI_BEGIN()
STD_PHP_INI_ENTRY("mysqlnd_azure.enableRedirect", "preferred", PHP_INI_ALL, OnUpdateEnableRedirect, enableRedirect, zend_mysqlnd_azure_globals, mysqlnd_azure_globals)
PHP_INI_END()
/* }}} */


/* {{{ PHP_GINIT_FUNCTION */
static PHP_GINIT_FUNCTION(mysqlnd_azure)
{
#if defined(COMPILE_DL_MYSQLND_AZURE) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	mysqlnd_azure_globals->enableRedirect = REDIRECT_PREFERRED;
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

  #ifdef ZTS
  mysqlnd_azure_redirect_cache_lock_alloc();
  #endif
  redirectCache = NULL;

  return SUCCESS;
}

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
static PHP_MSHUTDOWN_FUNCTION(mysqlnd_azure)
{
	UNREGISTER_INI_ENTRIES();

    if (redirectCache) {
		zend_hash_destroy(redirectCache);
		mnd_pefree(redirectCache, 1);
		redirectCache = NULL;
	}

    #ifdef ZTS
    mysqlnd_azure_redirect_cache_lock_free();
    #endif

	return SUCCESS;
}

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(mysqlnd_azure)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "mysqlnd_azure", "enableRedirect");
	php_info_print_table_row(2, "enableRedirect", MYSQLND_AZURE_G(enableRedirect) == REDIRECT_OFF ? "off" : (MYSQLND_AZURE_G(enableRedirect) == REDIRECT_ON ? "on" : "preferred"));
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
	NULL,
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
