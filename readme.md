

Better safe than sorry, remove the sensirion_i2c_hal.c from the submodule. To avoid modifying the repo, do a sparse checkout:
```cmd
git submodule update --init
cd embedded-i2c-scd4x
git sparse-checkout set --no-cone '/*' '!sensirion_i2c_hal.c'
```