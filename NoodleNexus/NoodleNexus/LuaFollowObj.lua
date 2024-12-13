function MoveObj(objectName, startX, startY, startZ, targetX, targetY, targetZ, followDistance, emp0, maxSpeed, slowDownRange, deltaTime)
    -- Calculate the vector from the object to the target position
    local dx = targetX - startX
    local dy = targetY - startY
    local dz = targetZ - startZ
    local distance = math.sqrt(dx * dx + dy * dy + dz * dz)

    -- Determine the speed based on the distance
    local speed = 0
    if distance > slowDownRange then
        speed = maxSpeed -- Beyond slowDownRange, move at max speed
    elseif distance > followDistance then
        -- Between followDistance and slowDownRange, proportionally slow down
        local normalizedDist = (distance - followDistance) / (slowDownRange - followDistance)
        speed = maxSpeed * normalizedDist
    end

    -- Calculate the movement vector
    if speed > 0 then
        local moveFactor = speed * deltaTime / distance
        local moveX = dx * moveFactor
        local moveY = dy * moveFactor
        local moveZ = dz * moveFactor

        -- Update the object's position
        MoveTo(objectName, startX + moveX, startY + moveY, startZ + moveZ)
    end

    -- Debugging output
    print(string.format("[LUA] %s following position: Target (%.2f, %.2f, %.2f), Distance %.2f, Speed %.2f",
        objectName, targetX, targetY, targetZ, distance, speed))
end