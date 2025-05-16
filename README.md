# 3D Scene Editor Controls

This scene editor provides a keyboard-controlled interface to adjust the **Position**, **Rotation**, and **Scale** of objects in a 3D scene. Toggle between different transformation modes, use the arrow keys for adjustments, and save your scene state.

## Controls Overview

### Mode Selection

- **P**: Select the next object in the scene.
- **U**: Switch to **Position** mode.
- **I**: Switch to **Rotation** mode.
- **O**: Switch to **Scale** mode (only affects x-axis).

### Transformation Controls

The function of the arrow keys changes based on the selected mode.

#### Position Mode (Press **U**)

Use the arrow keys to move the object in the 3D space.

- **↑ (Up Arrow)**: Move forward along **z-axis**.
- **↓ (Down Arrow)**: Move backward along **z-axis**.
- **→ (Right Arrow)**: Move right along **x-axis**.
- **← (Left Arrow)**: Move left along **x-axis**.
- **Control + ↑ (Up Arrow)**: Move upward along **y-axis**.
- **Control + ↓ (Down Arrow)**: Move downward along **y-axis**.

#### Rotation Mode (Press **I**)

Use the arrow keys to rotate the object.

- **↑ (Up Arrow)**: Rotate upward around **x-axis**.
- **↓ (Down Arrow)**: Rotate downward around **x-axis**.
- **→ (Right Arrow)**: Rotate clockwise around **z-axis**.
- **← (Left Arrow)**: Rotate counterclockwise around **z-axis**.
- **Control + ↑ (Up Arrow)**: Rotate clockwise around **y-axis**.
- **Control + ↓ (Down Arrow)**: Rotate counterclockwise around **y-axis**.

#### Scale Mode (Press **O**)

Use the arrow keys to scale the object **only on the x-axis**.

- **→ (Right Arrow)**: Increase x-axis scale.
- **← (Left Arrow)**: Decrease x-axis scale.

## Saving the Scene

To save the scene, use the `SaveScene(std::string name)` function. This will store the current states of each object in the scene, including:

- **Model File Path**
- **Position**
- **Rotation**
- **Scale**

The scene state will be saved in an XML-like format, making it easy to reload objects with their latest transformations.

---

### Example Save File Structure

An example of how scene information might appear in the save file:

```xml
<Scene>
    <Object>
        <ModelFilePath> assets/models/tree.ply </ModelFilePath>
        <Position x="0.0" y="5.0" z="10.0" />
        <Rotation x="0.0" y="45.0" z="0.0" />
        <Scale x="1.0" y="1.0" z="1.0" />
    </Object>
    <!-- Additional objects here -->
</Scene>
```

# Light Editing in Scene Editor

This document provides instructions on how to use the light editing functionality within the scene editor. The controls allow users to modify various properties of the lights in the scene dynamically.

## Table of Contents

- [Overview](#overview)
- [Controls](#controls)
- [Light Properties](#light-properties)
- [Tips and Tricks](#tips-and-tricks)

## Overview

In the scene editor, users can edit different light properties such as position, diffuse color, specular color, attenuation, direction, and other parameters. The editing mode can be toggled and adjusted using keyboard shortcuts.

## Controls

1. **Toggle Light Editing Mode:**

   - Press **`P`** to pick the next light in the scene.

2. **Select Light Property to Edit:**

   - Press **`Z`** to edit **Position**.
   - Press **`X`** to edit **Diffuse Color**.
   - Press **`C`** to edit **Specular Color**.
   - Press **`V`** to edit **Attenuation**.
   - Press **`B`** to edit **Param1** (Inner and Outer angles).
   - Press **`N`** to edit **Param2** (Toggle on/off).

3. **Edit Light Properties:**

   - **Position**: Use the arrow keys to move the light.

     - Up/Down: Move along the Z-axis.
     - Left/Right: Move along the X-axis.
     - Hold **Control** to move along the Y-axis.

   - **Diffuse Color**: Adjust the Y and Z channels using the Up/Down arrow keys.

     - Hold **Control** to adjust the Z channel (color component).

   - **Specular Color**: Adjust the power (W channel) using Up/Down keys.

     - Use Left/Right to adjust the X channel (color component) while holding **Control**.

   - **Attenuation**:

     - Hold **Alt** while pressing:
       - **UP**: Decrease linear attenuation (Y).
       - **DOWN**: Increase linear attenuation (Y).
       - **RIGHT**: Decrease quadratic attenuation (Z).
       - **LEFT**: Increase quadratic attenuation (Z).

   - **Param1**:

     - Adjust the inner angle (Y) using Up/Down keys.
     - Use Left/Right to adjust the outer angle (Z) while holding **Control**.

   - **Param2**:
     - Up arrow key to turn on the light.
     - Down arrow key to turn off the light.

## Light Properties

- **Position**: The position of the light in 3D space defined by `glm::vec4 position`.
- **Diffuse Color**: The diffuse color of the light, which affects the color seen on surfaces, defined by `glm::vec4 diffuse`.
- **Specular Color**: The color and power of highlights on surfaces defined by `glm::vec4 specular`.
- **Attenuation**: Controls how the light intensity diminishes over distance, defined by `glm::vec4 atten`.
  - X: Constant attenuation
  - Y: Linear attenuation
  - Z: Quadratic attenuation
- **Direction**: The direction of the light for spot and directional lights, defined by `glm::vec4 direction`.
- **Param1**: Contains parameters such as light type and angles for spotlights, defined by `glm::vec4 param1`.
- **Param2**: Used for toggling the light on or off, defined by `glm::vec4 param2`.

## Tips and Tricks

- Ensure the selected light is visible in the scene to see the effects of your changes.
- Use control and shift modifiers for more precise adjustments.
- Experiment with different combinations of adjustments to get the desired lighting effect in your scene.

## Conclusion

This functionality allows for a rich and interactive light editing experience in your scene editor, enabling fine-tuning of light properties on the fly. Happy editing!
