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
__attribute__((export_name("handle-http")))
int32_t __wasm_export_http_handler_handle_http(int32_t arg, int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6, int32_t arg7, int32_t arg8) {
  http_handler_option_body_t option;
  switch (arg6) {
    case 0: {
      option.is_some = false;
      
      break;
    }
    case 1: {
      option.is_some = true;
      
      option.val = (http_handler_body_t) { (uint8_t*)(arg7), (size_t)(arg8) };
      break;
    }
  }http_handler_request_t arg9 = (http_handler_request_t) {
    arg,
    (http_handler_string_t) { (char*)(arg0), (size_t)(arg1) },
    (http_handler_headers_t) { (http_handler_tuple2_string_string_t*)(arg2), (size_t)(arg3) },
    (http_handler_params_t) { (http_handler_tuple2_string_string_t*)(arg4), (size_t)(arg5) },
    option,
  };
  http_handler_expected_response_http_error_t ret;
  http_handler_handle_http(&arg9, &ret);
  int32_t ptr = (int32_t) &RET_AREA;
  
  if ((ret).is_err) {
    const http_handler_http_error_t *payload14 = &(ret).val.err;
    *((int8_t*)(ptr + 0)) = 1;
    switch ((int32_t) (*payload14).tag) {
      case 0: {
        const http_handler_string_t *payload15 = &(*payload14).val.invalid_url;
        *((int8_t*)(ptr + 4)) = 0;
        *((int32_t*)(ptr + 12)) = (int32_t) (*payload15).len;
        *((int32_t*)(ptr + 8)) = (int32_t) (*payload15).ptr;
        break;
      }
      case 1: {
        const http_handler_string_t *payload16 = &(*payload14).val.timeout_error;
        *((int8_t*)(ptr + 4)) = 1;
        *((int32_t*)(ptr + 12)) = (int32_t) (*payload16).len;
        *((int32_t*)(ptr + 8)) = (int32_t) (*payload16).ptr;
        break;
      }
      case 2: {
        const http_handler_string_t *payload17 = &(*payload14).val.protocol_error;
        *((int8_t*)(ptr + 4)) = 2;
        *((int32_t*)(ptr + 12)) = (int32_t) (*payload17).len;
        *((int32_t*)(ptr + 8)) = (int32_t) (*payload17).ptr;
        break;
      }
      case 3: {
        const uint16_t *payload18 = &(*payload14).val.status_error;
        *((int8_t*)(ptr + 4)) = 3;
        *((int16_t*)(ptr + 8)) = (int32_t) (*payload18);
        break;
      }
      case 4: {
        const http_handler_string_t *payload19 = &(*payload14).val.unexpected_error;
        *((int8_t*)(ptr + 4)) = 4;
        *((int32_t*)(ptr + 12)) = (int32_t) (*payload19).len;
        *((int32_t*)(ptr + 8)) = (int32_t) (*payload19).ptr;
        break;
      }
    }
    
  } else {
    const http_handler_response_t *payload = &(ret).val.ok;
    *((int8_t*)(ptr + 0)) = 0;
    *((int16_t*)(ptr + 4)) = (int32_t) ((*payload).status);
    
    if (((*payload).headers).is_some) {
      const http_handler_headers_t *payload11 = &((*payload).headers).val;
      *((int8_t*)(ptr + 8)) = 1;
      *((int32_t*)(ptr + 16)) = (int32_t) (*payload11).len;
      *((int32_t*)(ptr + 12)) = (int32_t) (*payload11).ptr;
      
    } else {
      *((int8_t*)(ptr + 8)) = 0;
      
    }
    
    if (((*payload).body).is_some) {
      const http_handler_body_t *payload13 = &((*payload).body).val;
      *((int8_t*)(ptr + 20)) = 1;
      *((int32_t*)(ptr + 28)) = (int32_t) (*payload13).len;
      *((int32_t*)(ptr + 24)) = (int32_t) (*payload13).ptr;
      
    } else {
      *((int8_t*)(ptr + 20)) = 0;
      
    }
    
  }
  return ptr;
}
