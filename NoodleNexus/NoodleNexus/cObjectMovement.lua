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



