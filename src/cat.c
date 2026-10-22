/* cat -- concatenate files and print on the standard output.
   Copyright (C) 1988-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* Differences from the Unix cat:
   * Always unbuffered, -u is ignored.
   * Usually much faster than other versions of cat, the difference
   is especially apparent when using the -v option.

   By tege@sics.se, Torbjörn Granlund, advised by rms, Richard Stallman.  */

#समावेश <config.h>

#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>

#अगर HAVE_STROPTS_H
# include <stropts.h>
#पूर्ण_अगर
#समावेश <sys/ioctl.h>

#समावेश "system.h"
#समावेश "alignalloc.h"
#समावेश "assure.h"
#समावेश "ioblksize.h"
#समावेश "fadvise.h"
#समावेश "full-write.h"
#समावेश "isapipe.h"
#समावेश "splice.h"
#समावेश "unistd--.h"
#समावेश "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "cat"

#घोषणा AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Name of input file.  May be "-".  */
अटल अक्षर स्थिर *infile;

/* Descriptor on which input file is open.  */
अटल पूर्णांक निवेश_desc;

/* Buffer for line numbers.
   An 11 digit counter may overflow within an hour on a P2/466,
   an 18 digit counter needs about 1000y */
#घोषणा पंक्ति_गणक_BUF_LEN 20
अटल अक्षर पंक्ति_buf[पंक्ति_गणक_BUF_LEN] =
  {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0',
    '\t', '\0'
  };

/* Position in 'line_buf' where printing starts.  This will not change
   unless the number of lines is larger than 999999.  */
अटल अक्षर *पंक्ति_अंक_print = पंक्ति_buf + पंक्ति_गणक_BUF_LEN - 8;

/* Position of the first digit in 'line_buf'.  */
अटल अक्षर *पंक्ति_अंक_start = पंक्ति_buf + पंक्ति_गणक_BUF_LEN - 3;

/* Position of the last digit in 'line_buf'.  */
अटल अक्षर *पंक्ति_अंक_end = पंक्ति_buf + पंक्ति_गणक_BUF_LEN - 3;

/* Preserves the 'cat' function's local 'newlines' between invocations.  */
अटल पूर्णांक newlines2 = 0;

