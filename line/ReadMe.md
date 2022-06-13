# Build Line Rasterizer

Compile the `LineRasterizer.cpp`, `draw_line.cpp`, and `Image.cpp` files.

Something like : 

```sh
g++ -Wall -Wextra -pedantic -O2 -std=c++17 LineRasterizer.cpp draw_line.cpp Image.cpp -o LineRasterizer
```

# Build Test Driver

Compile the `TestDriver.cpp` and `draw_line.cpp` files.

Something like : 

```sh
g++ -Wall -Wextra -pedantic -O2 -std=c++17 TestDriver.cpp draw_line.cpp -o TestDriver
```