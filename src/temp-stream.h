/* A wrapper around mkstemp that gives us both an open stream pointer,
   FP, and the corresponding FILE_NAME.  Always return the same FP/name
   pair, rewinding/truncating it upon each reuse.

   Note this honors $TMPDIR, unlike the standard defined tmpfile().  */
બાહ્ય bool temp_stream (FILE **fp, અક્ષર **file_name);
