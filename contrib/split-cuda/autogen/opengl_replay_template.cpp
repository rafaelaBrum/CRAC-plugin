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
        cursor += {{arg.buf_len()}} * sizeof({{arg.deref_type()}});
        {% else %}
        {{arg.type_}} _{{arg.name}} = *({{arg.type_}} *) cursor;
        cursor += sizeof({{arg.type_}});
        {% endif %}
    {% endfor %}

    {% if True %}
    DLOG(INFO, "Replaying {{opengl_func.name}}\n");
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
        "original return!\n");
    }
    {% endif %}

    int gl_error = glGetError();
    if (gl_error) {
        DLOG(ERROR, "GL error: %d\n", gl_error);
    }
}
{% endfor %}

void replay_opengl_call(LhCallLog_t &log_entry) {
    GL_Fncs_t fnc_id = *(GL_Fncs_t *) log_entry.fncargs;
    switch (fnc_id) {
        {% for opengl_func in funcs if opengl_func.log %}
        case GENERATE_ENUM({{opengl_func.name}}):
            replay_{{opengl_func.name}}(log_entry);
            break;
        {% endfor %}
        default:
            assert(false);
    }
}

Display *dpy;
Window win;
void init_opengl_replay() {
    int attribs[64];
    int i = 0;
    XSetWindowAttributes attr;
    unsigned long mask;
    Window root;
    GLXContext ctx;

    attribs[i++] = GLX_RGBA;
    attribs[i++] = GLX_DOUBLEBUFFER;

    /* Key/value attributes. */
    attribs[i++] = GLX_RED_SIZE;
    attribs[i++] = 1;
    attribs[i++] = GLX_GREEN_SIZE;
    attribs[i++] = 1;
    attribs[i++] = GLX_BLUE_SIZE;
    attribs[i++] = 1;
    attribs[i++] = GLX_DEPTH_SIZE;
    attribs[i++] = 1;

    attribs[i++] = None;

    dpy = XOpenDisplay(0);
    int scrnum = DefaultScreen( dpy );
    XVisualInfo *visinfo = glXChooseVisual(dpy, scrnum, attribs);
   if (!visinfo) {
      printf("Error: couldn't get an RGB, Double-buffered\n");
      assert(false);
   }
    root = RootWindow( dpy, scrnum );

    /* window attributes */
    attr.background_pixel = 0;
    attr.border_pixel = 0;
    attr.colormap = XCreateColormap( dpy, root, visinfo->visual, AllocNone);
    attr.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask;
    /* XXX this is a bad way to get a borderless window! */
    mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

    win = XCreateWindow( dpy, root, 0, 0, 164, 164,
                         0, visinfo->depth, InputOutput,
                         visinfo->visual, mask, &attr );
    ctx = glXCreateContext( dpy, visinfo, NULL, True );
   if (!ctx) {
      printf("Error: glXCreateContext failed\n");
      assert(false);
   }
    XFree(visinfo);

    XMapWindow(dpy, win);
    glXMakeCurrent(dpy, win, ctx);
}

void swap_buffers() {
    glXSwapBuffers(dpy, win);
}