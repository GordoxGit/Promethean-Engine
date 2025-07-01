function OnStart()
    local id = create_entity()
    add_position(id, 5, 10)
    log_info("spawned entity " .. id)
end

OnStart()
