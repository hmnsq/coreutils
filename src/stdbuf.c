/* stdbuf -- setup the standard streams for a command
   Copyright (C) 2009-2026 Free Software Foundation, Inc.

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

/* Written by Pádraig Brady.  */

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "filenamecat.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "xreadlink.h"
#अन्तर्भाव "xstrtol-error.h"
#अन्तर्भाव "xstrtol.h"
#अन्तर्भाव "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "stdbuf"
#परिभाषय LIB_NAME "libstdbuf.so" /* FIXME: don't hardcode  */

#परिभाषय AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

स्थिर वर्ण *क्रमादेशः_मार्गः;

स्थिर संरचना
{
  size_t आकार;
  पूर्णाङ्क optc;
  वर्ण *optarg;
} stdbuf[3];

स्थिर संरचना option ध्रुव longopts[] =
{
  {"input", required_तर्कः, रिक्त, 'i'},
  {"output", required_तर्कः, रिक्त, 'o'},
  {"error", required_तर्कः, रिक्त, 'e'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Set size to the value of STR, interpreted as a decimal integer,
   optionally multiplied by various values.
   Return LONGINT_OK on success, otherwise an error for use with
   xstrtol_fatal.

   This supports dd BLOCK size suffixes.
   Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats.  */
स्थिर गणना strtol_error
parse_आकार (वर्ण ध्रुव *str, size_t *आकार)
{
  uintmax_t tmp_आकार;
  गणना strtol_error e = xstrtoumax (str, रिक्त, 10,
                                    &tmp_आकार, "EGkKMPQRTYZ0");
  यदि (e == LONGINT_OK && ckd_सङ्कलय (आकार, tmp_आकार, 0))
    e = LONGINT_अतिप्रवाहः;
  प्रत्यागम e;
}

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("Usage: %s OPTION... COMMAND\n"), क्रमादेशः_name);
      fputs (_("\
सततम् COMMAND, with modified buffering operations प्रति its standard streams.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --निवेशः=बहुलकम्   adjust standard निवेशः प्रवाहः buffering\n\
"));
      oputs (_("\
  -o, --output=बहुलकम्  adjust standard output प्रवाहः buffering\n\
"));
      oputs (_("\
  -e, --error=बहुलकम्   adjust standard error प्रवाहः buffering\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      fputs (_("\n\
If बहुलकम् is 'L' the corresponding प्रवाहः will be पङ्क्तिः buffered.\n\
This option is invalid with standard निवेशः.\n"), stdout);
      fputs (_("\n\
If बहुलकम् is '0' the corresponding प्रवाहः will be unbuffered.\n\
"), stdout);
      fputs (_("\n\
Otherwise बहुलकम् is a सङ्ख्या which may be followed by one सम्बन्धि the following:\n\
KB 1000, K 1024, MB 1000*1000, M 1024*1024, and so on प्रति G,T,P,E,Z,Y,R,Q.\n\
द्विमान_2 prefixes can be प्रयुक्तम्, too: KiB=K, MiB=M, and so on.\n\
अन्तर्_2 this पक्ष the corresponding प्रवाहः will be fully buffered with the बफर\n\
आकार समुच्चयः_2 गन्तव्यम् बहुलकम् अष्टकानि.\n\
"), stdout);
      fputs (_("\n\
NOTE: If COMMAND adjusts the buffering सम्बन्धि its standard streams ('tee' does\n\
प्रति example) then that will override corresponding changes by 'stdbuf'.\n\
Also some filters (like 'dd' and 'cat' etc.) don't use streams प्रति I/O,\n\
and are thus unaffected by 'stdbuf' settings.\n\
"), stdout);
      emit_exec_स्थितिः (क्रमादेशः_NAME);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* argv[0] can be anything really, but generally it contains
   the path to the executable or just a name if it was executed
   using $PATH. In the latter case to get the path we can:
   search getenv("PATH"), readlink("/prof/self/exe"), getenv("_"),
   dladdr(), pstat_getpathname(), etc.  */

स्थिर शून्य
समुच्चयः_क्रमादेशः_मार्गः (वर्ण ध्रुव *arg)
{
  यदि (strchr (arg, '/'))        /* Use absolute or relative paths directly.  */
    {
      क्रमादेशः_मार्गः = dir_name (arg);
    }
  अन्यथा
    {
      वर्ण *मार्गः = xreadlink ("/proc/self/exe");
      यदि (मार्गः)
        क्रमादेशः_मार्गः = dir_name (मार्गः);
      अन्यथा यदि ((मार्गः = getenv ("PATH")))
        {
          मार्गः = xstrdup (मार्गः);
          प्रति (वर्ण *dir = strtok (मार्गः, ":"); dir != रिक्त;
               dir = strtok (रिक्त, ":"))
            {
              वर्ण *candidate = file_name_concat (dir, arg, रिक्त);
              यदि (access (candidate, X_OK) == 0)
                {
                  क्रमादेशः_मार्गः = dir_name (candidate);
                  मुक्त (candidate);
                  भङ्ग;
                }
              मुक्त (candidate);
            }
        }
      मुक्त (मार्गः);
    }
}

स्थिर पूर्णाङ्क
optc_गन्तव्यम्_fileno (पूर्णाङ्क c)
{
  पूर्णाङ्क ret = -1;

  विकल्प (c)
    {
    पक्ष 'e':
      ret = STDERR_FILENO;
      भङ्ग;
    पक्ष 'i':
      ret = STDIN_FILENO;
      भङ्ग;
    पक्ष 'o':
      ret = STDOUT_FILENO;
      भङ्ग;
    स्वभाव:
      affirm (false);
    }

  प्रत्यागम ret;
}

स्थिर शून्य
समुच्चयः_LD_PRELOAD (शून्य)
{
  पूर्णाङ्क ret;
#यदि_परिभाषितम् __APPLE__
  वर्ण ध्रुव *preload_env = "DYLD_INSERT_LIBRARIES";
#नोचेद्यदि defined _AIX
  वर्ण ध्रुव *preload_env = (परिमाण (शून्य *) < 8
                             ? "LDR_PRELOAD" : "LDR_PRELOAD64");
#अन्यथा
  वर्ण ध्रुव *preload_env = "LD_PRELOAD";
#यदि_अन्तः
  वर्ण *old_libs = getenv (preload_env);
  वर्ण *LD_PRELOAD;

  /* Note this would auto add the appropriate search path for "libstdbuf.so":
     gcc stdbuf.c -Wl,-rpath,'$ORIGIN' -Wl,-rpath,$PKGLIBEXECDIR
     However we want the lookup done for the exec'd command not stdbuf.

     Since we don't link against libstdbuf.so add it to PKGLIBEXECDIR
     rather than to LIBDIR.

     Note we could add "" as the penultimate item in the following list
     to enable searching for libstdbuf.so in the default system lib paths.
     However that would not indicate an error if libstdbuf.so was not found.
     Also while this could support auto selecting the right arch in a multilib
     environment, what we really want is to auto select based on the arch of the
     command being run, rather than that of stdbuf itself.  This is currently
     not supported due to the unusual need for controlling the stdio buffering
     of programs that are a different architecture to the default on the
     system (and that of stdbuf itself).  */
  वर्ण ध्रुव *ध्रुव अन्वेषणम्_मार्गः[] = {
    क्रमादेशः_मार्गः,
    PKGLIBEXECDIR,
    रिक्त
  };

  वर्ण ध्रुव *ध्रुव *मार्गः = अन्वेषणम्_मार्गः;
  वर्ण *libstdbuf;

  यावत् (true)
    {
      संरचना stat sb;

      यदि (!**मार्गः)              /* system default  */
        {
          libstdbuf = xstrdup (LIB_NAME);
          भङ्ग;
        }
      ret = asprintf (&libstdbuf, "%s/%s", *मार्गः, LIB_NAME);
      यदि (ret < 0)
        xalloc_die ();
      यदि (stat (libstdbuf, &sb) == 0)   /* file_exists  */
        भङ्ग;
      मुक्त (libstdbuf);

      ++मार्गः;
      यदि ( ! *मार्गः)
        error (EXIT_CANCELED, 0, _("failed to find %s"), quote (LIB_NAME));
    }

  /* FIXME: Do we need to support libstdbuf.dll, c:, '\' separators etc?  */

  यदि (old_libs)
    ret = asprintf (&LD_PRELOAD, "%s=%s:%s", preload_env, old_libs, libstdbuf);
  अन्यथा
    ret = asprintf (&LD_PRELOAD, "%s=%s", preload_env, libstdbuf);

  यदि (ret < 0)
    xalloc_die ();

  मुक्त (libstdbuf);

  ret = putenv (LD_PRELOAD);
#यदि_परिभाषितम् __APPLE__
  यदि (ret == 0)
    ret = setenv ("DYLD_FORCE_FLAT_NAMESPACE", "y", 1);
#यदि_अन्तः

  यदि (ret != 0)
    error (EXIT_CANCELED, errno,
           _("failed to update the environment with %s"),
           quote (LD_PRELOAD));
}

/* Populate environ with _STDBUF_I=$MODE _STDBUF_O=$MODE _STDBUF_E=$MODE.
   Return TRUE if any environment variables set.   */

स्थिर bool
समुच्चयः_libstdbuf_options (शून्य)
{
  bool env_समुच्चयः = false;

  प्रति (size_t i = 0; i < countof (stdbuf); i++)
    {
      यदि (stdbuf[i].optarg)
        {
          वर्ण *var;
          पूर्णाङ्क ret;

          यदि (*stdbuf[i].optarg == 'L')
            ret = asprintf (&var, "%s%c=L", "_STDBUF_",
                            c_toupper (stdbuf[i].optc));
          अन्यथा
            ret = asprintf (&var, "%s%c=%zu", "_STDBUF_",
                            c_toupper (stdbuf[i].optc),
                            stdbuf[i].आकार);
          यदि (ret < 0)
            xalloc_die ();

          यदि (putenv (var) != 0)
            error (EXIT_CANCELED, errno,
                   _("failed to update the environment with %s"),
                   quote (var));

          env_समुच्चयः = true;
        }
    }

  प्रत्यागम env_समुच्चयः;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क c;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  यावत् ((c = getopt_long (argc, argv, "+i:o:e:", longopts, रिक्त)) != -1)
    {
      पूर्णाङ्क ध्रुव oi = -1;
      पूर्णाङ्क opt_fileno;

      विकल्प (c)
        {
        /* Old McDonald had a farm ei...  */
        पक्ष 'e':
        पक्ष 'i':
        पक्ष 'o':
          opt_fileno = optc_गन्तव्यम्_fileno (c);
          affirm (0 <= opt_fileno && opt_fileno < countof (stdbuf));
          stdbuf[opt_fileno].optc = c;
          यावत् (c_isspace (*optarg))
            optarg++;
          stdbuf[opt_fileno].optarg = optarg;
          यदि (c == 'i' && *optarg == 'L')
            {
              /* -oL will be by far the most common use of this utility,
                 but one could easily think -iL might have the same affect,
                 so disallow it as it could be confusing.  */
              error (0, 0, _("line buffering standard input is meaningless"));
              usage (EXIT_CANCELED);
            }

          यदि (!streq (optarg, "L"))
            {
              गणना strtol_error e = parse_आकार (optarg,
                                                &stdbuf[opt_fileno].आकार);
              यदि (e != LONGINT_OK)
                xstrtol_fatal (e, oi, c, longopts, optarg);
            }

          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_CANCELED);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify at least 1 command.  */
  यदि (argc < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  यदि (! समुच्चयः_libstdbuf_options ())
    {
      error (0, 0, _("you must specify a buffering mode option"));
      usage (EXIT_CANCELED);
    }

  /* Try to preload libstdbuf first from the same path as
     stdbuf is running from.  */
  समुच्चयः_क्रमादेशः_मार्गः (क्रमादेशः_name);
  यदि (!क्रमादेशः_मार्गः)
    क्रमादेशः_मार्गः = xstrdup (PKGLIBDIR);  /* Need to init to non-null.  */
  समुच्चयः_LD_PRELOAD ();
  मुक्त (क्रमादेशः_मार्गः);

  execvp (*argv, argv);

  पूर्णाङ्क exit_स्थितिः = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  प्रत्यागम exit_स्थितिः;
}
