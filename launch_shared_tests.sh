#!/bin/bash
export LD_LIBRARY_PATH=lib/shared
./bin/tests/shared/test_log
./bin/tests/shared/test_window
./bin/tests/shared/test_shapes
rm test_result.png
rm test_result_05.png
./bin/tests/shared/test_image
./bin/tests/shared/test_keyboard
./bin/tests/shared/test_mouse

