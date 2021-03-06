# UnrealProceduralApartmentTest

This is a simple test project investigating the new Unreal Engine 5. I wanted to see the available tools within the engine that I could use to make a procedural apartment.
The design is very straightforward and partly dirty.

## JSON Structure

All the mesh data is taken from the JSON, you can see it [here](./Content/StarterContent/Config/building_test.json). The file has information about rooms which are created as parallelepipeds. So there is an array or those parallelepiped-blocks with shape coordinates described in `Transform`. You need only two points to create a block. The code creates a wall using coordinates and then removes parallelepipeds from `Subtract` using boolean operations. The next operation is to place the mesh in the newly created area. `Type 1` is for Window mesh, and `2` is for Doors. Note, `0` is unused but describes walls. Windows are stretched (scaled) to fill the area, but doors aren't.

```
# Array of Walls
[
 # First Wall
 {
 # Wall Transform Coordinates
 "Transform" : {
 # ' "Type" : 0 ', is for Wall Mesh (made for consistency, but unused)
 "Type" : 0,
 "Rot" : 0,
 
 "X" : 100,
 "Y" : 5,
 "Z" : 50,

 "XOff" : 0,
 "YOff" : 0,
 "ZOff" : 0
 },

 # Each Room has other shapes that are subtracted using boolean operations.
 # These shapes are also parallelepipeds and require two points
 "Subtract" : [
 {
 "Transform" : {
 # ' "Type" : 2 ', is for Door Mesh
 "Type" : 2,
 "Rot" : 90,
 
 "X" : 30,
 "Y" : 0,
 "Z" : 0,

 "XOff" : 50,
 "YOff" : 5,
 "ZOff" : 40
 }
 },
 {
 "Transform" : {
 # ' "Type" : 1 ', is for Window Mesh
 "Type" : 1,
 "Rot" : 90,
 
 "X" : 65,
 "Y" : 0,
 "Z" : 20,

 "XOff" : 85,
 "YOff" : 5,
 "ZOff" : 40
 }
 }
 ]
 },

 # Other Walls
...
```