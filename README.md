# pumpekontroller
Kontroller til pumpe.

## Build Instructions 

Navigate to the `sw`-folder then run:

```bash
$ mkdir build
$ cd build

# If Pico2:
$ cmake -DPICO_BOARD=pico2 -DPICO_PLATFORM=rp2350 ..
# Else:
$ cmake ..

$ make

```

From then on, just run `make` from the build folder.
You can specify more cores to speed up (eg. `make -j16` for building with 16 cores).
