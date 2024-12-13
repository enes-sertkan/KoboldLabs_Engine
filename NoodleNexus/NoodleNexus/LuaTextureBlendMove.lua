
function LerpVec3(startX, endPosX, t)
    -- Ensure t is clamped between 0 and 1
    if t < 0 then t = 0 elseif t > 1 then t = 1 end

    t = EaseInOut(t);

    -- Calculate interpolated vector
      local  x = startX + (endPosX - startX) * t,
  
    
    return x
end

function ChangeTexture(objectName, textureID, textureFillType, textureName, startBlend, endBlend,  duration, time)
    print("[LUA] [ChangeTexture] Got:" .. objectName .. ", " .. texutureID .. ", " .. textureFillType .. ", " .. textureName .. ', ' .. startBlend .. ', ' .. endBlend .. ', ' .. duration.. ', '..time)
    
    local t = time / seconds
    -- Interpolate with the specified easing function
    local blend = LerpVec3(startBlend, endBlend, t)

    -- Apply the movement to the object
    SetTexture(objectName, textureName, textureID, blend ,textureFillType)
end


int lua_SetTexture(lua_State* L) {
    // Retrieve arguments from the Lua stack
    const char* objectName = luaL_checkstring(L, 1);  // Object name
    const char* textureName = luaL_checkstring(L, 2);  // Texture name
    int textureId = luaL_checkinteger(L, 3);  // Texture ID
    float blendAmount = luaL_checknumber(L, 4);  // Blend amount (float)
    int fillType = luaL_checknumber(L, 5);  // Blend amount (float)


function EaseInOut(t)
    -- Quadratic ease-in-out function
    if t < 0.5 then
        return 2 * t * t
    else
        return -1 + (4 - 2 * t) * t
    end
end

