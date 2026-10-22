#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

صحیح iopoll (صحیح fdin, صحیح fdout, bool block);
bool iopoll_درون_داد_ok (صحیح fdin);
bool iopoll_output_ok (صحیح fdout);

bool write_wait (صحیح fd, تهی ثابت *buffer, size_t size);
