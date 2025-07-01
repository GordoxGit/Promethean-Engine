#pragma once
#include <string>

struct lua_State;

namespace pe { namespace ecs { class Registry; struct Position; } }

namespace pe::scripting {

class ScriptingManager {
public:
    static ScriptingManager& Instance();

    bool Init(pe::ecs::Registry* registry);
    void Shutdown();

    bool RunScript(const std::string& file);
    bool ReloadScript(const std::string& file = "");

    pe::ecs::Registry* registry() const { return m_registry; }

private:
    static int Lua_CreateEntity(lua_State* L);
    static int Lua_AddPosition(lua_State* L);
    static int Lua_Log(lua_State* L);

    lua_State* m_state{nullptr};
    pe::ecs::Registry* m_registry{nullptr};
    std::string m_lastFile;
};

} // namespace pe::scripting
