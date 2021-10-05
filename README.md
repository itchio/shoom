# shoom

shoom is a small C++11 library for creating & opening shared memory on Windows, Linux and macOS.

# Build

```sh
cmake -B build -DSHOOM_BUILD_TESTS=1
cmake --build build
```

# Test

```sh
./build/test/shoom_test && echo "OK" || echo "FAIL"
```

## License

shoom is released under the MIT license, see the `LICENSE` file.

