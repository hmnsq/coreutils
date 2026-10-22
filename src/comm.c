/* comm -- compare two sorted files line by line.
   Copyright (C) 1986-2026 Free Software Foundation, Inc.

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

/* Written by Richard Stallman and David MacKenzie. */

#समावेश <config.h>

#समावेश <getopt.h>
#समावेश <sys/types.h>
#समावेश "system.h"
#समावेश "linebuffer.h"
#समावेश "fadvise.h"
#समावेश "hard-locale.h"
#समावेश "quote.h"
#समावेश "stdio--.h"
#समावेश "memcmp2.h"
#समावेश "xmemcoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "comm"

#घोषणा AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

/* True if the LC_COLLATE locale is hard.  */
अटल bool hard_LC_COLLATE;

/* If true, print lines that are found only in file 1. */
अटल bool only_file_1 = true;

/* If true, print lines that are found only in file 2. */
अटल bool only_file_2 = true;

/* If true, print lines that are found in both files. */
अटल bool both = true;

/* If nonzero, we have seen at least one unpairable line. */
अटल bool seen_unpairable;

/* If nonzero, we have warned about disorder in that file. */
अटल bool issued_disorder_warning[2];

/* line delimiter.  */
अटल अचिन्हित अक्षर delim = '\n';

/* If true, print a summary.  */
अटल bool योग_option;

/* If nonzero, check that the input is correctly ordered. */
अटल क्रमागत
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_निवेश_order;

/* Output columns will be delimited with this string, which may be set
   on the command-line with --output-delimiter=STR.  */
अटल अक्षर स्थिर *col_sep = "\t";
अटल माप_प्रकार col_sep_len = 0;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  CHECK_ORDER_OPTION = अक्षर_उच्च + 1,
  NOCHECK_ORDER_OPTION,
  OUTPUT_DELIMITER_OPTION,
  योग_OPTION
};

