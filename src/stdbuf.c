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
#وصف_ڪريو پروگرام_NAME "stdbuf"
#وصف_ڪريو LIB_NAME "libstdbuf.so" /* FIXME: don't hardcode  */

#وصف_ڪريو AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

ساكن حرف *پروگرام_رستو;

ساكن بنية
{
  size_t سائيز;
  صحيح optc;
  حرف *optarg;
} stdbuf[3];

ساكن بنية option ثابت longopts[] =
{
  {"input", required_دليل, عدم, 'i'},
  {"output", required_دليل, عدم, 'o'},
  {"error", required_دليل, عدم, 'e'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Set size to the value of STR, interpreted as a decimal integer,
   optionally multiplied by various values.
   Return LONGINT_OK on success, otherwise an error for use with
   xstrtol_fatal.

   This supports dd BLOCK size suffixes.
   Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats.  */
ساكن تعداد strtol_error
parse_سائيز (حرف ثابت *str, size_t *سائيز)
{
  uintmax_t tmp_سائيز;
  تعداد strtol_error e = xstrtoumax (str, عدم, 10,
                                    &tmp_سائيز, "EGkKMPQRTYZ0");
  إذا (e == LONGINT_OK && ckd_جوڙيو (سائيز, tmp_سائيز, 0))
    e = LONGINT_حد_کان_مٿي;
  أرجع e;
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("Usage: %s OPTION... COMMAND\n"), پروگرام_name);
      fputs (_("\
لاڳيتو COMMAND, with modified buffering operations لكل its standard streams.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --داخلا=گهڻائي   adjust standard داخلا وهڪرو buffering\n\
"));
      oputs (_("\
  -o, --output=گهڻائي  adjust standard output وهڪرو buffering\n\
"));
      oputs (_("\
  -e, --error=گهڻائي   adjust standard error وهڪرو buffering\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      fputs (_("\n\
If گهڻائي is 'L' the corresponding وهڪرو will be سٽ buffered.\n\
This option is invalid with standard داخلا.\n"), stdout);
      fputs (_("\n\
If گهڻائي is '0' the corresponding وهڪرو will be unbuffered.\n\
"), stdout);
      fputs (_("\n\
Otherwise گهڻائي is a عدد which may be followed by one جو the following:\n\
KB 1000, K 1024, MB 1000*1000, M 1024*1024, and so on لكل G,T,P,E,Z,Y,R,Q.\n\
ٻٽو_2 prefixes can be استعمال_ٿيل, too: KiB=K, MiB=M, and so on.\n\
اندريون_2 this حالة the corresponding وهڪرو will be fully buffered with the بفر\n\
سائيز سيٽ_2 منزل گهڻائي بائيٽون.\n\
"), stdout);
      fputs (_("\n\
NOTE: If COMMAND adjusts the buffering جو its standard streams ('tee' does\n\
لكل example) then that will override corresponding changes by 'stdbuf'.\n\
Also some filters (like 'dd' and 'cat' etc.) don't use streams لكل I/O,\n\
and are thus unaffected by 'stdbuf' settings.\n\
"), stdout);
      emit_exec_حالت (پروگرام_NAME);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

/* argv[0] can be anything really, but generally it contains
   the path to the executable or just a name if it was executed
   using $PATH. In the latter case to get the path we can:
   search getenv("PATH"), readlink("/prof/self/exe"), getenv("_"),
   dladdr(), pstat_getpathname(), etc.  */

ساكن فراغ
سيٽ_پروگرام_رستو (حرف ثابت *arg)
{
  إذا (strchr (arg, '/'))        /* Use absolute or relative paths directly.  */
    {
      پروگرام_رستو = dir_name (arg);
    }
  وإلا
    {
      حرف *رستو = xreadlink ("/proc/self/exe");
      إذا (رستو)
        پروگرام_رستو = dir_name (رستو);
      وإلا إذا ((رستو = getenv ("PATH")))
        {
          رستو = xstrdup (رستو);
          لكل (حرف *dir = strtok (رستو, ":"); dir != عدم;
               dir = strtok (عدم, ":"))
            {
              حرف *candidate = file_name_concat (dir, arg, عدم);
              إذا (access (candidate, X_OK) == 0)
                {
                  پروگرام_رستو = dir_name (candidate);
                  حرر (candidate);
                  اكسر;
                }
              حرر (candidate);
            }
        }
      حرر (رستو);
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
سيٽ_LD_PRELOAD (فراغ)
{
  صحيح ret;
#جيڪڏهن_وصف_ٿيل __APPLE__
  حرف ثابت *preload_env = "DYLD_INSERT_LIBRARIES";
#وإلاإذا defined _AIX
  حرف ثابت *preload_env = (حجم (فراغ *) < 8
                             ? "LDR_PRELOAD" : "LDR_PRELOAD64");
#وإلا
  حرف ثابت *preload_env = "LD_PRELOAD";
#جيڪڏهن_پورو
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
  حرف ثابت *ثابت ڳولا_رستو[] = {
    پروگرام_رستو,
    PKGLIBEXECDIR,
    عدم
  };

  حرف ثابت *ثابت *رستو = ڳولا_رستو;
  حرف *libstdbuf;

  طالما (true)
    {
      بنية stat sb;

      إذا (!**رستو)              /* system default  */
        {
          libstdbuf = xstrdup (LIB_NAME);
          اكسر;
        }
      ret = asprintf (&libstdbuf, "%s/%s", *رستو, LIB_NAME);
      إذا (ret < 0)
        xalloc_die ();
      إذا (stat (libstdbuf, &sb) == 0)   /* file_exists  */
        اكسر;
      حرر (libstdbuf);

      ++رستو;
      إذا ( ! *رستو)
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
#جيڪڏهن_وصف_ٿيل __APPLE__
  إذا (ret == 0)
    ret = setenv ("DYLD_FORCE_FLAT_NAMESPACE", "y", 1);
#جيڪڏهن_پورو

  إذا (ret != 0)
    error (EXIT_CANCELED, errno,
           _("failed to update the environment with %s"),
           quote (LD_PRELOAD));
}

/* Populate environ with _STDBUF_I=$MODE _STDBUF_O=$MODE _STDBUF_E=$MODE.
   Return TRUE if any environment variables set.   */

ساكن bool
سيٽ_libstdbuf_options (فراغ)
{
  bool env_سيٽ = false;

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
                            stdbuf[i].سائيز);
          إذا (ret < 0)
            xalloc_die ();

          إذا (putenv (var) != 0)
            error (EXIT_CANCELED, errno,
                   _("failed to update the environment with %s"),
                   quote (var));

          env_سيٽ = true;
        }
    }

  أرجع env_سيٽ;
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  صحيح c;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
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
              تعداد strtol_error e = parse_سائيز (optarg,
                                                &stdbuf[opt_fileno].سائيز);
              إذا (e != LONGINT_OK)
                xstrtol_fatal (e, oi, c, longopts, optarg);
            }

          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);

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

  إذا (! سيٽ_libstdbuf_options ())
    {
      error (0, 0, _("you must specify a buffering mode option"));
      usage (EXIT_CANCELED);
    }

  /* Try to preload libstdbuf first from the same path as
     stdbuf is running from.  */
  سيٽ_پروگرام_رستو (پروگرام_name);
  إذا (!پروگرام_رستو)
    پروگرام_رستو = xstrdup (PKGLIBDIR);  /* Need to init to non-null.  */
  سيٽ_LD_PRELOAD ();
  حرر (پروگرام_رستو);

  execvp (*argv, argv);

  صحيح exit_حالت = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  أرجع exit_حالت;
}
