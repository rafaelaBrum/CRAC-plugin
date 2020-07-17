#include <GL/gl.h>
#include <jassert.h>
#include "dmtcp.h"
#include "switch_context.h"
#include "lower_half_opengl_if.h"
#include "common.h"
#include "upper-half-wrappers.h"

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

{% for opengl_func in opengl_funcs %}
#undef {{opengl_func.name}}
extern "C" {{opengl_func.return_type}} {{opengl_func.name}}({{opengl_func.args_sig}}) {
    typedef {{opengl_func.return_type}} (*{{opengl_func.name}}_t)({{opengl_func.args_sig}});
    {% if opengl_func.return_type != 'void' %}
        {{ opengl_func.return_type }} ret;
    {% endif %}
    DMTCP_PLUGIN_DISABLE_CKPT();
    JNOTE("Wrapper called for {{opengl_func.name}}");
    JUMP_TO_LOWER_HALF(lhInfo.lhFsAddr);
    {{opengl_func.name}}_t real_fnc = REAL_FNC({{opengl_func.name}});
    {% if opengl_func.return_type != 'void' %}
    ret =
    {% endif %}
        real_fnc({{opengl_func.args}});
    RETURN_TO_UPPER_HALF();
    /* Insert logging code here */
    DMTCP_PLUGIN_ENABLE_CKPT();
    JNOTE("Wrapper finished for {{opengl_func.name}}");
    {% if opengl_func.return_type != 'void' %}
    return ret;
    {% endif %}

}

{% endfor %}