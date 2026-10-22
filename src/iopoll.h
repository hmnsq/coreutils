#تعریف_کن IOPOLL_BROKEN_OUTPUT -2
#تعریف_کن IOPOLL_ERROR         -3

صحيح iopoll (صحيح fdin, صحيح fdout, bool بلوک);
bool iopoll_درون_داد_ok (صحيح fdin);
bool iopoll_output_ok (صحيح fdout);

bool write_wait (صحيح fd, فراغ ثابت *میانگیر, size_t اندازه);
