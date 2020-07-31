#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <assert.h>


{% for opengl_func in funcs %}

#undef {{opengl_func.name}}
extern "C"
{{opengl_func.ret.type_}} {{opengl_func.name}}({{opengl_func.args_signature()}}) {
    assert(0);
}

{% endfor %}