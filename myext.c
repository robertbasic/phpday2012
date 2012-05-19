#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_myext.h"

/* arguments for our fibonacci function */
ZEND_BEGIN_ARG_INFO(arginfo_fibonacci, 0)
    ZEND_ARG_INFO(0, integer)
ZEND_END_ARG_INFO(); 

ZEND_BEGIN_ARG_INFO(arginfo_my_dump, 0)
    ZEND_ARG_INFO(0, zval)
ZEND_END_ARG_INFO(); 


zend_function_entry myext_functions[] = 
{
    ZEND_FE(fibonacci, arginfo_fibonacci)
    ZEND_FE(my_dump, arginfo_my_dump)
    {NULL,NULL,NULL} /* Here ends the function entries */
};

/* {{{ myext_module_entry
 */
zend_module_entry myext_module_entry = {
    STANDARD_MODULE_HEADER,
    "myext",
    myext_functions, /* Function entries */
    NULL, /* Module init */
    NULL, /* Module shutdown */
    NULL, /* Request init */
    NULL, /* Request shutdown */
    NULL, /* Module information */
    "0.1", /* Replace with version number for your extension */
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MYEXT
ZEND_GET_MODULE(myext)
#endif

/**
 * this is the C function which will be called by the PHP userland
 * function (fibonacci), this will be faster than to call the userland always
 */
fibonacci_internal(long n)
{
    switch(n)
    {
        case 0:
        case 1:
            return n;
    }

    return fibonacci_internal(n-2) + fibonacci_internal(n-1);
}

/* this is the actual PHP function which will be called
 * from userland
 */
PHP_FUNCTION(fibonacci)
{
    long n;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &n) == FAILURE)
    {
        RETURN_FALSE;
    }

    RETURN_LONG(fibonacci_internal(n));
}


PHP_FUNCTION(my_dump)
{
    zval *myvar;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &myvar) == FAILURE)
    {
        RETURN_FALSE;
    }

    switch (Z_TYPE_P(myvar)) {
        case IS_BOOL:
            php_printf("It's a BOOL!");
            break;
        default:
            php_printf("It's not a BOOL!");
            break;
    }
}
