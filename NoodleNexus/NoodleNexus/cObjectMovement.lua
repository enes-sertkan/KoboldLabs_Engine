function MoveCar(x, y, z, speed)
    x = x+ speed -- Example of circular motion
--    z += math.cos(speed) * 50
--    z += math.cos(speed) * 50
    MoveObject("racing_desk", x, y, z)

end
