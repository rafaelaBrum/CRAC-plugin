#include "uh_logging.h"
#include "opengl_replay.h"
#include "getmmap.h"
#include "logging.h"

void replayAPI(LhCallLog_t *l) {
    replay_opengl_call(*l);
}
void logs_read_and_apply() {
    init_opengl_replay();
    GetCudaCallsLogFptr_t fnc = (GetCudaCallsLogFptr_t)uhInfo.cudaLogVectorFptr;
    std::vector<LhCallLog_t>& cudaCallsLog = fnc();
    for (auto it = cudaCallsLog.begin(); it != cudaCallsLog.end(); it++) {
        replayAPI(&(*it));
    }
    swap_buffers();
    DLOG(INFO, "Done replaying!\n");
    {int x = 1; while (x) {}}

}