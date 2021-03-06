#include "php_git2.h"
#include "php_git2_priv.h"
#include "checkout.h"

/* {{{ proto long git_checkout_head(resource $repo, array $opts)
 */
PHP_FUNCTION(git_checkout_head)
{
	int result, should_free = 0;
	zval *repo = NULL, *opts = NULL;
	php_git2_t *_repo = NULL;
	git_checkout_opts _opts = GIT_CHECKOUT_OPTS_INIT;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r|a", &repo, &opts) == FAILURE) {
		return;
	}

	if (opts != NULL) {
		php_git2_array_to_git_checkout_opts(&_opts, opts TSRMLS_CC);
		should_free = 1;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_checkout_head(PHP_GIT2_V(_repo, repository), &_opts);

	if (should_free) {
		php_git2_git_checkout_opts_free(&_opts TSRMLS_CC);
	}

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_checkout_index(resource $repo, resource $index, array $opts)
 */
PHP_FUNCTION(git_checkout_index)
{
	int result, should_free = 0;
	zval *repo = NULL, *index = NULL, *opts = NULL;
	php_git2_t *_repo = NULL, *_index = NULL;
	git_checkout_opts _opts = GIT_CHECKOUT_OPTS_INIT;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rr|a", &repo, &index, &opts) == FAILURE) {
		return;
	}

	if (opts != NULL) {
		php_git2_array_to_git_checkout_opts(&_opts, opts TSRMLS_CC);
		should_free = 1;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_checkout_index(PHP_GIT2_V(_repo, repository), PHP_GIT2_V(_index, index), &_opts);

	if (should_free) {
		php_git2_git_checkout_opts_free(&_opts TSRMLS_CC);
	}

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_checkout_tree(resource $repo, resource $treeish, array $opts)
 */
PHP_FUNCTION(git_checkout_tree)
{
	int result, should_free = 0;
	zval *repo = NULL, *treeish = NULL, *opts = NULL;
	php_git2_t *_repo = NULL, *_treeish = NULL;
	git_checkout_opts _opts = GIT_CHECKOUT_OPTS_INIT;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rr|a", &repo, &treeish, &opts) == FAILURE) {
		return;
	}

	if (opts != NULL) {
		php_git2_array_to_git_checkout_opts(&_opts, opts TSRMLS_CC);
		should_free = 1;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_treeish = (php_git2_t *) zend_fetch_resource(Z_RES_P(treeish), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_checkout_tree(PHP_GIT2_V(_repo, repository), PHP_GIT2_V(_treeish, object), &_opts);

	if (should_free) {
		php_git2_git_checkout_opts_free(&_opts TSRMLS_CC);
	}

	RETURN_LONG(result);
}
/* }}} */

