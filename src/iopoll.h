#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

целое iopoll (целое fdin, целое fdout, bool block);
bool iopoll_input_ok (целое fdin);
bool iopoll_output_ok (целое fdout);

bool write_wait (целое fd, void const *buffer, size_t size);
