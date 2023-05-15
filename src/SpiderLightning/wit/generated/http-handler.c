#include <stdlib.h>
#include <http-handler.h>

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

void http_handler_string_set(http_handler_string_t *ret, const char *s) {
  ret->ptr = (char*) s;
  ret->len = strlen(s);
}

void http_handler_string_dup(http_handler_string_t *ret, const char *s) {
  ret->len = strlen(s);
  ret->ptr = canonical_abi_realloc(NULL, 0, 1, ret->len);
  memcpy(ret->ptr, s, ret->len);
}

void http_handler_string_free(http_handler_string_t *ret) {
  canonical_abi_free(ret->ptr, ret->len, 1);
  ret->ptr = NULL;
  ret->len = 0;
}
void http_handler_uri_free(http_handler_uri_t *ptr) {
  http_handler_string_free(ptr);
}
void http_handler_tuple2_string_string_free(http_handler_tuple2_string_string_t *ptr) {
  http_handler_string_free(&ptr->f0);
  http_handler_string_free(&ptr->f1);
}
void http_handler_headers_free(http_handler_headers_t *ptr) {
  for (size_t i = 0; i < ptr->len; i++) {
    http_handler_tuple2_string_string_free(&ptr->ptr[i]);
  }
  canonical_abi_free(ptr->ptr, ptr->len * 16, 4);
}
void http_handler_params_free(http_handler_params_t *ptr) {
  for (size_t i = 0; i < ptr->len; i++) {
    http_handler_tuple2_string_string_free(&ptr->ptr[i]);
  }
  canonical_abi_free(ptr->ptr, ptr->len * 16, 4);
}
void http_handler_body_free(http_handler_body_t *ptr) {
  canonical_abi_free(ptr->ptr, ptr->len * 1, 1);
}
void http_handler_option_body_free(http_handler_option_body_t *ptr) {
  if (ptr->is_some) {
    http_handler_body_free(&ptr->val);
  }
}
void http_handler_request_free(http_handler_request_t *ptr) {
  http_handler_uri_free(&ptr->uri);
  http_handler_headers_free(&ptr->headers);
  http_handler_params_free(&ptr->params);
  http_handler_option_body_free(&ptr->body);
}
void http_handler_option_headers_free(http_handler_option_headers_t *ptr) {
  if (ptr->is_some) {
    http_handler_headers_free(&ptr->val);
  }
}
void http_handler_response_free(http_handler_response_t *ptr) {
  http_handler_option_headers_free(&ptr->headers);
  http_handler_option_body_free(&ptr->body);
}
void http_handler_http_error_free(http_handler_http_error_t *ptr) {
  switch ((int32_t) ptr->tag) {
    case 0: {
      http_handler_string_free(&ptr->val.invalid_url);
      break;
    }
    case 1: {
      http_handler_string_free(&ptr->val.timeout_error);
      break;
    }
    case 2: {
      http_handler_string_free(&ptr->val.protocol_error);
      break;
    }
    case 4: {
      http_handler_string_free(&ptr->val.unexpected_error);
      break;
    }
  }
}
void http_handler_expected_response_http_error_free(http_handler_expected_response_http_error_t *ptr) {
  if (!ptr->is_err) {
    http_handler_response_free(&ptr->val.ok);
  } else {
    http_handler_http_error_free(&ptr->val.err);
  }
}

__attribute__((aligned(4)))
static uint8_t RET_AREA[32];
__attribute__((import_module("http-handler"), import_name("handle-http")))
void __wasm_import_http_handler_handle_http(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);
void http_handler_handle_http(http_handler_request_t *req, http_handler_expected_response_http_error_t *ret0) {
  int32_t option;
  int32_t option1;
  int32_t option2;
  
  if (((*req).body).is_some) {
    const http_handler_body_t *payload0 = &((*req).body).val;
    option = 1;
    option1 = (int32_t) (*payload0).ptr;
    option2 = (int32_t) (*payload0).len;
    
  } else {
    option = 0;
    option1 = 0;
    option2 = 0;
    
  }
  int32_t ptr = (int32_t) &RET_AREA;
  __wasm_import_http_handler_handle_http((int32_t) (*req).method, (int32_t) ((*req).uri).ptr, (int32_t) ((*req).uri).len, (int32_t) ((*req).headers).ptr, (int32_t) ((*req).headers).len, (int32_t) ((*req).params).ptr, (int32_t) ((*req).params).len, option, option1, option2, ptr);
  http_handler_expected_response_http_error_t expected;
  switch ((int32_t) (*((uint8_t*) (ptr + 0)))) {
    case 0: {
      expected.is_err = false;
      http_handler_option_headers_t option3;
      switch ((int32_t) (*((uint8_t*) (ptr + 8)))) {
        case 0: {
          option3.is_some = false;
          
          break;
        }
        case 1: {
          option3.is_some = true;
          
          option3.val = (http_handler_headers_t) { (http_handler_tuple2_string_string_t*)(*((int32_t*) (ptr + 12))), (size_t)(*((int32_t*) (ptr + 16))) };
          break;
        }
      }http_handler_option_body_t option4;
      switch ((int32_t) (*((uint8_t*) (ptr + 20)))) {
        case 0: {
          option4.is_some = false;
          
          break;
        }
        case 1: {
          option4.is_some = true;
          
          option4.val = (http_handler_body_t) { (uint8_t*)(*((int32_t*) (ptr + 24))), (size_t)(*((int32_t*) (ptr + 28))) };
          break;
        }
      }
      expected.val.ok = (http_handler_response_t) {
        (uint16_t) ((int32_t) (*((uint16_t*) (ptr + 4)))),
        option3,
        option4,
      };
      break;
    }
    case 1: {
      expected.is_err = true;
      http_handler_http_error_t variant;
      variant.tag = (int32_t) (*((uint8_t*) (ptr + 4)));
      switch ((int32_t) variant.tag) {
        case 0: {
          variant.val.invalid_url = (http_handler_string_t) { (char*)(*((int32_t*) (ptr + 8))), (size_t)(*((int32_t*) (ptr + 12))) };
          break;
        }
        case 1: {
          variant.val.timeout_error = (http_handler_string_t) { (char*)(*((int32_t*) (ptr + 8))), (size_t)(*((int32_t*) (ptr + 12))) };
          break;
        }
        case 2: {
          variant.val.protocol_error = (http_handler_string_t) { (char*)(*((int32_t*) (ptr + 8))), (size_t)(*((int32_t*) (ptr + 12))) };
          break;
        }
        case 3: {
          variant.val.status_error = (uint16_t) ((int32_t) (*((uint16_t*) (ptr + 8))));
          break;
        }
        case 4: {
          variant.val.unexpected_error = (http_handler_string_t) { (char*)(*((int32_t*) (ptr + 8))), (size_t)(*((int32_t*) (ptr + 12))) };
          break;
        }
      }
      
      expected.val.err = variant;
      break;
    }
  }*ret0 = expected;
}
