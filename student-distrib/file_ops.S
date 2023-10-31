# file_ops.S - Jump tables for file operations, including generic and stdin and stdout
.globl stdin_table
.globl stdout_table
.globl file_ops_table
#define ASM     1

#  stdin_table
# DESCRIPTION: jump table for stdin file operations
# INPUTS:  uint32_t - Command number for jump table
#          int32_t  - file descriptor
#          char*    - buf
#          int32_t  - nbytes
# OUTPUTS: int32_t  - return value
# RETURN VALUE: For command #:
#               0 = open: 0 for success
#               1 = read: # of bytes written if success, -1 if failure
#               2 = write: 0 for success
#               3 = close: 0 for success
#               -1 for failure
# SIDE EFFECTS: 