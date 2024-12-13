-- Quadratic Bézier function
function QuadraticBezier(t, p0, p1, p2)
    local u = 1 - t
    return u * u * p0 + 2 * u * t * p1 + t * t * p2
end

function MoveObj(objectName, startTransparency, empty1, empty2, endTransparency, empty3, empty4, duration, elapsedTime)
    print("[LUA] [ChangeTexture] Got:" .. objectName .. ", " .. startTransparency .. ", " .. empty1 .. ", " .. empty2 .. ', ' 
    .. endTransparency .. ', ' .. empty3 .. ', ' .. empty4 .. ', ' .. duration .. ', ' .. elapsedTime) 

    -- Ensure transparency and object ID are numbers
    startTransparency = tonumber(startTransparency) or 0.0
    endTransparency = tonumber(endTransparency) or 0.0

    -- Validate inputs
    if elapsedTime == nil or duration == 0 then
        print("[Lua] Error: Invalid elapsedTime or duration is zero.")
        return
    end

    -- Calculate the interpolation factor (t)
    local t = math.min(elapsedTime / duration, 1.0) -- Clamp t to [0, 1]

    -- Interpolate the transparency
    local currentTransparency = startTransparency + (endTransparency - startTransparency) * t

    -- Debug information
    print("[Lua] Adjusting transparency: " ..currentTransparency)

    -- Call C++ function to apply the calculated transparency
    DAD(objectName, currentTransparency)  -- Ensure ApplyTransparency is correct
end
