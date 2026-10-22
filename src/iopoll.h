#تعریف_کرو IOPOLL_BROKEN_OUTPUT -2
#تعریف_کرو IOPOLL_ERROR         -3

صحيح iopoll (صحيح fdin, صحيح fdout, bool بلاک);
bool iopoll_داخلہ_ok (صحيح fdin);
bool iopoll_output_ok (صحيح fdout);

bool write_wait (صحيح fd, فراغ ثابت *بفر, size_t ناپ);