/* Whether there is a pending CR to process.  */
अटल bool pending_cr = false;

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
"),
              program_name);
      ख_माला_दो (_("\
Concatenate खाता(s) to standard output.\n\
"), मानक_निकास);

      emit_stdin_note ();

      oputs (_("\
  -A, --show-all           equivalent to -vET\n\
"));
      oputs (_("\
  -b, --संख्या-nonblank    संख्या nonempty output पंक्तियाँ, overrides -n\n\
"));
      oputs (_("\
  -e                       equivalent to -vE\n\
"));
      oputs (_("\
  -E, --show-ends          display $ or ^M$ at end of each पंक्ति\n\
"));
      oputs (_("\
  -n, --संख्या             संख्या all output पंक्तियाँ\n\
"));
      oputs (_("\
  -s, --squeeze-blank      suppress repeated empty output पंक्तियाँ\n\
"));
      oputs (_("\
  -t                       equivalent to -vT\n\
"));
      oputs (_("\
  -T, --show-tabs          display TAB characters as ^I\n\
"));
      oputs (_("\
  -u                       (ignored)\n\
"));
      oputs (_("\
  -v, --show-nonprinting   use ^ and M- notation, except क्रम LFD and TAB\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      म_लिखो (_("\
\n\
Examples:\n\
  %s f - g  Output f's contents, then standard input, then g's contents.\n\
  %s        Copy standard निवेश to standard output.\n\
"),
              program_name, program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Compute the next line number.  */

अटल व्योम
अग्र_पंक्ति_अंक (व्योम)
{
  अक्षर *endp = पंक्ति_अंक_end;
  करो
    {
      अगर ((*endp)++ < '9')
        वापस;
      *endp-- = '0';
    }
  जबतक (endp >= पंक्ति_अंक_start);

  अगर (पंक्ति_अंक_start > पंक्ति_buf)
    *--पंक्ति_अंक_start = '1';
  अन्यथा
    *पंक्ति_buf = '>';
  अगर (पंक्ति_अंक_start < पंक्ति_अंक_print)
    पंक्ति_अंक_print--;
}

/* Plain cat.  Copy the file behind 'input_desc' to STDOUT_FILENO.
   BUF (of size BUFSIZE) is the I/O buffer, used by reads and writes.
   Return true if successful.  */

अटल bool
simple_cat (अक्षर *buf, idx_t bufsize)
{
  /* Loop until the end of the file.  */

  जबतक (true)
    {
      /* Read a block of input.  */

      ssize_t n_read = पढ़ो (निवेश_desc, buf, bufsize);
      अगर (n_read < 0)
        {
          error (0, त्रुटि_सं, "%s", quotef (infile));
          वापस false;
        }

      /* End of this file?  */

      अगर (n_read == 0)
        वापस true;

      /* Write this block out.  */

      अगर (full_write (STDOUT_FILENO, buf, n_read) != n_read)
        write_error ();
    }
}

/* Write any pending output to STDOUT_FILENO.
   Pending is defined to be the *BPOUT - OUTBUF bytes starting at OUTBUF.
   Then set *BPOUT to OUTPUT if it's not already that value.  */

अटल अंतरभूत व्योम
write_pending (अक्षर *outbuf, अक्षर **bpout)
{
  idx_t n_write = *bpout - outbuf;
  अगर (0 < n_write)
    {
      अगर (full_write (STDOUT_FILENO, outbuf, n_write) != n_write)
        write_error ();
      *bpout = outbuf;
    }
}

/* Copy the file behind 'input_desc' to STDOUT_FILENO.
   Use INBUF and read INSIZE with each call,
   and OUTBUF and write OUTSIZE with each call.
   (The buffers are a bit larger than the I/O sizes.)
   The remaining boolean args say what 'cat' options to use.

   Return true if successful.
   Called if any option more than -u was specified.

   A newline character is always put at the end of the buffer, to make
   an explicit test for buffer end unnecessary.  */

अटल bool
cat (अक्षर *inbuf, idx_t insize, अक्षर *outbuf, idx_t outsize,
     bool show_nonprinting, bool show_tabs, bool संख्या, bool संख्या_nonblank,
     bool show_ends, bool squeeze_blank)
{
  /* Last character read from the input buffer.  */
  अचिन्हित अक्षर ch;

  /* Determines how many consecutive newlines there have been in the
     input.  0 newlines makes NEWLINES -1, 1 newline makes NEWLINES 1,
     etc.  Initially 0 to indicate that we are at the beginning of a
     new line.  The "state" of the procedure is determined by
     NEWLINES.  */
  पूर्णांक newlines = newlines2;

#अगर_घोषित FIONREAD
  /* If nonzero, use the FIONREAD ioctl, as an optimization.
     (On Ultrix, it is not supported on NFS file systems.)  */
  bool use_fionread = true;
#पूर्ण_अगर

  /* The inbuf pointers are initialized so that BPIN > EOB, and thereby input
     is read immediately.  */

  /* Pointer to the first non-valid byte in the input buffer, i.e., the
     current end of the buffer.  */
  अक्षर *eob = inbuf;

  /* Pointer to the next character in the input buffer.  */
  अक्षर *bpin = eob + 1;

  /* Pointer to the position where the next character shall be written.  */
  अक्षर *bpout = outbuf;

  जबतक (true)
    {
      करो
        {
          /* Write if there are at least OUTSIZE bytes in OUTBUF.  */

          अगर (outbuf + outsize <= bpout)
            {
              अक्षर *wp = outbuf;
              idx_t remaining_bytes;
              करो
                {
                  अगर (full_write (STDOUT_FILENO, wp, outsize) != outsize)
                    write_error ();
                  wp += outsize;
                  remaining_bytes = bpout - wp;
                }
              जबतक (outsize <= remaining_bytes);

              /* Move the remaining bytes to the beginning of the
                 buffer.  */

              स_हटाओ (outbuf, wp, remaining_bytes);
              bpout = outbuf + remaining_bytes;
            }

          /* Is INBUF empty?  */

          अगर (bpin > eob)
            {
              bool निवेश_pending = false;
#अगर_घोषित FIONREAD
              पूर्णांक n_to_read = 0;

              /* Is there any input to read immediately?
                 If not, we are about to wait,
                 so write all buffered output before waiting.  */

              अगर (use_fionread
                  && ioctl (निवेश_desc, FIONREAD, &n_to_read) < 0)
                {
                  /* Ultrix returns EOPNOTSUPP on NFS;
                     HP-UX returns ENOTTY on pipes.
                     SunOS returns EINVAL and
                     More/BSD returns ENODEV on special files
                     like /dev/null.
                     Irix-5 returns ENOSYS on pipes.  */
                  अगर (त्रुटि_सं == EOPNOTSUPP || त्रुटि_सं == ENOTTY
                      || त्रुटि_सं == EINVAL || त्रुटि_सं == ENODEV
                      || त्रुटि_सं == ENOSYS)
                    use_fionread = false;
                  अन्यथा
                    {
                      error (0, त्रुटि_सं, _("cannot do ioctl on %s"),
                             quoteaf (infile));
                      newlines2 = newlines;
                      वापस false;
                    }
                }
              अगर (n_to_read != 0)
                निवेश_pending = true;
#पूर्ण_अगर

              अगर (!निवेश_pending)
                write_pending (outbuf, &bpout);

              /* Read more input into INBUF.  */

              ssize_t n_read = पढ़ो (निवेश_desc, inbuf, insize);
              अगर (n_read < 0)
                {
                  error (0, त्रुटि_सं, "%s", quotef (infile));
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  वापस false;
                }
              अगर (n_read == 0)
                {
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  वापस true;
                }

              /* Update the pointers and insert a sentinel at the buffer
                 end.  */

              bpin = inbuf;
              eob = bpin + n_read;
              *eob = '\n';
            }
          अन्यथा
            {
              /* It was a real (not a sentinel) newline.  */

              /* Was the last line empty?
                 (i.e., have two or more consecutive newlines been read?)  */

              अगर (++newlines > 0)
                {
                  अगर (newlines >= 2)
                    {
                      /* Limit this to 2 here.  Otherwise, with lots of
                         consecutive newlines, the counter could wrap
                         around at INT_MAX.  */
                      newlines = 2;

                      /* Are multiple adjacent empty lines to be substituted
                         by single ditto (-s), and this was the second empty
                         line?  */
                      अगर (squeeze_blank)
                        {
                          ch = *bpin++;
                          जारी;
                        }
                    }

                  /* Are line numbers to be written at empty lines (-n)?  */

                  अगर (संख्या && !संख्या_nonblank)
                    {
                      अग्र_पंक्ति_अंक ();
                      bpout = stpcpy (bpout, पंक्ति_अंक_print);
                    }
                }

              /* Output a currency symbol if requested (-e).  */
              अगर (show_ends)
                {
                  अगर (pending_cr)
                    {
                      *bpout++ = '^';
                      *bpout++ = 'M';
                      pending_cr = false;
                    }
                  *bpout++ = '$';
                }

              /* Output the newline.  */

              *bpout++ = '\n';
            }
          ch = *bpin++;
        }
      जबतक (ch == '\n');

      /* Here CH cannot contain a newline character.  */

      अगर (pending_cr)
        {
          *bpout++ = '\r';
          pending_cr = false;
        }

      /* Are we at the beginning of a line, and line numbers are requested?  */

      अगर (newlines >= 0 && संख्या)
        {
          अग्र_पंक्ति_अंक ();
          bpout = stpcpy (bpout, पंक्ति_अंक_print);
        }

      /* The loops below continue until a newline character is found,
         which means that the buffer is empty or that a proper newline
         has been found.  */

      /* If quoting, i.e., at least one of -v, -e, or -t specified,
         scan for chars that need conversion.  */
      अगर (show_nonprinting)
        {
          जबतक (true)
            {
              अगर (ch >= 32)
                {
                  अगर (ch < 127)
                    *bpout++ = ch;
                  अन्यथा अगर (ch == 127)
                    {
                      *bpout++ = '^';
                      *bpout++ = '?';
                    }
                  अन्यथा
                    {
                      *bpout++ = 'M';
                      *bpout++ = '-';
                      अगर (ch >= 128 + 32)
                        {
                          अगर (ch < 128 + 127)
                            *bpout++ = ch - 128;
                          अन्यथा
                            {
                              *bpout++ = '^';
                              *bpout++ = '?';
                            }
                        }
                      अन्यथा
                        {
                          *bpout++ = '^';
                          *bpout++ = ch - 128 + 64;
                        }
                    }
                }
              अन्यथा अगर (ch == '\t' && !show_tabs)
                *bpout++ = '\t';
              अन्यथा अगर (ch == '\n')
                {
                  newlines = -1;
                  अवरोध;
                }
              अन्यथा
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }

              ch = *bpin++;
            }
        }
      अन्यथा
        {
          /* Not quoting, neither of -v, -e, or -t specified.  */
          जबतक (true)
            {
              अगर (ch == '\t' && show_tabs)
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }
              अन्यथा अगर (ch != '\n')
                {
                  अगर (ch == '\r' && *bpin == '\n' && show_ends)
                    {
                      अगर (bpin == eob)
                        pending_cr = true;
                      अन्यथा
                        {
                          *bpout++ = '^';
                          *bpout++ = 'M';
                        }
                    }
                  अन्यथा
                    *bpout++ = ch;
                }
              अन्यथा
                {
                  newlines = -1;
                  अवरोध;
                }

              ch = *bpin++;
            }
        }
    }
}

