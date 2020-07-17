#ifndef LOWER_HALF_CUDA_IF_H
#define LOWER_HALF_CUDA_IF_H

#include <GL/gl.h>


#define FOREACH_FNC(MACRO) \
MACRO(glBegin), \

#define GENERATE_ENUM(ENUM) GL_Fnc_##ENUM

#define GENERATE_FNC_PTR(FNC) ((void*)&FNC)

typedef enum __GL_Fncs {
    GL_Fnc_NULL,
    FOREACH_FNC(GENERATE_ENUM)
    GL_Fnc_Invalid,
} GL_Fncs_t;

#define GENERATE_FNC_NAME_LIST(FNC) \
        #FNC

static const char *GL_Fnc_to_str[]  __attribute__((used)) =
        {
                "GL_Fnc_NULL",
                FOREACH_FNC(GENERATE_FNC_NAME_LIST)
                "GL_Fnc_Invalid",
        };
#endif // LOWER_HALF_CUDA_IF_H