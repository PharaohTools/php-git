#include "php_git2.h"
#include "php_git2_priv.h"
#include "patch.h"

/* {{{ proto resource git_patch_from_diff(resource $diff, long $idx)
 */
PHP_FUNCTION(git_patch_from_diff)
{
	php_git2_t *result = NULL, *_diff = NULL;
	git_patch *out = NULL;
	zval *diff = NULL;
	zend_long idx;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &diff, &idx) == FAILURE) {
		return;
	}

	if ((_diff = (php_git2_t *) zend_fetch_resource(Z_RES_P(diff), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_patch_from_diff(&out, PHP_GIT2_V(_diff, diff), idx);

	if (php_git2_check_error(error, "git_patch_from_diff" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_PATCH, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_patch_from_blobs(resource $old_blob, string $old_as_path, resource $new_blob, string $new_as_path,  $opts)
 */
PHP_FUNCTION(git_patch_from_blobs)
{
	php_git2_t *result = NULL, *_old_blob = NULL, *_new_blob = NULL;
	git_patch *out = NULL;
	git_diff_options options = GIT_DIFF_OPTIONS_INIT;
	zval *old_blob = NULL, *new_blob = NULL, *opts = NULL;
	char *old_as_path = NULL, *new_as_path = NULL;
	int old_as_path_len = 0, new_as_path_len = 0, error = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsrsa", &old_blob, &old_as_path, &old_as_path_len, &new_blob, &new_as_path, &new_as_path_len, &opts) == FAILURE) {
		return;
	}

	if ((_old_blob = (php_git2_t *) zend_fetch_resource(Z_RES_P(old_blob), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_new_blob = (php_git2_t *) zend_fetch_resource(Z_RES_P(new_blob), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	php_git2_array_to_git_diff_options(&options, opts TSRMLS_CC);
	error = git_patch_from_blobs(&out, PHP_GIT2_V(_old_blob, blob), old_as_path, PHP_GIT2_V(_new_blob, blob), new_as_path, &options);
	php_git2_git_diff_options_free(&options);
	if (php_git2_check_error(error, "git_patch_from_blobs" TSRMLS_CC)) {
		RETURN_FALSE;
	}
	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_PATCH, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */


/* {{{ proto resource git_patch_from_blob_and_buffer(resource $old_blob, string $old_as_path, string $buffer, string $buffer_as_path,  $opts)
 */
PHP_FUNCTION(git_patch_from_blob_and_buffer)
{
	php_git2_t *result = NULL, *_old_blob = NULL;
	git_patch *out = NULL;
	zval *old_blob = NULL, *opts = NULL;
	char *old_as_path = NULL, *buffer = NULL, *buffer_as_path = NULL;
	int old_as_path_len = 0, buffer_len = 0, buffer_as_path_len = 0, error = 0;
	git_diff_options options = GIT_DIFF_OPTIONS_INIT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsslsa", &old_blob, &old_as_path, &old_as_path_len, &buffer, &buffer_len, &buffer_len, &buffer_as_path, &buffer_as_path_len, &opts) == FAILURE) {
		return;
	}

	if ((_old_blob = (php_git2_t *) zend_fetch_resource(Z_RES_P(old_blob), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	php_git2_array_to_git_diff_options(&options, opts TSRMLS_CC);
	error = git_patch_from_blob_and_buffer(&out, PHP_GIT2_V(_old_blob, blob), old_as_path, buffer, buffer_len, buffer_as_path, &options);
	php_git2_git_diff_options_free(&options);
	if (php_git2_check_error(error, "git_patch_from_blob_and_buffer" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_PATCH, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto void git_patch_free(resource $patch)
 */
PHP_FUNCTION(git_patch_free)
{
	zval *patch = NULL;
	php_git2_t *_patch = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &patch) == FAILURE) {
		return;
	}

	if ((_patch = (php_git2_t *) zend_fetch_resource(Z_RES_P(patch), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	// todo rework
	if (0 /* GIT2_SHOULD_FREE(_patch) */) {
		git_patch_free(PHP_GIT2_V(_patch, patch));
		GIT2_SHOULD_FREE(_patch) = 0;
	}

	//zval_ptr_dtor(patch);
}
/* }}} */

/* {{{ proto array git_patch_get_delta(resource $patch)
 */
PHP_FUNCTION(git_patch_get_delta)
{
	const git_diff_delta *result = NULL;
	zval *patch = NULL;
	php_git2_t *_patch = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &patch) == FAILURE) {
		return;
	}

	if ((_patch = (php_git2_t *) zend_fetch_resource(Z_RES_P(patch), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_patch_get_delta(PHP_GIT2_V(_patch, patch));
	if (result == NULL) {
		RETURN_FALSE;
	}

	array_init(return_value);
	php_git2_git_diff_delta_to_array(result, return_value TSRMLS_CC);

	RETVAL_ARR(Z_ARRVAL_P(return_value));
}
/* }}} */

/* {{{ proto long git_patch_num_hunks(resource $patch)
 */
PHP_FUNCTION(git_patch_num_hunks)
{
	size_t result;
	zval *patch = NULL;
	php_git2_t *_patch = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &patch) == FAILURE) {
		return;
	}

	if ((_patch = (php_git2_t *) zend_fetch_resource(Z_RES_P(patch), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_patch_num_hunks(PHP_GIT2_V(_patch, patch));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_patch_line_stats(resource $patch)
 */
PHP_FUNCTION(git_patch_line_stats)
{
	int result = 0;
	size_t total_context = 0, total_additions = 0, total_deletions = 0;
	zval *patch = NULL, *out = NULL;
	php_git2_t *_patch = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &patch) == FAILURE) {
		return;
	}

	if ((_patch = (php_git2_t *) zend_fetch_resource(Z_RES_P(patch), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_patch_line_stats(&total_context, &total_additions, &total_deletions, PHP_GIT2_V(_patch, patch));

	array_init(out);
	add_assoc_long_ex(out, ZEND_STRL("total_context"), total_context);
	add_assoc_long_ex(out, ZEND_STRL("total_additions"), total_additions);
	add_assoc_long_ex(out, ZEND_STRL("total_deletions"), total_deletions);
	RETURN_ZVAL(out, 0, 1);
}
/* }}} */

/* {{{ proto resource git_patch_get_hunk(resource $patch, long $hunk_idx)
 */
PHP_FUNCTION(git_patch_get_hunk)
{
	php_git2_t *result = NULL, *_patch = NULL;
	const git_diff_hunk *out = NULL;
	zend_long hunk_idx;
	size_t lines_in_hunk;
	zval *patch = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &patch, &hunk_idx) == FAILURE) {
		return;
	}

	if ((_patch = (php_git2_t *) zend_fetch_resource(Z_RES_P(patch), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_patch_get_hunk(&out, &lines_in_hunk, PHP_GIT2_V(_patch, patch), hunk_idx);

	if (php_git2_check_error(error, "git_patch_get_hunk" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_DIFF_HUNK, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_patch_num_lines_in_hunk(resource $patch, long $hunk_idx)
 */
PHP_FUNCTION(git_patch_num_lines_in_hunk)
{
	int result;
	zval *patch = NULL;
	php_git2_t *_patch = NULL;
	zend_long hunk_idx;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &patch, &hunk_idx) == FAILURE) {
		return;
	}

	if ((_patch = (php_git2_t *) zend_fetch_resource(Z_RES_P(patch), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_patch_num_lines_in_hunk(PHP_GIT2_V(_patch, patch), hunk_idx);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_patch_get_line_in_hunk(resource $patch, long $hunk_idx, long $line_of_hunk)
 */
PHP_FUNCTION(git_patch_get_line_in_hunk)
{
	php_git2_t *result = NULL, *_patch = NULL;
	const git_diff_line *out = NULL;
	zval *patch = NULL;
	zend_long hunk_idx, line_of_hunk;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rll", &patch, &hunk_idx, &line_of_hunk) == FAILURE) {
		return;
	}

	if ((_patch = (php_git2_t *) zend_fetch_resource(Z_RES_P(patch), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_patch_get_line_in_hunk(&out, PHP_GIT2_V(_patch, patch), hunk_idx, line_of_hunk);

	if (php_git2_check_error(error, "git_patch_get_line_in_hunk" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_DIFF_LINE, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_patch_size(resource $patch, long $include_context, long $include_hunk_headers, long $include_file_headers)
 */
PHP_FUNCTION(git_patch_size)
{
	size_t result;
	zval *patch = NULL;
	php_git2_t *_patch = NULL;
	zend_long include_context, include_hunk_headers, include_file_headers;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rlll", &patch, &include_context, &include_hunk_headers, &include_file_headers) == FAILURE) {
		return;
	}

	if ((_patch = (php_git2_t *) zend_fetch_resource(Z_RES_P(patch), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_patch_size(PHP_GIT2_V(_patch, patch), include_context, include_hunk_headers, include_file_headers);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_patch_print(resource $patch, Callable $print_cb,  $payload)
 */
PHP_FUNCTION(git_patch_print)
{
	int result = 0;
	zval *patch = NULL, *payload = NULL;
	php_git2_t *_patch = NULL;
	zend_fcall_info fci = empty_fcall_info;
	zend_fcall_info_cache fcc = empty_fcall_info_cache;
	php_git2_multi_cb_t *cb = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rfz", &patch, &fci, &fcc, &payload) == FAILURE) {
		return;
	}

	if ((_patch = (php_git2_t *) zend_fetch_resource(Z_RES_P(patch), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	php_git2_multi_cb_init(&cb, payload TSRMLS_CC, 6,
		&empty_fcall_info, &empty_fcall_info_cache,
		&empty_fcall_info, &empty_fcall_info_cache,
		&fci, &fcc
	);

	result = git_patch_print(PHP_GIT2_V(_patch, patch), php_git2_git_diff_line_cb, cb);
	php_git2_multi_cb_free(cb);
	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto string git_patch_to_str(resource $patch)
 */
PHP_FUNCTION(git_patch_to_str)
{
	char *string = NULL;
	zval *patch = NULL;
	php_git2_t *_patch = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &patch) == FAILURE) {
		return;
	}

	if ((_patch = (php_git2_t *) zend_fetch_resource(Z_RES_P(patch), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_patch_to_str(&string, PHP_GIT2_V(_patch, patch));

	if (php_git2_check_error(error, "git_patch_to_str" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_STRING(string);
}
/* }}} */

