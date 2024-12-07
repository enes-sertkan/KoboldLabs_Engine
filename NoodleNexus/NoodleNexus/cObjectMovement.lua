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


-- Helper function for quadratic Bézier curve interpolation
function QuadraticBezier(t, p0, p1, p2)
    local u = 1 - t
    return u * u * p0 + 2 * u * t * p1 + t * t * p2
end

-- Table to track ongoing curve movements for objects
local movingObjects = {}

-- Function to start moving an object along a quadratic Bézier curve
function MoveAlongCurve(objectName, p0x, p0y, p0z, p1x, p1y, p1z, p2x, p2y, p2z, duration)
    movingObjects[objectName] = {
        p0 = {x = p0x, y = p0y, z = p0z},
        p1 = {x = p1x, y = p1y, z = p1z},
        p2 = {x = p2x, y = p2y, z = p2z},
        duration = duration,
        elapsedTime = 0
    }
end



-- Function to update object movements (should be called every frame)
function UpdateCurveMovements(deltaTime)
    for objectName, movement in pairs(movingObjects) do
        -- Update elapsed time
        movement.elapsedTime = movement.elapsedTime + deltaTime
        local t = movement.elapsedTime / movement.duration

        -- Clamp t to ensure it doesn't exceed 1
        if t > 1 then
            t = 1
        end

        -- Calculate the interpolated position on the curve
        local x = QuadraticBezier(t, movement.p0.x, movement.p1.x, movement.p2.x)
        local y = QuadraticBezier(t, movement.p0.y, movement.p1.y, movement.p2.y)
        local z = QuadraticBezier(t, movement.p0.z, movement.p1.z, movement.p2.z)

        -- Move the object to the calculated position
        FollowCurve(objectName, x, y, z)

        -- Remove the object from the movingObjects table when the movement is complete
        if t >= 1 then
            movingObjects[objectName] = nil
        end
    end
end



-- Start moving an object
MoveAlongCurve("racing_desk", 0, 0, 0, 1, 2, 3, 4, 5, 6, 5)

-- In your game loop, update curve movements
deltaTime = 0.016 -- Example delta time (60 FPS)
while true do
    UpdateCurveMovements(deltaTime)
    -- Other game logic here...
end
