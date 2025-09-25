# NYU Shell (nyush)

A Unix shell implementation written in C as part of an Operating Systems course project. This shell provides a comprehensive command-line interface with advanced features including job control, piping, I/O redirection, and built-in commands.

## Features

### Built-in Commands
- `exit` - Exit the shell (only when no suspended jobs exist)
- `cd <directory>` - Change current working directory
- `jobs` - List all suspended/background jobs
- `fg <job_number>` - Bring a suspended job to foreground

### Advanced Features
- **Job Control**: Background process management with job suspension and resumption
- **Piping**: Support for command chaining with `|` operator
- **I/O Redirection**: 
  - Input redirection with `<`
  - Output redirection with `>`
  - Output append with `>>`
- **Signal Handling**: Proper handling of SIGINT, SIGQUIT, and SIGTSTP signals
- **Path Resolution**: Automatic program path resolution for commands in `/usr/bin/`
- **Error Handling**: Comprehensive error checking and user-friendly error messages

### Shell Features
- Interactive prompt showing current directory
- Command parsing and tokenization
- Process forking and execution
- Background job tracking
- Signal management for parent and child processes

## Building

### Prerequisites
- GCC compiler
- Make
- Unix-like operating system (Linux, macOS, etc.)

### Compilation
```bash
make
```

This will create the `nyush` executable.

### Clean Build Artifacts
```bash
make clean
```

## Usage

### Basic Usage
```bash
./nyush
```

### Example Commands
```bash
# Basic commands
ls -la
pwd
echo "Hello World"

# I/O redirection
echo "Hello" > output.txt
cat < input.txt
ls -la >> log.txt

# Piping
ls -la | grep ".txt" | wc -l

# Job control
sleep 10 &  # Start background job
jobs        # List jobs
fg 1        # Bring job 1 to foreground
```

### Built-in Commands
```bash
# Change directory
cd /home/user

# List jobs
jobs

# Bring job to foreground
fg 2

# Exit shell
exit
```

## Implementation Details

### Architecture
- **Single-threaded**: Main loop handles command input and processing
- **Process Management**: Uses `fork()` and `execv()` for command execution
- **Job Tracking**: Array-based job storage with PID and command tracking
- **Signal Handling**: Parent process ignores signals, children use default handlers

### Key Components
1. **Command Parser**: Tokenizes input and handles special characters
2. **Built-in Handler**: Processes shell-specific commands
3. **Process Executor**: Forks and executes external programs
4. **Job Manager**: Tracks background and suspended processes
5. **Pipe Handler**: Manages command chaining and data flow

### Error Handling
- Invalid commands and arguments
- File I/O errors
- Process execution failures
- Job management errors
- Signal handling errors

## Technical Specifications

- **Language**: C (C17 standard)
- **Compiler Flags**: `-g -pedantic -std=gnu17 -Wall -Werror -Wextra`
- **Maximum Jobs**: 100 concurrent jobs
- **Maximum Arguments**: 100 per command
- **Maximum Pipes**: 100 in a single command chain

## File Structure

```
nyush/
├── nyush.c      # Main source code
├── Makefile     # Build configuration
├── README.md    # This file
├── .gitignore   # Git ignore rules
└── LICENSE      # License file
```

## Development

### Code Style
- Follows GNU C coding standards
- Comprehensive error checking
- Clear variable naming
- Extensive commenting

### Testing
The shell has been tested with various command combinations and edge cases including:
- Complex piping scenarios
- I/O redirection combinations
- Job control operations
- Error condition handling

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Academic Context

This project was developed as part of an Operating Systems course, demonstrating understanding of:
- Process management and control
- Inter-process communication
- Signal handling
- File I/O operations
- System programming concepts

## Contributing

This is an academic project, but suggestions and improvements are welcome. Please ensure any contributions maintain the educational value and code quality standards.

## Known Limitations

- Maximum job limit of 100
- Limited to `/usr/bin/` for program resolution
- No history functionality
- No tab completion
- No aliases support
