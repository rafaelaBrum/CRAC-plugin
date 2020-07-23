#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <jassert.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include "dmtcp.h"
#include "switch_context.h"
#include "lower_half_opengl_if.h"
#include "common.h"
#include "upper-half-wrappers.h"
#include "uh_logging.h"

#define REAL_FNC(fnc) \
  ({ fnc##_t fnc##Fnc = (fnc##_t) -1; \
  if (!initialized) { \
    initialize_wrappers(); \
  } \
  if (fnc##Fnc == (fnc##_t) -1) { \
    LhDlsym_t dlsymFptr = (LhDlsym_t)lhInfo.lhDlsym; \
    fnc##Fnc = (fnc##_t)dlsymFptr(GL_Fnc_##fnc); \
  } \
  fnc##Fnc; })

#define USER_DEFINED_WRAPPER(rettype, fnc, args) \
  typedef rettype (*fnc##_t)args;              \
  EXTERNC rettype fnc args

#define DECLARE_WRAPPER(rettype, fnc, args) \
  extern rettype fnc args __attribute__((weak));



{% for opengl_func in funcs %}

{% if opengl_func.log %}
void log_{{opengl_func.name}}({{opengl_func.args_signature()}}
{% if opengl_func.return_type != 'void' %}
, {{opengl_func.return_type}} _ret
{% endif %}
) {
    size_t args_buf_size = sizeof(int); // for function #
    {% for arg in opengl_func.get_args() %}
        {% if arg.copy_as_buf() %}
        args_buf_size += {{ arg.buf_len() }} * sizeof({{arg.deref_type()}});
        {% else %}
        args_buf_size += sizeof({{arg.type_}});
        {% endif %}
    {% endfor %}

    char *args_buf = (char *) malloc(args_buf_size);

    char *cursor = args_buf;
    *(int *) cursor= GL_Fnc_{{opengl_func.name}};
    cursor += sizeof(int);

    {% for arg in opengl_func.get_args() %}
        {% if arg.copy_as_buf() %}
        memcpy(cursor, {{arg.name}}, {{ arg.buf_len() }} * sizeof({{arg.deref_type()}}));
        cursor += {{ arg.buf_len() }} * sizeof({{arg.deref_type()}});
        {% else %}
        memcpy(cursor, &{{arg.name}}, sizeof({{arg.type_}}));
        cursor += sizeof({{arg.type_}});
        {% endif %}
    {% endfor %}

{% if opengl_func.return_type != 'void' %}
    size_t ret_buf_size = sizeof({{opengl_func.return_type}});
    void *ret_buf = malloc(ret_buf_size);

    memcpy(ret_buf, &_ret, sizeof({{opengl_func.return_type}}));
{% else %}
    size_t ret_buf_size = 0;
    void *ret_buf = NULL;
{% endif %}

    LhCallLog_t log_entry;
    log_entry.fncargs = (char *) args_buf;
    log_entry.size = args_buf_size;
    log_entry.results = (char *) ret_buf;
    log_entry.res_size = ret_buf_size;
    log_lh_call(log_entry);

}
{% endif %}

#undef {{opengl_func.name}}
extern "C"
{{opengl_func.return_type}} {{opengl_func.name}}({{opengl_func.args_signature()}}) {
    typedef {{opengl_func.return_type}}
        (*{{opengl_func.name}}_t)({{opengl_func.args_signature()}});

    {% if opengl_func.return_type != 'void' %}
        {{ opengl_func.return_type }} ret;
    {% endif %}

    DMTCP_PLUGIN_DISABLE_CKPT();

    {% if False %}
    JNOTE("Wrapper called for {{opengl_func.name}}")
    {% for arg_name in opengl_func.arg_names() %}
        ({{arg_name}})
    {% endfor %};
    {% endif %}

    JUMP_TO_LOWER_HALF(lhInfo.lhFsAddr);

    {{opengl_func.name}}_t real_fnc = REAL_FNC({{opengl_func.name}});
    {% if opengl_func.return_type != 'void' %}
    ret =
    {% endif %}
        real_fnc({{opengl_func.arg_names_commas()}});

    RETURN_TO_UPPER_HALF();

    /* Insert logging code here */
    {% if opengl_func.log %}
    log_{{opengl_func.name}}({{opengl_func.arg_names_commas()}}
        {% if opengl_func.return_type != 'void' %}
        , ret
        {% endif %}
    );
    {% endif %}

    DMTCP_PLUGIN_ENABLE_CKPT();

    {% if opengl_func.return_type != 'void' %}
    // JNOTE("Wrapper finished for {{opengl_func.name}}") (ret);
    return ret;
    {% endif %}
}

{% endfor %}
