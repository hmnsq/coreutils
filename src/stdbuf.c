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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "filenamecat.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "xreadlink.h"
#அந்தர்பாவ "xstrtol-error.h"
#அந்தர்பாவ "xstrtol.h"
#அந்தர்பாவ "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "stdbuf"
#define LIB_NAME "libstdbuf.so" /* FIXME: don't hardcode  */

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

நிலையான எழுத்து *program_path;

நிலையான கட்டமைப்பு
{
  size_t size;
  முழு optc;
  எழுத்து *optarg;
} stdbuf[3];

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"input", required_argument, வெற்று, 'i'},
  {"output", required_argument, வெற்று, 'o'},
  {"error", required_argument, வெற்று, 'e'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

/* Set size to the value of STR, interpreted as a decimal integer,
   optionally multiplied by various values.
   Return LONGINT_OK on success, otherwise an error for use with
   xstrtol_fatal.

   This supports dd BLOCK size suffixes.
   Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats.  */
நிலையான எண்ணிக்கை strtol_error
parse_size (எழுத்து மாறிலி *str, size_t *size)
{
  uintmax_t tmp_size;
  எண்ணிக்கை strtol_error e = xstrtoumax (str, வெற்று, 10,
                                    &tmp_size, "EGkKMPQRTYZ0");
  எனில் (e == LONGINT_OK && ckd_add (size, tmp_size, 0))
    e = LONGINT_OVERFLOW;
  திரும்பு e;
}

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("Usage: %s OPTION... COMMAND\n"), program_name);
      fputs (_("\
Run COMMAND, with modified buffering operations க்கு its standard streams.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --உள்ளீடு=MODE   adjust standard உள்ளீடு stream buffering\n\
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
If MODE is 'L' the corresponding stream will be வரி buffered.\n\
This option is invalid with standard உள்ளீடு.\n"), stdout);
      fputs (_("\n\
If MODE is '0' the corresponding stream will be unbuffered.\n\
"), stdout);
      fputs (_("\n\
Otherwise MODE is a எண் which may be followed by one of the following:\n\
KB 1000, K 1024, MB 1000*1000, M 1024*1024, and so on க்கு G,T,P,E,Z,Y,R,Q.\n\
இருமம்_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
In this நிலை the corresponding stream will be fully buffered with the buffer\n\
size set to MODE bytes.\n\
"), stdout);
      fputs (_("\n\
NOTE: If COMMAND adjusts the buffering of its standard streams ('tee' does\n\
க்கு example) then that will override corresponding changes by 'stdbuf'.\n\
Also some filters (like 'dd' and 'cat' etc.) don't use streams க்கு I/O,\n\
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

நிலையான வெறுமை
set_program_path (எழுத்து மாறிலி *arg)
{
  எனில் (strchr (arg, '/'))        /* Use absolute or relative paths directly.  */
    {
      program_path = dir_name (arg);
    }
  இல்லையெனில்
    {
      எழுத்து *path = xreadlink ("/proc/self/exe");
      எனில் (path)
        program_path = dir_name (path);
      இல்லையெனில் எனில் ((path = getenv ("PATH")))
        {
          path = xstrdup (path);
          க்கு (எழுத்து *dir = strtok (path, ":"); dir != வெற்று;
               dir = strtok (வெற்று, ":"))
            {
              எழுத்து *candidate = file_name_concat (dir, arg, வெற்று);
              எனில் (access (candidate, X_OK) == 0)
                {
                  program_path = dir_name (candidate);
                  விடுவி (candidate);
                  நிறுத்து;
                }
              விடுவி (candidate);
            }
        }
      விடுவி (path);
    }
}

நிலையான முழு
optc_to_fileno (முழு c)
{
  முழு ret = -1;

  தேர்வு (c)
    {
    நிலை 'e':
      ret = STDERR_FILENO;
      நிறுத்து;
    நிலை 'i':
      ret = STDIN_FILENO;
      நிறுத்து;
    நிலை 'o':
      ret = STDOUT_FILENO;
      நிறுத்து;
    இயல்பு:
      affirm (false);
    }

  திரும்பு ret;
}

நிலையான வெறுமை
set_LD_PRELOAD (வெறுமை)
{
  முழு ret;
#ifdef __APPLE__
  எழுத்து மாறிலி *preload_env = "DYLD_INSERT_LIBRARIES";
#அல்லதெனில் defined _AIX
  எழுத்து மாறிலி *preload_env = (அளவு (வெறுமை *) < 8
                             ? "LDR_PRELOAD" : "LDR_PRELOAD64");
#இல்லையெனில்
  எழுத்து மாறிலி *preload_env = "LD_PRELOAD";
#endif
  எழுத்து *old_libs = getenv (preload_env);
  எழுத்து *LD_PRELOAD;

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
  எழுத்து மாறிலி *மாறிலி தேடல்_path[] = {
    program_path,
    PKGLIBEXECDIR,
    வெற்று
  };

  எழுத்து மாறிலி *மாறிலி *path = தேடல்_path;
  எழுத்து *libstdbuf;

  போது (true)
    {
      கட்டமைப்பு stat sb;

      எனில் (!**path)              /* system default  */
        {
          libstdbuf = xstrdup (LIB_NAME);
          நிறுத்து;
        }
      ret = asprintf (&libstdbuf, "%s/%s", *path, LIB_NAME);
      எனில் (ret < 0)
        xalloc_die ();
      எனில் (stat (libstdbuf, &sb) == 0)   /* file_exists  */
        நிறுத்து;
      விடுவி (libstdbuf);

      ++path;
      எனில் ( ! *path)
        error (EXIT_CANCELED, 0, _("failed to find %s"), quote (LIB_NAME));
    }

  /* FIXME: Do we need to support libstdbuf.dll, c:, '\' separators etc?  */

  எனில் (old_libs)
    ret = asprintf (&LD_PRELOAD, "%s=%s:%s", preload_env, old_libs, libstdbuf);
  இல்லையெனில்
    ret = asprintf (&LD_PRELOAD, "%s=%s", preload_env, libstdbuf);

  எனில் (ret < 0)
    xalloc_die ();

  விடுவி (libstdbuf);

  ret = putenv (LD_PRELOAD);
#ifdef __APPLE__
  எனில் (ret == 0)
    ret = setenv ("DYLD_FORCE_FLAT_NAMESPACE", "y", 1);
#endif

  எனில் (ret != 0)
    error (EXIT_CANCELED, errno,
           _("failed to update the environment with %s"),
           quote (LD_PRELOAD));
}

/* Populate environ with _STDBUF_I=$MODE _STDBUF_O=$MODE _STDBUF_E=$MODE.
   Return TRUE if any environment variables set.   */

நிலையான bool
set_libstdbuf_options (வெறுமை)
{
  bool env_set = false;

  க்கு (size_t i = 0; i < countof (stdbuf); i++)
    {
      எனில் (stdbuf[i].optarg)
        {
          எழுத்து *var;
          முழு ret;

          எனில் (*stdbuf[i].optarg == 'L')
            ret = asprintf (&var, "%s%c=L", "_STDBUF_",
                            c_toupper (stdbuf[i].optc));
          இல்லையெனில்
            ret = asprintf (&var, "%s%c=%zu", "_STDBUF_",
                            c_toupper (stdbuf[i].optc),
                            stdbuf[i].size);
          எனில் (ret < 0)
            xalloc_die ();

          எனில் (putenv (var) != 0)
            error (EXIT_CANCELED, errno,
                   _("failed to update the environment with %s"),
                   quote (var));

          env_set = true;
        }
    }

  திரும்பு env_set;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  முழு c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  போது ((c = getopt_long (argc, argv, "+i:o:e:", longopts, வெற்று)) != -1)
    {
      முழு மாறிலி oi = -1;
      முழு opt_fileno;

      தேர்வு (c)
        {
        /* Old McDonald had a farm ei...  */
        நிலை 'e':
        நிலை 'i':
        நிலை 'o':
          opt_fileno = optc_to_fileno (c);
          affirm (0 <= opt_fileno && opt_fileno < countof (stdbuf));
          stdbuf[opt_fileno].optc = c;
          போது (c_isspace (*optarg))
            optarg++;
          stdbuf[opt_fileno].optarg = optarg;
          எனில் (c == 'i' && *optarg == 'L')
            {
              /* -oL will be by far the most common use of this utility,
                 but one could easily think -iL might have the same affect,
                 so disallow it as it could be confusing.  */
              error (0, 0, _("line buffering standard input is meaningless"));
              usage (EXIT_CANCELED);
            }

          எனில் (!streq (optarg, "L"))
            {
              எண்ணிக்கை strtol_error e = parse_size (optarg,
                                                &stdbuf[opt_fileno].size);
              எனில் (e != LONGINT_OK)
                xstrtol_fatal (e, oi, c, longopts, optarg);
            }

          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_CANCELED);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify at least 1 command.  */
  எனில் (argc < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  எனில் (! set_libstdbuf_options ())
    {
      error (0, 0, _("you must specify a buffering mode option"));
      usage (EXIT_CANCELED);
    }

  /* Try to preload libstdbuf first from the same path as
     stdbuf is running from.  */
  set_program_path (program_name);
  எனில் (!program_path)
    program_path = xstrdup (PKGLIBDIR);  /* Need to init to non-null.  */
  set_LD_PRELOAD ();
  விடுவி (program_path);

  execvp (*argv, argv);

  முழு exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  திரும்பு exit_status;
}
