# particles

Implements particle demos

![avatar](/arts/fireworks.png)

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
particles $ mkdir build
particles $ cd build
particles/build $ cmake ..
particles/build $ start particles.sln
```

Your solution file should contain four projects.
To run from the git bash command shell, 

```
particles/build $ ../bin/Debug/billboard-axis.exe
particles/build $ ../bin/Debug/explosion.exe
particles/build $ ../bin/Debug/sparkle-trail.exe
particles/build $ ../bin/Debug/sparkles.exe
```

*macOS*

Open terminal to the directory containing this repository.

```
particles $ mkdir build
particles $ cd build
particles/build $ cmake ..
particles/build $ make
```

To run each program from build, you would type

```
particles/build $ ../bin/billboard-axis
particles/build $ ../bin/sparkles
particles/build $ ../bin/sparkle-trail
particles/build $ ../bin/explosion
```

## Demo of basic features
```
a sparkle trailing effect
```
![avatar](/arts/sparkle-trail.gif)
```
implement an explosion effect based on a sprite sheet
```
![avatar](/arts/explosion.gif)
```
implement an axis billboard that rotates around the Y axis
```
![avatar](/arts/billboard-axis.gif)

## Unique features 

```
Fireworks
```
![avatar](/arts/fireworks.gif)

