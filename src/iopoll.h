#घोषणा IOPOLL_BROKEN_OUTPUT -2
#घोषणा IOPOLL_ERROR         -3

पूर्णांक iopoll (पूर्णांक fdin, पूर्णांक fdout, bool block);
bool iopoll_निवेश_ok (पूर्णांक fdin);
bool iopoll_output_ok (पूर्णांक fdout);

bool write_wait (पूर्णांक fd, व्योम स्थिर *बफर, माप_प्रकार आकार);
