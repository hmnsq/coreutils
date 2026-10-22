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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "stdbuf"
#ವ್ಯಾಖ್ಯಾನಿಸು LIB_NAME "libstdbuf.so" /* FIXME: don't hardcode  */

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

ಸ್ಥಿರ ಅಕ್ಷರ *ಕಾರ್ಯಕ್ರಮ_ಮಾರ್ಗ;

ಸ್ಥಿರ ರಚನೆ
{
  size_t ಗಾತ್ರ_2;
  ಪೂರ್ಣಾಂಕ optc;
  ಅಕ್ಷರ *optarg;
} stdbuf[3];

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"input", required_ವಾದ, ಖಾಲಿ, 'i'},
  {"output", required_ವಾದ, ಖಾಲಿ, 'o'},
  {"error", required_ವಾದ, ಖಾಲಿ, 'e'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* Set size to the value of STR, interpreted as a decimal integer,
   optionally multiplied by various values.
   Return LONGINT_OK on success, otherwise an error for use with
   xstrtol_fatal.

   This supports dd BLOCK size suffixes.
   Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats.  */
ಸ್ಥಿರ ಎಣಿಕೆ strtol_error
parse_ಗಾತ್ರ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str, size_t *ಗಾತ್ರ_2)
{
  uintmax_t tmp_ಗಾತ್ರ;
  ಎಣಿಕೆ strtol_error e = xstrtoumax (str, ಖಾಲಿ, 10,
                                    &tmp_ಗಾತ್ರ, "EGkKMPQRTYZ0");
  ಆದರೆ (e == LONGINT_OK && ckd_ಕೂಡಿಸು (ಗಾತ್ರ_2, tmp_ಗಾತ್ರ, 0))
    e = LONGINT_ಅತಿಪ್ರವಾಹ;
  ಹಿಂತಿರುಗಿ e;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s OPTION... COMMAND\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
ಸತತ COMMAND, with modified buffering operations ಗಾಗಿ its standard streams.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --ಒಳಹರಿವು=ಬಹುಲಕ   adjust standard ಒಳಹರಿವು ಹರಿವು buffering\n\
"));
      oputs (_("\
  -o, --output=ಬಹುಲಕ  adjust standard output ಹರಿವು buffering\n\
"));
      oputs (_("\
  -e, --error=ಬಹುಲಕ   adjust standard error ಹರಿವು buffering\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\n\
If ಬಹುಲಕ is 'L' the corresponding ಹರಿವು will be ಸಾಲು buffered.\n\
This option is invalid with standard ಒಳಹರಿವು.\n"), stdout);
      fputs (_("\n\
If ಬಹುಲಕ is '0' the corresponding ಹರಿವು will be unbuffered.\n\
"), stdout);
      fputs (_("\n\
Otherwise ಬಹುಲಕ is a ಸಂಖ್ಯೆ which may be followed by one ದ the following:\n\
KB 1000, K 1024, MB 1000*1000, M 1024*1024, and so on ಗಾಗಿ G,T,P,E,Z,Y,R,Q.\n\
ದ್ವಿಮಾನ_2 prefixes can be ಬಳಸಿದ, too: KiB=K, MiB=M, and so on.\n\
ಒಳ_2 this ಸ್ಥಿತಿ the corresponding ಹರಿವು will be fully buffered with the ಬಫರ್\n\
ಗಾತ್ರ_2 ಗಣ_2 ಗಮ್ಯ ಬಹುಲಕ ಬೈಟುಗಳು.\n\
"), stdout);
      fputs (_("\n\
NOTE: If COMMAND adjusts the buffering ದ its standard streams ('tee' does\n\
ಗಾಗಿ example) then that will override corresponding changes by 'stdbuf'.\n\
Also some filters (like 'dd' and 'cat' etc.) don't use streams ಗಾಗಿ I/O,\n\
and are thus unaffected by 'stdbuf' settings.\n\
"), stdout);
      emit_exec_ಸ್ಥಿತಿ (ಕಾರ್ಯಕ್ರಮ_NAME);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* argv[0] can be anything really, but generally it contains
   the path to the executable or just a name if it was executed
   using $PATH. In the latter case to get the path we can:
   search getenv("PATH"), readlink("/prof/self/exe"), getenv("_"),
   dladdr(), pstat_getpathname(), etc.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_ಕಾರ್ಯಕ್ರಮ_ಮಾರ್ಗ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg)
{
  ಆದರೆ (strchr (arg, '/'))        /* Use absolute or relative paths directly.  */
    {
      ಕಾರ್ಯಕ್ರಮ_ಮಾರ್ಗ = dir_name (arg);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ *ಮಾರ್ಗ = xreadlink ("/proc/self/exe");
      ಆದರೆ (ಮಾರ್ಗ)
        ಕಾರ್ಯಕ್ರಮ_ಮಾರ್ಗ = dir_name (ಮಾರ್ಗ);
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((ಮಾರ್ಗ = getenv ("PATH")))
        {
          ಮಾರ್ಗ = xstrdup (ಮಾರ್ಗ);
          ಗಾಗಿ (ಅಕ್ಷರ *dir = strtok (ಮಾರ್ಗ, ":"); dir != ಖಾಲಿ;
               dir = strtok (ಖಾಲಿ, ":"))
            {
              ಅಕ್ಷರ *candidate = file_name_concat (dir, arg, ಖಾಲಿ);
              ಆದರೆ (access (candidate, X_OK) == 0)
                {
                  ಕಾರ್ಯಕ್ರಮ_ಮಾರ್ಗ = dir_name (candidate);
                  ಬಿಡುಗಡೆ (candidate);
                  ನಿಲ್ಲಿಸು;
                }
              ಬಿಡುಗಡೆ (candidate);
            }
        }
      ಬಿಡುಗಡೆ (ಮಾರ್ಗ);
    }
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
optc_ಗಮ್ಯ_fileno (ಪೂರ್ಣಾಂಕ c)
{
  ಪೂರ್ಣಾಂಕ ret = -1;

  ಆಯ್ಕೆ (c)
    {
    ಸ್ಥಿತಿ 'e':
      ret = STDERR_FILENO;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'i':
      ret = STDIN_FILENO;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'o':
      ret = STDOUT_FILENO;
      ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
      affirm (false);
    }

  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_LD_PRELOAD (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಂಕ ret;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __APPLE__
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *preload_env = "DYLD_INSERT_LIBRARIES";
#ಅಥವಾಆದರೆ defined _AIX
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *preload_env = (ಗಾತ್ರ (ಶೂನ್ಯ *) < 8
                             ? "LDR_PRELOAD" : "LDR_PRELOAD64");
#ಇಲ್ಲದಿದ್ದರೆ
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *preload_env = "LD_PRELOAD";
#ಆದರೆ_ಅಂತ್ಯ
  ಅಕ್ಷರ *old_libs = getenv (preload_env);
  ಅಕ್ಷರ *LD_PRELOAD;

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
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ ಹುಡುಕು_ಮಾರ್ಗ[] = {
    ಕಾರ್ಯಕ್ರಮ_ಮಾರ್ಗ,
    PKGLIBEXECDIR,
    ಖಾಲಿ
  };

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ *ಮಾರ್ಗ = ಹುಡುಕು_ಮಾರ್ಗ;
  ಅಕ್ಷರ *libstdbuf;

  ಆಗ (true)
    {
      ರಚನೆ stat sb;

      ಆದರೆ (!**ಮಾರ್ಗ)              /* system default  */
        {
          libstdbuf = xstrdup (LIB_NAME);
          ನಿಲ್ಲಿಸು;
        }
      ret = asprintf (&libstdbuf, "%s/%s", *ಮಾರ್ಗ, LIB_NAME);
      ಆದರೆ (ret < 0)
        xalloc_die ();
      ಆದರೆ (stat (libstdbuf, &sb) == 0)   /* file_exists  */
        ನಿಲ್ಲಿಸು;
      ಬಿಡುಗಡೆ (libstdbuf);

      ++ಮಾರ್ಗ;
      ಆದರೆ ( ! *ಮಾರ್ಗ)
        error (EXIT_CANCELED, 0, _("failed to find %s"), quote (LIB_NAME));
    }

  /* FIXME: Do we need to support libstdbuf.dll, c:, '\' separators etc?  */

  ಆದರೆ (old_libs)
    ret = asprintf (&LD_PRELOAD, "%s=%s:%s", preload_env, old_libs, libstdbuf);
  ಇಲ್ಲದಿದ್ದರೆ
    ret = asprintf (&LD_PRELOAD, "%s=%s", preload_env, libstdbuf);

  ಆದರೆ (ret < 0)
    xalloc_die ();

  ಬಿಡುಗಡೆ (libstdbuf);

  ret = putenv (LD_PRELOAD);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __APPLE__
  ಆದರೆ (ret == 0)
    ret = setenv ("DYLD_FORCE_FLAT_NAMESPACE", "y", 1);
#ಆದರೆ_ಅಂತ್ಯ

  ಆದರೆ (ret != 0)
    error (EXIT_CANCELED, errno,
           _("failed to update the environment with %s"),
           quote (LD_PRELOAD));
}

/* Populate environ with _STDBUF_I=$MODE _STDBUF_O=$MODE _STDBUF_E=$MODE.
   Return TRUE if any environment variables set.   */

ಸ್ಥಿರ bool
ಗಣ_libstdbuf_options (ಶೂನ್ಯ)
{
  bool env_ಗಣ = false;

  ಗಾಗಿ (size_t i = 0; i < countof (stdbuf); i++)
    {
      ಆದರೆ (stdbuf[i].optarg)
        {
          ಅಕ್ಷರ *var;
          ಪೂರ್ಣಾಂಕ ret;

          ಆದರೆ (*stdbuf[i].optarg == 'L')
            ret = asprintf (&var, "%s%c=L", "_STDBUF_",
                            c_toupper (stdbuf[i].optc));
          ಇಲ್ಲದಿದ್ದರೆ
            ret = asprintf (&var, "%s%c=%zu", "_STDBUF_",
                            c_toupper (stdbuf[i].optc),
                            stdbuf[i].ಗಾತ್ರ_2);
          ಆದರೆ (ret < 0)
            xalloc_die ();

          ಆದರೆ (putenv (var) != 0)
            error (EXIT_CANCELED, errno,
                   _("failed to update the environment with %s"),
                   quote (var));

          env_ಗಣ = true;
        }
    }

  ಹಿಂತಿರುಗಿ env_ಗಣ;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ c;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ಆಗ ((c = getopt_long (argc, argv, "+i:o:e:", longopts, ಖಾಲಿ)) != -1)
    {
      ಪೂರ್ಣಾಂಕ ಸ್ಥಿರಾಂಕ oi = -1;
      ಪೂರ್ಣಾಂಕ opt_fileno;

      ಆಯ್ಕೆ (c)
        {
        /* Old McDonald had a farm ei...  */
        ಸ್ಥಿತಿ 'e':
        ಸ್ಥಿತಿ 'i':
        ಸ್ಥಿತಿ 'o':
          opt_fileno = optc_ಗಮ್ಯ_fileno (c);
          affirm (0 <= opt_fileno && opt_fileno < countof (stdbuf));
          stdbuf[opt_fileno].optc = c;
          ಆಗ (c_isspace (*optarg))
            optarg++;
          stdbuf[opt_fileno].optarg = optarg;
          ಆದರೆ (c == 'i' && *optarg == 'L')
            {
              /* -oL will be by far the most common use of this utility,
                 but one could easily think -iL might have the same affect,
                 so disallow it as it could be confusing.  */
              error (0, 0, _("line buffering standard input is meaningless"));
              usage (EXIT_CANCELED);
            }

          ಆದರೆ (!streq (optarg, "L"))
            {
              ಎಣಿಕೆ strtol_error e = parse_ಗಾತ್ರ (optarg,
                                                &stdbuf[opt_fileno].ಗಾತ್ರ_2);
              ಆದರೆ (e != LONGINT_OK)
                xstrtol_fatal (e, oi, c, longopts, optarg);
            }

          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_CANCELED);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify at least 1 command.  */
  ಆದರೆ (argc < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  ಆದರೆ (! ಗಣ_libstdbuf_options ())
    {
      error (0, 0, _("you must specify a buffering mode option"));
      usage (EXIT_CANCELED);
    }

  /* Try to preload libstdbuf first from the same path as
     stdbuf is running from.  */
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_ಮಾರ್ಗ (ಕಾರ್ಯಕ್ರಮ_name);
  ಆದರೆ (!ಕಾರ್ಯಕ್ರಮ_ಮಾರ್ಗ)
    ಕಾರ್ಯಕ್ರಮ_ಮಾರ್ಗ = xstrdup (PKGLIBDIR);  /* Need to init to non-null.  */
  ಗಣ_LD_PRELOAD ();
  ಬಿಡುಗಡೆ (ಕಾರ್ಯಕ್ರಮ_ಮಾರ್ಗ);

  execvp (*argv, argv);

  ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  ಹಿಂತಿರುಗಿ exit_ಸ್ಥಿತಿ;
}
