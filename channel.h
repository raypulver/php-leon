#ifndef LEON_CHANNEL_H
#define LEON_CHANNEL_H

#include "php.h"
#if PHP_API_VERSION <= 20131106
#include "ext/standard/php_smart_str.h"
#else
#include "zend_smart_str.h"
#endif
#include "php_leon.h"

PHP_METHOD(Channel, __construct);
PHP_METHOD(Channel, encode);
PHP_METHOD(Channel, decode);

PHP_LEON_API void php_leon_channel_decode(zval *ret, zval *this, char *s, size_t len);
PHP_LEON_API void php_leon_channel_encode(smart_str *, zval *this, zval *payload);

#endif