/* Copy data from input to output using copy_file_range if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

अटल पूर्णांक
copy_cat (व्योम)
{
  /* Copy at most COPY_MAX bytes at a time; this is min
     (SSIZE_MAX, SIZE_MAX) truncated to a value that is
     surely aligned well.  */
  ssize_t copy_max = MIN (SSIZE_MAX, SIZE_MAX) >> 30 << 30;

  /* copy_file_range does not support some cases, and it
     incorrectly returns 0 when reading from the proc file
     system on the Linux kernel through at least 5.6.19 (2020),
     so fall back on read+write if the copy_file_range is
     unsupported or the input file seems empty.  */

  क्रम (bool some_copied = false; ; some_copied = true)
    चयन (copy_file_range (निवेश_desc, शून्य, STDOUT_FILENO, शून्य,
                             copy_max, 0))
      {
      हाल 0:
        वापस some_copied;

      हाल -1:
        अगर (त्रुटि_सं == ENOSYS || is_ENOTSUP (त्रुटि_सं) || त्रुटि_सं == EINVAL
            || त्रुटि_सं == EBADF || त्रुटि_सं == EXDEV || त्रुटि_सं == ETXTBSY
            || त्रुटि_सं == EPERM || त्रुटि_सं == EFBIG)
          वापस 0;
        error (0, त्रुटि_सं, "%s", quotef (infile));
        वापस -1;
      }
}

