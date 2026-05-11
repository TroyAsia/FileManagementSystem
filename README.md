# File Management System (Linux C Only)

This project implements a simple file management system in a Linux environment using only the C language.

## Required Operations

- Create file
- Open file
- Close file
- Search file

## Project Structure

- `cfs/` - C implementation for file operations over `./virtual_disk`
- `report/PROJECT_REPORT.md` - formal written report

## Build and Run (Linux)

```bash
cd cfs
gcc -Wall -Wextra -o fms main.c file_system.c
./fms create demo.txt
./fms open demo.txt
./fms close demo.txt
./fms search demo.txt
```

## Notes

- This repository is intentionally C-only for implementation code.
- The report includes research comparisons and project documentation required by the assignment.
