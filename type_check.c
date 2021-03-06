#include <zend.h>
#include <php.h>
#include "types.h"
#include "classes.h"
#include "string_buffer.h"
#include "type_check.h"

unsigned char integer_type_check(long l) {
  if (( (long) ((unsigned char) l) ) == l) return LEON_UNSIGNED_CHAR;
  if (( (long) ((char) l) ) == l) return LEON_CHAR;
  if (( (long) ((unsigned short) l) ) == l) return LEON_UNSIGNED_SHORT;
  if (( (long) ((short) l) ) == l) return LEON_SHORT;
  if (( (long) ((unsigned int) l) ) == l) return LEON_UNSIGNED_INT;
  if (( (long) ((int) l) ) == l) return LEON_INT;
  return LEON_DOUBLE;
}

unsigned char fp_type_check(double d) {
  if (( (double) ((float) d) ) == d) return LEON_FLOAT;
  return LEON_DOUBLE;
}

unsigned char type_check(zval *val) {
  long l;
  double d;
  HashTable *ht;
  switch (Z_TYPE_P(val)) {
    case IS_LONG:
      l = Z_LVAL_P(val);
      return integer_type_check(l);
      break;
    case IS_DOUBLE:
      d = Z_DVAL_P(val);
      return fp_type_check(d);
      break;
    case IS_NULL:
      return LEON_NULL;
      break;
#if PHP_API_VERSION <= 20131106
    case IS_BOOL:
      return (Z_LVAL_P(val) ? LEON_TRUE : LEON_FALSE);
      break;
#else
    case IS_FALSE:
      return LEON_FALSE;
      break;
    case IS_TRUE:
      return LEON_TRUE;
      break;
#endif
    case IS_STRING:
      return LEON_STRING;
      break;
#if PHP_API_VERSION > 20131106
    case IS_REFERENCE:
      val = Z_REFVAL_P(val);
      return type_check(val);
      break;
#endif
    case IS_OBJECT:
      if (instanceof_function(Z_OBJCE_P(val), undefined_ce)) return LEON_UNDEFINED;
      else if (instanceof_function(Z_OBJCE_P(val), date_ce)) return LEON_DATE;
      else if (instanceof_function(Z_OBJCE_P(val), nan_ce)) return LEON_NAN;
      else if (instanceof_function(Z_OBJCE_P(val), infinity_ce)) return LEON_INFINITY;
      else if (instanceof_function(Z_OBJCE_P(val), minus_infinity_ce)) return LEON_MINUS_INFINITY;
      else if (instanceof_function(Z_OBJCE_P(val), string_buffer_ce)) return LEON_BUFFER;
      else if (instanceof_function(Z_OBJCE_P(val), regexp_ce)) return LEON_REGEXP;
      return LEON_NATIVE_OBJECT;
      break;
    case IS_ARRAY:
      ht = Z_ARRVAL_P(val);
      return determine_array_type(ht);
      break;
#if PHP_API_VERSION > 20131106
    case IS_INDIRECT:
      return LEON_INDIRECT;
#endif
    case IS_CONSTANT:
    case IS_CONSTANT_AST:
      return LEON_CONSTANT;
    default:
      php_error_docref(NULL, E_WARNING, "Unsupported type.");
      break;
  }
}

#if PHP_API_VERSION > 20131106
unsigned char determine_array_type(HashTable *ht) {
  int i = ht ? zend_hash_num_elements(ht) : 0;
  if (i > 0) {
    zend_string *key;
    zend_ulong index, idx;
    idx = 0;
    ZEND_HASH_FOREACH_KEY(ht, index, key) {
      if (key) return LEON_OBJECT;
      else {
        if (index != idx) return LEON_OBJECT;
      }
      ++idx;
    } ZEND_HASH_FOREACH_END();
  }
  return LEON_ARRAY;
}
#else
unsigned char determine_array_type(HashTable *ht) {
  int i = ht ? zend_hash_num_elements(ht) : 0;
  if (i > 0) {
    char *key;
    ulong index, idx;
    uint key_len;
    HashPosition pos;
    zend_hash_internal_pointer_reset_ex(ht, &pos);
    idx = 0;
    for (;; zend_hash_move_forward_ex(ht, &pos)) {
      i = zend_hash_get_current_key_ex(ht, &key, &key_len, &index, 0, &pos);
      if (i == HASH_KEY_NON_EXISTENT) {
        break;
      }
      if (i == HASH_KEY_IS_STRING) {
        return LEON_OBJECT;
      } else {
        if (index != idx) {
          return LEON_OBJECT;
        }
      }
      idx++;
    }
  }
  return LEON_ARRAY;
}
#endif
