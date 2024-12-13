
function LerpVec3(startX, startY, startZ, endPosX, endPosY, endPosZ, t)
    -- Ensure t is clamped between 0 and 1
    if t < 0 then t = 0 elseif t > 1 then t = 1 end

    t = EaseInOut(t);

    -- Calculate interpolated vector
    local result = {
        x = startX + (endPosX - startX) * t,
        y = startY + (endPosY - startY) * t,
        z = startZ + (endPosZ - startZ) * t
    }
    return result
end

function MoveObj(objectName, startX, startY, startZ, endPosX, endPosY, endPosZ, seconds, time)
    print("[LUA] [MoveObj] Got:" .. objectName .. ", " .. startX .. ", " .. startY .. ", " .. startZ .. ', ' .. endPosX .. ', ' .. endPosY .. ', ' .. endPosZ)
    
    local t = time / seconds
    -- Interpolate with the specified easing function
    local newPosition = LerpVec3(startX, startY, startZ, endPosX, endPosY, endPosZ, t)

    -- Apply the movement to the object
    RotateTo(objectName, newPosition.x, newPosition.y, newPosition.z)
end

function EaseInOut(t)
    -- Quadratic ease-in-out function
    if t < 0.5 then
        return 2 * t * t
    else
        return -1 + (4 - 2 * t) * t
    end
end

