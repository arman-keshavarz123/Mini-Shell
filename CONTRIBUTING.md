# Contributing to NYU Shell

Thank you for your interest in contributing to the NYU Shell project! This document provides guidelines for contributing to the project.

## Project Overview

NYU Shell (nyush) is an educational Unix shell implementation developed as part of an Operating Systems course. It demonstrates core concepts in system programming including process management, inter-process communication, and signal handling.

## Development Setup

### Prerequisites
- GCC compiler (C17 standard)
- Make
- Unix-like operating system (Linux, macOS, etc.)
- Git (for version control)

### Building the Project
```bash
# Clone the repository
git clone <repository-url>
cd nyush

# Build the project
make

# Run tests
make check

# Clean build artifacts
make clean
```

## Code Style Guidelines

### C Coding Standards
- Follow GNU C coding standards
- Use consistent indentation (tabs for indentation, spaces for alignment)
- Maximum line length: 80 characters
- Use meaningful variable and function names

### Commenting
- Use `/* */` for block comments
- Use `//` for single-line comments (sparingly)
- Document complex algorithms and logic
- Include function descriptions for public APIs

### Error Handling
- Always check return values of system calls
- Provide meaningful error messages
- Use consistent error message format
- Handle edge cases gracefully

## Contribution Areas

### Bug Fixes
- Fix memory leaks
- Correct signal handling issues
- Improve error handling
- Fix edge cases in command parsing

### Feature Enhancements
- Add new built-in commands
- Improve job control functionality
- Add command history
- Implement tab completion
- Add alias support

### Code Quality
- Improve code documentation
- Add unit tests
- Refactor complex functions
- Optimize performance

### Documentation
- Improve README and documentation
- Add code examples
- Create tutorials
- Update architecture documentation

## Submitting Changes

### Before Submitting
1. Ensure code compiles without warnings
2. Run `make check` to verify functionality
3. Test your changes thoroughly
4. Update documentation if needed

### Pull Request Process
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request with:
   - Clear description of changes
   - Testing instructions
   - Any breaking changes noted

### Commit Messages
Use clear, descriptive commit messages:
```
Add support for command history

- Implement history storage with circular buffer
- Add up/down arrow key navigation
- Update help documentation
```

## Testing Guidelines

### Manual Testing
- Test all built-in commands
- Verify I/O redirection functionality
- Test piping with various commands
- Check job control operations
- Test error conditions

### Test Cases
- Valid command execution
- Invalid command handling
- File redirection (input/output/append)
- Pipe chaining
- Job suspension and resumption
- Signal handling
- Error recovery

## Code Review Process

### Review Criteria
- Code correctness and functionality
- Adherence to coding standards
- Proper error handling
- Memory management
- Documentation quality
- Test coverage

### Review Process
1. Automated checks (compilation, linting)
2. Manual code review
3. Testing verification
4. Documentation review
5. Final approval

## Development Workflow

### Branch Naming
- `feature/description` - New features
- `bugfix/description` - Bug fixes
- `docs/description` - Documentation updates
- `refactor/description` - Code refactoring

### Issue Tracking
- Use GitHub issues for bug reports
- Use labels to categorize issues
- Provide detailed reproduction steps
- Include system information

## Educational Context

### Learning Objectives
This project serves educational purposes and contributions should:
- Maintain educational value
- Demonstrate good programming practices
- Show understanding of system programming concepts
- Provide clear, readable code

### Academic Integrity
- Ensure all code is original work
- Properly attribute any external resources
- Follow academic integrity guidelines
- Maintain professional standards

## Resources

### Documentation
- [GNU C Library Manual](https://www.gnu.org/software/libc/manual/)
- [POSIX.1-2017 Specification](https://pubs.opengroup.org/onlinepubs/9699919799/)
- [Advanced Programming in the UNIX Environment](https://www.apue.com/)

### Tools
- `gdb` - Debugging
- `valgrind` - Memory leak detection
- `strace` - System call tracing
- `make` - Build automation

## Contact

For questions about contributing or the project:
- Create an issue for bug reports or feature requests
- Use discussions for general questions
- Follow the code of conduct

## License

By contributing to this project, you agree that your contributions will be licensed under the same MIT License that covers the project.
