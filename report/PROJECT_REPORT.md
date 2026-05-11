# Formal Report: File Management System (Linux, C)

## Abstract

This group project implements a simple file management system in a Linux environment using only the C language. The system supports core operations required in the assignment: create files, open files, close files, and search for files. In addition to implementation, this report presents research on modern file systems (macOS, Windows/PC, Linux, and flash drives), compares their characteristics, and discusses what happens when files are copied across different file systems.

## Introduction

A file management system is responsible for storing, organizing, and retrieving files while tracking metadata and access state. It is a fundamental part of every operating system.  

This project is completed as a group project (group size: 3 or fewer members) and is graded as homework-equivalent work (worth five homework assignments). The project topic is a file management system, and implementation is performed under Linux with C language only, as required.

## Body

### 1) Implemented File Management System

The implemented system is located in `cfs/` and runs as a command-line program. It uses a folder named `virtual_disk` to simulate a storage area. The system supports:

- `create <filename>`: create a new file
- `open <filename>`: open an existing file and track it in an open-file table
- `close <filename>`: close a previously opened file
- `search <filename>`: verify whether a file exists

The implementation uses C standard/POSIX-style file handling techniques appropriate for Linux development. The design keeps a small in-memory structure to track opened files and prevent duplicate opens.

### 2) Program Demonstration and Results

The program compiles and runs in Linux with:

```bash
cd cfs
gcc -Wall -Wextra -o fms main.c file_system.c
./fms create demo.txt
./fms open demo.txt
./fms close demo.txt
./fms search demo.txt
```

Observed behavior:

- Create reports success when the file is newly created.
- Open reports whether the file is opened, already opened, or missing.
- Close reports whether the file was actually open.
- Search reports whether the file exists.

These results confirm the required working implementation of a basic file management system.

### 3) Research and Comparison of Modern File Systems

#### macOS
- **APFS** is the primary file system in modern macOS.
- Optimized for SSDs and includes snapshots, cloning, and strong encryption support.

#### PCs (Windows and Linux)
- **NTFS (Windows PC):** supports ACL permissions, journaling, compression, and large files.
- **exFAT (often used on Windows/external media):** good cross-platform compatibility, limited advanced metadata features.
- **ext4 (Linux PC):** journaling, stable performance, widely used in Linux systems.

#### Flash Drives
- Commonly formatted as **FAT32** or **exFAT**.
- FAT32 has broad compatibility but file size limits.
- exFAT supports larger files and is widely used for modern USB drives.

### 4) What Happens When Copying Across File Systems

When a file is copied between different file systems, file data is usually preserved, but some metadata may be changed or lost depending on source/destination formats and OS rules.

Possible changes include:

- Permission bits or ACL details may not map exactly (for example, Linux permissions to FAT32/exFAT).
- Extended attributes (xattrs), resource forks, or platform-specific metadata may be removed.
- Timestamp precision can differ by file system.
- Filename rules/encoding constraints can force renaming or sanitization.

Therefore, the file contents generally remain intact, but metadata fidelity is not guaranteed across all combinations.

### 5) Group Work Division

The project tasks can be divided among up to three members as follows:

- **Member 1:** core C implementation (create/open/close/search logic)
- **Member 2:** testing, validation, and Linux build/run verification
- **Member 3:** research, comparison analysis, and report/presentation preparation

This division ensures balanced technical and documentation responsibilities.

## Conclusion

From the research work, we learned that different operating systems use different file systems with different strengths (for example, APFS on macOS, NTFS on Windows, and ext4 on Linux), and portable media often uses FAT32/exFAT for compatibility.  

From the group work, we learned the importance of task division, integration, and validating implementation against requirements.  

From the implementation results, we confirmed a working Linux C program that performs create, open, close, and search operations as required for a basic file management system.

## Presentation and Grading Alignment

At presentation time, the team will demonstrate the working Linux C program and explain implementation decisions and research findings. The report is prepared to be submitted with the presentation.

Assignment scoring alignment:

- Project Assignment-1: 5 points
- Implementation and working program complexity: 25 points
- Presentation: 5 points
- Report: 15 points
  - Research work and comparison: 5 points
  - Explanation of implementation: 7 points
  - Conclusion and introduction: 3 points
