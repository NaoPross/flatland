#pragma once

#include <sol/sol.hpp>
#include <string.h>

namespace flatland::lua {

    class lua_state;
}


class flatland::lua::lua_state {

    class lua_general_type;

    template <class T>
    class lua_type;

public:

    lua_state();

    /*
     * Script loading and running section
     */

    // run code given by string
    template <typename ...Args>
    void run_code(const std::string& code, Args... args)
    {
        // TODO, handle exception
        auto result = lua.load(code);

        if (result) // if valid
            result(args);
    }

    // run code given by an external file
    sol::load_result load_script(const std::string& path);

    // remove a loaded script from the queue
    void rm_script(const std::string& path);

    // run a specific script with the given arguments
    template <typename ...Args>
    bool run_script(const std::string& path, Args... args)
    {
        auto it = scripts.find(path);

        // TODO, handle exception
        if (it != scripts.end())
            auto result = (it.second)(args);

        return result.valid();
    }

    // run all loaded scripts
    // it's supposed there are no arguments
    // in case of invalid execution, an exception is thrown
    void run_scripts();

    /*
     * Errors on loading or running scripts
     */

    class script_error : public std::exception
    {
    public:
        script_error(const char * _scp) : scp(_scp) {}

    protected:
        const char * scp;
    };

    class load_script_error : public script_error
    {
    public:
        using script_error::script_error;

        virtual const char * what() override
        {
            return sprintf("Failed to load script %s", scp);
        }
    };

    class run_script_error : public script_error
    {
    public:
        run_script_error(const char * scp, const char * _reason = "unknown")
            : script_error(scp) reason(_reason) {}

        virtual const char * what() override
        {
            return sprintf("Execution of script %s failed, reason: %s", scp, reason);
        }

    private:
        const char * reason;
    };

    /*
     * Usertype definition, TODO maybe not necessary
     */

    template <class T>
    void register_type(const std::string& id, const sol::usertype<T>& t)
    {
        auto it = types.find(id);

        if (it != types.end())
            return; // TODO, throw exception

        types[id] = std::make_unique(lua_type<T>(std::forward(t)));
    }

    template <class T>
    std::optional<usertype<T>&> get_type(const std::string& id) const
    {
        auto it = types.find(id);

        return (it == types.end()) ?
            std::nullopt :
            std::optional(static_cast<usertype<T>&>(*it.second));
    }

protected:

    // direct access
    inline sol::state& state()
    {
        return lua;
    }

    inline const sol::state& state() const
    {
        return lua;
    }

private:

    // lua state
    sol::state lua;

    // scripts loaded by files
    std::unordered_map<std::string, sol::load_result> scripts;

    // defined user types in lua, TODO, maybe not necessary
    std::unordered_map<std::string, std::unique_ptr<lua_general_type>> types;
};

class flatland::lua::lua_state::lua_general_type {

public:
    virtual ~lua_general_type() {}

    template <class T>
    virtual sol::usertype<T> get() = 0;
};

template <class T>
class flatland::lua::lua_state::lua_type : public flatland::lua::lua_general_type, public sol::usertype<T> {

public:

    // construct only by moving a usertype
    lua_type(const sol::usertype<T>& t) : sol::usertype<T>(std::forward(t)) {}

    lua_type(sol::usertype<T>&& t) : sol::usertype<T>(t) {}

    virtual sol::usertype<T> get() override {
        return *this;
    }
};

