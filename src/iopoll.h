#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

પૂર્ણાંક iopoll (પૂર્ણાંક fdin, પૂર્ણાંક fdout, bool block);
bool iopoll_આગત_ok (પૂર્ણાંક fdin);
bool iopoll_output_ok (પૂર્ણાંક fdout);

bool write_wait (પૂર્ણાંક fd, શૂન્ય અચલ *buffer, size_t size);
