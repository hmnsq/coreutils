#وصف_ڪريو IOPOLL_BROKEN_OUTPUT -2
#وصف_ڪريو IOPOLL_ERROR         -3

صحيح iopoll (صحيح fdin, صحيح fdout, bool بلاڪ);
bool iopoll_داخلا_ok (صحيح fdin);
bool iopoll_output_ok (صحيح fdout);

bool write_wait (صحيح fd, فراغ ثابت *بفر, size_t سائيز);
