#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

ཧྲིལ iopoll (ཧྲིལ fdin, ཧྲིལ fdout, bool block);
bool iopoll_input_ok (ཧྲིལ fdin);
bool iopoll_output_ok (ཧྲིལ fdout);

bool write_wait (ཧྲིལ fd, void const *buffer, size_t size);
