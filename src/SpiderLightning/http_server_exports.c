#include <stdio.h>
#include <string.h>
#include <wasm/driver.h>
#include "wit/generated/http-handler.h"
#include "wit/generated/http-server-export.h"

int dotnet_started = 0;
void _start();
void mono_wasm_invoke_method_ref(MonoMethod* method, MonoObject** this_arg_in, void* params[], MonoObject** _out_exc, MonoObject** out_result);

void ensure_dotnet_started() {
    if (!dotnet_started) {
        _start();
        dotnet_started = 1;
    }
}

void http_server_export_on_server_init(http_server_export_expected_unit_string_t* ret0) {
    ensure_dotnet_started();

    MonoMethod* method = lookup_dotnet_method("SpiderLightning", "SpiderLightning", "HttpServer", "Export_OnServerInit", -1);
    void* method_params[] = { };
    MonoObject* exception;
    MonoObject* result;
    mono_wasm_invoke_method_ref(method, NULL, method_params, &exception, &result);

    if (exception) {
        char* error_utf8 = mono_string_to_utf8((MonoString*)result);
        *ret0 = (http_server_export_expected_unit_string_t){
            .is_err = 1,
            .val = {
                .err = { .ptr = error_utf8, .len = strlen(error_utf8) }
            }
        };
    } else {
        *ret0 = (http_server_export_expected_unit_string_t){ .is_err = 0 };
    }
}

void http_handler_handle_http(http_handler_request_t* req, http_handler_expected_response_http_error_t* ret0) {
    ensure_dotnet_started();

    MonoMethod* method = lookup_dotnet_method("SpiderLightning", "SpiderLightning", "HttpServer", "Export_HandleIncomingRequest", -1);
    void* method_params[] = { req, ret0 };
    MonoObject* exception;
    MonoObject* result;    
    mono_wasm_invoke_method_ref(method, NULL, method_params, &exception, &result);

    if (exception) {
        char* exception_string_utf8 = mono_string_to_utf8((MonoString*)result);
        *ret0 = (http_handler_expected_response_http_error_t){
            .is_err = 1,
            .val = {
                .err = {
                    .tag = HTTP_HANDLER_HTTP_ERROR_UNEXPECTED_ERROR,
                    .val = {
                        .unexpected_error = {
                            .ptr = exception_string_utf8,
                            .len = strlen(exception_string_utf8)
                        }
                    }
                }
            }
        };
    }
}
