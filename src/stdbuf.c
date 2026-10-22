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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <sys/types.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "assure.h"
#অন্তর্ভাৱ "filenamecat.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "xreadlink.h"
#অন্তর্ভাৱ "xstrtol-error.h"
#অন্তর্ভাৱ "xstrtol.h"
#অন্তর্ভাৱ "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "stdbuf"
#define LIB_NAME "libstdbuf.so" /* FIXME: don't hardcode  */

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

স্থির ৱর্ণ *program_path;

স্থির সংরচনা
{
  size_t size;
  পূর্ণাঙ্ক optc;
  ৱর্ণ *optarg;
} stdbuf[3];

স্থির সংরচনা option ধ্রুৱ longopts[] =
{
  {"input", required_argument, রিক্ত, 'i'},
  {"output", required_argument, রিক্ত, 'o'},
  {"error", required_argument, রিক্ত, 'e'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

/* Set size to the value of STR, interpreted as a decimal integer,
   optionally multiplied by various values.
   Return LONGINT_OK on success, otherwise an error for use with
   xstrtol_fatal.

   This supports dd BLOCK size suffixes.
   Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats.  */
স্থির গণনা strtol_error
parse_size (ৱর্ণ ধ্রুৱ *str, size_t *size)
{
  uintmax_t tmp_size;
  গণনা strtol_error e = xstrtoumax (str, রিক্ত, 10,
                                    &tmp_size, "EGkKMPQRTYZ0");
  যদি (e == LONGINT_OK && ckd_add (size, tmp_size, 0))
    e = LONGINT_OVERFLOW;
  ঘূৰাই e;
}

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s OPTION... COMMAND\n"), program_name);
      fputs (_("\
Run COMMAND, with modified buffering operations বাবে its standard streams.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --প্ৰবেশ=MODE   adjust standard প্ৰবেশ stream buffering\n\
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
If MODE is 'L' the corresponding stream will be সাৰি buffered.\n\
This option is invalid with standard প্ৰবেশ.\n"), stdout);
      fputs (_("\n\
If MODE is '0' the corresponding stream will be unbuffered.\n\
"), stdout);
      fputs (_("\n\
Otherwise MODE is a সংখ্যা which may be followed by one of the following:\n\
KB 1000, K 1024, MB 1000*1000, M 1024*1024, and so on বাবে G,T,P,E,Z,Y,R,Q.\n\
দ্বিমিক_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
In this পক্ষ the corresponding stream will be fully buffered with the buffer\n\
size set to MODE bytes.\n\
"), stdout);
      fputs (_("\n\
NOTE: If COMMAND adjusts the buffering of its standard streams ('tee' does\n\
বাবে example) then that will override corresponding changes by 'stdbuf'.\n\
Also some filters (like 'dd' and 'cat' etc.) don't use streams বাবে I/O,\n\
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
set_program_path (ৱর্ণ ধ্রুৱ *arg)
{
  যদি (strchr (arg, '/'))        /* Use absolute or relative paths directly.  */
    {
      program_path = dir_name (arg);
    }
  অন্যথা
    {
      ৱর্ণ *path = xreadlink ("/proc/self/exe");
      যদি (path)
        program_path = dir_name (path);
      অন্যথা যদি ((path = getenv ("PATH")))
        {
          path = xstrdup (path);
          বাবে (ৱর্ণ *dir = strtok (path, ":"); dir != রিক্ত;
               dir = strtok (রিক্ত, ":"))
            {
              ৱর্ণ *candidate = file_name_concat (dir, arg, রিক্ত);
              যদি (access (candidate, X_OK) == 0)
                {
                  program_path = dir_name (candidate);
                  মুক্ত (candidate);
                  ভঙ্গ;
                }
              মুক্ত (candidate);
            }
        }
      মুক্ত (path);
    }
}

স্থির পূর্ণাঙ্ক
optc_to_fileno (পূর্ণাঙ্ক c)
{
  পূর্ণাঙ্ক ret = -1;

  ৱিকল্প (c)
    {
    পক্ষ 'e':
      ret = STDERR_FILENO;
      ভঙ্গ;
    পক্ষ 'i':
      ret = STDIN_FILENO;
      ভঙ্গ;
    পক্ষ 'o':
      ret = STDOUT_FILENO;
      ভঙ্গ;
    স্ৱভাৱ:
      affirm (false);
    }

  ঘূৰাই ret;
}

স্থির শূন্য
set_LD_PRELOAD (শূন্য)
{
  পূর্ণাঙ্ক ret;
#ifdef __APPLE__
  ৱর্ণ ধ্রুৱ *preload_env = "DYLD_INSERT_LIBRARIES";
#নোচেদ্যদি defined _AIX
  ৱর্ণ ধ্রুৱ *preload_env = (পরিমাণ (শূন্য *) < 8
                             ? "LDR_PRELOAD" : "LDR_PRELOAD64");
#অন্যথা
  ৱর্ণ ধ্রুৱ *preload_env = "LD_PRELOAD";
#endif
  ৱর্ণ *old_libs = getenv (preload_env);
  ৱর্ণ *LD_PRELOAD;

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
  ৱর্ণ ধ্রুৱ *ধ্রুৱ অনুসন্ধান_path[] = {
    program_path,
    PKGLIBEXECDIR,
    রিক্ত
  };

  ৱর্ণ ধ্রুৱ *ধ্রুৱ *path = অনুসন্ধান_path;
  ৱর্ণ *libstdbuf;

  যেতিয়ালৈ (true)
    {
      সংরচনা stat sb;

      যদি (!**path)              /* system default  */
        {
          libstdbuf = xstrdup (LIB_NAME);
          ভঙ্গ;
        }
      ret = asprintf (&libstdbuf, "%s/%s", *path, LIB_NAME);
      যদি (ret < 0)
        xalloc_die ();
      যদি (stat (libstdbuf, &sb) == 0)   /* file_exists  */
        ভঙ্গ;
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

  বাবে (size_t i = 0; i < countof (stdbuf); i++)
    {
      যদি (stdbuf[i].optarg)
        {
          ৱর্ণ *var;
          পূর্ণাঙ্ক ret;

          যদি (*stdbuf[i].optarg == 'L')
            ret = asprintf (&var, "%s%c=L", "_STDBUF_",
                            c_toupper (stdbuf[i].optc));
          অন্যথা
            ret = asprintf (&var, "%s%c=%zu", "_STDBUF_",
                            c_toupper (stdbuf[i].optc),
                            stdbuf[i].size);
          যদি (ret < 0)
            xalloc_die ();

          যদি (putenv (var) != 0)
            error (EXIT_CANCELED, errno,
                   _("failed to update the environment with %s"),
                   quote (var));

          env_set = true;
        }
    }

  ঘূৰাই env_set;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  পূর্ণাঙ্ক c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  যেতিয়ালৈ ((c = getopt_long (argc, argv, "+i:o:e:", longopts, রিক্ত)) != -1)
    {
      পূর্ণাঙ্ক ধ্রুৱ oi = -1;
      পূর্ণাঙ্ক opt_fileno;

      ৱিকল্প (c)
        {
        /* Old McDonald had a farm ei...  */
        পক্ষ 'e':
        পক্ষ 'i':
        পক্ষ 'o':
          opt_fileno = optc_to_fileno (c);
          affirm (0 <= opt_fileno && opt_fileno < countof (stdbuf));
          stdbuf[opt_fileno].optc = c;
          যেতিয়ালৈ (c_isspace (*optarg))
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
              গণনা strtol_error e = parse_size (optarg,
                                                &stdbuf[opt_fileno].size);
              যদি (e != LONGINT_OK)
                xstrtol_fatal (e, oi, c, longopts, optarg);
            }

          ভঙ্গ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্ৱভাৱ:
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

  পূর্ণাঙ্ক exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  ঘূৰাই exit_status;
}
