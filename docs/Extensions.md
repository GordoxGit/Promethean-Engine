# Extensions & Plugins

Promethean Engine can load external Lua scripts placed in the `plugins/` directory.
On start, `PluginManager` scans this folder and executes every `.lua` file.
Plugins can use the scripting API (`create_entity`, `add_position`, `log_info`) to
modify the running game or editor without recompiling.

Example plugin creating an entity:

```lua
-- plugins/spawn_entity.lua
function plugin_register()
    local id = create_entity()
    add_position(id, 1, 1)
    log_info("plugin spawned entity " .. id)
end
plugin_register()
```

Load all plugins from code:

```cpp
pe::scripting::ScriptingManager::Instance().Init(&registry);
Promethean::PluginManager::Instance().LoadFromDirectory("plugins");
```

The editor exposes a simple view listing loaded plugins under the
"Extensions" tab. Hot reload is not implemented yet but scripts can
be modified and reloaded by restarting the engine.
