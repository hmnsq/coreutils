#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

จำนวนเต็ม iopoll (จำนวนเต็ม fdin, จำนวนเต็ม fdout, bool block);
bool iopoll_input_ok (จำนวนเต็ม fdin);
bool iopoll_output_ok (จำนวนเต็ม fdout);

bool write_wait (จำนวนเต็ม fd, void const *buffer, size_t size);
