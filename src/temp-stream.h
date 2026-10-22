/* A wrapper around mkstemp that gives us both an open stream pointer,
   FP, and the corresponding FILE_NAME.  Always return the same FP/name
   pair, rewinding/truncating it upon each reuse.

   Note this honors $TMPDIR, unlike the standard defined tmpfile().  */
ಬಾಹ್ಯ bool ತಾತ್ಕಾಲಿಕ_ಹರಿವು (FILE **fp, ಅಕ್ಷರ **file_name);
