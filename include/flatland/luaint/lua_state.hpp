#pragma once

#include <sol/sol.hpp>
#include <string.h>

namespace flat::lua {

    class state;
}

class flat::lua::state : private sol::state {

    class lua_general_type;

    template <class T>
    class lua_type;

public:

    state();

    /*
     * Script loading and running section
     */

    // run code given by string
    template <typename ...Args>
    void run_code(const std::string& code, Args... args)
    {
        // TODO, handle exception
        auto result = sol::state::load(code);

        if (result) // if valid
            result(args);
    }

    // load code given by an external file as a command
    sol::load_result load_script(const std::string& cmd, const std::string& path);

    // load code given by string as a command
    sol::load_result load_code(const std::string& cmd, const std::string& code);

    // deactivate a command
    void rm_cmd(const std::string& cmd);

    // run directly a specific script with the given arguments
    template <typename ...Args>
    bool run_script(const std::string& path, Args... args)
    {
        auto it = scripts.find(path);

        // TODO, handle exception
        if (it != scripts.end())
            auto result = (it.second)(args);

        return result.valid();
    }


    /*
     * Base commands
     */

    // run an arbitrary command
    bool exec(const std::string& cmd);



    /*
     * Errors on loading or running scripts
     */

    /*class script_error : public std::exception
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

    *
     * Usertype definition, TODO maybe not necessary
     */

    /*template <class T>
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
    }*/

    using sol::state::new_usertype;

private:

    // scripts loaded by files
    std::unordered_map<std::string, sol::load_result> scripts;

    // defined user types in lua, TODO, maybe not necessary
    //std::unordered_map<std::string, std::unique_ptr<lua_general_type>> types;
};

/*class flat::lua::state::lua_general_type {

public:
    virtual ~lua_general_type() {}

    template <class T>
    virtual sol::usertype<T> get() = 0;
};

template <class T>
class flat::lua::state::lua_type : public flat::lua::state::lua_general_type, public sol::usertype<T> {

public:

    // construct only by moving a usertype
    lua_type(const sol::usertype<T>& t) : sol::usertype<T>(std::forward(t)) {}

    lua_type(sol::usertype<T>&& t) : sol::usertype<T>(t) {}

    virtual sol::usertype<T> get() override {
        return *this;
    }
};*/

