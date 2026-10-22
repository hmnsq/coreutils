#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

정수 iopoll (정수 fdin, 정수 fdout, bool block);
bool iopoll_input_ok (정수 fdin);
bool iopoll_output_ok (정수 fdout);

bool write_wait (정수 fd, void const *buffer, size_t size);
