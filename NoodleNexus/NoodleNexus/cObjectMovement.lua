-- Function to handle rotation calculations and apply the rotation to the object
function RotateObj(objectName, startX, startY, startZ, endPosX, endPosY, endPosZ, seconds, deltaTime)
    -- Compute the direction vector (as a simplified rotation example)
    local directionX = endPosX - startX
    local directionY = endPosY - startY
    local directionZ = endPosZ - startZ

    -- Calculate distance
    local distance = math.sqrt(directionX^2 + directionY^2 + directionZ^2)

    -- Normalize the direction vector
    local directionNormX = directionX / distance
    local directionNormY = directionY / distance
    local directionNormZ = directionZ / distance

    -- Define rotation speed (e.g., 1 degree per second)
    local rotationSpeed = 10.0  -- degrees per second

    -- Calculate the rotation to be applied for this frame
    local rotationX = startX + rotationSpeed * deltaTime
    local rotationY = startY + rotationSpeed * deltaTime
    local rotationZ = startZ + rotationSpeed * deltaTime

    -- Call RotateTo to apply the calculated rotation
    RotateTo(objectName, rotationX, rotationY, rotationZ)
end


function MovObj(objectName, startX, startY, startZ, endPosX, endPosY, endPosZ, seconds, deltaTime)
    -- Compute the direction and velocity
    local directionX = endPosX - startX
    local directionY = endPosY - startY
    local directionZ = endPosZ - startZ

    local distance = math.sqrt(directionX^2 + directionY^2 + directionZ^2)
    local directionNormX = directionX / distance
    local directionNormY = directionY / distance
    local directionNormZ = directionZ / distance

    local velocityX = directionNormX * (distance / seconds)
    local velocityY = directionNormY * (distance / seconds)
    local velocityZ = directionNormZ * (distance / seconds)

    -- Compute the new position
    local newX = startX + velocityX * deltaTime
    local newY = startY + velocityY * deltaTime
    local newZ = startZ + velocityZ * deltaTime

    -- Move the object using the calculated position
    MoveTo(objectName, newX, newY, newZ)
end

function QuadraticBezier(t, p0, p1, p2)
    local u = 1 - t
    return u * u * p0 + 2 * u * t * p1 + t * t * p2
end



function MoveAlongCurve(objectName, p0x, p0y, p0z, p1x, p1y, p1z, p2x, p2y, p2z, duration, time)
    local t = duration/time;
    local x = QuadraticBezier(t, p0x, p1x, p2x)
    local y = QuadraticBezier(t, p0y, p1y, p2y)
    local z = QuadraticBezier(t, p0z, p1z, p2z)

    print("[LUA] New position for "..objectName.." is "..x..","..y..","..z)

    -- Move the object using the calculated position
    MoveTo(objectName, x, y, z)
end


