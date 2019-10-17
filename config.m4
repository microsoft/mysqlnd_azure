dnl
dnl $Id$
dnl config.m4 for extension mysqlnd_azure

PHP_ARG_ENABLE(mysqlnd_azure, whether to enable mysqlnd_azure support for redirection,
[  --enable-mysqlnd_azure           Enable mysqlnd_azure support for redirection])

if test "$PHP_MYSQLND_AZURE" != "no"; then
  PHP_SUBST(mysqlnd_azure_SHARED_LIBADD)

  mysqlnd_azure_sources="php_mysqlnd_azure.c mysqlnd_azure.c redirect_cache.c"

  PHP_ADD_EXTENSION_DEP(mysqlnd_azure, mysqlnd)

  PHP_NEW_EXTENSION(mysqlnd_azure, $mysqlnd_azure_sources, $ext_shared)
fi
