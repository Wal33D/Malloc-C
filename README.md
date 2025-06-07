# Custom Memory Allocation Library
### Title: Custom Memory Allocation Library
### Files: nu_malloc.c, nu_malloc.h

## Description

This is a custom implementation of the C standard library memory allocation functions
`malloc`, `calloc`, `realloc`, and `free` in files `nu_malloc.c` and `nu_malloc.h`.

## Usage

To use the custom memory allocation functions, include the `nu_malloc.h` header file
in your program and use the functions like you would use the standard library functions.
The function prototypes are as follows:
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

A simple `Makefile` is included. The project uses `mmap` and `munmap` on POSIX
systems. When `_POSIX_VERSION` is not defined (for example on Windows), the code
falls back to the standard `malloc` family.

### POSIX systems

```
make
```

This compiles `example`, `memory_test`, and `libnu_malloc.a`. Run `make test` to execute the test suite.

### Non‑POSIX systems

The same command works because the source automatically switches to
`malloc`/`realloc`/`free` when `mmap` is unavailable.

```
make
```

To compile manually without `make` on any platform:

```
gcc -std=c11 example.c nu_malloc.c -o example
```

To build just the static library:

```
make libnu_malloc.a
```

### Testing

You can either run `make test` or invoke the helper script directly:

```
./test_example.sh
```

The script expects the program to print `Value: 42`.

## Note
This is a sample code and is not recommended for production use. It is recommended to test it thoroughly before using it in production.

## Compatibility
The library is written in standard C. On POSIX systems it depends on
`mmap`/`munmap` from `<sys/mman.h>`. When those calls are unavailable the
implementation falls back to the standard memory functions, so it can also be
used on non‑POSIX platforms.

## Contributing

If you want to contribute to this project, please feel free to open a pull request or issue on GitHub.

## License

This project is licensed under the [MIT License](LICENSE).
