# File Management System (Linux C Only)

This project implements a simple file management system in a Linux environment using only the C language.

## Operations

- **mkdir** — create subdirectories under `virtual_disk` (nested paths supported)
- **create** — create a new empty file (fails if it already exists); parent directories are created automatically
- **open** / **close** — track open files in a persistent open-file table
- **read** — print file contents to stdout (file must be open first)
- **write** — overwrite file contents (file must be open first); optional text after the path is stored as the file body
- **search** — check whether a regular file exists at the given path
- **delete** — remove a file (and drop it from the open table if it was open)

Paths are relative to `virtual_disk`, for example `docs/notes.txt`. `..` and absolute paths are rejected.

## Project Structure

- `cfs/` - C implementation for file operations over `./virtual_disk`
- `report/PROJECT_REPORT.md` - formal written report

## Build and Run (Linux)

```bash
cd cfs
gcc -Wall -Wextra -o fms main.c file_system.c

./fms mkdir subdir
./fms create subdir/demo.txt
./fms open subdir/demo.txt
./fms write subdir/demo.txt Hello, this is a test.
./fms read subdir/demo.txt
./fms search subdir/demo.txt
./fms close subdir/demo.txt
./fms delete subdir/demo.txt
```

## Notes

- This repository is intentionally C-only for implementation code.
- The report includes research comparisons and project documentation required by the assignment.
