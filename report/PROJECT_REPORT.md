# Written Report: File Management System

## 1. Introduction

A file management system organizes files and directories, controls file access, and provides operations like create, open, close, and search. It is a core service in operating systems that enables users and applications to store and retrieve data safely and efficiently.

This project implements a small educational file management system. The implementation targets Linux and uses the C language for the file system logic, while JavaFX provides a user-friendly graphical interface.

## 2. Examples of Modern File Management Systems

Modern operating systems use advanced file systems, including:

- **ext4 (Linux):** journaling file system with strong reliability and good performance.
- **XFS (Linux):** high-performance journaling file system, often used for large files and servers.
- **NTFS (Windows):** supports permissions, journaling, encryption, and large volume sizes.
- **APFS (macOS/iOS):** optimized for SSDs, supports snapshots and strong encryption.

These systems include sophisticated features such as metadata journaling, caching, crash recovery, permission models, and high scalability.

## 3. Description of Implemented System

### 3.1 Implementation Constraints

- Language for file system core: **C**
- Environment: **Linux**
- User interface: **Java with JavaFX**

### 3.2 Design Overview

The project contains two layers:

1. **C backend (`cfs/`)**
   - Handles file operations in a dedicated folder called `virtual_disk`.
   - Supports:
     - create file
     - open file
     - close file
     - search file
   - Maintains a small in-memory open-file table (`MAX_OPEN_FILES`) to simulate file state tracking.

2. **JavaFX frontend (`javafx-ui/`)**
   - Provides buttons for each operation.
   - Uses `ProcessBuilder` to execute the C backend command-line program.
   - Displays command responses in a text output area.

### 3.3 Operation Flow

- **Create:** creates a file in `virtual_disk`.
- **Open:** checks existence and marks file as open in the open table.
- **Close:** removes file from open table.
- **Search:** checks whether file exists in `virtual_disk`.

### 3.4 Limitations and Future Improvements

Current implementation is intentionally simple for educational goals. Potential improvements:

- hierarchical directories
- file metadata display (size/time/owner)
- persistent open-state tracking
- delete/rename/move operations
- better validation and error mapping in UI

## 4. Conclusion

This project demonstrates a practical and clear file management system design using a Linux C backend and JavaFX frontend. It satisfies required operations (create, open, close, search) and provides both implementation and written documentation consistent with the project topic.
