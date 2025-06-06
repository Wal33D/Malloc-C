# **Custom Memory Allocation Library**
###### **Title:** Custom Memory Allocation Library
###### **Files:** nu_malloc.c, nu_malloc.h

## **Description**

		This is a custom implementation of the C standard library memory allocation functions
		 `malloc`, `calloc`, `realloc`, and `free` in files `nu_malloc.c` and `nu_malloc.h` .

## **Usage**

	To use the custom memory allocation functions, include the `nu_malloc.h` header file 
	in your program and use the functions like you would use the standard library functions.
	The function prototypes are as follows:
```c

	void* nu_malloc(size_t);
	void* nu_calloc(size_t, size_t);
	void* nu_realloc(void*, size_t);
	void nu_free(void*);
```
## **Example**
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
## **Building**

A simple `Makefile` is included for compiling the provided `example.c` program against `nu_malloc.c`.

```
make
./example
```

To compile manually without `make`:

```
gcc -std=c11 example.c nu_malloc.c -o example
```

## **Note**
This is a sample code and is not recommended for production use. It is recommended to test it thoroughly before using it in production.

## **Compatibility**
The code is compatible with C programming language and should work with most systems that support the `mmap` system call.

## **Contributing**

If you want to contribute to this project, please feel free to open a pull request or issue on GitHub.

## **License**

This project is licensed under the [MIT License](LICENSE).



