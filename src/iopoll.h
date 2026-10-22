#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

整数 iopoll (整数 fdin, 整数 fdout, bool block);
bool iopoll_input_ok (整数 fdin);
bool iopoll_output_ok (整数 fdout);

bool write_wait (整数 fd, void const *buffer, size_t size);
