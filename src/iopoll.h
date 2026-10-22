#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

ціле iopoll (ціле fdin, ціле fdout, bool block);
bool iopoll_input_ok (ціле fdin);
bool iopoll_output_ok (ціле fdout);

bool write_wait (ціле fd, void const *buffer, size_t size);
