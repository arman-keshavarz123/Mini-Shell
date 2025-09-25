# NYU Shell Architecture

This document describes the internal architecture and design decisions of the NYU Shell (nyush) implementation.

## Overview

The NYU Shell is a Unix shell implementation written in C that provides a command-line interface with advanced features including job control, piping, I/O redirection, and built-in commands.

## Core Components

### 1. Main Loop Structure

The shell operates in a continuous loop that:
1. Displays a prompt with the current directory
2. Reads user input
3. Parses and validates commands
4. Executes commands (built-in or external)
5. Manages job control and process lifecycle

### 2. Command Processing Pipeline

```
Input → Validation → Parsing → Classification → Execution → Cleanup
```

#### Input Validation
- Checks for empty input
- Validates pipe syntax (no leading/trailing pipes)
- Ensures proper redirection syntax

#### Command Classification
- **Built-in Commands**: `exit`, `cd`, `jobs`, `fg`
- **External Commands**: All other programs
- **Piped Commands**: Commands containing `|` operator

### 3. Built-in Commands

#### Exit Command
- Validates no arguments provided
- Ensures no suspended jobs exist
- Performs cleanup and exits

#### Change Directory (cd)
- Validates exactly one argument
- Uses `chdir()` system call
- Provides error feedback for invalid directories

#### Jobs Command
- Lists all suspended/background jobs
- Shows job numbers and command strings
- No arguments allowed

#### Foreground (fg) Command
- Validates job number argument
- Removes job from job list
- Sends SIGCONT to resume process
- Waits for completion or suspension
- Re-adds to job list if suspended

### 4. Process Management

#### Job Control
- **Job Storage**: Arrays for PIDs and command strings
- **Maximum Jobs**: 100 concurrent jobs
- **Job States**: Running, Suspended, Completed

#### Process Execution
- **Fork-Exec Pattern**: Uses `fork()` and `execv()`
- **Signal Handling**: Different handlers for parent/child
- **Path Resolution**: Automatic `/usr/bin/` prefix for external commands

### 5. I/O Redirection

#### Input Redirection (`<`)
- Opens specified file for reading
- Redirects stdin using `dup2()`
- Validates file existence and permissions

#### Output Redirection (`>`)
- Creates/truncates output file
- Redirects stdout using `dup2()`
- Sets appropriate file permissions (0644)

#### Append Redirection (`>>`)
- Opens file in append mode
- Preserves existing content
- Same permission handling as `>`

### 6. Piping Implementation

#### Pipe Creation
- Creates pipes between consecutive commands
- Uses `pipe()` system call
- Manages file descriptors carefully

#### Process Chaining
- Each command in pipe runs in separate process
- Input/output connected via pipes
- Proper cleanup of all file descriptors

#### Redirection in Pipes
- Input redirection only in first command
- Output redirection only in last command
- Maintains pipe data flow integrity

### 7. Signal Handling

#### Parent Process Signals
- `SIGINT`, `SIGQUIT`, `SIGTSTP`: Ignored
- Prevents shell termination from user signals
- Maintains job control functionality

#### Child Process Signals
- `SIGINT`, `SIGQUIT`, `SIGTSTP`: Default handlers
- Allows normal signal behavior in child processes
- Enables job suspension and termination

### 8. Memory Management

#### Dynamic Allocation
- Uses `strdup()` for command string storage
- Proper cleanup with `free()` calls
- Handles allocation failures gracefully

#### Static Arrays
- Job arrays with fixed maximum sizes
- Command argument arrays
- Pipe file descriptor arrays

## Data Structures

### Global Variables
```c
static int num_job = 0;                    // Number of active jobs
static pid_t jb_pid[MAX_JOBS];            // Job process IDs
static char *jb_cmd[MAX_JOBS];            // Job command strings
```

### Constants
```c
#define MAX_JOBS 100                       // Maximum concurrent jobs
#define MAX_ARGS 100                       // Maximum arguments per command
#define MAX_PIPES 100                      // Maximum pipes in command chain
#define BUFFER_SIZE 1024                   // General purpose buffer size
```

## Error Handling

### Error Categories
1. **Invalid Commands**: Malformed syntax, wrong arguments
2. **File Errors**: Missing files, permission denied
3. **Process Errors**: Fork failures, execution failures
4. **Job Errors**: Invalid job numbers, job limits exceeded

### Error Reporting
- Consistent error message format
- Descriptive error messages
- Graceful error recovery where possible

## Security Considerations

### Input Validation
- Bounds checking on all array accesses
- Validation of command arguments
- Prevention of buffer overflows

### Process Isolation
- Each command runs in separate process
- Proper signal handling prevents interference
- Clean separation of parent/child responsibilities

## Performance Considerations

### Memory Usage
- Fixed-size arrays for predictable memory usage
- Dynamic allocation only where necessary
- Proper cleanup prevents memory leaks

### Process Efficiency
- Minimal overhead for built-in commands
- Efficient pipe implementation
- Proper file descriptor management

## Limitations

### Current Limitations
- Maximum 100 jobs
- Limited to `/usr/bin/` for command resolution
- No command history
- No tab completion
- No aliases

### Design Trade-offs
- Simplicity vs. feature completeness
- Fixed limits vs. dynamic allocation
- Single-threaded vs. multi-threaded design

## Future Enhancements

### Potential Improvements
- Command history with up/down arrow support
- Tab completion for commands and files
- Alias support
- Environment variable expansion
- Background job execution with `&`
- More sophisticated job control

### Architectural Considerations
- Modular design would support easy feature addition
- Clear separation of concerns enables independent development
- Consistent error handling simplifies debugging and maintenance
