
function QuadraticBezier(t, p0, p1, p2)
    local u = 1 - t
    return u * u * p0 + 2 * u * t * p1 + t * t * p2
end

function EaseInOut(t)
    -- Quadratic ease-in-out function
    if t < 0.5 then
        return 2 * t * t
    else
        return -1 + (4 - 2 * t) * t
    end
end

function MoveObj(objectName, p0x, p0y, p0z, p2x, p2y, p2z, duration, elapsedTime, p1x, p1y, p1z)
    -- Validate elapsedTime and avoid division by zero
    if elapsedTime == nil or duration == 0 then
        print("[LUA] Error: Invalid elapsedTime or duration is zero.")
        return
    end

    print("[LUA] [MoveAlongCurve] Got:" .. objectName .. ", " .. p0x .. ", " .. p0y .. ", " .. p0z .. ", " .. p2x .. ", " .. p2y .. ", " .. p2z .. ", " .. p1x .. ", " .. p1y .. ", " .. p1z)

    -- Calculate t as the fraction of elapsed time over the total duration
    local t = math.min(elapsedTime / duration, 1.0) -- Clamp t to the range [0, 1]

    -- Apply easing function
    t = EaseInOut(t)

    -- Interpolate position using the Quadratic Bézier curve formula
    local x = QuadraticBezier(t, p0x, p1x, p2x)
    local y = QuadraticBezier(t, p0y, p1y, p2y)
    local z = QuadraticBezier(t, p0z, p1z, p2z)

    -- Debug the new position
    print(string.format("[LUA] New position for %s is (%.2f, %.2f, %.2f)", objectName, x, y, z))

    -- Move the object to the calculated position
    MoveTo(objectName, x, y, z)
end