function MoveCar(x, y, z, speed)
    x = x+ speed -- Example of circular motion
    y = y+ speed
    z = z+ speed
    MoveObject("racing_desk", x, y, z)

end

function RotateCar(x, y, z, rotationSpeed)
    x = x+ rotationSpeed -- Example of circular motion
    y = x+ rotationSpeed
    z = x+ rotationSpeed

    RotateObject("racing_desk", x, y, z)


end

function MovObj(objectName, startX, startY, startZ, endPosX, endPosY, endPosZ, seconds, deltaTime)
    -- Call the C++ function
    local x, y, z = MoveTo("racing_desk", 
                           startX, startY, startZ, 
                           endPosX, endPosY, endPosZ, 
                           seconds, deltaTime)

    print("New Position: ", x, y, z)
end




