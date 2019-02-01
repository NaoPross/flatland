#ifndef __FORCE_QUIT_H__
#define __FORCE_QUIT_H__

namespace flat {
    struct ForceQuit {
        const char * m_reason;

        ForceQuit(const char *reason) : m_reason(reason) {}
    };
}

#endif
