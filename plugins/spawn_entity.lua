function plugin_register()
    local id = create_entity()
    add_position(id, 1, 1)
    log_info("plugin spawned entity " .. id)
end

plugin_register()
