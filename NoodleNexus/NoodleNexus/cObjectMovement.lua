-- Function to handle rotation calculations and apply the rotation to the object

function LerpVec3(startX, startY, startZ, endPosX, endPosY, endPosZ, t, p1x, p1y, p1z)
    -- Ensure t is clamped between 0 and 1
    if t < 0 then t = 0 elseif t > 1 then t = 1 end

    -- Calculate interpolated vector
    local result = {
        x = startX + (endPosX - startX) * t,
        y = startZ + (endPosY - startY) * t,
        z = startZ + (endPosZ - startZ) * t

    }
    return result
end



function RotateObj(objectName, startX, startY, startZ, endPosX, endPosY, endPosZ, seconds, time)
    print("[LUA] [RotateObj] Got:"..objectName..", "..startX..", "..startY..", "..startZ..', '..endPosX..', '..endPosY..', '..endPosZ) 
    local t = time/seconds;
    -- Compute the direction vector (as a simplified rotation example)
    local newRotation = LerpVec3(startX, startY, startZ, endPosX, endPosY, endPosZ, t)
    -- Call RotateTo to apply the calculated rotation
    RotateTo(objectName, newRotation.x, newRotation.y, newRotation.z)
end


function MoveObj(objectName, startX, startY, startZ, endPosX, endPosY, endPosZ, seconds, time)
   print("[LUA] [MovObj] Got:"..objectName..", "..startX..", "..startY..", "..startZ..', '..endPosX..', '..endPosY..', '..endPosZ)     

   
    local t = time/seconds;

    -- Compute the direction vector (as a simplified rotation example)
    local newPosition = LerpVec3(startX, startY, startZ, endPosX, endPosY, endPosZ, t)
    
    -- Move the object using the calculated position
    MoveTo(objectName, newPosition.x, newPosition.y, newPosition.z)
end

function QuadraticBezier(t, p0, p1, p2)
    local u = 1 - t
    return u * u * p0 + 2 * u * t * p1 + t * t * p2
end

function MoveAlongCurve(objectName, p0x, p0y, p0z, p2x, p2y, p2z, duration, elapsedTime, p1x, p1y, p1z)
    -- Validate elapsedTime and avoid division by zero
    if elapsedTime == nil or duration == 0 then
        print("[LUA] Error: Invalid elapsedTime or duration is zero.")
        return
    end

    print("[LUA] [MoveAlongCurve] Got:" .. objectName .. ", " .. p0x .. ", " .. p0y .. ", " .. p0z .. ", " .. p2x .. ", " .. p2y .. ", " .. p2z .. ", " .. p1x .. ", " .. p1y .. ", " .. p1z)

    -- Calculate t as the fraction of elapsed time over the total duration
    local t = math.min(elapsedTime / duration, 1.0) -- Clamp t to the range [0, 1]

    -- Interpolate position using the Quadratic Bézier curve formula
    local x = QuadraticBezier(t, p0x, p1x, p2x)
    local y = QuadraticBezier(t, p0y, p1y, p2y)
    local z = QuadraticBezier(t, p0z, p1z, p2z)

    -- Debug the new position
    print(string.format("[LUA] New position for %s is (%.2f, %.2f, %.2f)", objectName, x, y, z))

    -- Move the object to the calculated position
    MoveTo(objectName, x, y, z)
end


function FollowObject(followerName, targetName, followDistance, followOffsetX, followOffsetY, followOffsetZ, maxSpeed, slowDownRange, speedUpRange, deltaTime)
    -- Get positions of the follower and the target
    local followerPos = GetObjectPosition(followerName)
    local targetPos = GetObjectPosition(targetName)

    if not followerPos or not targetPos then
        print("[LUA] Error: Invalid follower or target object.")
        return
    end

    -- Calculate the desired position to follow
    local desiredPosX = targetPos.x + followOffsetX
    local desiredPosY = targetPos.y + followOffsetY
    local desiredPosZ = targetPos.z + followOffsetZ

    -- Calculate the vector from the follower to the desired position
    local dx = desiredPosX - followerPos.x
    local dy = desiredPosY - followerPos.y
    local dz = desiredPosZ - followerPos.z
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

        -- Update the follower's position
        MoveTo(followerName, followerPos.x + moveX, followerPos.y + moveY, followerPos.z + moveZ)
    end

    -- Debugging output
    print(string.format("[LUA] %s following %s: Distance %.2f, Speed %.2f", followerName, targetName, distance, speed))
end


function FollowPosition(objectName, startX, startY, startZ, targetX, targetY, targetZ, followDistance, maxSpeed, slowDownRange, deltaTime)
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

-- Lua function to set the texture of an object
function SetTexture(objectName, textureName, textureId, blendAmount)
    -- Call the C++ function bound to Lua to set the texture
    lua_SetTexture(objectName, textureName, textureId, blendAmount)
end

function ToggleVisibility(objectName, isVisible)
    lua_ToggleVisibility(objectName, isVisible) -- Calls the C++ function
end