/* Copy data from input to output using splice if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

अटल पूर्णांक
splice_cat (व्योम)
{
  bool some_copied = false;
  bool in_ok = true;
  bool out_ok = true;

#अगर HAVE_SPLICE

  /* If PIPEFD[0] is a non-negative value, we have an open pipe from a
     previous call to this function.  At the start of this function the
     pipe will always be empty.  */
  अटल पूर्णांक pipefd[2] = { -1, -1 };

  /* The size of the pipe referred to by PIPEFD.  */
  अटल idx_t pipefd_pipe_आकार = 0;

  /* Create an intermediate pipe if it is not already open.
     Even if both input and output are pipes,
     so that read and write errors can be distinguished.  */
  अगर (pipefd[0] < 0)
    {
      अगर (pipe (pipefd) < 0)
        वापस false;
      pipefd_pipe_आकार = increase_pipe_आकार (pipefd[1]);
    }

  /* Increase the size of the pipe referred to by standard output.  */
  अटल पूर्णांक stdout_is_pipe = -1;
  अटल idx_t stdout_pipe_आकार = 0;
  अगर (stdout_is_pipe == -1)
    {
      stdout_is_pipe = 0 < isapipe (STDOUT_FILENO);
      अगर (stdout_is_pipe)
        stdout_pipe_आकार = increase_pipe_आकार (STDOUT_FILENO);
    }

  idx_t pipe_आकार = MAX (pipefd_pipe_आकार, stdout_pipe_आकार);

  जबतक (true)
    {
      ssize_t bytes_read = splice (निवेश_desc, शून्य, pipefd[1], शून्य,
                                   pipe_आकार, 0);
      /* If we successfully splice'd input previously, assume that any
         subsequent error is fatal.  If not, then fall back to read
         and write.  */
      in_ok = 0 <= bytes_read || ! some_copied;
      अगर (bytes_read == 0)
        some_copied = true;  /* Indicate splice complete.  */
      अगर (bytes_read <= 0)
        जाओ done;
      /* We need to drain the intermediate pipe to standard output.  */
      जबतक (0 < bytes_read)
        {
          ssize_t bytes_written = splice (pipefd[0], शून्य, STDOUT_FILENO, शून्य,
                                          pipe_आकार, 0);
          /* If we successfully splice'd output, assume any subsequent
             error is fatal.  If not, than drain the intermediate pipe and
             continue using read and write.  */
          अगर (bytes_written < 0)
            {
              अगर (some_copied)
                out_ok = false;
              अन्यथा
                {
                  अक्षर buf[बफ_मान];
                  जबतक (0 < bytes_read)
                    {
                      ssize_t गिनती = MIN (bytes_read, माप buf);
                      ssize_t n_read = पढ़ो (pipefd[0], buf, गिनती);
                      /* Failure not associated with in or out.  */
                      in_ok = out_ok = 0 <= n_read;
                      अगर (n_read <= 0)
                        जाओ done;
                      अगर (full_write (STDOUT_FILENO, buf, n_read) != n_read)
                        write_error ();
                      bytes_read -= n_read;
                    }
                }
            }
          अगर (bytes_written <= 0)
            जाओ done;
          some_copied = true;
          bytes_read -= bytes_written;
        }
    }

 done:
  अगर (! in_ok && ! out_ok)
    {
      /* Recreate the pipe on internal error.  */
      पूर्णांक saved_errno = त्रुटि_सं;
      बंद (pipefd[0]);
      बंद (pipefd[1]);
      त्रुटि_सं = saved_errno;
      pipefd[0] = pipefd[1] = -1;
      pipefd_pipe_आकार = 0;
      error (0, त्रुटि_सं, "%s", _("splice error"));
    }
  अन्यथा अगर (! in_ok)
    error (0, त्रुटि_सं, "%s", quotef (infile));
  अन्यथा अगर (! out_ok)
    write_error ();
