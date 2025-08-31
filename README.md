# Quill Editor

**Version:** 1.0.0  
**Author:** T3chron

Quill Editor is a lightweight console-based text editor for Windows. It supports multiple modes (Insert and Read), a status bar, and cursor control to enhance text editing experience in the console.

---

## Features

- **Modes**:
  - **Insert Mode**: Allows typing text.
  - **Read Mode**: Read-only mode with cursor navigation using `Ctrl + W/A/S/D`.
  - **None Mode**: No active mode.

- **Status Bar**:
  - Always displayed at the bottom of the console.
  - Shows the current mode and useful shortcuts, e.g., `Ctrl+S [Save]`.

- **Cursor Control**:
  - Cursor cannot move above the title line.
  - Status bar remains at the bottom even when scrolling.

- **Character Input Control**:
  - Only allowed characters can be typed (letters, numbers, and special symbols).
  - Backspace support to delete characters.

- **Keyboard Shortcuts**:
  - `Ctrl + I`: Activate Insert Mode
  - `Ctrl + R`: Activate Read Mode
  - `Ctrl + W/A/S/D`: Move cursor in Read Mode
  - `ESC`: Exit the active mode

---

## Installation and Usage

1. Clone the repository:

```bash
git clone https://github.com/T3chron/Quill.git

2. Installation
```bash
cd Quill | g++ main.cpp -o Quill.exe -std=c++17 -pthread
```

3. Run
```bash
 ./Quill.exe

