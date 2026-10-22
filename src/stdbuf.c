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
#تعریف_کرو پروگرام_NAME "stdbuf"
#تعریف_کرو LIB_NAME "libstdbuf.so" /* FIXME: don't hardcode  */

#تعریف_کرو AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

ساکن حرف *پروگرام_راستہ;

ساکن ساخت
{
  size_t سائز;
  صحیح optc;
  حرف *optarg;
} stdbuf[3];

ساکن ساخت option ثابت longopts[] =
{
  {"input", required_دلیل, تہی, 'i'},
  {"output", required_دلیل, تہی, 'o'},
  {"error", required_دلیل, تہی, 'e'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

/* Set size to the value of STR, interpreted as a decimal integer,
   optionally multiplied by various values.
   Return LONGINT_OK on success, otherwise an error for use with
   xstrtol_fatal.

   This supports dd BLOCK size suffixes.
   Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats.  */
ساکن شمار strtol_error
parse_سائز (حرف ثابت *str, size_t *سائز)
{
  uintmax_t tmp_سائز;
  شمار strtol_error e = xstrtoumax (str, تہی, 10,
                                    &tmp_سائز, "EGkKMPQRTYZ0");
  اگر (e == LONGINT_OK && ckd_جوڑو (سائز, tmp_سائز, 0))
    e = LONGINT_حد_سے_تجاوز;
  واپس e;
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s OPTION... COMMAND\n"), پروگرام_name);
      fputs (_("\
تسلسل COMMAND, with modified buffering operations برائے its standard streams.\n\
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
Otherwise منوال is a نمبر which may be followed by one کا the following:\n\
KB 1000, K 1024, MB 1000*1000, M 1024*1024, and so on برائے G,T,P,E,Z,Y,R,Q.\n\
ثنائی_2 prefixes can be مستعمل, too: KiB=K, MiB=M, and so on.\n\
اندر_2 this حالت the corresponding دھارا will be fully buffered with the بفر\n\
سائز سیٹ_2 مقصد منوال بائٹیں.\n\
"), stdout);
      fputs (_("\n\
NOTE: If COMMAND adjusts the buffering کا its standard streams ('tee' does\n\
برائے example) then that will override corresponding changes by 'stdbuf'.\n\
Also some filters (like 'dd' and 'cat' etc.) don't use streams برائے I/O,\n\
and are thus unaffected by 'stdbuf' settings.\n\
"), stdout);
      emit_exec_حالت (پروگرام_NAME);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* argv[0] can be anything really, but generally it contains
   the path to the executable or just a name if it was executed
   using $PATH. In the latter case to get the path we can:
   search getenv("PATH"), readlink("/prof/self/exe"), getenv("_"),
   dladdr(), pstat_getpathname(), etc.  */

ساکن خالی
سیٹ_پروگرام_راستہ (حرف ثابت *arg)
{
  اگر (strchr (arg, '/'))        /* Use absolute or relative paths directly.  */
    {
      پروگرام_راستہ = dir_name (arg);
    }
  ورنہ
    {
      حرف *راستہ = xreadlink ("/proc/self/exe");
      اگر (راستہ)
        پروگرام_راستہ = dir_name (راستہ);
      ورنہ اگر ((راستہ = getenv ("PATH")))
        {
          راستہ = xstrdup (راستہ);
          برائے (حرف *dir = strtok (راستہ, ":"); dir != تہی;
               dir = strtok (تہی, ":"))
            {
              حرف *candidate = file_name_concat (dir, arg, تہی);
              اگر (access (candidate, X_OK) == 0)
                {
                  پروگرام_راستہ = dir_name (candidate);
                  آزاد (candidate);
                  توڑو;
                }
              آزاد (candidate);
            }
        }
      آزاد (راستہ);
    }
}

ساکن صحیح
optc_مقصد_fileno (صحیح c)
{
  صحیح ret = -1;

  انتخاب (c)
    {
    حالت 'e':
      ret = STDERR_FILENO;
      توڑو;
    حالت 'i':
      ret = STDIN_FILENO;
      توڑو;
    حالت 'o':
      ret = STDOUT_FILENO;
      توڑو;
    طےشدہ:
      affirm (false);
    }

  واپس ret;
}

ساکن خالی
سیٹ_LD_PRELOAD (خالی)
{
  صحیح ret;
#اگر_تعریف_شدہ __APPLE__
  حرف ثابت *preload_env = "DYLD_INSERT_LIBRARIES";
#وگرنہ_اگر defined _AIX
  حرف ثابت *preload_env = (حجم (خالی *) < 8
                             ? "LDR_PRELOAD" : "LDR_PRELOAD64");
#ورنہ
  حرف ثابت *preload_env = "LD_PRELOAD";
#اگر_ختم
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
  حرف ثابت *ثابت تلاش_راستہ[] = {
    پروگرام_راستہ,
    PKGLIBEXECDIR,
    تہی
  };

  حرف ثابت *ثابت *راستہ = تلاش_راستہ;
  حرف *libstdbuf;

  جب (true)
    {
      ساخت stat sb;

      اگر (!**راستہ)              /* system default  */
        {
          libstdbuf = xstrdup (LIB_NAME);
          توڑو;
        }
      ret = asprintf (&libstdbuf, "%s/%s", *راستہ, LIB_NAME);
      اگر (ret < 0)
        xalloc_die ();
      اگر (stat (libstdbuf, &sb) == 0)   /* file_exists  */
        توڑو;
      آزاد (libstdbuf);

      ++راستہ;
      اگر ( ! *راستہ)
        error (EXIT_CANCELED, 0, _("failed to find %s"), quote (LIB_NAME));
    }

  /* FIXME: Do we need to support libstdbuf.dll, c:, '\' separators etc?  */

  اگر (old_libs)
    ret = asprintf (&LD_PRELOAD, "%s=%s:%s", preload_env, old_libs, libstdbuf);
  ورنہ
    ret = asprintf (&LD_PRELOAD, "%s=%s", preload_env, libstdbuf);

  اگر (ret < 0)
    xalloc_die ();

  آزاد (libstdbuf);

  ret = putenv (LD_PRELOAD);
#اگر_تعریف_شدہ __APPLE__
  اگر (ret == 0)
    ret = setenv ("DYLD_FORCE_FLAT_NAMESPACE", "y", 1);
#اگر_ختم

  اگر (ret != 0)
    error (EXIT_CANCELED, errno,
           _("failed to update the environment with %s"),
           quote (LD_PRELOAD));
}

/* Populate environ with _STDBUF_I=$MODE _STDBUF_O=$MODE _STDBUF_E=$MODE.
   Return TRUE if any environment variables set.   */

ساکن bool
سیٹ_libstdbuf_options (خالی)
{
  bool env_سیٹ = false;

  برائے (size_t i = 0; i < countof (stdbuf); i++)
    {
      اگر (stdbuf[i].optarg)
        {
          حرف *var;
          صحیح ret;

          اگر (*stdbuf[i].optarg == 'L')
            ret = asprintf (&var, "%s%c=L", "_STDBUF_",
                            c_toupper (stdbuf[i].optc));
          ورنہ
            ret = asprintf (&var, "%s%c=%zu", "_STDBUF_",
                            c_toupper (stdbuf[i].optc),
                            stdbuf[i].سائز);
          اگر (ret < 0)
            xalloc_die ();

          اگر (putenv (var) != 0)
            error (EXIT_CANCELED, errno,
                   _("failed to update the environment with %s"),
                   quote (var));

          env_سیٹ = true;
        }
    }

  واپس env_سیٹ;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح c;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  جب ((c = getopt_long (argc, argv, "+i:o:e:", longopts, تہی)) != -1)
    {
      صحیح ثابت oi = -1;
      صحیح opt_fileno;

      انتخاب (c)
        {
        /* Old McDonald had a farm ei...  */
        حالت 'e':
        حالت 'i':
        حالت 'o':
          opt_fileno = optc_مقصد_fileno (c);
          affirm (0 <= opt_fileno && opt_fileno < countof (stdbuf));
          stdbuf[opt_fileno].optc = c;
          جب (c_isspace (*optarg))
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
              شمار strtol_error e = parse_سائز (optarg,
                                                &stdbuf[opt_fileno].سائز);
              اگر (e != LONGINT_OK)
                xstrtol_fatal (e, oi, c, longopts, optarg);
            }

          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
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

  اگر (! سیٹ_libstdbuf_options ())
    {
      error (0, 0, _("you must specify a buffering mode option"));
      usage (EXIT_CANCELED);
    }

  /* Try to preload libstdbuf first from the same path as
     stdbuf is running from.  */
  سیٹ_پروگرام_راستہ (پروگرام_name);
  اگر (!پروگرام_راستہ)
    پروگرام_راستہ = xstrdup (PKGLIBDIR);  /* Need to init to non-null.  */
  سیٹ_LD_PRELOAD ();
  آزاد (پروگرام_راستہ);

  execvp (*argv, argv);

  صحیح exit_حالت = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  واپس exit_حالت;
}
