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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>

#ضمن "system.h"
#ضمن "assure.h"
#ضمن "filenamecat.h"
#ضمن "quote.h"
#ضمن "xreadlink.h"
#ضمن "xstrtol-error.h"
#ضمن "xstrtol.h"
#ضمن "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کړه پروګرام_NAME "stdbuf"
#تعریف_کړه LIB_NAME "libstdbuf.so" /* FIXME: don't hardcode  */

#تعریف_کړه AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

ساكن حرف *پروګرام_لار;

ساكن بنية
{
  size_t اندازه;
  صحيح optc;
  حرف *optarg;
} stdbuf[3];

ساكن بنية option ثابت longopts[] =
{
  {"input", required_دلیل, عدم, 'i'},
  {"output", required_دلیل, عدم, 'o'},
  {"error", required_دلیل, عدم, 'e'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Set size to the value of STR, interpreted as a decimal integer,
   optionally multiplied by various values.
   Return LONGINT_OK on success, otherwise an error for use with
   xstrtol_fatal.

   This supports dd BLOCK size suffixes.
   Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats.  */
ساكن تعداد strtol_error
parse_اندازه (حرف ثابت *str, size_t *اندازه)
{
  uintmax_t tmp_اندازه;
  تعداد strtol_error e = xstrtoumax (str, عدم, 10,
                                    &tmp_اندازه, "EGkKMPQRTYZ0");
  إذا (e == LONGINT_OK && ckd_جمع_کړه (اندازه, tmp_اندازه, 0))
    e = LONGINT_ډکېدنه;
  أرجع e;
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("Usage: %s OPTION... COMMAND\n"), پروګرام_name);
      fputs (_("\
پرلپسې COMMAND, with modified buffering operations لكل its standard streams.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --ننوت=ډېرپېښ   adjust standard ننوت بهیر buffering\n\
"));
      oputs (_("\
  -o, --output=ډېرپېښ  adjust standard output بهیر buffering\n\
"));
      oputs (_("\
  -e, --error=ډېرپېښ   adjust standard error بهیر buffering\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\n\
If ډېرپېښ is 'L' the corresponding بهیر will be کرښه buffered.\n\
This option is invalid with standard ننوت.\n"), stdout);
      fputs (_("\n\
If ډېرپېښ is '0' the corresponding بهیر will be unbuffered.\n\
"), stdout);
      fputs (_("\n\
Otherwise ډېرپېښ is a شمېره which may be followed by one د the following:\n\
KB 1000, K 1024, MB 1000*1000, M 1024*1024, and so on لكل G,T,P,E,Z,Y,R,Q.\n\
دوګونی_2 prefixes can be کارول_شوی, too: KiB=K, MiB=M, and so on.\n\
دننه_2 this حالة the corresponding بهیر will be fully buffered with the بفر\n\
اندازه سېټ_2 منزل ډېرپېښ بایټونه.\n\
"), stdout);
      fputs (_("\n\
NOTE: If COMMAND adjusts the buffering د its standard streams ('tee' does\n\
لكل example) then that will override corresponding changes by 'stdbuf'.\n\
Also some filters (like 'dd' and 'cat' etc.) don't use streams لكل I/O,\n\
and are thus unaffected by 'stdbuf' settings.\n\
"), stdout);
      emit_exec_حالت (پروګرام_NAME);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
}

/* argv[0] can be anything really, but generally it contains
   the path to the executable or just a name if it was executed
   using $PATH. In the latter case to get the path we can:
   search getenv("PATH"), readlink("/prof/self/exe"), getenv("_"),
   dladdr(), pstat_getpathname(), etc.  */

ساكن فراغ
سېټ_پروګرام_لار (حرف ثابت *arg)
{
  إذا (strchr (arg, '/'))        /* Use absolute or relative paths directly.  */
    {
      پروګرام_لار = dir_name (arg);
    }
  وإلا
    {
      حرف *لار = xreadlink ("/proc/self/exe");
      إذا (لار)
        پروګرام_لار = dir_name (لار);
      وإلا إذا ((لار = getenv ("PATH")))
        {
          لار = xstrdup (لار);
          لكل (حرف *dir = strtok (لار, ":"); dir != عدم;
               dir = strtok (عدم, ":"))
            {
              حرف *candidate = file_name_concat (dir, arg, عدم);
              إذا (access (candidate, X_OK) == 0)
                {
                  پروګرام_لار = dir_name (candidate);
                  حرر (candidate);
                  اكسر;
                }
              حرر (candidate);
            }
        }
      حرر (لار);
    }
}

ساكن صحيح
optc_منزل_fileno (صحيح c)
{
  صحيح ret = -1;

  بدل (c)
    {
    حالة 'e':
      ret = STDERR_FILENO;
      اكسر;
    حالة 'i':
      ret = STDIN_FILENO;
      اكسر;
    حالة 'o':
      ret = STDOUT_FILENO;
      اكسر;
    افتراضي:
      affirm (false);
    }

  أرجع ret;
}

ساكن فراغ
سېټ_LD_PRELOAD (فراغ)
{
  صحيح ret;
#که_تعریف_شوی __APPLE__
  حرف ثابت *preload_env = "DYLD_INSERT_LIBRARIES";
#وإلاإذا defined _AIX
  حرف ثابت *preload_env = (حجم (فراغ *) < 8
                             ? "LDR_PRELOAD" : "LDR_PRELOAD64");
#وإلا
  حرف ثابت *preload_env = "LD_PRELOAD";
#د_که_پای
  حرف *old_libs = getenv (preload_env);
  حرف *LD_PRELOAD;

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
  حرف ثابت *ثابت پلټنه_لار[] = {
    پروګرام_لار,
    PKGLIBEXECDIR,
    عدم
  };

  حرف ثابت *ثابت *لار = پلټنه_لار;
  حرف *libstdbuf;

  طالما (true)
    {
      بنية stat sb;

      إذا (!**لار)              /* system default  */
        {
          libstdbuf = xstrdup (LIB_NAME);
          اكسر;
        }
      ret = asprintf (&libstdbuf, "%s/%s", *لار, LIB_NAME);
      إذا (ret < 0)
        xalloc_die ();
      إذا (stat (libstdbuf, &sb) == 0)   /* file_exists  */
        اكسر;
      حرر (libstdbuf);

      ++لار;
      إذا ( ! *لار)
        error (EXIT_CANCELED, 0, _("failed to find %s"), quote (LIB_NAME));
    }

  /* FIXME: Do we need to support libstdbuf.dll, c:, '\' separators etc?  */

  إذا (old_libs)
    ret = asprintf (&LD_PRELOAD, "%s=%s:%s", preload_env, old_libs, libstdbuf);
  وإلا
    ret = asprintf (&LD_PRELOAD, "%s=%s", preload_env, libstdbuf);

  إذا (ret < 0)
    xalloc_die ();

  حرر (libstdbuf);

  ret = putenv (LD_PRELOAD);
#که_تعریف_شوی __APPLE__
  إذا (ret == 0)
    ret = setenv ("DYLD_FORCE_FLAT_NAMESPACE", "y", 1);
#د_که_پای

  إذا (ret != 0)
    error (EXIT_CANCELED, errno,
           _("failed to update the environment with %s"),
           quote (LD_PRELOAD));
}

/* Populate environ with _STDBUF_I=$MODE _STDBUF_O=$MODE _STDBUF_E=$MODE.
   Return TRUE if any environment variables set.   */

ساكن bool
سېټ_libstdbuf_options (فراغ)
{
  bool env_سېټ = false;

  لكل (size_t i = 0; i < countof (stdbuf); i++)
    {
      إذا (stdbuf[i].optarg)
        {
          حرف *var;
          صحيح ret;

          إذا (*stdbuf[i].optarg == 'L')
            ret = asprintf (&var, "%s%c=L", "_STDBUF_",
                            c_toupper (stdbuf[i].optc));
          وإلا
            ret = asprintf (&var, "%s%c=%zu", "_STDBUF_",
                            c_toupper (stdbuf[i].optc),
                            stdbuf[i].اندازه);
          إذا (ret < 0)
            xalloc_die ();

          إذا (putenv (var) != 0)
            error (EXIT_CANCELED, errno,
                   _("failed to update the environment with %s"),
                   quote (var));

          env_سېټ = true;
        }
    }

  أرجع env_سېټ;
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  صحيح c;

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  طالما ((c = getopt_long (argc, argv, "+i:o:e:", longopts, عدم)) != -1)
    {
      صحيح ثابت oi = -1;
      صحيح opt_fileno;

      بدل (c)
        {
        /* Old McDonald had a farm ei...  */
        حالة 'e':
        حالة 'i':
        حالة 'o':
          opt_fileno = optc_منزل_fileno (c);
          affirm (0 <= opt_fileno && opt_fileno < countof (stdbuf));
          stdbuf[opt_fileno].optc = c;
          طالما (c_isspace (*optarg))
            optarg++;
          stdbuf[opt_fileno].optarg = optarg;
          إذا (c == 'i' && *optarg == 'L')
            {
              /* -oL will be by far the most common use of this utility,
                 but one could easily think -iL might have the same affect,
                 so disallow it as it could be confusing.  */
              error (0, 0, _("line buffering standard input is meaningless"));
              usage (EXIT_CANCELED);
            }

          إذا (!streq (optarg, "L"))
            {
              تعداد strtol_error e = parse_اندازه (optarg,
                                                &stdbuf[opt_fileno].اندازه);
              إذا (e != LONGINT_OK)
                xstrtol_fatal (e, oi, c, longopts, optarg);
            }

          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_CANCELED);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify at least 1 command.  */
  إذا (argc < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  إذا (! سېټ_libstdbuf_options ())
    {
      error (0, 0, _("you must specify a buffering mode option"));
      usage (EXIT_CANCELED);
    }

  /* Try to preload libstdbuf first from the same path as
     stdbuf is running from.  */
  سېټ_پروګرام_لار (پروګرام_name);
  إذا (!پروګرام_لار)
    پروګرام_لار = xstrdup (PKGLIBDIR);  /* Need to init to non-null.  */
  سېټ_LD_PRELOAD ();
  حرر (پروګرام_لار);

  execvp (*argv, argv);

  صحيح exit_حالت = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  أرجع exit_حالت;
}
