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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "filenamecat.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xreadlink.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol-error.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "stdbuf"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LIB_NAME "libstdbuf.so" /* FIXME: don't hardcode  */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

ਸਥਿਰ ਅੱਖਰ *ਪ੍ਰੋਗਰਾਮ_ਰਸਤਾ;

ਸਥਿਰ ਬਣਤਰ
{
  size_t ਮਾਪ;
  ਪੂਰਨ optc;
  ਅੱਖਰ *optarg;
} stdbuf[3];

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"input", required_ਦਲੀਲ, ਨਿਰਾ, 'i'},
  {"output", required_ਦਲੀਲ, ਨਿਰਾ, 'o'},
  {"error", required_ਦਲੀਲ, ਨਿਰਾ, 'e'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* Set size to the value of STR, interpreted as a decimal integer,
   optionally multiplied by various values.
   Return LONGINT_OK on success, otherwise an error for use with
   xstrtol_fatal.

   This supports dd BLOCK size suffixes.
   Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats.  */
ਸਥਿਰ ਗਿਣਤੀ strtol_error
parse_ਮਾਪ (ਅੱਖਰ ਅਚਲ *str, size_t *ਮਾਪ)
{
  uintmax_t tmp_ਮਾਪ;
  ਗਿਣਤੀ strtol_error e = xstrtoumax (str, ਨਿਰਾ, 10,
                                    &tmp_ਮਾਪ, "EGkKMPQRTYZ0");
  ਜੇ (e == LONGINT_OK && ckd_ਜੋੜੋ (ਮਾਪ, tmp_ਮਾਪ, 0))
    e = LONGINT_ਉਛਾਲ;
  ਵਾਪਸ e;
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s OPTION... COMMAND\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
ਲੜੀਵਾਰ COMMAND, with modified buffering operations ਲਈ its standard streams.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --ਨਿਵੇਸ਼=ਬਹੁਲਕ   adjust standard ਨਿਵੇਸ਼ ਧਾਰਾ buffering\n\
"));
      oputs (_("\
  -o, --output=ਬਹੁਲਕ  adjust standard output ਧਾਰਾ buffering\n\
"));
      oputs (_("\
  -e, --error=ਬਹੁਲਕ   adjust standard error ਧਾਰਾ buffering\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\n\
If ਬਹੁਲਕ is 'L' the corresponding ਧਾਰਾ will be ਸਤਰ buffered.\n\
This option is invalid with standard ਨਿਵੇਸ਼.\n"), stdout);
      fputs (_("\n\
If ਬਹੁਲਕ is '0' the corresponding ਧਾਰਾ will be unbuffered.\n\
"), stdout);
      fputs (_("\n\
Otherwise ਬਹੁਲਕ is a ਸੰਖਿਆ which may be followed by one ਦਾ the following:\n\
KB 1000, K 1024, MB 1000*1000, M 1024*1024, and so on ਲਈ G,T,P,E,Z,Y,R,Q.\n\
ਦੁਆਧਾਰੀ_2 prefixes can be ਵਰਤਿਆ, too: KiB=K, MiB=M, and so on.\n\
ਵਿੱਚ_2 this ਹਾਲਤ the corresponding ਧਾਰਾ will be fully buffered with the ਬਫਰ\n\
ਮਾਪ ਸੈੱਟ_2 ਮੰਜ਼ਿਲ ਬਹੁਲਕ ਬਾਈਟਾਂ.\n\
"), stdout);
      fputs (_("\n\
NOTE: If COMMAND adjusts the buffering ਦਾ its standard streams ('tee' does\n\
ਲਈ example) then that will override corresponding changes by 'stdbuf'.\n\
Also some filters (like 'dd' and 'cat' etc.) don't use streams ਲਈ I/O,\n\
and are thus unaffected by 'stdbuf' settings.\n\
"), stdout);
      emit_exec_ਸਥਿਤੀ (ਪ੍ਰੋਗਰਾਮ_NAME);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* argv[0] can be anything really, but generally it contains
   the path to the executable or just a name if it was executed
   using $PATH. In the latter case to get the path we can:
   search getenv("PATH"), readlink("/prof/self/exe"), getenv("_"),
   dladdr(), pstat_getpathname(), etc.  */

ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_ਰਸਤਾ (ਅੱਖਰ ਅਚਲ *arg)
{
  ਜੇ (strchr (arg, '/'))        /* Use absolute or relative paths directly.  */
    {
      ਪ੍ਰੋਗਰਾਮ_ਰਸਤਾ = dir_name (arg);
    }
  ਨਹੀਂਤਾਂ
    {
      ਅੱਖਰ *ਰਸਤਾ = xreadlink ("/proc/self/exe");
      ਜੇ (ਰਸਤਾ)
        ਪ੍ਰੋਗਰਾਮ_ਰਸਤਾ = dir_name (ਰਸਤਾ);
      ਨਹੀਂਤਾਂ ਜੇ ((ਰਸਤਾ = getenv ("PATH")))
        {
          ਰਸਤਾ = xstrdup (ਰਸਤਾ);
          ਲਈ (ਅੱਖਰ *dir = strtok (ਰਸਤਾ, ":"); dir != ਨਿਰਾ;
               dir = strtok (ਨਿਰਾ, ":"))
            {
              ਅੱਖਰ *candidate = file_name_concat (dir, arg, ਨਿਰਾ);
              ਜੇ (access (candidate, X_OK) == 0)
                {
                  ਪ੍ਰੋਗਰਾਮ_ਰਸਤਾ = dir_name (candidate);
                  ਮੁਕਤ (candidate);
                  ਰੋਕੋ;
                }
              ਮੁਕਤ (candidate);
            }
        }
      ਮੁਕਤ (ਰਸਤਾ);
    }
}

ਸਥਿਰ ਪੂਰਨ
optc_ਮੰਜ਼ਿਲ_fileno (ਪੂਰਨ c)
{
  ਪੂਰਨ ret = -1;

  ਚੋਣ (c)
    {
    ਹਾਲਤ 'e':
      ret = STDERR_FILENO;
      ਰੋਕੋ;
    ਹਾਲਤ 'i':
      ret = STDIN_FILENO;
      ਰੋਕੋ;
    ਹਾਲਤ 'o':
      ret = STDOUT_FILENO;
      ਰੋਕੋ;
    ਮੂਲ:
      affirm (false);
    }

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_LD_PRELOAD (ਖਾਲੀ)
{
  ਪੂਰਨ ret;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __APPLE__
  ਅੱਖਰ ਅਚਲ *preload_env = "DYLD_INSERT_LIBRARIES";
#ਜਾਂਜੇ defined _AIX
  ਅੱਖਰ ਅਚਲ *preload_env = (ਆਕਾਰ (ਖਾਲੀ *) < 8
                             ? "LDR_PRELOAD" : "LDR_PRELOAD64");
#ਨਹੀਂਤਾਂ
  ਅੱਖਰ ਅਚਲ *preload_env = "LD_PRELOAD";
#ਜੇ_ਅੰਤ
  ਅੱਖਰ *old_libs = getenv (preload_env);
  ਅੱਖਰ *LD_PRELOAD;

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
  ਅੱਖਰ ਅਚਲ *ਅਚਲ ਖੋਜ_ਰਸਤਾ[] = {
    ਪ੍ਰੋਗਰਾਮ_ਰਸਤਾ,
    PKGLIBEXECDIR,
    ਨਿਰਾ
  };

  ਅੱਖਰ ਅਚਲ *ਅਚਲ *ਰਸਤਾ = ਖੋਜ_ਰਸਤਾ;
  ਅੱਖਰ *libstdbuf;

  ਜਦੋਂ (true)
    {
      ਬਣਤਰ stat sb;

      ਜੇ (!**ਰਸਤਾ)              /* system default  */
        {
          libstdbuf = xstrdup (LIB_NAME);
          ਰੋਕੋ;
        }
      ret = asprintf (&libstdbuf, "%s/%s", *ਰਸਤਾ, LIB_NAME);
      ਜੇ (ret < 0)
        xalloc_die ();
      ਜੇ (stat (libstdbuf, &sb) == 0)   /* file_exists  */
        ਰੋਕੋ;
      ਮੁਕਤ (libstdbuf);

      ++ਰਸਤਾ;
      ਜੇ ( ! *ਰਸਤਾ)
        error (EXIT_CANCELED, 0, _("failed to find %s"), quote (LIB_NAME));
    }

  /* FIXME: Do we need to support libstdbuf.dll, c:, '\' separators etc?  */

  ਜੇ (old_libs)
    ret = asprintf (&LD_PRELOAD, "%s=%s:%s", preload_env, old_libs, libstdbuf);
  ਨਹੀਂਤਾਂ
    ret = asprintf (&LD_PRELOAD, "%s=%s", preload_env, libstdbuf);

  ਜੇ (ret < 0)
    xalloc_die ();

  ਮੁਕਤ (libstdbuf);

  ret = putenv (LD_PRELOAD);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __APPLE__
  ਜੇ (ret == 0)
    ret = setenv ("DYLD_FORCE_FLAT_NAMESPACE", "y", 1);
#ਜੇ_ਅੰਤ

  ਜੇ (ret != 0)
    error (EXIT_CANCELED, errno,
           _("failed to update the environment with %s"),
           quote (LD_PRELOAD));
}

/* Populate environ with _STDBUF_I=$MODE _STDBUF_O=$MODE _STDBUF_E=$MODE.
   Return TRUE if any environment variables set.   */

ਸਥਿਰ bool
ਸੈੱਟ_libstdbuf_options (ਖਾਲੀ)
{
  bool env_ਸੈੱਟ = false;

  ਲਈ (size_t i = 0; i < countof (stdbuf); i++)
    {
      ਜੇ (stdbuf[i].optarg)
        {
          ਅੱਖਰ *var;
          ਪੂਰਨ ret;

          ਜੇ (*stdbuf[i].optarg == 'L')
            ret = asprintf (&var, "%s%c=L", "_STDBUF_",
                            c_toupper (stdbuf[i].optc));
          ਨਹੀਂਤਾਂ
            ret = asprintf (&var, "%s%c=%zu", "_STDBUF_",
                            c_toupper (stdbuf[i].optc),
                            stdbuf[i].ਮਾਪ);
          ਜੇ (ret < 0)
            xalloc_die ();

          ਜੇ (putenv (var) != 0)
            error (EXIT_CANCELED, errno,
                   _("failed to update the environment with %s"),
                   quote (var));

          env_ਸੈੱਟ = true;
        }
    }

  ਵਾਪਸ env_ਸੈੱਟ;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ c;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ਜਦੋਂ ((c = getopt_long (argc, argv, "+i:o:e:", longopts, ਨਿਰਾ)) != -1)
    {
      ਪੂਰਨ ਅਚਲ oi = -1;
      ਪੂਰਨ opt_fileno;

      ਚੋਣ (c)
        {
        /* Old McDonald had a farm ei...  */
        ਹਾਲਤ 'e':
        ਹਾਲਤ 'i':
        ਹਾਲਤ 'o':
          opt_fileno = optc_ਮੰਜ਼ਿਲ_fileno (c);
          affirm (0 <= opt_fileno && opt_fileno < countof (stdbuf));
          stdbuf[opt_fileno].optc = c;
          ਜਦੋਂ (c_isspace (*optarg))
            optarg++;
          stdbuf[opt_fileno].optarg = optarg;
          ਜੇ (c == 'i' && *optarg == 'L')
            {
              /* -oL will be by far the most common use of this utility,
                 but one could easily think -iL might have the same affect,
                 so disallow it as it could be confusing.  */
              error (0, 0, _("line buffering standard input is meaningless"));
              usage (EXIT_CANCELED);
            }

          ਜੇ (!streq (optarg, "L"))
            {
              ਗਿਣਤੀ strtol_error e = parse_ਮਾਪ (optarg,
                                                &stdbuf[opt_fileno].ਮਾਪ);
              ਜੇ (e != LONGINT_OK)
                xstrtol_fatal (e, oi, c, longopts, optarg);
            }

          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_CANCELED);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify at least 1 command.  */
  ਜੇ (argc < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  ਜੇ (! ਸੈੱਟ_libstdbuf_options ())
    {
      error (0, 0, _("you must specify a buffering mode option"));
      usage (EXIT_CANCELED);
    }

  /* Try to preload libstdbuf first from the same path as
     stdbuf is running from.  */
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_ਰਸਤਾ (ਪ੍ਰੋਗਰਾਮ_name);
  ਜੇ (!ਪ੍ਰੋਗਰਾਮ_ਰਸਤਾ)
    ਪ੍ਰੋਗਰਾਮ_ਰਸਤਾ = xstrdup (PKGLIBDIR);  /* Need to init to non-null.  */
  ਸੈੱਟ_LD_PRELOAD ();
  ਮੁਕਤ (ਪ੍ਰੋਗਰਾਮ_ਰਸਤਾ);

  execvp (*argv, argv);

  ਪੂਰਨ exit_ਸਥਿਤੀ = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  ਵਾਪਸ exit_ਸਥਿਤੀ;
}
