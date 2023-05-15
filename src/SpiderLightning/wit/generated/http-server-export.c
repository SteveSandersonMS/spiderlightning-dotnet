#include <stdlib.h>
#include <http-server-export.h>

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

void http_server_export_string_set(http_server_export_string_t *ret, const char *s) {
  ret->ptr = (char*) s;
  ret->len = strlen(s);
}

void http_server_export_string_dup(http_server_export_string_t *ret, const char *s) {
  ret->len = strlen(s);
  ret->ptr = canonical_abi_realloc(NULL, 0, 1, ret->len);
  memcpy(ret->ptr, s, ret->len);
}

void http_server_export_string_free(http_server_export_string_t *ret) {
  canonical_abi_free(ret->ptr, ret->len, 1);
  ret->ptr = NULL;
  ret->len = 0;
}
void http_server_export_expected_unit_string_free(http_server_export_expected_unit_string_t *ptr) {
  if (!ptr->is_err) {
  } else {
    http_server_export_string_free(&ptr->val.err);
  }
}

__attribute__((aligned(4)))
static uint8_t RET_AREA[12];
__attribute__((export_name("on-server-init")))
int32_t __wasm_export_http_server_export_on_server_init(void) {
  http_server_export_expected_unit_string_t ret;
  http_server_export_on_server_init(&ret);
  int32_t ptr = (int32_t) &RET_AREA;
  
  if ((ret).is_err) {
    const http_server_export_string_t *payload0 = &(ret).val.err;
    *((int8_t*)(ptr + 0)) = 1;
    *((int32_t*)(ptr + 8)) = (int32_t) (*payload0).len;
    *((int32_t*)(ptr + 4)) = (int32_t) (*payload0).ptr;
    
  } else {
    
    *((int8_t*)(ptr + 0)) = 0;
    
  }
  return ptr;
}
