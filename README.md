# Custom Memory Allocation Library

## Description

This project re-implements the standard `malloc`, `calloc`, `realloc`
and `free` routines. The implementation lives in `src/nu_malloc.c`
with its public header in `include/nu_malloc.h`.

### Project layout

- `src/` ‑ library and example sources
- `include/` ‑ public headers
- `tests/` ‑ unit tests and helper scripts

## Usage

To use the custom memory functions, include `nu_malloc.h` in your program.
The header is C++ friendly and wraps the declarations in `extern "C"`
when included from C++ code. Call the functions just like their standard
library counterparts. The
function prototypes are as follows:
```c

        void* nu_malloc(size_t);
        void* nu_calloc(size_t, size_t);
        void* nu_realloc(void*, size_t);
        void nu_free(void*);
```
## Example
```c

        #include "nu_malloc.h"

        int main() {
        int* ptr = (int*) nu_malloc(sizeof(int));
        *ptr = 5;
        printf("Value at ptr: %d\n", *ptr);
        nu_free(ptr);
        return 0;
        }
```
## Building

A simple `Makefile` is included. The project uses `mmap` and `munmap` on
POSIX systems. When `_POSIX_VERSION` is undefined the code falls back to
the standard `malloc` family. Both a static library (`libnu_malloc.a`)
and a shared library (`libnu_malloc.so`) are produced.

### POSIX systems

```
make
```

This compiles the example program, the test binary and the
`libnu_malloc.a` library into the `build/` directory. Run `make test` to
execute the test suite.

### Non‑POSIX systems

The same command works because the source automatically switches to
`malloc`/`realloc`/`free`. It does so when `mmap` is unavailable.

```
make
```

To compile manually without `make` on any platform:

```
gcc -std=c11 src/example.c src/nu_malloc.c -Iinclude -o example
```

To build just the static library:

```
make libnu_malloc.a
```

To build just the shared library:

```
make libnu_malloc.so
```

You can install the header and libraries with:

```
make install
```

The install location can be customized with `PREFIX`, `INCLUDEDIR` and
`LIBDIR` variables. The defaults install into `/usr/local`.
### Using CMake

```sh
cmake -B build
cmake --build build
```

This will place `libnu_malloc.a`, `libnu_malloc.so`, `example` and `memory_test` inside the `build/` directory. To run the tests use:

```sh
cd build && ctest
```


You can remove the installed files with:

```
make uninstall
```

### Windows (MinGW/MSVC)

```sh
gcc src/example.c src/nu_malloc.c -Iinclude -o example.exe
```

## Documentation

A minimal `Doxyfile` is provided. Run `doxygen` from the project root
to generate HTML documentation in `docs/html`. The configuration scans
the `include/` and `src/` directories.

### Testing

You can either run `make test` or invoke the helper script directly:

```
./tests/test_example.sh
```

The script expects the program to print `Value: 42` and requires a POSIX
shell. It isn't supported on Windows unless you run it inside such an
environment.

## Note
This sample code is not recommended for production use. Test it
thoroughly before deploying.

## Compatibility
The library is written in standard C and uses `mmap`/`munmap` from
`<sys/mman.h>` when available. When `mmap` is unavailable the
implementation falls back to the standard memory functions. This allows
the library to work on non‑POSIX platforms.

## Contributing

If you want to contribute to this project, feel free to open a pull
request or issue on GitHub.

## License

This project is licensed under the [MIT License](LICENSE).
