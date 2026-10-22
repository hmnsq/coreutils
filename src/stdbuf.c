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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "filenamecat.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "xreadlink.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol-error.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol.h"
#ଅନ୍ତର୍ଭାଵ "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "stdbuf"
#define LIB_NAME "libstdbuf.so" /* FIXME: don't hardcode  */

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

ସ୍ଥିର ଅକ୍ଷର *program_path;

ସ୍ଥିର ସଂରଚନା
{
  size_t size;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  ଅକ୍ଷର *optarg;
} stdbuf[3];

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"input", required_argument, ଖାଲି, 'i'},
  {"output", required_argument, ଖାଲି, 'o'},
  {"error", required_argument, ଖାଲି, 'e'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

/* Set size to the value of STR, interpreted as a decimal integer,
   optionally multiplied by various values.
   Return LONGINT_OK on success, otherwise an error for use with
   xstrtol_fatal.

   This supports dd BLOCK size suffixes.
   Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats.  */
ସ୍ଥିର ଗଣନା strtol_error
parse_size (ଅକ୍ଷର ଅଚଳ *str, size_t *size)
{
  uintmax_t tmp_size;
  ଗଣନା strtol_error e = xstrtoumax (str, ଖାଲି, 10,
                                    &tmp_size, "EGkKMPQRTYZ0");
  ଯଦି (e == LONGINT_OK && ckd_add (size, tmp_size, 0))
    e = LONGINT_OVERFLOW;
  ଫେରନ୍ତୁ e;
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("Usage: %s OPTION... COMMAND\n"), program_name);
      fputs (_("\
Run COMMAND, with modified buffering operations ପାଇଁ its standard streams.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --ନିବେଶ=MODE   adjust standard ନିବେଶ stream buffering\n\
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
If MODE is 'L' the corresponding stream will be ଧାଡ଼ି buffered.\n\
This option is invalid with standard ନିବେଶ.\n"), stdout);
      fputs (_("\n\
If MODE is '0' the corresponding stream will be unbuffered.\n\
"), stdout);
      fputs (_("\n\
Otherwise MODE is a ସଂଖ୍ୟା which may be followed by one of the following:\n\
KB 1000, K 1024, MB 1000*1000, M 1024*1024, and so on ପାଇଁ G,T,P,E,Z,Y,R,Q.\n\
ଦ୍ୱିମିକ_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
In this ସ୍ଥିତି the corresponding stream will be fully buffered with the buffer\n\
size set to MODE bytes.\n\
"), stdout);
      fputs (_("\n\
NOTE: If COMMAND adjusts the buffering of its standard streams ('tee' does\n\
ପାଇଁ example) then that will override corresponding changes by 'stdbuf'.\n\
Also some filters (like 'dd' and 'cat' etc.) don't use streams ପାଇଁ I/O,\n\
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

ସ୍ଥିର ଶୂନ୍ୟ
set_program_path (ଅକ୍ଷର ଅଚଳ *arg)
{
  ଯଦି (strchr (arg, '/'))        /* Use absolute or relative paths directly.  */
    {
      program_path = dir_name (arg);
    }
  ଅନ୍ୟଥା
    {
      ଅକ୍ଷର *path = xreadlink ("/proc/self/exe");
      ଯଦି (path)
        program_path = dir_name (path);
      ଅନ୍ୟଥା ଯଦି ((path = getenv ("PATH")))
        {
          path = xstrdup (path);
          ପାଇଁ (ଅକ୍ଷର *dir = strtok (path, ":"); dir != ଖାଲି;
               dir = strtok (ଖାଲି, ":"))
            {
              ଅକ୍ଷର *candidate = file_name_concat (dir, arg, ଖାଲି);
              ଯଦି (access (candidate, X_OK) == 0)
                {
                  program_path = dir_name (candidate);
                  ମୁକ୍ତ (candidate);
                  ଭାଙ୍ଗ;
                }
              ମୁକ୍ତ (candidate);
            }
        }
      ମୁକ୍ତ (path);
    }
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
optc_to_fileno (ପୂର୍ଣ୍ଣସଂଖ୍ୟା c)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;

  ବାଛ (c)
    {
    ସ୍ଥିତି 'e':
      ret = STDERR_FILENO;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'i':
      ret = STDIN_FILENO;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'o':
      ret = STDOUT_FILENO;
      ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      affirm (false);
    }

  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ
set_LD_PRELOAD (ଶୂନ୍ୟ)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
#ifdef __APPLE__
  ଅକ୍ଷର ଅଚଳ *preload_env = "DYLD_INSERT_LIBRARIES";
#ନଚେତଯଦି defined _AIX
  ଅକ୍ଷର ଅଚଳ *preload_env = (ଆକାର (ଶୂନ୍ୟ *) < 8
                             ? "LDR_PRELOAD" : "LDR_PRELOAD64");
#ଅନ୍ୟଥା
  ଅକ୍ଷର ଅଚଳ *preload_env = "LD_PRELOAD";
#endif
  ଅକ୍ଷର *old_libs = getenv (preload_env);
  ଅକ୍ଷର *LD_PRELOAD;

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
  ଅକ୍ଷର ଅଚଳ *ଅଚଳ ସନ୍ଧାନ_path[] = {
    program_path,
    PKGLIBEXECDIR,
    ଖାଲି
  };

  ଅକ୍ଷର ଅଚଳ *ଅଚଳ *path = ସନ୍ଧାନ_path;
  ଅକ୍ଷର *libstdbuf;

  ଯେତେବେଳେ (true)
    {
      ସଂରଚନା stat sb;

      ଯଦି (!**path)              /* system default  */
        {
          libstdbuf = xstrdup (LIB_NAME);
          ଭାଙ୍ଗ;
        }
      ret = asprintf (&libstdbuf, "%s/%s", *path, LIB_NAME);
      ଯଦି (ret < 0)
        xalloc_die ();
      ଯଦି (stat (libstdbuf, &sb) == 0)   /* file_exists  */
        ଭାଙ୍ଗ;
      ମୁକ୍ତ (libstdbuf);

      ++path;
      ଯଦି ( ! *path)
        error (EXIT_CANCELED, 0, _("failed to find %s"), quote (LIB_NAME));
    }

  /* FIXME: Do we need to support libstdbuf.dll, c:, '\' separators etc?  */

  ଯଦି (old_libs)
    ret = asprintf (&LD_PRELOAD, "%s=%s:%s", preload_env, old_libs, libstdbuf);
  ଅନ୍ୟଥା
    ret = asprintf (&LD_PRELOAD, "%s=%s", preload_env, libstdbuf);

  ଯଦି (ret < 0)
    xalloc_die ();

  ମୁକ୍ତ (libstdbuf);

  ret = putenv (LD_PRELOAD);
#ifdef __APPLE__
  ଯଦି (ret == 0)
    ret = setenv ("DYLD_FORCE_FLAT_NAMESPACE", "y", 1);
#endif

  ଯଦି (ret != 0)
    error (EXIT_CANCELED, errno,
           _("failed to update the environment with %s"),
           quote (LD_PRELOAD));
}

/* Populate environ with _STDBUF_I=$MODE _STDBUF_O=$MODE _STDBUF_E=$MODE.
   Return TRUE if any environment variables set.   */

ସ୍ଥିର bool
set_libstdbuf_options (ଶୂନ୍ୟ)
{
  bool env_set = false;

  ପାଇଁ (size_t i = 0; i < countof (stdbuf); i++)
    {
      ଯଦି (stdbuf[i].optarg)
        {
          ଅକ୍ଷର *var;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

          ଯଦି (*stdbuf[i].optarg == 'L')
            ret = asprintf (&var, "%s%c=L", "_STDBUF_",
                            c_toupper (stdbuf[i].optc));
          ଅନ୍ୟଥା
            ret = asprintf (&var, "%s%c=%zu", "_STDBUF_",
                            c_toupper (stdbuf[i].optc),
                            stdbuf[i].size);
          ଯଦି (ret < 0)
            xalloc_die ();

          ଯଦି (putenv (var) != 0)
            error (EXIT_CANCELED, errno,
                   _("failed to update the environment with %s"),
                   quote (var));

          env_set = true;
        }
    }

  ଫେରନ୍ତୁ env_set;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ଯେତେବେଳେ ((c = getopt_long (argc, argv, "+i:o:e:", longopts, ଖାଲି)) != -1)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଚଳ oi = -1;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_fileno;

      ବାଛ (c)
        {
        /* Old McDonald had a farm ei...  */
        ସ୍ଥିତି 'e':
        ସ୍ଥିତି 'i':
        ସ୍ଥିତି 'o':
          opt_fileno = optc_to_fileno (c);
          affirm (0 <= opt_fileno && opt_fileno < countof (stdbuf));
          stdbuf[opt_fileno].optc = c;
          ଯେତେବେଳେ (c_isspace (*optarg))
            optarg++;
          stdbuf[opt_fileno].optarg = optarg;
          ଯଦି (c == 'i' && *optarg == 'L')
            {
              /* -oL will be by far the most common use of this utility,
                 but one could easily think -iL might have the same affect,
                 so disallow it as it could be confusing.  */
              error (0, 0, _("line buffering standard input is meaningless"));
              usage (EXIT_CANCELED);
            }

          ଯଦି (!streq (optarg, "L"))
            {
              ଗଣନା strtol_error e = parse_size (optarg,
                                                &stdbuf[opt_fileno].size);
              ଯଦି (e != LONGINT_OK)
                xstrtol_fatal (e, oi, c, longopts, optarg);
            }

          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_CANCELED);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify at least 1 command.  */
  ଯଦି (argc < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  ଯଦି (! set_libstdbuf_options ())
    {
      error (0, 0, _("you must specify a buffering mode option"));
      usage (EXIT_CANCELED);
    }

  /* Try to preload libstdbuf first from the same path as
     stdbuf is running from.  */
  set_program_path (program_name);
  ଯଦି (!program_path)
    program_path = xstrdup (PKGLIBDIR);  /* Need to init to non-null.  */
  set_LD_PRELOAD ();
  ମୁକ୍ତ (program_path);

  execvp (*argv, argv);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  ଫେରନ୍ତୁ exit_status;
}
