#ifndef __FORCE_QUIT_H__
#define __FORCE_QUIT_H__

namespace flat {
    struct ForceQuit {
        const char * reason;

        ForceQuit(const char *reason) : reason(reason) {}
    };
}

#endif
