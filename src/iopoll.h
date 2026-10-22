#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

முழு iopoll (முழு fdin, முழு fdout, bool block);
bool iopoll_உள்ளீடு_ok (முழு fdin);
bool iopoll_output_ok (முழு fdout);

bool write_wait (முழு fd, வெறுமை மாறிலி *buffer, size_t size);
