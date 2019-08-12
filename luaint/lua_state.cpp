#include "luaint/lua_state.hpp"

using namespace flat::lua;

state::state()
{
    // TODO, evaluate libraries to open
    open_libraries(sol::lib::base);
}

sol::load_result state::load_script(const std::string& cmd, const std::string& path)
{
    auto result = this->load_file(path);

    if (result)
        scripts.insert_or_assign(std::move(result));

    return result;
}


sol::load_result state::load_code(const std::string& cmd, const std::string& code)
{
    auto result = this->load(code);

    if (result)
        scripts.insert_or_assign(std::move(result));

    return result;
}

void state::rm_cmd(const std::string& cmd)
{
    auto it = scripts.find(cmd);

    if (it != scripts.end())
        scripts.erase(it);
}

int state::exec(const string& cmd)
{
    std::stringstream ss(cmd);
    std::string arg;
    int code; // exit code

    // command name
    ss >> arg;

    // call to basic commands
    if (arg == "def") // define command by a code
        return cmd_def(ss);
    else if (arg == "sdef") // define command by a script
        return cmd_sdef(ss);
    else if (arg == "run") // run code
        return cmd_run(ss);
    else if (arg == "srun") // run external script
        return cmd_srun(ss);
    else if (arg == "exit") // send "console_exit signal"
        return cmd_exit(ss);
    else if (arg == "poweroff") // send "quit signal"
        return cmd_poweroff(ss);
    else if (arg == "reboot") // send "quit signal" and restart
        return cmd_reboot(ss);
    else {

        bool found(false);

        for (auto it : scripts) {
            if (arg == it.first) {

                found = true;
                std::vector<std::string> args;

                while(ss >> arg)
                    args.push_back(arg);

                try {
                    // pass other arguments inside
                    code = (it.second)(args);
                } catch (const std::exception& exc) {
                    // TODO better management
                    std::cerr << exc.what() << std::endl;
                }

                break;
            }
        }

        if (!found)
            std::cerr << "Command " << arg << " not found" << std::endl;
    }
}
