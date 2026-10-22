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

#شامل <config.h>
#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "assure.h"
#شامل "filenamecat.h"
#شامل "quote.h"
#شامل "xreadlink.h"
#شامل "xstrtol-error.h"
#شامل "xstrtol.h"
#شامل "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "stdbuf"
#define LIB_NAME "libstdbuf.so" /* FIXME: don't hardcode  */

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

ایستا نویسه *program_path;

ایستا ساختار
{
  size_t size;
  صحیح optc;
  نویسه *optarg;
} stdbuf[3];

ایستا ساختار option ثابت longopts[] =
{
  {"input", required_argument, پوچ, 'i'},
  {"output", required_argument, پوچ, 'o'},
  {"error", required_argument, پوچ, 'e'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

/* Set size to the value of STR, interpreted as a decimal integer,
   optionally multiplied by various values.
   Return LONGINT_OK on success, otherwise an error for use with
   xstrtol_fatal.

   This supports dd BLOCK size suffixes.
   Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats.  */
ایستا شمارش strtol_error
parse_size (نویسه ثابت *str, size_t *size)
{
  uintmax_t tmp_size;
  شمارش strtol_error e = xstrtoumax (str, پوچ, 10,
                                    &tmp_size, "EGkKMPQRTYZ0");
  اگر (e == LONGINT_OK && ckd_add (size, tmp_size, 0))
    e = LONGINT_OVERFLOW;
  بازگشت e;
}

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("Usage: %s OPTION... COMMAND\n"), program_name);
      fputs (_("\
Run COMMAND, with modified buffering operations برای its standard streams.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --درون_داد=MODE   adjust standard درون_داد stream buffering\n\
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
If MODE is 'L' the corresponding stream will be سطر buffered.\n\
This option is invalid with standard درون_داد.\n"), stdout);
      fputs (_("\n\
If MODE is '0' the corresponding stream will be unbuffered.\n\
"), stdout);
      fputs (_("\n\
Otherwise MODE is a عدد which may be followed by one of the following:\n\
KB 1000, K 1024, MB 1000*1000, M 1024*1024, and so on برای G,T,P,E,Z,Y,R,Q.\n\
دودویی_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
In this حالت the corresponding stream will be fully buffered with the buffer\n\
size set to MODE bytes.\n\
"), stdout);
      fputs (_("\n\
NOTE: If COMMAND adjusts the buffering of its standard streams ('tee' does\n\
برای example) then that will override corresponding changes by 'stdbuf'.\n\
Also some filters (like 'dd' and 'cat' etc.) don't use streams برای I/O,\n\
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

ایستا تهی
set_program_path (نویسه ثابت *arg)
{
  اگر (strchr (arg, '/'))        /* Use absolute or relative paths directly.  */
    {
      program_path = dir_name (arg);
    }
  وگرنه
    {
      نویسه *path = xreadlink ("/proc/self/exe");
      اگر (path)
        program_path = dir_name (path);
      وگرنه اگر ((path = getenv ("PATH")))
        {
          path = xstrdup (path);
          برای (نویسه *dir = strtok (path, ":"); dir != پوچ;
               dir = strtok (پوچ, ":"))
            {
              نویسه *candidate = file_name_concat (dir, arg, پوچ);
              اگر (access (candidate, X_OK) == 0)
                {
                  program_path = dir_name (candidate);
                  آزاد (candidate);
                  بشکن;
                }
              آزاد (candidate);
            }
        }
      آزاد (path);
    }
}

ایستا صحیح
optc_to_fileno (صحیح c)
{
  صحیح ret = -1;

  انتخاب (c)
    {
    حالت 'e':
      ret = STDERR_FILENO;
      بشکن;
    حالت 'i':
      ret = STDIN_FILENO;
      بشکن;
    حالت 'o':
      ret = STDOUT_FILENO;
      بشکن;
    پیشفرض:
      affirm (false);
    }

  بازگشت ret;
}

ایستا تهی
set_LD_PRELOAD (تهی)
{
  صحیح ret;
#ifdef __APPLE__
  نویسه ثابت *preload_env = "DYLD_INSERT_LIBRARIES";
#وگرنهاگر defined _AIX
  نویسه ثابت *preload_env = (اندازه (تهی *) < 8
                             ? "LDR_PRELOAD" : "LDR_PRELOAD64");
#وگرنه
  نویسه ثابت *preload_env = "LD_PRELOAD";
#endif
  نویسه *old_libs = getenv (preload_env);
  نویسه *LD_PRELOAD;

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
  نویسه ثابت *ثابت جستجو_path[] = {
    program_path,
    PKGLIBEXECDIR,
    پوچ
  };

  نویسه ثابت *ثابت *path = جستجو_path;
  نویسه *libstdbuf;

  وقتی (true)
    {
      ساختار stat sb;

      اگر (!**path)              /* system default  */
        {
          libstdbuf = xstrdup (LIB_NAME);
          بشکن;
        }
      ret = asprintf (&libstdbuf, "%s/%s", *path, LIB_NAME);
      اگر (ret < 0)
        xalloc_die ();
      اگر (stat (libstdbuf, &sb) == 0)   /* file_exists  */
        بشکن;
      آزاد (libstdbuf);

      ++path;
      اگر ( ! *path)
        error (EXIT_CANCELED, 0, _("failed to find %s"), quote (LIB_NAME));
    }

  /* FIXME: Do we need to support libstdbuf.dll, c:, '\' separators etc?  */

  اگر (old_libs)
    ret = asprintf (&LD_PRELOAD, "%s=%s:%s", preload_env, old_libs, libstdbuf);
  وگرنه
    ret = asprintf (&LD_PRELOAD, "%s=%s", preload_env, libstdbuf);

  اگر (ret < 0)
    xalloc_die ();

  آزاد (libstdbuf);

  ret = putenv (LD_PRELOAD);
#ifdef __APPLE__
  اگر (ret == 0)
    ret = setenv ("DYLD_FORCE_FLAT_NAMESPACE", "y", 1);
#endif

  اگر (ret != 0)
    error (EXIT_CANCELED, errno,
           _("failed to update the environment with %s"),
           quote (LD_PRELOAD));
}

/* Populate environ with _STDBUF_I=$MODE _STDBUF_O=$MODE _STDBUF_E=$MODE.
   Return TRUE if any environment variables set.   */

ایستا bool
set_libstdbuf_options (تهی)
{
  bool env_set = false;

  برای (size_t i = 0; i < countof (stdbuf); i++)
    {
      اگر (stdbuf[i].optarg)
        {
          نویسه *var;
          صحیح ret;

          اگر (*stdbuf[i].optarg == 'L')
            ret = asprintf (&var, "%s%c=L", "_STDBUF_",
                            c_toupper (stdbuf[i].optc));
          وگرنه
            ret = asprintf (&var, "%s%c=%zu", "_STDBUF_",
                            c_toupper (stdbuf[i].optc),
                            stdbuf[i].size);
          اگر (ret < 0)
            xalloc_die ();

          اگر (putenv (var) != 0)
            error (EXIT_CANCELED, errno,
                   _("failed to update the environment with %s"),
                   quote (var));

          env_set = true;
        }
    }

  بازگشت env_set;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  صحیح c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  وقتی ((c = getopt_long (argc, argv, "+i:o:e:", longopts, پوچ)) != -1)
    {
      صحیح ثابت oi = -1;
      صحیح opt_fileno;

      انتخاب (c)
        {
        /* Old McDonald had a farm ei...  */
        حالت 'e':
        حالت 'i':
        حالت 'o':
          opt_fileno = optc_to_fileno (c);
          affirm (0 <= opt_fileno && opt_fileno < countof (stdbuf));
          stdbuf[opt_fileno].optc = c;
          وقتی (c_isspace (*optarg))
            optarg++;
          stdbuf[opt_fileno].optarg = optarg;
          اگر (c == 'i' && *optarg == 'L')
            {
              /* -oL will be by far the most common use of this utility,
                 but one could easily think -iL might have the same affect,
                 so disallow it as it could be confusing.  */
              error (0, 0, _("line buffering standard input is meaningless"));
              usage (EXIT_CANCELED);
            }

          اگر (!streq (optarg, "L"))
            {
              شمارش strtol_error e = parse_size (optarg,
                                                &stdbuf[opt_fileno].size);
              اگر (e != LONGINT_OK)
                xstrtol_fatal (e, oi, c, longopts, optarg);
            }

          بشکن;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (EXIT_CANCELED);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify at least 1 command.  */
  اگر (argc < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  اگر (! set_libstdbuf_options ())
    {
      error (0, 0, _("you must specify a buffering mode option"));
      usage (EXIT_CANCELED);
    }

  /* Try to preload libstdbuf first from the same path as
     stdbuf is running from.  */
  set_program_path (program_name);
  اگر (!program_path)
    program_path = xstrdup (PKGLIBDIR);  /* Need to init to non-null.  */
  set_LD_PRELOAD ();
  آزاد (program_path);

  execvp (*argv, argv);

  صحیح exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  بازگشت exit_status;
}
