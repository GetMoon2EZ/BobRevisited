# Bob revisited

A revision of the Bob project. This University project was supposed to be done in a group of 6 students the full requirements are described in the [assignment.pdf](./assignment.pdf) file. The end-result of the original project can be found in the [BobDylan repository](https://github.com/GetMoon2EZ/BobDylan).

## How to build

To build, run the following commands from the project's root directory:

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Modifications

As I personally don't feel like coding this in Python, I will amend the instructions and use only CPP. I would also like to learn about graphics and design an engine that would run this "Game" using Vulkan. Having no prior knowledge in this field, it should be a good learning experience. Being able to compare the before and after of those two projects should also be interesting.

## Development plan

This is the current state of development. Note that some items might be amended in the future.

* Logic
    * [ ] Project architecture
    * [ ] Implementation of simple logic
    * [ ] Addition of the different features
* Rendering
    * [ ] Output to console
    * [ ] 2D rendering using Vulkan
    * [ ] Isometric view
    * [ ] 3D rendering
* Graphical Used Interface
    * [ ] No GUI (hardcoded value)
    * [ ] Console GUI
    * [ ] Proper GUI using Vulkan ?