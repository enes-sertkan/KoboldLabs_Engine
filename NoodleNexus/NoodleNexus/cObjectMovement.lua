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

MovObj("racing_desk", startX, startY, startZ, endPosX, endPosY, endPosZ, seconds, deltaTime)
RotateObj("racing_desk", startX, startY, startZ, endPosX, endPosY, endPosZ, seconds, deltaTime)




