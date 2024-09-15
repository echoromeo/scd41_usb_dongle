git submodule update --inot
cd embedded-i2c-scd4x
git sparse-checkout set --no-cone '/*' '!sensirion_i2c_hal.c'