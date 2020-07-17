/****************************************************************************
 *  Copyright (C) 2019-2020 by Twinkle Jain, Rohan garg, and Gene Cooperman *
 *  jain.t@husky.neu.edu, rohgarg@ccs.neu.edu, gene@ccs.neu.edu             *
 *                                                                          *
 *  This file is part of DMTCP.                                             *
 *                                                                          *
 *  DMTCP is free software: you can redistribute it and/or                  *
 *  modify it under the terms of the GNU Lesser General Public License as   *
 *  published by the Free Software Foundation, either version 3 of the      *
 *  License, or (at your option) any later version.                         *
 *                                                                          *
 *  DMTCP is distributed in the hope that it will be useful,                *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *  GNU Lesser General Public License for more details.                     *
 *                                                                          *
 *  You should have received a copy of the GNU Lesser General Public        *
 *  License along with DMTCP:dmtcp/src.  If not, see                        *
 *  <http://www.gnu.org/licenses/>.                                         *
 ****************************************************************************/

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <dlfcn.h>

#include "common.h"
#include "logging.h"

static void* GL_Fnc_Ptrs[] = {
        NULL,
        FOREACH_FNC(GENERATE_FNC_PTR)
        NULL,
};

void*
lhDlsym(GL_Fncs_t fnc)
{
    DLOG(INFO, "LH: Dlsym called with: %s\n", GL_Fnc_to_str[fnc]);
    if (fnc < GL_Fnc_NULL || fnc > GL_Fnc_Invalid) {
        return NULL;
    }
    void *addr = GL_Fnc_Ptrs[fnc];
    return addr;
}
