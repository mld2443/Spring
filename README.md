# Spring
A springy mesh simulation, including wind simulation and rudimentary self-intersection. I wrote this in the fall of 2015 for a class, and have not updated it since. As such, it is very rough, has poor coding conventions, few comments, and perhaps bugs.

<img width="400" alt="screen shot 2017-10-17 at 10 25 43 am" src="https://user-images.githubusercontent.com/5340992/31673834-d2da0806-b325-11e7-9e15-0bde35414a6e.png"> <img width="400" alt="screen shot 2017-10-17 at 10 24 52 am" src="https://user-images.githubusercontent.com/5340992/31673772-b5b5a2bc-b325-11e7-9622-41eaa093e48f.png">

## About the simulation

The simulation is composed of several simple springy meshes. A mesh consists of weights with springy edges between them. The springs behave according to [Hooke's law](https://en.wikipedia.org/wiki/Hooke%27s_law) and that's how they are defined, in terms of a stiffness with a dampening factor. All springs are initialized and stored in a [state vector](https://en.wikipedia.org/wiki/State_variable), which is integrated at each refresh. It is integrated using [RK4](https://en.wikipedia.org/wiki/Runge–Kutta_methods) to prevent oscillation build-up and explosion. I use elapsed time rather than a fixed update rate, meaning the simulation is independant of framerate.

### Scenes

I have included several different scenarios, involving shapes, collisions, intersections, wind and other things. This is unfortunately hardcoded, so update the value on line `main.cpp:77` for the different scenes. 

## Clang and G++

The syntax I use compiles using clang, however I have found issues when compiling with g++. For an alternative, please see [this springy mesh simulation](https://github.com/mld2443/RealtimeSubdivisionSurfaces) I adapted to work in a Windows environment.

In adapting the simulation to work for my subdivision surfaces program, I found precisely what causes g++ to fail compilation where clang succeeds. Specifically, I use [aggregate initialization](http://en.cppreference.com/w/cpp/language/aggregate_initialization) hardcoded nearly everywhere in this project. When doing so, to initialize `float` types, g++ requires the literal be of a `float` type as well, (e.g. `0.0` vs `0.0f`). It appears clang converts the literals upon compilation or otherwise does not complain. I do this everywhere in the project, *this (along with many other qualities about this specific project) is bad coding practice.* I wrote this a few years ago, I would never do this again.
