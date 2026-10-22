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
#define PROGRAM_NAME "stdbuf"
#define LIB_NAME "libstdbuf.so" /* FIXME: don't hardcode  */

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

स्थिर वर्ण *program_path;

स्थिर संरचना
{
  size_t size;
  पूर्णाङ्क optc;
  वर्ण *optarg;
} stdbuf[3];

स्थिर संरचना option ध्रुव longopts[] =
{
  {"input", required_argument, रिक्त, 'i'},
  {"output", required_argument, रिक्त, 'o'},
  {"error", required_argument, रिक्त, 'e'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Set size to the value of STR, interpreted as a decimal integer,
   optionally multiplied by various values.
   Return LONGINT_OK on success, otherwise an error for use with
   xstrtol_fatal.

   This supports dd BLOCK size suffixes.
   Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats.  */
स्थिर गणना strtol_error
parse_size (वर्ण ध्रुव *str, size_t *size)
{
  uintmax_t tmp_size;
  गणना strtol_error e = xstrtoumax (str, रिक्त, 10,
                                    &tmp_size, "EGkKMPQRTYZ0");
  यदि (e == LONGINT_OK && ckd_add (size, tmp_size, 0))
    e = LONGINT_OVERFLOW;
  प्रत्यागम e;
}

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      printf (_("Usage: %s OPTION... COMMAND\n"), program_name);
      fputs (_("\
Run COMMAND, with modified buffering operations प्रति its standard streams.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --निवेश=MODE   adjust standard निवेश stream buffering\n\
"));
      oputs (_("\
  -o, --output=MODE  adjust standard output stream buffering\n\
"));
      oputs (_("\
  -e, --error=MODE   adjust standard error stream buffering\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\n\
If MODE is 'L' the corresponding stream will be पंक्ति buffered.\n\
This option is invalid with standard निवेश.\n"), stdout);
      fputs (_("\n\
If MODE is '0' the corresponding stream will be unbuffered.\n\
"), stdout);
      fputs (_("\n\
Otherwise MODE is a संख्या which may be followed by one of the following:\n\
KB 1000, K 1024, MB 1000*1000, M 1024*1024, and so on प्रति G,T,P,E,Z,Y,R,Q.\n\
द्विआधारी_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
In this पक्ष the corresponding stream will be fully buffered with the buffer\n\
size set to MODE bytes.\n\
"), stdout);
      fputs (_("\n\
NOTE: If COMMAND adjusts the buffering of its standard streams ('tee' does\n\
प्रति example) then that will override corresponding changes by 'stdbuf'.\n\
Also some filters (like 'dd' and 'cat' etc.) don't use streams प्रति I/O,\n\
and are thus unaffected by 'stdbuf' settings.\n\
"), stdout);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* argv[0] can be anything really, but generally it contains
   the path to the executable or just a name if it was executed
   using $PATH. In the latter case to get the path we can:
   search getenv("PATH"), readlink("/prof/self/exe"), getenv("_"),
   dladdr(), pstat_getpathname(), etc.  */

स्थिर शून्य
set_program_path (वर्ण ध्रुव *arg)
{
  यदि (strchr (arg, '/'))        /* Use absolute or relative paths directly.  */
    {
      program_path = dir_name (arg);
    }
  अन्यथा
    {
      वर्ण *path = xreadlink ("/proc/self/exe");
      यदि (path)
        program_path = dir_name (path);
      अन्यथा यदि ((path = getenv ("PATH")))
        {
          path = xstrdup (path);
          प्रति (वर्ण *dir = strtok (path, ":"); dir != रिक्त;
               dir = strtok (रिक्त, ":"))
            {
              वर्ण *candidate = file_name_concat (dir, arg, रिक्त);
              यदि (access (candidate, X_OK) == 0)
                {
                  program_path = dir_name (candidate);
                  मुक्त (candidate);
                  भङ्ग;
                }
              मुक्त (candidate);
            }
        }
      मुक्त (path);
    }
}

स्थिर पूर्णाङ्क
optc_to_fileno (पूर्णाङ्क c)
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
set_LD_PRELOAD (शून्य)
{
  पूर्णाङ्क ret;
#ifdef __APPLE__
  वर्ण ध्रुव *preload_env = "DYLD_INSERT_LIBRARIES";
#नोचेद्यदि defined _AIX
  वर्ण ध्रुव *preload_env = (परिमाण (शून्य *) < 8
                             ? "LDR_PRELOAD" : "LDR_PRELOAD64");
#अन्यथा
  वर्ण ध्रुव *preload_env = "LD_PRELOAD";
#endif
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
  वर्ण ध्रुव *ध्रुव खोज_path[] = {
    program_path,
    PKGLIBEXECDIR,
    रिक्त
  };

  वर्ण ध्रुव *ध्रुव *path = खोज_path;
  वर्ण *libstdbuf;

  यावत् (true)
    {
      संरचना stat sb;

      यदि (!**path)              /* system default  */
        {
          libstdbuf = xstrdup (LIB_NAME);
          भङ्ग;
        }
      ret = asprintf (&libstdbuf, "%s/%s", *path, LIB_NAME);
      यदि (ret < 0)
        xalloc_die ();
      यदि (stat (libstdbuf, &sb) == 0)   /* file_exists  */
        भङ्ग;
      मुक्त (libstdbuf);

      ++path;
      यदि ( ! *path)
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
#ifdef __APPLE__
  यदि (ret == 0)
    ret = setenv ("DYLD_FORCE_FLAT_NAMESPACE", "y", 1);
#endif

  यदि (ret != 0)
    error (EXIT_CANCELED, errno,
           _("failed to update the environment with %s"),
           quote (LD_PRELOAD));
}

/* Populate environ with _STDBUF_I=$MODE _STDBUF_O=$MODE _STDBUF_E=$MODE.
   Return TRUE if any environment variables set.   */

स्थिर bool
set_libstdbuf_options (शून्य)
{
  bool env_set = false;

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
                            stdbuf[i].size);
          यदि (ret < 0)
            xalloc_die ();

          यदि (putenv (var) != 0)
            error (EXIT_CANCELED, errno,
                   _("failed to update the environment with %s"),
                   quote (var));

          env_set = true;
        }
    }

  प्रत्यागम env_set;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
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
          opt_fileno = optc_to_fileno (c);
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
              गणना strtol_error e = parse_size (optarg,
                                                &stdbuf[opt_fileno].size);
              यदि (e != LONGINT_OK)
                xstrtol_fatal (e, oi, c, longopts, optarg);
            }

          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

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

  यदि (! set_libstdbuf_options ())
    {
      error (0, 0, _("you must specify a buffering mode option"));
      usage (EXIT_CANCELED);
    }

  /* Try to preload libstdbuf first from the same path as
     stdbuf is running from.  */
  set_program_path (program_name);
  यदि (!program_path)
    program_path = xstrdup (PKGLIBDIR);  /* Need to init to non-null.  */
  set_LD_PRELOAD ();
  मुक्त (program_path);

  execvp (*argv, argv);

  पूर्णाङ्क exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  प्रत्यागम exit_status;
}
