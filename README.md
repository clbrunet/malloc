# 42_malloc
Implementation of malloc, realloc and free functions using mmap

## Usage
- Include the libft_malloc.h file in the include folder
```shell
make
cc FLAGS -I ./PATH_TO_42_malloc/include -L ./PATH_TO_42_malloc -o EXECUTABLE FILES -lft_malloc
./PATH_TO_42_malloc/setup.sh ./EXECUTABLE
```

## Features
- malloc, realloc, free, show_alloc_mem, show_alloc_mem_ex, show_alloc_stats functions
- Functions are thread safe
- Defragmentation of the freed memory
- debug environment variables : MALLOC_PERTURB_, MALLOC_FAIL_AT_, MALLOC_MAX_BYTES_, MALLOC_ENABLE_HISTORY_

### Debug environment variable example
To make the 42nd call to my allocation functions fail :
```shell
make ENABLE_DEBUG_VARIABLES=1 re
export MALLOC_FAIL_AT_="42"
./PATH_TO_42_malloc/setup.sh ./EXECUTABLE
```
