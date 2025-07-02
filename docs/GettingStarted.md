# Getting Started

This short guide helps you build and run Promethean Engine in release mode.

```bash
# Install dependencies (Ubuntu)
sudo apt install cmake build-essential libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev \
  libsdl2-mixer-dev libgl1-mesa-dev libglew-dev libglm-dev libtinyxml2-dev libspdlog-dev

# Configure and build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

Run the engine with the sample assets:

```bash
./build/bin/promethean --map=assets/sample_project/sample.tmx
```

The `/assets/sample_project` folder contains demo data you can experiment with.

To generate the API documentation run:

```bash
doxygen Doxyfile
```

HTML files will be created in `docs/api`.
