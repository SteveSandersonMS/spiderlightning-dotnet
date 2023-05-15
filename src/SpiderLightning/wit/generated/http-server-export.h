#ifndef __BINDINGS_HTTP_SERVER_EXPORT_H
#define __BINDINGS_HTTP_SERVER_EXPORT_H
#ifdef __cplusplus
extern "C"
{
  #endif
  
  #include <stdint.h>
  #include <stdbool.h>
  
  typedef struct {
    char *ptr;
    size_t len;
  } http_server_export_string_t;
  
  void http_server_export_string_set(http_server_export_string_t *ret, const char *s);
  void http_server_export_string_dup(http_server_export_string_t *ret, const char *s);
  void http_server_export_string_free(http_server_export_string_t *ret);
  typedef struct {
    bool is_err;
    union {
      http_server_export_string_t err;
    } val;
  } http_server_export_expected_unit_string_t;
  void http_server_export_expected_unit_string_free(http_server_export_expected_unit_string_t *ptr);
  void http_server_export_on_server_init(http_server_export_expected_unit_string_t *ret0);
  #ifdef __cplusplus
}
#endif
#endif
