#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

صحيح iopoll (صحيح fdin, صحيح fdout, bool block);
bool iopoll_مدخل_ok (صحيح fdin);
bool iopoll_output_ok (صحيح fdout);

bool write_wait (صحيح fd, فراغ ثابت *buffer, size_t size);
