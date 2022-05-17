# Runtime Class Selection
Experiments for selecting a C++ class by it's name, at runtime. Usefull for game engine scene serialization, as it could choose wich C++ script class to instantiate for an entity.

---

### Libraries used
>EnTT | An entity component system (ECS) library  
>https://github.com/skypjack/ent

---
### Building
Requires CMake and a C++ 17 compatible compiler.  
All dependencies are managed by git submodules, so use `--recursive` for cloning the repo.  

### Run the command:
> `cmake -S . -B build && cmake --build build`  

Your executable will be at ./build/RuntimeClassSelection

---
### Usage
Create a *class* or *struct* that inherits from the *Behavior* base class.  
Inside main(), use the REGCLASS(ClassName) macro function to register your class. This must happens before using the `AddClassComponentFunction` from *RegisteredClasses* map.

Then run the program passing the name of the class as the first argument.
>./build/RuntimeClassSelection.exe Camera