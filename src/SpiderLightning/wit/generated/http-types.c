#include <stdlib.h>
#include <http-types.h>

__attribute__((weak, export_name("canonical_abi_realloc")))
void *canonical_abi_realloc(
void *ptr,
size_t orig_size,
size_t org_align,
size_t new_size
) {
  void *ret = realloc(ptr, new_size);
  if (!ret)
  abort();
  return ret;
}

__attribute__((weak, export_name("canonical_abi_free")))
void canonical_abi_free(
void *ptr,
size_t size,
size_t align
) {
  free(ptr);
}
#include <string.h>

void http_types_string_set(http_types_string_t *ret, const char *s) {
  ret->ptr = (char*) s;
  ret->len = strlen(s);
}

void http_types_string_dup(http_types_string_t *ret, const char *s) {
  ret->len = strlen(s);
  ret->ptr = canonical_abi_realloc(NULL, 0, 1, ret->len);
  memcpy(ret->ptr, s, ret->len);
}

void http_types_string_free(http_types_string_t *ret) {
  canonical_abi_free(ret->ptr, ret->len, 1);
  ret->ptr = NULL;
  ret->len = 0;
}
void http_types_body_free(http_types_body_t *ptr) {
  canonical_abi_free(ptr->ptr, ptr->len * 1, 1);
}
void http_types_tuple2_string_string_free(http_types_tuple2_string_string_t *ptr) {
  http_types_string_free(&ptr->f0);
  http_types_string_free(&ptr->f1);
}
void http_types_headers_free(http_types_headers_t *ptr) {
  for (size_t i = 0; i < ptr->len; i++) {
    http_types_tuple2_string_string_free(&ptr->ptr[i]);
  }
  canonical_abi_free(ptr->ptr, ptr->len * 16, 4);
}
void http_types_params_free(http_types_params_t *ptr) {
  for (size_t i = 0; i < ptr->len; i++) {
    http_types_tuple2_string_string_free(&ptr->ptr[i]);
  }
  canonical_abi_free(ptr->ptr, ptr->len * 16, 4);
}
void http_types_uri_free(http_types_uri_t *ptr) {
  http_types_string_free(ptr);
}
void http_types_option_body_free(http_types_option_body_t *ptr) {
  if (ptr->is_some) {
    http_types_body_free(&ptr->val);
  }
}
void http_types_request_free(http_types_request_t *ptr) {
  http_types_uri_free(&ptr->uri);
  http_types_headers_free(&ptr->headers);
  http_types_params_free(&ptr->params);
  http_types_option_body_free(&ptr->body);
}
void http_types_option_headers_free(http_types_option_headers_t *ptr) {
  if (ptr->is_some) {
    http_types_headers_free(&ptr->val);
  }
}
void http_types_response_free(http_types_response_t *ptr) {
  http_types_option_headers_free(&ptr->headers);
  http_types_option_body_free(&ptr->body);
}
void http_types_http_error_free(http_types_http_error_t *ptr) {
  switch ((int32_t) ptr->tag) {
    case 0: {
      http_types_string_free(&ptr->val.invalid_url);
      break;
    }
    case 1: {
      http_types_string_free(&ptr->val.timeout_error);
      break;
    }
    case 2: {
      http_types_string_free(&ptr->val.protocol_error);
      break;
    }
    case 4: {
      http_types_string_free(&ptr->val.unexpected_error);
      break;
    }
  }
}
void http_types_http_router_error_free(http_types_http_router_error_t *ptr) {
  switch ((int32_t) ptr->tag) {
    case 0: {
      http_types_string_free(&ptr->val.invalid_url);
      break;
    }
    case 1: {
      http_types_string_free(&ptr->val.timeout_error);
      break;
    }
    case 2: {
      http_types_string_free(&ptr->val.protocol_error);
      break;
    }
    case 4: {
      http_types_string_free(&ptr->val.unexpected_error);
      break;
    }
  }
}
