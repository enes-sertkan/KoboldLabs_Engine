-- Helper function for quadratic Bézier curve interpolation
function QuadraticBezier(t, p0, p1, p2)
    local u = 1 - t
    return u * u * p0 + 2 * u * t * p1 + t * t * p2
end

-- Helper function for linear interpolation (for waypoints)
function Lerp(start, stop, t)
    return start + (stop - start) * t
end

-- Function to move an object along a quadratic Bézier curve
function MoveAlongCurve(objectName, p0x, p0y, p0z, p1x, p1y, p1z, p2x, p2y, p2z, duration, time)
    local t = time / duration
    if t < 0 then t = 0 elseif t > 1 then t = 1 end -- Clamp t between 0 and 1

    -- Calculate interpolated position on the curve
    local x = QuadraticBezier(t, p0x, p1x, p2x)
    local y = QuadraticBezier(t, p0y, p1y, p2y)
    local z = QuadraticBezier(t, p0z, p1z, p2z)

    print("[LUA] New position for " .. objectName .. " is " .. x .. ", " .. y .. ", " .. z)

    -- Move the object using the calculated position
    MoveTo(objectName, x, y, z)
end

-- Function to move an object through a series of waypoints
function FollowWaypoints(objectName, waypoints, duration, time)
    local numPoints = #waypoints
    if numPoints < 2 then
        print("[LUA] Not enough waypoints!")
        return
    end

    -- Calculate segment duration
    local segmentDuration = duration / (numPoints - 1)
    local segmentIndex = math.floor(time / segmentDuration) + 1

    if segmentIndex >= numPoints then
        -- End at the last waypoint
        local last = waypoints[numPoints]
        MoveTo(objectName, last.x, last.y, last.z)
        return
    end

    -- Get current and next waypoints
    local t = (time % segmentDuration) / segmentDuration
    local current = waypoints[segmentIndex]
    local next = waypoints[segmentIndex + 1]

    -- Interpolate between waypoints
    local x = Lerp(current.x, next.x, t)
    local y = Lerp(current.y, next.y, t)
    local z = Lerp(current.z, next.z, t)

    -- Move the object to the calculated position
    MoveTo(objectName, x, y, z)
end
