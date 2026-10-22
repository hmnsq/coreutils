#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

ຈຳນວນເຕັມ iopoll (ຈຳນວນເຕັມ fdin, ຈຳນວນເຕັມ fdout, bool block);
bool iopoll_input_ok (ຈຳນວນເຕັມ fdin);
bool iopoll_output_ok (ຈຳນວນເຕັມ fdout);

bool write_wait (ຈຳນວນເຕັມ fd, void const *buffer, size_t size);
