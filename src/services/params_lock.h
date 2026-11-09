// Lock for the params::* data shared between the CAN task (core 1) and the web task
// (core 0). Hold it only for short copies or single writes.
#ifndef PARAMS_LOCK_H
#define PARAMS_LOCK_H

void paramsLockInit();
void paramsLock();
void paramsUnlock();

class ParamsGuard {
public:
    ParamsGuard() { paramsLock(); }
    ~ParamsGuard() { paramsUnlock(); }
    ParamsGuard(const ParamsGuard &) = delete;
    ParamsGuard &operator=(const ParamsGuard &) = delete;
};

#endif  // PARAMS_LOCK_H
