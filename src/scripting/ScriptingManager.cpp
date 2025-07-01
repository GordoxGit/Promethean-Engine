#include "scripting/ScriptingManager.h"
#include "ecs/Registry.h"
#include "ecs/Component.h"
#include "core/LogSystem.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

namespace pe::scripting {

ScriptingManager& ScriptingManager::Instance()
{
    static ScriptingManager inst;
    return inst;
}

bool ScriptingManager::Init(pe::ecs::Registry* registry)
{
    m_registry = registry;
    m_state = luaL_newstate();
    if(!m_state)
        return false;
    luaL_openlibs(m_state);
    lua_register(m_state, "create_entity", Lua_CreateEntity);
    lua_register(m_state, "add_position", Lua_AddPosition);
    lua_register(m_state, "log_info",     Lua_Log);
    return true;
}

void ScriptingManager::Shutdown()
{
    if(m_state) {
        lua_close(m_state);
        m_state = nullptr;
    }
    m_registry = nullptr;
}

bool ScriptingManager::RunScript(const std::string& file)
{
    if(!m_state)
        return false;
    m_lastFile = file;
    if(luaL_dofile(m_state, file.c_str()) != LUA_OK) {
        LogSystem::Instance().Error("Lua error: {}", lua_tostring(m_state, -1));
        lua_pop(m_state, 1);
        return false;
    }
    return true;
}

bool ScriptingManager::ReloadScript(const std::string& file)
{
    if(file.empty())
        return RunScript(m_lastFile);
    return RunScript(file);
}

int ScriptingManager::Lua_CreateEntity(lua_State* L)
{
    auto* reg = Instance().m_registry;
    if(!reg)
        return 0;
    auto e = reg->create();
    lua_pushinteger(L, static_cast<lua_Integer>(e.id()));
    return 1;
}

int ScriptingManager::Lua_AddPosition(lua_State* L)
{
    auto* reg = Instance().m_registry;
    if(!reg)
        return 0;
    pe::ecs::EntityID id = static_cast<pe::ecs::EntityID>(luaL_checkinteger(L, 1));
    float x = static_cast<float>(luaL_checknumber(L, 2));
    float y = static_cast<float>(luaL_checknumber(L, 3));
    reg->add<pe::ecs::Position>(id, pe::ecs::Position{x,y});
    return 0;
}

int ScriptingManager::Lua_Log(lua_State* L)
{
    const char* msg = luaL_checkstring(L, 1);
    LogSystem::Instance().Info("{}", msg);
    return 0;
}

} // namespace pe::scripting
