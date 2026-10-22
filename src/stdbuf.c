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
#تعریف_کرو پروگرام_NAME "stdbuf"
#تعریف_کرو LIB_NAME "libstdbuf.so" /* FIXME: don't hardcode  */

#تعریف_کرو AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

ساكن حرف *پروگرام_رستہ;

ساكن بنية
{
  size_t ناپ;
  صحيح optc;
  حرف *optarg;
} stdbuf[3];

ساكن بنية option ثابت longopts[] =
{
  {"input", required_دلیل, عدم, 'i'},
  {"output", required_دلیل, عدم, 'o'},
  {"error", required_دلیل, عدم, 'e'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Set size to the value of STR, interpreted as a decimal integer,
   optionally multiplied by various values.
   Return LONGINT_OK on success, otherwise an error for use with
   xstrtol_fatal.

   This supports dd BLOCK size suffixes.
   Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats.  */
ساكن تعداد strtol_error
parse_ناپ (حرف ثابت *str, size_t *ناپ)
{
  uintmax_t tmp_ناپ;
  تعداد strtol_error e = xstrtoumax (str, عدم, 10,
                                    &tmp_ناپ, "EGkKMPQRTYZ0");
  إذا (e == LONGINT_OK && ckd_جوڑو (ناپ, tmp_ناپ, 0))
    e = LONGINT_حد_توں_ودھ;
  أرجع e;
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("Usage: %s OPTION... COMMAND\n"), پروگرام_name);
      fputs (_("\
لگاتار COMMAND, with modified buffering operations لكل its standard streams.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --داخلہ=منوال   adjust standard داخلہ دھارا buffering\n\
"));
      oputs (_("\
  -o, --output=منوال  adjust standard output دھارا buffering\n\
"));
      oputs (_("\
  -e, --error=منوال   adjust standard error دھارا buffering\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\n\
If منوال is 'L' the corresponding دھارا will be سطر buffered.\n\
This option is invalid with standard داخلہ.\n"), stdout);
      fputs (_("\n\
If منوال is '0' the corresponding دھارا will be unbuffered.\n\
"), stdout);
      fputs (_("\n\
Otherwise منوال is a نمبر which may be followed by one دا the following:\n\
KB 1000, K 1024, MB 1000*1000, M 1024*1024, and so on لكل G,T,P,E,Z,Y,R,Q.\n\
ثنائی_2 prefixes can be ورتیا, too: KiB=K, MiB=M, and so on.\n\
وچ_2 this حالة the corresponding دھارا will be fully buffered with the بفر\n\
ناپ سیٹ_2 منزل منوال بائٹاں.\n\
"), stdout);
      fputs (_("\n\
NOTE: If COMMAND adjusts the buffering دا its standard streams ('tee' does\n\
لكل example) then that will override corresponding changes by 'stdbuf'.\n\
Also some filters (like 'dd' and 'cat' etc.) don't use streams لكل I/O,\n\
and are thus unaffected by 'stdbuf' settings.\n\
"), stdout);
      emit_exec_حالت (پروگرام_NAME);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

/* argv[0] can be anything really, but generally it contains
   the path to the executable or just a name if it was executed
   using $PATH. In the latter case to get the path we can:
   search getenv("PATH"), readlink("/prof/self/exe"), getenv("_"),
   dladdr(), pstat_getpathname(), etc.  */

ساكن فراغ
سیٹ_پروگرام_رستہ (حرف ثابت *arg)
{
  إذا (strchr (arg, '/'))        /* Use absolute or relative paths directly.  */
    {
      پروگرام_رستہ = dir_name (arg);
    }
  وإلا
    {
      حرف *رستہ = xreadlink ("/proc/self/exe");
      إذا (رستہ)
        پروگرام_رستہ = dir_name (رستہ);
      وإلا إذا ((رستہ = getenv ("PATH")))
        {
          رستہ = xstrdup (رستہ);
          لكل (حرف *dir = strtok (رستہ, ":"); dir != عدم;
               dir = strtok (عدم, ":"))
            {
              حرف *candidate = file_name_concat (dir, arg, عدم);
              إذا (access (candidate, X_OK) == 0)
                {
                  پروگرام_رستہ = dir_name (candidate);
                  حرر (candidate);
                  اكسر;
                }
              حرر (candidate);
            }
        }
      حرر (رستہ);
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
سیٹ_LD_PRELOAD (فراغ)
{
  صحيح ret;
#جے_تعریف_شدہ __APPLE__
  حرف ثابت *preload_env = "DYLD_INSERT_LIBRARIES";
#وإلاإذا defined _AIX
  حرف ثابت *preload_env = (حجم (فراغ *) < 8
                             ? "LDR_PRELOAD" : "LDR_PRELOAD64");
#وإلا
  حرف ثابت *preload_env = "LD_PRELOAD";
#جے_انت
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
  حرف ثابت *ثابت تلاش_رستہ[] = {
    پروگرام_رستہ,
    PKGLIBEXECDIR,
    عدم
  };

  حرف ثابت *ثابت *رستہ = تلاش_رستہ;
  حرف *libstdbuf;

  طالما (true)
    {
      بنية stat sb;

      إذا (!**رستہ)              /* system default  */
        {
          libstdbuf = xstrdup (LIB_NAME);
          اكسر;
        }
      ret = asprintf (&libstdbuf, "%s/%s", *رستہ, LIB_NAME);
      إذا (ret < 0)
        xalloc_die ();
      إذا (stat (libstdbuf, &sb) == 0)   /* file_exists  */
        اكسر;
      حرر (libstdbuf);

      ++رستہ;
      إذا ( ! *رستہ)
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
#جے_تعریف_شدہ __APPLE__
  إذا (ret == 0)
    ret = setenv ("DYLD_FORCE_FLAT_NAMESPACE", "y", 1);
#جے_انت

  إذا (ret != 0)
    error (EXIT_CANCELED, errno,
           _("failed to update the environment with %s"),
           quote (LD_PRELOAD));
}

/* Populate environ with _STDBUF_I=$MODE _STDBUF_O=$MODE _STDBUF_E=$MODE.
   Return TRUE if any environment variables set.   */

ساكن bool
سیٹ_libstdbuf_options (فراغ)
{
  bool env_سیٹ = false;

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
                            stdbuf[i].ناپ);
          إذا (ret < 0)
            xalloc_die ();

          إذا (putenv (var) != 0)
            error (EXIT_CANCELED, errno,
                   _("failed to update the environment with %s"),
                   quote (var));

          env_سیٹ = true;
        }
    }

  أرجع env_سیٹ;
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  صحيح c;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
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
              تعداد strtol_error e = parse_ناپ (optarg,
                                                &stdbuf[opt_fileno].ناپ);
              إذا (e != LONGINT_OK)
                xstrtol_fatal (e, oi, c, longopts, optarg);
            }

          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

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

  إذا (! سیٹ_libstdbuf_options ())
    {
      error (0, 0, _("you must specify a buffering mode option"));
      usage (EXIT_CANCELED);
    }

  /* Try to preload libstdbuf first from the same path as
     stdbuf is running from.  */
  سیٹ_پروگرام_رستہ (پروگرام_name);
  إذا (!پروگرام_رستہ)
    پروگرام_رستہ = xstrdup (PKGLIBDIR);  /* Need to init to non-null.  */
  سیٹ_LD_PRELOAD ();
  حرر (پروگرام_رستہ);

  execvp (*argv, argv);

  صحيح exit_حالت = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  أرجع exit_حالت;
}