#पूर्ण_अगर

  वापस (in_ok && out_ok) ? some_copied : -1;
}

/* Reuse an aligned buffer across inputs, growing it only as needed.  */

अटल अक्षर *
ensure_buf_आकार (अक्षर *buf, idx_t *buf_alloc, idx_t alignment, idx_t आकार)
{
  affirm (buf != शून्य || *buf_alloc < आकार);

  अगर (*buf_alloc < आकार)
    {
      alignfree (buf);
      buf = xalignalloc (alignment, आकार);
      *buf_alloc = आकार;
    }

  वापस buf;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  /* Nonzero if we have ever read standard input.  */
  bool have_read_stdin = false;

  काष्ठा stat ostat_buf;

  /* Variables that are set according to the specified options.  */
  bool संख्या = false;
  bool संख्या_nonblank = false;
  bool squeeze_blank = false;
  bool show_ends = false;
  bool show_nonprinting = false;
  bool show_tabs = false;
  पूर्णांक file_open_mode = O_RDONLY;

  अटल काष्ठा option स्थिर long_options[] =
  {
    {"number-nonblank", no_argument, शून्य, 'b'},
    {"number", no_argument, शून्य, 'n'},
    {"squeeze-blank", no_argument, शून्य, 's'},
    {"show-nonprinting", no_argument, शून्य, 'v'},
    {"show-ends", no_argument, शून्य, 'E'},
    {"show-tabs", no_argument, शून्य, 'T'},
    {"show-all", no_argument, शून्य, 'A'},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {शून्य, 0, शून्य, 0}
  };

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* Arrange to close stdout if we exit via the
     case_GETOPT_HELP_CHAR or case_GETOPT_VERSION_CHAR code.
     Normally STDOUT_FILENO is used rather than stdout, so
     close_stdout does nothing.  */
  निकास_पर (close_stdout);

  /* Parse command line options.  */

  पूर्णांक c;
  जबतक ((c = getopt_long (argc, argv, "benstuvAET", long_options, शून्य))
         != -1)
    {
      चयन (c)
        {
        हाल 'b':
          संख्या = true;
          संख्या_nonblank = true;
          अवरोध;

        हाल 'e':
          show_ends = true;
          show_nonprinting = true;
          अवरोध;

        हाल 'n':
          संख्या = true;
          अवरोध;

        हाल 's':
          squeeze_blank = true;
          अवरोध;

        हाल 't':
          show_tabs = true;
          show_nonprinting = true;
          अवरोध;

        हाल 'u':
          /* We provide the -u feature unconditionally.  */
          अवरोध;

        हाल 'v':
          show_nonprinting = true;
          अवरोध;

        हाल 'A':
          show_nonprinting = true;
          show_ends = true;
          show_tabs = true;
          अवरोध;

        हाल 'E':
          show_ends = true;
          अवरोध;

        हाल 'T':
          show_tabs = true;
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
        }
    }

  /* Get device, i-node number, and optimal blocksize of output.  */

  अगर (ख_स्थिति (STDOUT_FILENO, &ostat_buf) < 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("standard output"));

  /* Optimal size of i/o operations of output.  */
  idx_t outsize = io_blksize (&ostat_buf);

  /* Device, I-node number and lazily-acquired flags of the output.  */
  पूर्णांक out_flags = -2;

  /* True if the output is a regular file.  */
  bool out_isreg = S_ISREG (ostat_buf.st_mode) != 0;

  अगर (! (संख्या || show_ends || squeeze_blank))
    {
      file_open_mode |= O_द्विआधारी;
      xset_द्विआधारी_mode (STDOUT_FILENO, O_द्विआधारी);
    }

  /* Main loop.  */

  infile = "-";
  पूर्णांक argind = optind;
  bool ok = true;
  idx_t page_आकार = getpagesize ();
  अक्षर *inbuf = शून्य;
  अक्षर *outbuf = शून्य;
  idx_t inbuf_alloc = 0;
  idx_t outbuf_alloc = 0;

  करो
    {
      अगर (argind < argc)
        infile = argv[argind];

      bool reading_stdin = streq (infile, "-");
      अगर (reading_stdin)
        {
          have_read_stdin = true;
          निवेश_desc = STDIN_FILENO;
          अगर (file_open_mode & O_द्विआधारी)
            xset_द्विआधारी_mode (STDIN_FILENO, O_द्विआधारी);
        }
      अन्यथा
        {
          निवेश_desc = खोलो (infile, file_open_mode);
          अगर (निवेश_desc < 0)
            {
              error (0, त्रुटि_सं, "%s", quotef (infile));
              ok = false;
              जारी;
            }
        }

      काष्ठा stat istat_buf;
      अगर (ख_स्थिति (निवेश_desc, &istat_buf) < 0)
        {
          error (0, त्रुटि_सं, "%s", quotef (infile));
          ok = false;
          जाओ contin;
        }

      /* Optimal size of i/o operations of input.  */
      idx_t insize = io_blksize (&istat_buf);

      fdadvise (निवेश_desc, 0, 0, FADVISE_SEQUENTIAL);

      /* Don't copy a file to itself if that would merely exhaust the
         output device.  It's better to catch this error earlier
         rather than later.  */

      अगर (! (S_ISFIFO (istat_buf.st_mode) || S_ISSOCK (istat_buf.st_mode)
             || S_TYPEISSHM (&istat_buf) || S_TYPEISTMO (&istat_buf))
          && ! (S_TYPEISSHM (&ostat_buf) || S_TYPEISTMO (&ostat_buf))
          && SAME_INODE (istat_buf, ostat_buf))
        {
          off_t in_pos = lseek (निवेश_desc, 0, प्रस्तुत_से);
          अगर (0 <= in_pos)
            {
              अगर (out_flags < -1)
                out_flags = fcntl (STDOUT_FILENO, F_GETFL);
              पूर्णांक whence = (0 <= out_flags && out_flags & O_APPEND
                            ? अंत_से : प्रस्तुत_से);
              अगर (in_pos < lseek (STDOUT_FILENO, 0, whence))
                {
                  error (0, 0, _("%s: input file is output file"),
                         quotef (infile));
                  ok = false;
                  जाओ contin;
                }
            }
        }

      /* Select which version of 'cat' to use.  If any format-oriented
         options were given use 'cat'; if not, use 'copy_cat' if it
         works, 'simple_cat' otherwise.  */

      अगर (! (संख्या || show_ends || show_nonprinting
             || show_tabs || squeeze_blank))
        {
          पूर्णांक copy_cat_status =
            out_isreg && S_ISREG (istat_buf.st_mode) ? copy_cat () : 0;
          अगर (copy_cat_status != 0)
            ok &= 0 < copy_cat_status;
          अन्यथा
            {
              /* Note 32768 was determined as the limit when splice
                 starts to have a performance advantage.
                 It also excludes zero length files which may not
                 be compatible with splice in some edge cases.  */
              पूर्णांक splice_cat_status = ((usable_st_आकार (&istat_buf)
                                        && istat_buf.st_आकार <= 32768)
                                       ? 0 : splice_cat ());
              अगर (splice_cat_status != 0)
                ok &= 0 < splice_cat_status;
              अन्यथा
                {
                  insize = MAX (insize, outsize);
                  inbuf = ensure_buf_आकार (inbuf, &inbuf_alloc,
                                           page_आकार, insize);
                  ok &= simple_cat (inbuf, insize);
                }
            }
        }
      अन्यथा
        {
          /* Allocate, with an extra byte for a newline sentinel.  */
          inbuf = ensure_buf_आकार (inbuf, &inbuf_alloc,
                                   page_आकार, insize + 1);

          /* Why are
             (OUTSIZE - 1 + INSIZE * 4 + LINE_COUNTER_BUF_LEN)
             bytes allocated for the output buffer?

             A test whether output needs to be written is done when the input
             buffer empties or when a newline appears in the input.  After
             output is written, at most (OUTSIZE - 1) bytes will remain in the
             buffer.  Now INSIZE bytes of input is read.  Each input character
             may grow by a factor of 4 (by the prepending of M-^).  If all
             characters do, and no newlines appear in this block of input, we
             will have at most (OUTSIZE - 1 + INSIZE * 4) bytes in the buffer.
             If the last character in the preceding block of input was a
             newline, a line number may be written (according to the given
             options) as the first thing in the output buffer. (Done after the
             new input is read, but before processing of the input begins.)
             A line number requires seldom more than LINE_COUNTER_BUF_LEN
             positions.

             Align the output buffer to a page size boundary, for efficiency
             on some paging implementations.  */

          idx_t bufsize;
          अगर (ckd_mul (&bufsize, insize, 4)
              || ckd_add (&bufsize, bufsize, outsize)
              || ckd_add (&bufsize, bufsize, पंक्ति_गणक_BUF_LEN - 1))
            xalloc_die ();
          outbuf = ensure_buf_आकार (outbuf, &outbuf_alloc,
                                    page_आकार, bufsize);

          ok &= cat (inbuf, insize, outbuf, outsize, show_nonprinting,
                     show_tabs, संख्या, संख्या_nonblank, show_ends,
                     squeeze_blank);
        }

    contin:
      अगर (!reading_stdin && बंद (निवेश_desc) < 0)
        {
          error (0, त्रुटि_सं, "%s", quotef (infile));
          ok = false;
        }
    }
  जबतक (++argind < argc);

  अगर (pending_cr)
    {
      अगर (full_write (STDOUT_FILENO, "\r", 1) != 1)
        write_error ();
    }

#अगर_घोषित lint
  alignfree (outbuf);
  alignfree (inbuf);
#पूर्ण_अगर

  अगर (have_read_stdin && बंद (STDIN_FILENO) < 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("closing standard input"));

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}