अटल काष्ठा option स्थिर long_options[] =
{
  {"check-order", no_argument, शून्य, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, शून्य, NOCHECK_ORDER_OPTION},
  {"output-delimiter", required_argument, शून्य, OUTPUT_DELIMITER_OPTION},
  {"total", no_argument, शून्य, योग_OPTION},
  {"zero-terminated", no_argument, शून्य, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};


व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      ख_माला_दो (_("\
Compare sorted files FILE1 and FILE2 पंक्ति by पंक्ति.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
When FILE1 or FILE2 is -, पढ़ो standard निवेश.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
With no options, produce three-column output.  Column one contains\n\
पंक्तियाँ unique to FILE1, column two contains पंक्तियाँ unique to FILE2,\n\
and column three contains पंक्तियाँ common to both files.\n\
"), मानक_निकास);
      oputs (_("\
  -1     suppress column 1 (पंक्तियाँ unique to FILE1)\n\
"));
      oputs (_("\
  -2     suppress column 2 (पंक्तियाँ unique to FILE2)\n\
"));
      oputs (_("\
  -3     suppress column 3 (पंक्तियाँ that appear in both files)\n\
"));
      oputs (_("\
      --check-order\n\
         check that the निवेश is correctly sorted,\n\
         even अगर all निवेश पंक्तियाँ are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         करो not check that the निवेश is correctly sorted\n\
"));
      oputs (_("\
      --output-delimiter=STR\n\
         separate columns with STR\n\
"));
      oputs (_("\
      --योग_2\n\
         output a summary\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         पंक्ति delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
Comparisons honor the rules specified by 'LC_COLLATE'.\n\
"), मानक_निकास);
      म_लिखो (_("\
\n\
Examples:\n\
  %s -12 file1 file2  Print only पंक्तियाँ present in both file1 and file2.\n\
  %s -3 file1 file2  Print पंक्तियाँ in file1 not in file2, and vice versa.\n\
"),
              program_name, program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Output the line in linebuffer LINE to stdout
   provided the switches say it should be output.
   CLASS is 1 for a line found only in file 1,
   2 for a line only in file 2, 3 for a line in both. */

अटल व्योम
writeline (काष्ठा linebuffer स्थिर *पंक्ति, पूर्णांक class)
{
  चयन (class)
    {
    हाल 1:
      अगर (!only_file_1)
        वापस;
      अवरोध;

    हाल 2:
      अगर (!only_file_2)
        वापस;
      अगर (only_file_1)
        ख_डालो (col_sep, 1, col_sep_len, मानक_निकास);
      अवरोध;

    हाल 3:
      अगर (!both)
        वापस;
      अगर (only_file_1)
        ख_डालो (col_sep, 1, col_sep_len, मानक_निकास);
      अगर (only_file_2)
        ख_डालो (col_sep, 1, col_sep_len, मानक_निकास);
      अवरोध;
    }

  ख_डालो (पंक्ति->बफर, माप (अक्षर), पंक्ति->लंबाई, मानक_निकास);

  अगर (ख_त्रुटि (मानक_निकास))
    write_error ();
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file.

   This function was copied (nearly) verbatim from 'src/join.c'. */

अटल व्योम
check_order (काष्ठा linebuffer स्थिर *पूर्व,
             काष्ठा linebuffer स्थिर *current,
             पूर्णांक whatfile)
{

  अगर (check_निवेश_order != CHECK_ORDER_DISABLED
      && ((check_निवेश_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      अगर (!issued_disorder_warning[whatfile - 1])
        {
          पूर्णांक order;

          अगर (hard_LC_COLLATE)
            order = xmemcoll (पूर्व->बफर, पूर्व->लंबाई - 1,
                              current->बफर, current->लंबाई - 1);
          अन्यथा
            order = memcmp2 (पूर्व->बफर, पूर्व->लंबाई - 1,
                             current->बफर, current->लंबाई - 1);

          अगर (0 < order)
            {
              error ((check_निवेश_order == CHECK_ORDER_ENABLED
                      ? निकास_त्रुटि : 0),
                     0, _("file %d is not in sorted order"), whatfile);

              /* If we get to here, the message was just a warning, but we
                 want only to issue it once. */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

/* Compare INFILES[0] and INFILES[1].
   If either is "-", use the standard input for that file.
   Assume that each input file is sorted;
   merge them and output the result.
   Exit the program when done.  */

अटल _Noreturn व्योम
compare_files (अक्षर **infiles)
{
  /* For each file, we have four linebuffers in lba. */
  काष्ठा linebuffer lba[2][4];

  /* thisline[i] points to the linebuffer holding the next available line
     in file i, or is null if there are no lines left in that file.  */
  काष्ठा linebuffer *thisline[2];

  /* all_line[i][alt[i][0]] also points to the linebuffer holding the
     current line in file i. We keep two buffers of history around so we
     can look two lines back when we get to the end of a file. */
  काष्ठा linebuffer *all_पंक्ति[2][4];

  /* This is used to rotate through the buffers for each input file. */
  पूर्णांक alt[2][3];

  /* streams[i] holds the input stream for file i.  */
  खाता *streams[2];

  /* Counters for the summary.  */
  uintmax_t योग_2[] = {0, 0, 0};

  /* Initialize the storage. */
  क्रम (पूर्णांक i = 0; i < 2; i++)
    {
      क्रम (पूर्णांक j = 0; j < 4; j++)
        {
          initbuffer (&lba[i][j]);
          all_पंक्ति[i][j] = &lba[i][j];
        }
      alt[i][0] = 0;
      alt[i][1] = 0;
      alt[i][2] = 0;
      streams[i] = (streq (infiles[i], "-") ? मानक_निवेश : ख_खोलो (infiles[i], "r"));
      अगर (!streams[i])
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infiles[i]));

      fadvise (streams[i], FADVISE_SEQUENTIAL);

      thisline[i] = readlinebuffer_delim (all_पंक्ति[i][alt[i][0]], streams[i],
                                          delim);
      अगर (ख_त्रुटि (streams[i]))
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infiles[i]));
    }

  जबतक (thisline[0] || thisline[1])
    {
      पूर्णांक order;
      bool fill_up[2] = { false, false };

      /* Compare the next available lines of the two files.  */

      अगर (!thisline[0])
        order = 1;
      अन्यथा अगर (!thisline[1])
        order = -1;
      अन्यथा
        {
          अगर (hard_LC_COLLATE)
            order = xmemcoll (thisline[0]->बफर, thisline[0]->लंबाई - 1,
                              thisline[1]->बफर, thisline[1]->लंबाई - 1);
          अन्यथा
            {
              माप_प्रकार len = MIN (thisline[0]->लंबाई, thisline[1]->लंबाई) - 1;
              order = स_भेद (thisline[0]->बफर, thisline[1]->बफर, len);
              अगर (order == 0)
                order = _GL_CMP (thisline[0]->लंबाई, thisline[1]->लंबाई);
            }
        }

      /* Output the line that is lesser. */
      अगर (order == 0)
        {
          /* Line is seen in both files.  */
          योग_2[2]++;
          writeline (thisline[1], 3);
        }
      अन्यथा
        {
          seen_unpairable = true;
          अगर (order <= 0)
            {
              /* Line is seen in file 1 only.  */
              योग_2[0]++;
              writeline (thisline[0], 1);
            }
          अन्यथा
            {
              /* Line is seen in file 2 only.  */
              योग_2[1]++;
              writeline (thisline[1], 2);
            }
        }

      /* Step the file the line came from.
         If the files match, step both files.  */
      अगर (0 <= order)
        fill_up[1] = true;
      अगर (order <= 0)
        fill_up[0] = true;

      क्रम (पूर्णांक i = 0; i < 2; i++)
        अगर (fill_up[i])
          {
            /* Rotate the buffers for this file. */
            alt[i][2] = alt[i][1];
            alt[i][1] = alt[i][0];
            alt[i][0] = (alt[i][0] + 1) & 0x03;

            thisline[i] = readlinebuffer_delim (all_पंक्ति[i][alt[i][0]],
                                                streams[i], delim);

            अगर (thisline[i])
              check_order (all_पंक्ति[i][alt[i][1]], thisline[i], i + 1);

            /* If this is the end of the file we may need to re-check
               the order of the previous two lines, since we might have
               discovered an unpairable match since we checked before. */
            अन्यथा अगर (all_पंक्ति[i][alt[i][2]]->बफर)
              check_order (all_पंक्ति[i][alt[i][2]],
                           all_पंक्ति[i][alt[i][1]], i + 1);

            अगर (ख_त्रुटि (streams[i]))
              error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infiles[i]));

            fill_up[i] = false;
          }
    }

  /* Avoid closing standard input twice when invoking 'comm - -'.  */
  स्थिर पूर्णांक n_streams = 2 - (streams[0] == streams[1]);
  क्रम (पूर्णांक i = 0; i < n_streams; i++)
    अगर (ख_बंद (streams[i]) != 0)
      error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infiles[i]));

  अगर (योग_option)
    {
      /* Print the summary, minding the column and line delimiters.  */
      अगर (col_sep_len == 1)
        { /* Separate to handle NUL char.  */
          म_लिखो ("%ju%c%ju%c%ju%c%s%c",
                  योग_2[0], *col_sep,
                  योग_2[1], *col_sep,
                  योग_2[2], *col_sep,
                  _("total"), delim);
        }
      अन्यथा
        {
          म_लिखो ("%ju%s%ju%s%ju%s%s%c",
                  योग_2[0], col_sep,
                  योग_2[1], col_sep,
                  योग_2[2], col_sep,
                  _("total"), delim);
        }
    }

  अगर (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (निकास_त्रुटि, 0, _("input is not in sorted order"));

  /* Exit here to pacify gcc -fsanitizer=leak.  */
  निकास (निकास_सफल);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  निकास_पर (close_stdout);

  जबतक ((c = getopt_long (argc, argv, "123z", long_options, शून्य)) != -1)
    चयन (c)
      {
      हाल '1':
        only_file_1 = false;
        अवरोध;

      हाल '2':
        only_file_2 = false;
        अवरोध;

      हाल '3':
        both = false;
        अवरोध;

      हाल 'z':
        delim = '\0';
        अवरोध;

      हाल NOCHECK_ORDER_OPTION:
        check_निवेश_order = CHECK_ORDER_DISABLED;
        अवरोध;

      हाल CHECK_ORDER_OPTION:
        check_निवेश_order = CHECK_ORDER_ENABLED;
        अवरोध;

      हाल OUTPUT_DELIMITER_OPTION:
        अगर (col_sep_len && !streq (col_sep, optarg))
          error (निकास_त्रुटि, 0, _("multiple output delimiters specified"));
        col_sep = optarg;
        col_sep_len = *optarg ? म_माप (optarg) : 1;
        अवरोध;

      हाल योग_OPTION:
        योग_option = true;
        अवरोध;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      शेष:
        usage (निकास_त्रुटि);
      }

  अगर (! col_sep_len)
    col_sep_len = 1;

  अगर (argc - optind < 2)
    {
      अगर (argc <= optind)
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (निकास_त्रुटि);
    }

  अगर (2 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (निकास_त्रुटि);
    }

  compare_files (argv + optind);
}
