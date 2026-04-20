# File Management System (C + JavaFX)

This project implements a simple file management system with:

- **Backend (Linux, C language):** core file-system-like operations.
- **Frontend (Java + JavaFX):** graphical interface that calls the C backend.

## Features

- Create file
- Open file
- Close file
- Search file

## Project Structure

- `cfs/` - C implementation for file operations over `./virtual_disk`
- `javafx-ui/` - JavaFX interface
- `report/PROJECT_REPORT.md` - written portion and analysis

## 1) Build and run C backend (Linux)

```bash
cd cfs
gcc -Wall -Wextra -o fms main.c file_system.c
./fms create demo.txt
./fms open demo.txt
./fms close demo.txt
./fms search demo.txt
```

## 2) Run JavaFX UI

Compile and run in `javafx-ui` with your installed JavaFX SDK.

Example (update `PATH_TO_FX`):

```bash
cd javafx-ui
javac --module-path PATH_TO_FX/lib --add-modules javafx.controls src/main/java/com/fms/*.java
java --module-path PATH_TO_FX/lib --add-modules javafx.controls -cp src/main/java com.fms.MainApp
```

From the UI, enter a file name and click:

- `Create`
- `Open`
- `Close`
- `Search`

The output area shows responses from the C backend.
