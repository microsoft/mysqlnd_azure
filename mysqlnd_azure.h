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

#ifndef MYSQLND_AZURE_H
#define MYSQLND_AZURE_H

#ifdef ZTS
#include "TSRM.h"
#endif

#include "ext/mysqlnd/mysqlnd.h"
#include "ext/mysqlnd/mysqlnd_debug.h"

/*struct to store redirection chache info in hash table*/
typedef struct st_mysqlnd_azure_redirect_info {
	char* redirect_user;
	char* redirect_host;
	unsigned int redirect_port;
} MYSQLND_AZURE_REDIRECT_INFO;

#define MAX_REDIRECT_HOST_LEN 128
#define MAX_REDIRECT_USER_LEN 128

void mysqlnd_azure_minit_register_hooks();

enum_func_status mysqlnd_azure_add_redirect_cache(const char* user, const char* host, int port, const char* redirect_user, const char* redirect_host, int redirect_port);
enum_func_status mysqlnd_azure_remove_redirect_cache(const char* user, const char* host, int port);
MYSQLND_AZURE_REDIRECT_INFO* mysqlnd_azure_find_redirect_cache(const char* user, const char* host, int port);

#if defined(ZTS) && defined(COMPILE_DL_MYSQLND_AZURE)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif	/* MYSQLND_AZURE_H */

