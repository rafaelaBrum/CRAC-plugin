// replay from lower half

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <assert.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include "lower_half_opengl_if.h"
#include "common.h"
#include "logging.h"


{% for opengl_func in funcs if opengl_func.log %}
void replay_{{opengl_func.name}}(LhCallLog_t &log_entry) {
    char *log_buf = log_entry.fncargs;
    char *cursor = log_buf + sizeof(int); // skip function id

    {% for arg in opengl_func.get_args() %}
        {% if arg.copy_as_buf() %}
        {{arg.type_}} _{{arg.name}} = ({{arg.type_}}) cursor;
        log_buf += {{arg.buf_len()}} * sizeof({{arg.deref_type()}});
        {% else %}
        {{arg.type_}} _{{arg.name}} = *({{arg.type_}} *) cursor;
        log_buf += sizeof({{arg.type_}});
        {% endif %}
    {% endfor %}

    {% if True %}
    DLOG(INFO, "Replaying {{opengl_func.name}}");
    {% endif %}

    {% if opengl_func.return_type != 'void' %}
    {{opengl_func.return_type}} _ret =
    {% endif %}
        {% set comma = joiner(',') %}
        {{opengl_func.name}}(
            {% for arg in opengl_func.get_args() %}
                {{comma()}} _{{arg.name}}
            {% endfor %}
        );

    {% if opengl_func.return_type != 'void' %}
    if (_ret != *({{opengl_func.return_type}} *) log_entry.results) {
        DLOG(INFO, "Return of logged function does not match\n"
        "original return!");
    }
    {% endif %}
}
{% endfor %}

void replay_opengl_call(LhCallLog_t &log_entry) {
    GL_Fncs_t fnc_id = *(GL_Fncs_t *) log_entry.fncargs;
    switch (fnc_id) {
        {% for opengl_func in func %}
        case GENERATE_ENUM({{opengl_func.name}}):
            replay_{{opengl_func.name}}(log_entry);
            break;
        {% endfor %}
        default:
            assert(false);
    }
}