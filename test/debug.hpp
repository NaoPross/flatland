#pragma once

#ifndef NPDEBUG
#define NPDEBUG

#ifdef DEBUG
#include <iostream>
#include <sstream>

#define __FILENAME__ (\
    __builtin_strrchr(__FILE__, '/') ? \
    __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

#define npdebug(...); { \
    std::cerr << "[" << __FILENAME__ \
              << ":" << __LINE__ \
              << ", " << __func__ \
              << "] " ; \
    _np::debug(__VA_ARGS__); \
}

#define npinspect(msg, expr, ...); _np::inspect(msg, expr, __VA_ARGS__);

namespace _np {
    template<typename... Args>
    inline void debug(const Args&... args) {
        (std::cerr << ... << args) << std::endl;
    }

    template<typename Msg, typename Expr, typename... Args>
    inline Expr& inspect(const Msg& msg, Expr& expr, const Args&... args) {
        npdebug(msg, expr, args...);
        return expr;
    }
}

#else

#define npdebug(...); {}
#define npinspect(...); {}

namespace _np {
    template<typename... Args>
    void debug(Args&... args) {}

    template<typename Msg, typename Expr, typename... Args>
    inline Expr& inspect(const Msg& msg, Expr& expr, const Args&... args) {
        return expr;
    }
}

#endif // defined DEBUG
#endif // not defined NPDEBUG
