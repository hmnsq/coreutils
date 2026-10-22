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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "filenamecat.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "xreadlink.h"
#ಅನ್ತರ್ಭಾವ "xstrtol-error.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "stdbuf"
#define LIB_NAME "libstdbuf.so" /* FIXME: don't hardcode  */

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

ಸ್ಥಿರ ವರ್ಣ *program_path;

ಸ್ಥಿರ ಸಂರಚನಾ
{
  size_t size;
  ಪೂರ್ಣಾಙ್ಕ optc;
  ವರ್ಣ *optarg;
} stdbuf[3];

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"input", required_argument, ರಿಕ್ತ, 'i'},
  {"output", required_argument, ರಿಕ್ತ, 'o'},
  {"error", required_argument, ರಿಕ್ತ, 'e'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

/* Set size to the value of STR, interpreted as a decimal integer,
   optionally multiplied by various values.
   Return LONGINT_OK on success, otherwise an error for use with
   xstrtol_fatal.

   This supports dd BLOCK size suffixes.
   Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats.  */
ಸ್ಥಿರ ಗಣನಾ strtol_error
parse_size (ವರ್ಣ ಧ್ರುವ *str, size_t *size)
{
  uintmax_t tmp_size;
  ಗಣನಾ strtol_error e = xstrtoumax (str, ರಿಕ್ತ, 10,
                                    &tmp_size, "EGkKMPQRTYZ0");
  ಯದಿ (e == LONGINT_OK && ckd_add (size, tmp_size, 0))
    e = LONGINT_OVERFLOW;
  ಪ್ರತ್ಯಾಗಮ e;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("Usage: %s OPTION... COMMAND\n"), program_name);
      fputs (_("\
Run COMMAND, with modified buffering operations ಪ್ರತಿ its standard streams.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --ಒಳಹರಿವು=MODE   adjust standard ಒಳಹರಿವು stream buffering\n\
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
If MODE is 'L' the corresponding stream will be ಸಾಲು buffered.\n\
This option is invalid with standard ಒಳಹರಿವು.\n"), stdout);
      fputs (_("\n\
If MODE is '0' the corresponding stream will be unbuffered.\n\
"), stdout);
      fputs (_("\n\
Otherwise MODE is a ಸಂಖ್ಯೆ which may be followed by one of the following:\n\
KB 1000, K 1024, MB 1000*1000, M 1024*1024, and so on ಪ್ರತಿ G,T,P,E,Z,Y,R,Q.\n\
ದ್ವಿಮಾನ_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
In this ಪಕ್ಷ the corresponding stream will be fully buffered with the buffer\n\
size set to MODE bytes.\n\
"), stdout);
      fputs (_("\n\
NOTE: If COMMAND adjusts the buffering of its standard streams ('tee' does\n\
ಪ್ರತಿ example) then that will override corresponding changes by 'stdbuf'.\n\
Also some filters (like 'dd' and 'cat' etc.) don't use streams ಪ್ರತಿ I/O,\n\
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

ಸ್ಥಿರ ಶೂನ್ಯ
set_program_path (ವರ್ಣ ಧ್ರುವ *arg)
{
  ಯದಿ (strchr (arg, '/'))        /* Use absolute or relative paths directly.  */
    {
      program_path = dir_name (arg);
    }
  ಅನ್ಯಥಾ
    {
      ವರ್ಣ *path = xreadlink ("/proc/self/exe");
      ಯದಿ (path)
        program_path = dir_name (path);
      ಅನ್ಯಥಾ ಯದಿ ((path = getenv ("PATH")))
        {
          path = xstrdup (path);
          ಪ್ರತಿ (ವರ್ಣ *dir = strtok (path, ":"); dir != ರಿಕ್ತ;
               dir = strtok (ರಿಕ್ತ, ":"))
            {
              ವರ್ಣ *candidate = file_name_concat (dir, arg, ರಿಕ್ತ);
              ಯದಿ (access (candidate, X_OK) == 0)
                {
                  program_path = dir_name (candidate);
                  ಮುಕ್ತ (candidate);
                  ಭಙ್ಗ;
                }
              ಮುಕ್ತ (candidate);
            }
        }
      ಮುಕ್ತ (path);
    }
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
optc_to_fileno (ಪೂರ್ಣಾಙ್ಕ c)
{
  ಪೂರ್ಣಾಙ್ಕ ret = -1;

  ವಿಕಲ್ಪ (c)
    {
    ಪಕ್ಷ 'e':
      ret = STDERR_FILENO;
      ಭಙ್ಗ;
    ಪಕ್ಷ 'i':
      ret = STDIN_FILENO;
      ಭಙ್ಗ;
    ಪಕ್ಷ 'o':
      ret = STDOUT_FILENO;
      ಭಙ್ಗ;
    ಸ್ವಭಾವ:
      affirm (false);
    }

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ
set_LD_PRELOAD (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
#ifdef __APPLE__
  ವರ್ಣ ಧ್ರುವ *preload_env = "DYLD_INSERT_LIBRARIES";
#ನೋಚೇದ್ಯದಿ defined _AIX
  ವರ್ಣ ಧ್ರುವ *preload_env = (ಪರಿಮಾಣ (ಶೂನ್ಯ *) < 8
                             ? "LDR_PRELOAD" : "LDR_PRELOAD64");
#ಅನ್ಯಥಾ
  ವರ್ಣ ಧ್ರುವ *preload_env = "LD_PRELOAD";
#endif
  ವರ್ಣ *old_libs = getenv (preload_env);
  ವರ್ಣ *LD_PRELOAD;

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
  ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ ಹುಡುಕು_path[] = {
    program_path,
    PKGLIBEXECDIR,
    ರಿಕ್ತ
  };

  ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ *path = ಹುಡುಕು_path;
  ವರ್ಣ *libstdbuf;

  ಯಾವತ್ (true)
    {
      ಸಂರಚನಾ stat sb;

      ಯದಿ (!**path)              /* system default  */
        {
          libstdbuf = xstrdup (LIB_NAME);
          ಭಙ್ಗ;
        }
      ret = asprintf (&libstdbuf, "%s/%s", *path, LIB_NAME);
      ಯದಿ (ret < 0)
        xalloc_die ();
      ಯದಿ (stat (libstdbuf, &sb) == 0)   /* file_exists  */
        ಭಙ್ಗ;
      ಮುಕ್ತ (libstdbuf);

      ++path;
      ಯದಿ ( ! *path)
        error (EXIT_CANCELED, 0, _("failed to find %s"), quote (LIB_NAME));
    }

  /* FIXME: Do we need to support libstdbuf.dll, c:, '\' separators etc?  */

  ಯದಿ (old_libs)
    ret = asprintf (&LD_PRELOAD, "%s=%s:%s", preload_env, old_libs, libstdbuf);
  ಅನ್ಯಥಾ
    ret = asprintf (&LD_PRELOAD, "%s=%s", preload_env, libstdbuf);

  ಯದಿ (ret < 0)
    xalloc_die ();

  ಮುಕ್ತ (libstdbuf);

  ret = putenv (LD_PRELOAD);
#ifdef __APPLE__
  ಯದಿ (ret == 0)
    ret = setenv ("DYLD_FORCE_FLAT_NAMESPACE", "y", 1);
#endif

  ಯದಿ (ret != 0)
    error (EXIT_CANCELED, errno,
           _("failed to update the environment with %s"),
           quote (LD_PRELOAD));
}

/* Populate environ with _STDBUF_I=$MODE _STDBUF_O=$MODE _STDBUF_E=$MODE.
   Return TRUE if any environment variables set.   */

ಸ್ಥಿರ bool
set_libstdbuf_options (ಶೂನ್ಯ)
{
  bool env_set = false;

  ಪ್ರತಿ (size_t i = 0; i < countof (stdbuf); i++)
    {
      ಯದಿ (stdbuf[i].optarg)
        {
          ವರ್ಣ *var;
          ಪೂರ್ಣಾಙ್ಕ ret;

          ಯದಿ (*stdbuf[i].optarg == 'L')
            ret = asprintf (&var, "%s%c=L", "_STDBUF_",
                            c_toupper (stdbuf[i].optc));
          ಅನ್ಯಥಾ
            ret = asprintf (&var, "%s%c=%zu", "_STDBUF_",
                            c_toupper (stdbuf[i].optc),
                            stdbuf[i].size);
          ಯದಿ (ret < 0)
            xalloc_die ();

          ಯದಿ (putenv (var) != 0)
            error (EXIT_CANCELED, errno,
                   _("failed to update the environment with %s"),
                   quote (var));

          env_set = true;
        }
    }

  ಪ್ರತ್ಯಾಗಮ env_set;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ಯಾವತ್ ((c = getopt_long (argc, argv, "+i:o:e:", longopts, ರಿಕ್ತ)) != -1)
    {
      ಪೂರ್ಣಾಙ್ಕ ಧ್ರುವ oi = -1;
      ಪೂರ್ಣಾಙ್ಕ opt_fileno;

      ವಿಕಲ್ಪ (c)
        {
        /* Old McDonald had a farm ei...  */
        ಪಕ್ಷ 'e':
        ಪಕ್ಷ 'i':
        ಪಕ್ಷ 'o':
          opt_fileno = optc_to_fileno (c);
          affirm (0 <= opt_fileno && opt_fileno < countof (stdbuf));
          stdbuf[opt_fileno].optc = c;
          ಯಾವತ್ (c_isspace (*optarg))
            optarg++;
          stdbuf[opt_fileno].optarg = optarg;
          ಯದಿ (c == 'i' && *optarg == 'L')
            {
              /* -oL will be by far the most common use of this utility,
                 but one could easily think -iL might have the same affect,
                 so disallow it as it could be confusing.  */
              error (0, 0, _("line buffering standard input is meaningless"));
              usage (EXIT_CANCELED);
            }

          ಯದಿ (!streq (optarg, "L"))
            {
              ಗಣನಾ strtol_error e = parse_size (optarg,
                                                &stdbuf[opt_fileno].size);
              ಯದಿ (e != LONGINT_OK)
                xstrtol_fatal (e, oi, c, longopts, optarg);
            }

          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (EXIT_CANCELED);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify at least 1 command.  */
  ಯದಿ (argc < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  ಯದಿ (! set_libstdbuf_options ())
    {
      error (0, 0, _("you must specify a buffering mode option"));
      usage (EXIT_CANCELED);
    }

  /* Try to preload libstdbuf first from the same path as
     stdbuf is running from.  */
  set_program_path (program_name);
  ಯದಿ (!program_path)
    program_path = xstrdup (PKGLIBDIR);  /* Need to init to non-null.  */
  set_LD_PRELOAD ();
  ಮುಕ್ತ (program_path);

  execvp (*argv, argv);

  ಪೂರ್ಣಾಙ್ಕ exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  ಪ್ರತ್ಯಾಗಮ exit_status;
}
