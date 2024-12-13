
function LerpVec3(startX, startY, startZ, endPosX, endPosY, endPosZ, t, p1x, p1y, p1z)
    -- Ensure t is clamped between 0 and 1
    if t < 0 then t = 0 elseif t > 1 then t = 1 end

    t = EaseInOut(t);

    -- Calculate interpolated vector
    local result = {
        x = startX + (endPosX - startX) * t,
        y = startZ + (endPosY - startY) * t,
        z = startZ + (endPosZ - startZ) * t

    }
    return result
end

--Regular lerp movement between 2 points
function MoveObj(objectName, startX, startY, startZ, endPosX, endPosY, endPosZ, seconds, time)

   print("[LUA] [MovObj] Got:"..objectName..", "..startX..", "..startY..", "..startZ..', '..endPosX..', '..endPosY..', '..endPosZ..', '..seconds..', '..time)     

   
    local t = time/seconds;

    -- Compute the direction vector (as a simplified rotation example)
    local newPosition = LerpVec3(startX, startY, startZ, endPosX, endPosY, endPosZ, t)
    
    -- Move the object using the calculated position
    MoveTo(objectName, newPosition.x, newPosition.y, newPosition.z)
end

function EaseInOut(t)
    -- Quadratic ease-in-out function
    if t < 0.5 then
        return 2 * t * t
    else
        return -1 + (4 - 2 * t) * t
    end
end