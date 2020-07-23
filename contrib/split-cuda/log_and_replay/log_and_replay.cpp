#include "log_and_replay.h"

std::map<void *, lhckpt_pages_t> lh_page_map;

std::vector<LhCallLog_t> lh_call_logs;
std::vector<LhCallLog_t>& get_lh_call_logs() {
    return lh_call_logs;
}

void log_lh_call(LhCallLog_t log_entry) {
    lh_call_logs.push_back(log_entry);
}

std::map<void *, lhckpt_pages_t>& get_lh_page_map() {
    return lh_page_map;
}

std::map<int, virt_class_t> virtualization_store;


void display_lh_page_map() {
    for (auto lh_page : lh_page_map) {
        printf("\n Address = %p with size = %lu", lh_page.first, lh_page.second.mem_len);
    }
}

void set_virtualization_type(int cls_id, virt_type_t type) {
    virtualization_store[cls_id].type = type;
}

void *virtualize_identifier(int cls_id, void *identifier, void *aux) {
    virt_class_t &cls = virtualization_store[cls_id];
    void *virt_id;
    switch (cls.type) {
        case VIRT_TYPE_SEQUENTIAL: {
            if (cls.mapping.empty()) {
                virt_id = (void *) 0;
            } else {
                virt_id = (void *) ((intptr_t) cls.mapping.rbegin()->first + 1);
            }
            break;
        }
        case VIRT_TYPE_MALLOC: {
            void *new_buf = malloc((size_t) aux);
            virt_id = new_buf;
            break;
        }
    }

    cls.mapping[virt_id] = identifier;
    return virt_id;
}

void *devirtualize_identifier(int cls_id, void *v_identifier) {
    virt_class_t &cls = virtualization_store[cls_id];
    return cls.mapping[v_identifier];
}

void update_virtualize_identifier(int cls_id, void *v_identifier, void *identifier) {
    virt_class_t &cls = virtualization_store[cls_id];
    // old and new identifier should contain the same stuff if this is a malloc
    // virt lol
    cls.mapping[v_identifier] = identifier;
}