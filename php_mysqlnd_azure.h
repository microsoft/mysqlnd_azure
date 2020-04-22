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

#ifndef PHP_MYSQLND_AZURE_H
#define PHP_MYSQLND_AZURE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

extern zend_module_entry mysqlnd_azure_module_entry;
#define phpext_mysqlnd_azure_ptr &mysqlnd_azure_module_entry

#define PHP_MYSQLND_AZURE_NAME      "mysqlnd_azure"
#define PHP_MYSQLND_AZURE_VERSION   "1.1.0"

#define STRING_EQUALS(z_str,str) (ZSTR_LEN((z_str)) == strlen((str)) && strcasecmp((str), ZSTR_VAL((z_str))) == 0)

typedef enum _mysqlnd_azure_redirect_mode {
    REDIRECT_OFF = 0,       /* completely disabled */
    REDIRECT_ON = 1,        /* enabled without fallback, block if redirection fail */
    REDIRECT_PREFERRED = 2  /* enabled with fallback */
} mysqlnd_azure_redirect_mode;

ZEND_BEGIN_MODULE_GLOBALS(mysqlnd_azure)
    mysqlnd_azure_redirect_mode     enableRedirect;
    HashTable*                      redirectCache;
    zend_string*                    logfilePath;
    int                             logLevel;
    int                             logOutput;
ZEND_END_MODULE_GLOBALS(mysqlnd_azure)

PHPAPI ZEND_EXTERN_MODULE_GLOBALS(mysqlnd_azure)
#define MYSQLND_AZURE_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(mysqlnd_azure, v)

#endif  /* PHP_MYSQLND_AZURE_H */

