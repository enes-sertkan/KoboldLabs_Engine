function LerpVec3(startX, endPosX, t)
    -- Ensure t is clamped between 0 and 1
    if t < 0 then t = 0 elseif t > 1 then t = 1 end

    t = EaseInOut(t)

    -- Calculate interpolated vector
    local x = startX + (endPosX - startX) * t

    return x
end

function MoveObj(objectName, startBlend, emp1, emp2, endBlend, emp3, emp4, duration, time, textureID, emp5, emp6, textureName)
    print("[LUA] [ChangeTexture] Got:" .. objectName .. ", " .. startBlend .. ", " .. emp1 .. ", " .. emp2 .. ', ' .. endBlend .. ', ' .. emp3 .. ', ' .. emp4 .. ', ' .. duration .. ', ' .. time .. ', ' .. textureID .. ', ' .. emp5 .. ', ' .. emp6 .. ', ' .. textureName)
    
    -- Ensure 'seconds' is defined and not a string
    local seconds = duration -- Replace or initialize appropriately
    local t = time / seconds

    -- Interpolate with the specified easing function
    local blend = LerpVec3(startBlend, endBlend, t)

    -- Apply the movement to the object
    SetTexture(objectName, textureName, textureID, blend)
end

function EaseInOut(t)
    -- Quadratic ease-in-out function
    if t < 0.5 then
        return 2 * t * t
    else
        return -1 + (4 - 2 * t) * t
    end
end
