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

#ifndef LOG_AND_REPLAY_H
#define LOG_AND_REPLAY_H

#include <map>
#include <vector>
#include "lower_half_opengl_if.h"
#include "common.h"

// enum for types
enum pages_t {
  CUDA_MALLOC_PAGE = 0,
  CUDA_UVM_PAGE,
  CUDA_HOST_ALLOC_PAGE,
  CUDA_HEAP_PAGE,
  CUMEM_ALLOC_PAGE
};

typedef struct Lhckpt_pages_t {
  pages_t mem_type;
  void * mem_addr;
  size_t mem_len;
} lhckpt_pages_t;

enum virt_type_t {
    VIRT_TYPE_SEQUENTIAL,
    VIRT_TYPE_MALLOC,
};

typedef struct virt_class {
    virt_type_t type;
    std::map<void *, void *> mapping;
} virt_class_t;

// void logAPI(GL_Fncs_t cuda_fnc, ...);
void replayAPI(LhCallLog_t *l);
void logs_read_and_apply();
void disableLogging();
void enableLogging();
bool isLoggingDisabled();



std::vector<LhCallLog_t>& get_lh_call_logs();
void log_lh_call(LhCallLog_t log_entry);
std::map<void *, lhckpt_pages_t> & get_lh_page_map();

void *virtualize_identifier(int cls_id, void *identifier, void *aux);
void *devirtualize_identifier(int cld_id, void *v_identifier);
void set_virtualization_type(int cls_id, virt_type_t type);
void *update_virtualize_identifier(int cls_id, void *v_identifier, void *identifier, void *aux);
#endif