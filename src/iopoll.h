#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

പൂർണ്ണസംഖ്യ iopoll (പൂർണ്ണസംഖ്യ fdin, പൂർണ്ണസംഖ്യ fdout, bool block);
bool iopoll_നിവേശം_ok (പൂർണ്ണസംഖ്യ fdin);
bool iopoll_output_ok (പൂർണ്ണസംഖ്യ fdout);

bool write_wait (പൂർണ്ണസംഖ്യ fd, ശൂന്യം സ്ഥിരം *buffer, size_t size);
