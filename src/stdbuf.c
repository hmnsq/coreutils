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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "filenamecat.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "xreadlink.h"
#അന്തര്ഭാവ "xstrtol-error.h"
#അന്തര്ഭാവ "xstrtol.h"
#അന്തര്ഭാവ "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "stdbuf"
#define LIB_NAME "libstdbuf.so" /* FIXME: don't hardcode  */

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

സ്ഥിതം അക്ഷരം *program_path;

സ്ഥിതം ഘടന
{
  size_t size;
  പൂർണ്ണസംഖ്യ optc;
  അക്ഷരം *optarg;
} stdbuf[3];

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"input", required_argument, ശൂന്യ, 'i'},
  {"output", required_argument, ശൂന്യ, 'o'},
  {"error", required_argument, ശൂന്യ, 'e'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

/* Set size to the value of STR, interpreted as a decimal integer,
   optionally multiplied by various values.
   Return LONGINT_OK on success, otherwise an error for use with
   xstrtol_fatal.

   This supports dd BLOCK size suffixes.
   Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats.  */
സ്ഥിതം എണ്ണൽ strtol_error
parse_size (അക്ഷരം സ്ഥിരം *str, size_t *size)
{
  uintmax_t tmp_size;
  എണ്ണൽ strtol_error e = xstrtoumax (str, ശൂന്യ, 10,
                                    &tmp_size, "EGkKMPQRTYZ0");
  എങ്കിൽ (e == LONGINT_OK && ckd_add (size, tmp_size, 0))
    e = LONGINT_OVERFLOW;
  തിരികെ e;
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("Usage: %s OPTION... COMMAND\n"), program_name);
      fputs (_("\
Run COMMAND, with modified buffering operations വേണ്ടി its standard streams.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --നിവേശം=MODE   adjust standard നിവേശം stream buffering\n\
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
If MODE is 'L' the corresponding stream will be വരി buffered.\n\
This option is invalid with standard നിവേശം.\n"), stdout);
      fputs (_("\n\
If MODE is '0' the corresponding stream will be unbuffered.\n\
"), stdout);
      fputs (_("\n\
Otherwise MODE is a സംഖ്യ which may be followed by one of the following:\n\
KB 1000, K 1024, MB 1000*1000, M 1024*1024, and so on വേണ്ടി G,T,P,E,Z,Y,R,Q.\n\
ദ്വിമാന_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
In this അവസ്ഥ the corresponding stream will be fully buffered with the buffer\n\
size set to MODE bytes.\n\
"), stdout);
      fputs (_("\n\
NOTE: If COMMAND adjusts the buffering of its standard streams ('tee' does\n\
വേണ്ടി example) then that will override corresponding changes by 'stdbuf'.\n\
Also some filters (like 'dd' and 'cat' etc.) don't use streams വേണ്ടി I/O,\n\
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

സ്ഥിതം ശൂന്യം
set_program_path (അക്ഷരം സ്ഥിരം *arg)
{
  എങ്കിൽ (strchr (arg, '/'))        /* Use absolute or relative paths directly.  */
    {
      program_path = dir_name (arg);
    }
  അല്ലെങ്കിൽ
    {
      അക്ഷരം *path = xreadlink ("/proc/self/exe");
      എങ്കിൽ (path)
        program_path = dir_name (path);
      അല്ലെങ്കിൽ എങ്കിൽ ((path = getenv ("PATH")))
        {
          path = xstrdup (path);
          വേണ്ടി (അക്ഷരം *dir = strtok (path, ":"); dir != ശൂന്യ;
               dir = strtok (ശൂന്യ, ":"))
            {
              അക്ഷരം *candidate = file_name_concat (dir, arg, ശൂന്യ);
              എങ്കിൽ (access (candidate, X_OK) == 0)
                {
                  program_path = dir_name (candidate);
                  സ്വതന്ത്രം (candidate);
                  നിർത്തുക;
                }
              സ്വതന്ത്രം (candidate);
            }
        }
      സ്വതന്ത്രം (path);
    }
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
optc_to_fileno (പൂർണ്ണസംഖ്യ c)
{
  പൂർണ്ണസംഖ്യ ret = -1;

  തിരഞ്ഞെടുപ്പ് (c)
    {
    അവസ്ഥ 'e':
      ret = STDERR_FILENO;
      നിർത്തുക;
    അവസ്ഥ 'i':
      ret = STDIN_FILENO;
      നിർത്തുക;
    അവസ്ഥ 'o':
      ret = STDOUT_FILENO;
      നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
      affirm (false);
    }

  തിരികെ ret;
}

സ്ഥിതം ശൂന്യം
set_LD_PRELOAD (ശൂന്യം)
{
  പൂർണ്ണസംഖ്യ ret;
#ifdef __APPLE__
  അക്ഷരം സ്ഥിരം *preload_env = "DYLD_INSERT_LIBRARIES";
#അല്ലെങ്കിലെങ്കിൽ defined _AIX
  അക്ഷരം സ്ഥിരം *preload_env = (വലുപ്പം (ശൂന്യം *) < 8
                             ? "LDR_PRELOAD" : "LDR_PRELOAD64");
#അല്ലെങ്കിൽ
  അക്ഷരം സ്ഥിരം *preload_env = "LD_PRELOAD";
#endif
  അക്ഷരം *old_libs = getenv (preload_env);
  അക്ഷരം *LD_PRELOAD;

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
  അക്ഷരം സ്ഥിരം *സ്ഥിരം തിരയൽ_path[] = {
    program_path,
    PKGLIBEXECDIR,
    ശൂന്യ
  };

  അക്ഷരം സ്ഥിരം *സ്ഥിരം *path = തിരയൽ_path;
  അക്ഷരം *libstdbuf;

  ആകുമ്പോൾ (true)
    {
      ഘടന stat sb;

      എങ്കിൽ (!**path)              /* system default  */
        {
          libstdbuf = xstrdup (LIB_NAME);
          നിർത്തുക;
        }
      ret = asprintf (&libstdbuf, "%s/%s", *path, LIB_NAME);
      എങ്കിൽ (ret < 0)
        xalloc_die ();
      എങ്കിൽ (stat (libstdbuf, &sb) == 0)   /* file_exists  */
        നിർത്തുക;
      സ്വതന്ത്രം (libstdbuf);

      ++path;
      എങ്കിൽ ( ! *path)
        error (EXIT_CANCELED, 0, _("failed to find %s"), quote (LIB_NAME));
    }

  /* FIXME: Do we need to support libstdbuf.dll, c:, '\' separators etc?  */

  എങ്കിൽ (old_libs)
    ret = asprintf (&LD_PRELOAD, "%s=%s:%s", preload_env, old_libs, libstdbuf);
  അല്ലെങ്കിൽ
    ret = asprintf (&LD_PRELOAD, "%s=%s", preload_env, libstdbuf);

  എങ്കിൽ (ret < 0)
    xalloc_die ();

  സ്വതന്ത്രം (libstdbuf);

  ret = putenv (LD_PRELOAD);
#ifdef __APPLE__
  എങ്കിൽ (ret == 0)
    ret = setenv ("DYLD_FORCE_FLAT_NAMESPACE", "y", 1);
#endif

  എങ്കിൽ (ret != 0)
    error (EXIT_CANCELED, errno,
           _("failed to update the environment with %s"),
           quote (LD_PRELOAD));
}

/* Populate environ with _STDBUF_I=$MODE _STDBUF_O=$MODE _STDBUF_E=$MODE.
   Return TRUE if any environment variables set.   */

സ്ഥിതം bool
set_libstdbuf_options (ശൂന്യം)
{
  bool env_set = false;

  വേണ്ടി (size_t i = 0; i < countof (stdbuf); i++)
    {
      എങ്കിൽ (stdbuf[i].optarg)
        {
          അക്ഷരം *var;
          പൂർണ്ണസംഖ്യ ret;

          എങ്കിൽ (*stdbuf[i].optarg == 'L')
            ret = asprintf (&var, "%s%c=L", "_STDBUF_",
                            c_toupper (stdbuf[i].optc));
          അല്ലെങ്കിൽ
            ret = asprintf (&var, "%s%c=%zu", "_STDBUF_",
                            c_toupper (stdbuf[i].optc),
                            stdbuf[i].size);
          എങ്കിൽ (ret < 0)
            xalloc_die ();

          എങ്കിൽ (putenv (var) != 0)
            error (EXIT_CANCELED, errno,
                   _("failed to update the environment with %s"),
                   quote (var));

          env_set = true;
        }
    }

  തിരികെ env_set;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ആകുമ്പോൾ ((c = getopt_long (argc, argv, "+i:o:e:", longopts, ശൂന്യ)) != -1)
    {
      പൂർണ്ണസംഖ്യ സ്ഥിരം oi = -1;
      പൂർണ്ണസംഖ്യ opt_fileno;

      തിരഞ്ഞെടുപ്പ് (c)
        {
        /* Old McDonald had a farm ei...  */
        അവസ്ഥ 'e':
        അവസ്ഥ 'i':
        അവസ്ഥ 'o':
          opt_fileno = optc_to_fileno (c);
          affirm (0 <= opt_fileno && opt_fileno < countof (stdbuf));
          stdbuf[opt_fileno].optc = c;
          ആകുമ്പോൾ (c_isspace (*optarg))
            optarg++;
          stdbuf[opt_fileno].optarg = optarg;
          എങ്കിൽ (c == 'i' && *optarg == 'L')
            {
              /* -oL will be by far the most common use of this utility,
                 but one could easily think -iL might have the same affect,
                 so disallow it as it could be confusing.  */
              error (0, 0, _("line buffering standard input is meaningless"));
              usage (EXIT_CANCELED);
            }

          എങ്കിൽ (!streq (optarg, "L"))
            {
              എണ്ണൽ strtol_error e = parse_size (optarg,
                                                &stdbuf[opt_fileno].size);
              എങ്കിൽ (e != LONGINT_OK)
                xstrtol_fatal (e, oi, c, longopts, optarg);
            }

          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_CANCELED);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify at least 1 command.  */
  എങ്കിൽ (argc < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  എങ്കിൽ (! set_libstdbuf_options ())
    {
      error (0, 0, _("you must specify a buffering mode option"));
      usage (EXIT_CANCELED);
    }

  /* Try to preload libstdbuf first from the same path as
     stdbuf is running from.  */
  set_program_path (program_name);
  എങ്കിൽ (!program_path)
    program_path = xstrdup (PKGLIBDIR);  /* Need to init to non-null.  */
  set_LD_PRELOAD ();
  സ്വതന്ത്രം (program_path);

  execvp (*argv, argv);

  പൂർണ്ണസംഖ്യ exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  തിരികെ exit_status;
}
