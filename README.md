# CS488 Winter 2016 Project Demo

---
(Anti-aliasing is A4 extra feature)

## 1. Custom Primitive
A close ended cone and a double ended cylinder are created as extra primitives.
![Cone](./Project/Screenshot & Scripts/A_Cone.png)
![Cylinder](./Project/Screenshot & Scripts/A_Cylinder.png)
![Together](./Project/Screenshot & Scripts/Cone&Cylinder.png)

## 2. Reflection
Reflection is done by tracing a new reflected ray when primary ray hits an reflected surface.
![Reflection](./Project/Screenshot & Scripts/Reflection.png)

## 3. Refraction
Refraction is done by tracing a new refracted ray when primary ray pass through a glass.
![Reflection](./Project/Screenshot & Scripts/Refraction.png)

## 4. Path Tracing
Path tracing follows fresnel equation to properly select percentage of reflection and refraction.
A little reflection of side walls (red and blue) can be seen in the glass sphere.
![Together](./Project/Screenshot & Scripts/BeautifulReflection.png)

## 5. Soft Shadow
Soft shadow is implemented throught distributed ray tracing.
A spherical area light is implemented.
![SoftShadow](./Project/Screenshot & Scripts/SoftShadow.png)

## 6. Glossy Reflection
Glossy reflection can be identified on the top left glass sphere.
![GlossyReflection](./Project/Screenshot & Scripts/GlossyReflection.png)

## 7. Glossy Refraction
Glossy refraction is shown on the spherical glass.
![GlossyRefraction](./Project/Screenshot & Scripts/GlossyRefraction.png)

## 8. Perlin Noise
Perlin Noise is used to generate a marble like texture. Perlin noise is divided into octave. Octave with higher frequence has its magnitude decreased. 4 Octaves are summed up, then used as a phase shift to a sine function.
![PerlinNoise](./Project/Screenshot & Scripts/PerlinNoise.png)

## 9. Animation
![Animation](./Project/Screenshot & Scripts/animation.gif)

## 10. Final scene
![FinalScene1](./Project/Screenshot & Scripts/finalScene.png)
![FinalScene2](./Project/Screenshot & Scripts/coolimage.png)
![FinalScene3](./Project/Screenshot & Scripts/room.png)


## Extra
1. Texture Mapping & Transparent Cone
![TextureMapping](./Project/Screenshot & Scripts/TextureMapping.png)

2. Multiprocessing is enabled to help with the speed of processing.