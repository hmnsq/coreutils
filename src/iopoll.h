#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

цео iopoll (цео fdin, цео fdout, bool block);
bool iopoll_input_ok (цео fdin);
bool iopoll_output_ok (цео fdout);

bool write_wait (цео fd, void const *buffer, size_t size);
