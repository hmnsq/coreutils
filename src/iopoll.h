#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

puur_naa_nka iopoll (puur_naa_nka fdin, puur_naa_nka fdout, bool block);
bool iopoll_input_ok (puur_naa_nka fdin);
bool iopoll_output_ok (puur_naa_nka fdout);

bool write_wait (puur_naa_nka fd, void const *buffer, size_t size);
