#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

số_nguyên iopoll (số_nguyên fdin, số_nguyên fdout, bool block);
bool iopoll_input_ok (số_nguyên fdin);
bool iopoll_output_ok (số_nguyên fdout);

bool write_wait (số_nguyên fd, void const *buffer, size_t size);
