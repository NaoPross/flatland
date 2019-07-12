#include "luaint/lua_state.hpp"

using namespace flatland::lua;

lua_state::lua_state()
{
    lua.open_libraries(sol::lib::base);
}

sol::load_result lua_state::load_script(const std::string& path)
{
    auto result = lua.load_file(path);

    if (result)
        scripts.insert_or_assign(std::move(result));

    return result;
}

void lua_state::rm_script(const std::string& path)
{
    auto it = scripts.find(path);

    if (it != scripts.end())
        scripts.erase(it);
}

void lua_state::run_scripts()
{
    for (auto& scp : scripts) {
        if (!(scp.second)().valid()) // TODO, get lua stack trace
            throw run_script_error(scp.first.c_str(), "");
    }
}
