#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

цяло iopoll (цяло fdin, цяло fdout, bool block);
bool iopoll_input_ok (цяло fdin);
bool iopoll_output_ok (цяло fdout);

bool write_wait (цяло fd, void const *buffer, size_t size);
