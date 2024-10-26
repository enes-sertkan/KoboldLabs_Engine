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
