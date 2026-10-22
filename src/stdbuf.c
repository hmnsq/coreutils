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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "filenamecat.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "xreadlink.h"
#অন্তর্ভাব "xstrtol-error.h"
#অন্তর্ভাব "xstrtol.h"
#অন্তর্ভাব "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "stdbuf"
#define LIB_NAME "libstdbuf.so" /* FIXME: don't hardcode  */

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

স্থির অক্ষর *program_path;

স্থির কাঠামো
{
  size_t আকার_2;
  পূর্ণসংখ্যা optc;
  অক্ষর *optarg;
} stdbuf[3];

স্থির কাঠামো option ধ্রুবক longopts[] =
{
  {"input", required_argument, ফাঁকা, 'i'},
  {"output", required_argument, ফাঁকা, 'o'},
  {"error", required_argument, ফাঁকা, 'e'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

/* Set size to the value of STR, interpreted as a decimal integer,
   optionally multiplied by various values.
   Return LONGINT_OK on success, otherwise an error for use with
   xstrtol_fatal.

   This supports dd BLOCK size suffixes.
   Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats.  */
স্থির গণনা strtol_error
parse_আকার (অক্ষর ধ্রুবক *str, size_t *আকার_2)
{
  uintmax_t tmp_আকার;
  গণনা strtol_error e = xstrtoumax (str, ফাঁকা, 10,
                                    &tmp_আকার, "EGkKMPQRTYZ0");
  যদি (e == LONGINT_OK && ckd_add (আকার_2, tmp_আকার, 0))
    e = LONGINT_OVERFLOW;
  ফেরত e;
}

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s OPTION... COMMAND\n"), program_name);
      fputs (_("\
Run COMMAND, with modified buffering operations জন্য its standard streams.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --প্রবেশ=MODE   adjust standard প্রবেশ stream buffering\n\
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
If MODE is 'L' the corresponding stream will be সারি buffered.\n\
This option is invalid with standard প্রবেশ.\n"), stdout);
      fputs (_("\n\
If MODE is '0' the corresponding stream will be unbuffered.\n\
"), stdout);
      fputs (_("\n\
Otherwise MODE is a সংখ্যা which may be followed by one of the following:\n\
KB 1000, K 1024, MB 1000*1000, M 1024*1024, and so on জন্য G,T,P,E,Z,Y,R,Q.\n\
দ্বিমিক_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
In this ক্ষেত্র the corresponding stream will be fully buffered with the বাফার\n\
আকার_2 set to MODE bytes.\n\
"), stdout);
      fputs (_("\n\
NOTE: If COMMAND adjusts the buffering of its standard streams ('tee' does\n\
জন্য example) then that will override corresponding changes by 'stdbuf'.\n\
Also some filters (like 'dd' and 'cat' etc.) don't use streams জন্য I/O,\n\
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

স্থির শূন্য
set_program_path (অক্ষর ধ্রুবক *arg)
{
  যদি (strchr (arg, '/'))        /* Use absolute or relative paths directly.  */
    {
      program_path = dir_name (arg);
    }
  অন্যথা
    {
      অক্ষর *path = xreadlink ("/proc/self/exe");
      যদি (path)
        program_path = dir_name (path);
      অন্যথা যদি ((path = getenv ("PATH")))
        {
          path = xstrdup (path);
          জন্য (অক্ষর *dir = strtok (path, ":"); dir != ফাঁকা;
               dir = strtok (ফাঁকা, ":"))
            {
              অক্ষর *candidate = file_name_concat (dir, arg, ফাঁকা);
              যদি (access (candidate, X_OK) == 0)
                {
                  program_path = dir_name (candidate);
                  মুক্ত (candidate);
                  বিরতি;
                }
              মুক্ত (candidate);
            }
        }
      মুক্ত (path);
    }
}

স্থির পূর্ণসংখ্যা
optc_to_fileno (পূর্ণসংখ্যা c)
{
  পূর্ণসংখ্যা ret = -1;

  নির্বাচন (c)
    {
    ক্ষেত্র 'e':
      ret = STDERR_FILENO;
      বিরতি;
    ক্ষেত্র 'i':
      ret = STDIN_FILENO;
      বিরতি;
    ক্ষেত্র 'o':
      ret = STDOUT_FILENO;
      বিরতি;
    পূর্বনির্ধারিত:
      affirm (false);
    }

  ফেরত ret;
}

স্থির শূন্য
set_LD_PRELOAD (শূন্য)
{
  পূর্ণসংখ্যা ret;
#ifdef __APPLE__
  অক্ষর ধ্রুবক *preload_env = "DYLD_INSERT_LIBRARIES";
#নতুবা_যদি defined _AIX
  অক্ষর ধ্রুবক *preload_env = (আকার (শূন্য *) < 8
                             ? "LDR_PRELOAD" : "LDR_PRELOAD64");
#অন্যথা
  অক্ষর ধ্রুবক *preload_env = "LD_PRELOAD";
#endif
  অক্ষর *old_libs = getenv (preload_env);
  অক্ষর *LD_PRELOAD;

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
  অক্ষর ধ্রুবক *ধ্রুবক অনুসন্ধান_path[] = {
    program_path,
    PKGLIBEXECDIR,
    ফাঁকা
  };

  অক্ষর ধ্রুবক *ধ্রুবক *path = অনুসন্ধান_path;
  অক্ষর *libstdbuf;

  যখন (true)
    {
      কাঠামো stat sb;

      যদি (!**path)              /* system default  */
        {
          libstdbuf = xstrdup (LIB_NAME);
          বিরতি;
        }
      ret = asprintf (&libstdbuf, "%s/%s", *path, LIB_NAME);
      যদি (ret < 0)
        xalloc_die ();
      যদি (stat (libstdbuf, &sb) == 0)   /* file_exists  */
        বিরতি;
      মুক্ত (libstdbuf);

      ++path;
      যদি ( ! *path)
        error (EXIT_CANCELED, 0, _("failed to find %s"), quote (LIB_NAME));
    }

  /* FIXME: Do we need to support libstdbuf.dll, c:, '\' separators etc?  */

  যদি (old_libs)
    ret = asprintf (&LD_PRELOAD, "%s=%s:%s", preload_env, old_libs, libstdbuf);
  অন্যথা
    ret = asprintf (&LD_PRELOAD, "%s=%s", preload_env, libstdbuf);

  যদি (ret < 0)
    xalloc_die ();

  মুক্ত (libstdbuf);

  ret = putenv (LD_PRELOAD);
#ifdef __APPLE__
  যদি (ret == 0)
    ret = setenv ("DYLD_FORCE_FLAT_NAMESPACE", "y", 1);
#endif

  যদি (ret != 0)
    error (EXIT_CANCELED, errno,
           _("failed to update the environment with %s"),
           quote (LD_PRELOAD));
}

/* Populate environ with _STDBUF_I=$MODE _STDBUF_O=$MODE _STDBUF_E=$MODE.
   Return TRUE if any environment variables set.   */

স্থির bool
set_libstdbuf_options (শূন্য)
{
  bool env_set = false;

  জন্য (size_t i = 0; i < countof (stdbuf); i++)
    {
      যদি (stdbuf[i].optarg)
        {
          অক্ষর *var;
          পূর্ণসংখ্যা ret;

          যদি (*stdbuf[i].optarg == 'L')
            ret = asprintf (&var, "%s%c=L", "_STDBUF_",
                            c_toupper (stdbuf[i].optc));
          অন্যথা
            ret = asprintf (&var, "%s%c=%zu", "_STDBUF_",
                            c_toupper (stdbuf[i].optc),
                            stdbuf[i].আকার_2);
          যদি (ret < 0)
            xalloc_die ();

          যদি (putenv (var) != 0)
            error (EXIT_CANCELED, errno,
                   _("failed to update the environment with %s"),
                   quote (var));

          env_set = true;
        }
    }

  ফেরত env_set;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  পূর্ণসংখ্যা c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  যখন ((c = getopt_long (argc, argv, "+i:o:e:", longopts, ফাঁকা)) != -1)
    {
      পূর্ণসংখ্যা ধ্রুবক oi = -1;
      পূর্ণসংখ্যা opt_fileno;

      নির্বাচন (c)
        {
        /* Old McDonald had a farm ei...  */
        ক্ষেত্র 'e':
        ক্ষেত্র 'i':
        ক্ষেত্র 'o':
          opt_fileno = optc_to_fileno (c);
          affirm (0 <= opt_fileno && opt_fileno < countof (stdbuf));
          stdbuf[opt_fileno].optc = c;
          যখন (c_isspace (*optarg))
            optarg++;
          stdbuf[opt_fileno].optarg = optarg;
          যদি (c == 'i' && *optarg == 'L')
            {
              /* -oL will be by far the most common use of this utility,
                 but one could easily think -iL might have the same affect,
                 so disallow it as it could be confusing.  */
              error (0, 0, _("line buffering standard input is meaningless"));
              usage (EXIT_CANCELED);
            }

          যদি (!streq (optarg, "L"))
            {
              গণনা strtol_error e = parse_আকার (optarg,
                                                &stdbuf[opt_fileno].আকার_2);
              যদি (e != LONGINT_OK)
                xstrtol_fatal (e, oi, c, longopts, optarg);
            }

          বিরতি;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        পূর্বনির্ধারিত:
          usage (EXIT_CANCELED);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify at least 1 command.  */
  যদি (argc < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  যদি (! set_libstdbuf_options ())
    {
      error (0, 0, _("you must specify a buffering mode option"));
      usage (EXIT_CANCELED);
    }

  /* Try to preload libstdbuf first from the same path as
     stdbuf is running from.  */
  set_program_path (program_name);
  যদি (!program_path)
    program_path = xstrdup (PKGLIBDIR);  /* Need to init to non-null.  */
  set_LD_PRELOAD ();
  মুক্ত (program_path);

  execvp (*argv, argv);

  পূর্ণসংখ্যা exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  ফেরত exit_status;
}
