
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


function MoveObj(objectName, startX, startY, startZ, endPosX, endPosY, endPosZ, seconds, time)
    print("[LUA] [RotateObj] Got:"..objectName..", "..startX..", "..startY..", "..startZ..', '..endPosX..', '..endPosY..', '..endPosZ) 
    local t = time/seconds;
    -- Compute the direction vector (as a simplified rotation example)
    local newRotation = LerpVec3(startX, startY, startZ, endPosX, endPosY, endPosZ, t)
    -- Call RotateTo to apply the calculated rotation
    RotateTo(objectName, newRotation.x, newRotation.y, newRotation.z)
end