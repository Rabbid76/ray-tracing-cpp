# Notes

## TODO

- Refraction index range

## Core

- Light
- Texture filter
- Shapes

### Texture

- wood texture, add scene with wooden cube/sphere?
  https://shaderfrog.com/app/view/123 Procedural Wood

### Materials

- Dielectric with attenuation by distance in the volume
- Correct color mapping of dielectric with refraction dependent on the wavelength of the light

### Shapes

- [Inigo Quilez::articles::distance functions](https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm)
- [Raymarching - Primitives](https://www.shadertoy.com/view/Xds3zN)
- Matrix44
- Tetraeder, Hexaeder, Oktaeder, Dodekaeder, Ikosaeder 
- Sphere with thickness, use with (Ornament)
- Ray marching bodies
- AND, OR, MINUS
- 2 Sight, 3 sight body
- curvature of triangle projected on a sphere

## Utility

### Texture generators

- Voronoi generator
- Perlin noise generator

## Libraries that could possibly be used

### Boost

[boost C++ libraries](https://www.boost.org/)  
[Boost Library Documentation](https://www.boost.org/doc/libs/)  

#### Boost PropertyTree

[Index listing for boost/property_tree/](https://www.boost.org/doc/libs/1_79_0/boost/property_tree/)  
[Chapter 30. Boost.PropertyTree](https://www.boost.org/doc/libs/1_79_0/boost/property_tree/)  

> The Property Tree library provides a data structure that stores an arbitrarily deeply nested tree of values,
> indexed at each level by some key. Each node of the tree stores its own value,
> plus an ordered list of its subnodes and their keys. 
> The tree allows easy access to any of its nodes by means of a path, which is a concatenation of multiple keys.  
> In addition, the library provides parsers and generators for a number of data formats
> that can be represented by such a tree, including XML, INI, and JSON.

[GitHub - boostorg/property_tree](https://github.com/boostorg/property_tree)  
