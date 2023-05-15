#ifndef __BINDINGS_HTTP_SERVER_H
#define __BINDINGS_HTTP_SERVER_H
#ifdef __cplusplus
extern "C"
{
  #endif
  
  #include <stdint.h>
  #include <stdbool.h>
  
  typedef struct {
    uint32_t idx;
  } http_server_router_t;
  void http_server_router_free(http_server_router_t *ptr);
  http_server_router_t http_server_router_clone(http_server_router_t *ptr);
  
  typedef struct {
    uint32_t idx;
  } http_server_server_t;
  void http_server_server_free(http_server_server_t *ptr);
  http_server_server_t http_server_server_clone(http_server_server_t *ptr);
  
  typedef struct {
    char *ptr;
    size_t len;
  } http_server_string_t;
  
  void http_server_string_set(http_server_string_t *ret, const char *s);
  void http_server_string_dup(http_server_string_t *ret, const char *s);
  void http_server_string_free(http_server_string_t *ret);
  // The HTTP URI of the current request.
  typedef http_server_string_t http_server_uri_t;
  void http_server_uri_free(http_server_uri_t *ptr);
  // The HTTP status code.
  typedef uint16_t http_server_http_status_t;
  // HTTP errors returned by the runtime.
  typedef struct {
    uint8_t tag;
    union {
      http_server_string_t invalid_url;
      http_server_string_t timeout_error;
      http_server_string_t protocol_error;
      http_server_http_status_t status_error;
      http_server_string_t unexpected_error;
    } val;
  } http_server_http_router_error_t;
  #define HTTP_SERVER_HTTP_ROUTER_ERROR_INVALID_URL 0
  #define HTTP_SERVER_HTTP_ROUTER_ERROR_TIMEOUT_ERROR 1
  #define HTTP_SERVER_HTTP_ROUTER_ERROR_PROTOCOL_ERROR 2
  #define HTTP_SERVER_HTTP_ROUTER_ERROR_STATUS_ERROR 3
  #define HTTP_SERVER_HTTP_ROUTER_ERROR_UNEXPECTED_ERROR 4
  void http_server_http_router_error_free(http_server_http_router_error_t *ptr);
  typedef struct {
    bool is_err;
    union {
      http_server_router_t ok;
      http_server_http_router_error_t err;
    } val;
  } http_server_expected_router_http_router_error_t;
  void http_server_expected_router_http_router_error_free(http_server_expected_router_http_router_error_t *ptr);
  typedef struct {
    bool is_err;
    union {
      http_server_server_t ok;
      http_server_http_router_error_t err;
    } val;
  } http_server_expected_server_http_router_error_t;
  void http_server_expected_server_http_router_error_free(http_server_expected_server_http_router_error_t *ptr);
  typedef struct {
    bool is_err;
    union {
      http_server_http_router_error_t err;
    } val;
  } http_server_expected_unit_http_router_error_t;
  void http_server_expected_unit_http_router_error_free(http_server_expected_unit_http_router_error_t *ptr);
  void http_server_router_new(http_server_expected_router_http_router_error_t *ret0);
  void http_server_router_new_with_base(http_server_uri_t *base, http_server_expected_router_http_router_error_t *ret0);
  void http_server_router_get(http_server_router_t self, http_server_string_t *route, http_server_string_t *handler, http_server_expected_router_http_router_error_t *ret0);
  void http_server_router_put(http_server_router_t self, http_server_string_t *route, http_server_string_t *handler, http_server_expected_router_http_router_error_t *ret0);
  void http_server_router_post(http_server_router_t self, http_server_string_t *route, http_server_string_t *handler, http_server_expected_router_http_router_error_t *ret0);
  void http_server_router_delete(http_server_router_t self, http_server_string_t *route, http_server_string_t *handler, http_server_expected_router_http_router_error_t *ret0);
  void http_server_server_serve(http_server_string_t *address, http_server_router_t router, http_server_expected_server_http_router_error_t *ret0);
  void http_server_server_stop(http_server_server_t self, http_server_expected_unit_http_router_error_t *ret0);
  #ifdef __cplusplus
}
#endif
#endif
