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

#ifndef PHP_MYSQLND_AZURE_H
# define PHP_MYSQLND_AZURE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

extern zend_module_entry mysqlnd_azure_module_entry;
# define phpext_mysqlnd_azure_ptr &mysqlnd_azure_module_entry

#define EXT_MYSQLND_AZURE_NAME      "mysqlnd_azure"
#define EXT_MYSQLND_AZURE_VERSION   "1.0.3"

ZEND_BEGIN_MODULE_GLOBALS(mysqlnd_azure)
	zend_bool		enableRedirect;
	HashTable*		redirectCache;
ZEND_END_MODULE_GLOBALS(mysqlnd_azure)

PHPAPI ZEND_EXTERN_MODULE_GLOBALS(mysqlnd_azure)
#define MYSQLND_AZURE_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(mysqlnd_azure, v)

#endif	/* PHP_MYSQLND_AZURE_H */

