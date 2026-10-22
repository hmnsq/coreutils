#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

ចំនួនគត់ iopoll (ចំនួនគត់ fdin, ចំនួនគត់ fdout, bool block);
bool iopoll_input_ok (ចំនួនគត់ fdin);
bool iopoll_output_ok (ចំនួនគត់ fdout);

bool write_wait (ចំនួនគត់ fd, void const *buffer, size_t size);
