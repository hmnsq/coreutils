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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "filenamecat.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "xreadlink.h"
#અન્તર્ભાવ "xstrtol-error.h"
#અન્તર્ભાવ "xstrtol.h"
#અન્તર્ભાવ "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "stdbuf"
#define LIB_NAME "libstdbuf.so" /* FIXME: don't hardcode  */

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

સ્થિર અક્ષર *program_path;

સ્થિર સંરચના
{
  size_t size;
  પૂર્ણાંક optc;
  અક્ષર *optarg;
} stdbuf[3];

સ્થિર સંરચના option અચલ longopts[] =
{
  {"input", required_argument, ખાલી, 'i'},
  {"output", required_argument, ખાલી, 'o'},
  {"error", required_argument, ખાલી, 'e'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

/* Set size to the value of STR, interpreted as a decimal integer,
   optionally multiplied by various values.
   Return LONGINT_OK on success, otherwise an error for use with
   xstrtol_fatal.

   This supports dd BLOCK size suffixes.
   Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats.  */
સ્થિર ગણના strtol_error
parse_size (અક્ષર અચલ *str, size_t *size)
{
  uintmax_t tmp_size;
  ગણના strtol_error e = xstrtoumax (str, ખાલી, 10,
                                    &tmp_size, "EGkKMPQRTYZ0");
  જો (e == LONGINT_OK && ckd_add (size, tmp_size, 0))
    e = LONGINT_OVERFLOW;
  પાછા e;
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("Usage: %s OPTION... COMMAND\n"), program_name);
      fputs (_("\
Run COMMAND, with modified buffering operations માટે its standard streams.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --આગત=MODE   adjust standard આગત stream buffering\n\
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
If MODE is 'L' the corresponding stream will be લીટી buffered.\n\
This option is invalid with standard આગત.\n"), stdout);
      fputs (_("\n\
If MODE is '0' the corresponding stream will be unbuffered.\n\
"), stdout);
      fputs (_("\n\
Otherwise MODE is a સંખ્યા which may be followed by one of the following:\n\
KB 1000, K 1024, MB 1000*1000, M 1024*1024, and so on માટે G,T,P,E,Z,Y,R,Q.\n\
દ્વિઅંકી_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
In this સ્થિતિ the corresponding stream will be fully buffered with the buffer\n\
size set to MODE bytes.\n\
"), stdout);
      fputs (_("\n\
NOTE: If COMMAND adjusts the buffering of its standard streams ('tee' does\n\
માટે example) then that will override corresponding changes by 'stdbuf'.\n\
Also some filters (like 'dd' and 'cat' etc.) don't use streams માટે I/O,\n\
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

સ્થિર શૂન્ય
set_program_path (અક્ષર અચલ *arg)
{
  જો (strchr (arg, '/'))        /* Use absolute or relative paths directly.  */
    {
      program_path = dir_name (arg);
    }
  નહીંતર
    {
      અક્ષર *path = xreadlink ("/proc/self/exe");
      જો (path)
        program_path = dir_name (path);
      નહીંતર જો ((path = getenv ("PATH")))
        {
          path = xstrdup (path);
          માટે (અક્ષર *dir = strtok (path, ":"); dir != ખાલી;
               dir = strtok (ખાલી, ":"))
            {
              અક્ષર *candidate = file_name_concat (dir, arg, ખાલી);
              જો (access (candidate, X_OK) == 0)
                {
                  program_path = dir_name (candidate);
                  મુક્ત (candidate);
                  તોડો;
                }
              મુક્ત (candidate);
            }
        }
      મુક્ત (path);
    }
}

સ્થિર પૂર્ણાંક
optc_to_fileno (પૂર્ણાંક c)
{
  પૂર્ણાંક ret = -1;

  પસંદગી (c)
    {
    સ્થિતિ 'e':
      ret = STDERR_FILENO;
      તોડો;
    સ્થિતિ 'i':
      ret = STDIN_FILENO;
      તોડો;
    સ્થિતિ 'o':
      ret = STDOUT_FILENO;
      તોડો;
    મૂળભૂત:
      affirm (false);
    }

  પાછા ret;
}

સ્થિર શૂન્ય
set_LD_PRELOAD (શૂન્ય)
{
  પૂર્ણાંક ret;
#ifdef __APPLE__
  અક્ષર અચલ *preload_env = "DYLD_INSERT_LIBRARIES";
#અથવાજો defined _AIX
  અક્ષર અચલ *preload_env = (કદ (શૂન્ય *) < 8
                             ? "LDR_PRELOAD" : "LDR_PRELOAD64");
#નહીંતર
  અક્ષર અચલ *preload_env = "LD_PRELOAD";
#endif
  અક્ષર *old_libs = getenv (preload_env);
  અક્ષર *LD_PRELOAD;

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
  અક્ષર અચલ *અચલ શોધ_path[] = {
    program_path,
    PKGLIBEXECDIR,
    ખાલી
  };

  અક્ષર અચલ *અચલ *path = શોધ_path;
  અક્ષર *libstdbuf;

  જ્યારે (true)
    {
      સંરચના stat sb;

      જો (!**path)              /* system default  */
        {
          libstdbuf = xstrdup (LIB_NAME);
          તોડો;
        }
      ret = asprintf (&libstdbuf, "%s/%s", *path, LIB_NAME);
      જો (ret < 0)
        xalloc_die ();
      જો (stat (libstdbuf, &sb) == 0)   /* file_exists  */
        તોડો;
      મુક્ત (libstdbuf);

      ++path;
      જો ( ! *path)
        error (EXIT_CANCELED, 0, _("failed to find %s"), quote (LIB_NAME));
    }

  /* FIXME: Do we need to support libstdbuf.dll, c:, '\' separators etc?  */

  જો (old_libs)
    ret = asprintf (&LD_PRELOAD, "%s=%s:%s", preload_env, old_libs, libstdbuf);
  નહીંતર
    ret = asprintf (&LD_PRELOAD, "%s=%s", preload_env, libstdbuf);

  જો (ret < 0)
    xalloc_die ();

  મુક્ત (libstdbuf);

  ret = putenv (LD_PRELOAD);
#ifdef __APPLE__
  જો (ret == 0)
    ret = setenv ("DYLD_FORCE_FLAT_NAMESPACE", "y", 1);
#endif

  જો (ret != 0)
    error (EXIT_CANCELED, errno,
           _("failed to update the environment with %s"),
           quote (LD_PRELOAD));
}

/* Populate environ with _STDBUF_I=$MODE _STDBUF_O=$MODE _STDBUF_E=$MODE.
   Return TRUE if any environment variables set.   */

સ્થિર bool
set_libstdbuf_options (શૂન્ય)
{
  bool env_set = false;

  માટે (size_t i = 0; i < countof (stdbuf); i++)
    {
      જો (stdbuf[i].optarg)
        {
          અક્ષર *var;
          પૂર્ણાંક ret;

          જો (*stdbuf[i].optarg == 'L')
            ret = asprintf (&var, "%s%c=L", "_STDBUF_",
                            c_toupper (stdbuf[i].optc));
          નહીંતર
            ret = asprintf (&var, "%s%c=%zu", "_STDBUF_",
                            c_toupper (stdbuf[i].optc),
                            stdbuf[i].size);
          જો (ret < 0)
            xalloc_die ();

          જો (putenv (var) != 0)
            error (EXIT_CANCELED, errno,
                   _("failed to update the environment with %s"),
                   quote (var));

          env_set = true;
        }
    }

  પાછા env_set;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  જ્યારે ((c = getopt_long (argc, argv, "+i:o:e:", longopts, ખાલી)) != -1)
    {
      પૂર્ણાંક અચલ oi = -1;
      પૂર્ણાંક opt_fileno;

      પસંદગી (c)
        {
        /* Old McDonald had a farm ei...  */
        સ્થિતિ 'e':
        સ્થિતિ 'i':
        સ્થિતિ 'o':
          opt_fileno = optc_to_fileno (c);
          affirm (0 <= opt_fileno && opt_fileno < countof (stdbuf));
          stdbuf[opt_fileno].optc = c;
          જ્યારે (c_isspace (*optarg))
            optarg++;
          stdbuf[opt_fileno].optarg = optarg;
          જો (c == 'i' && *optarg == 'L')
            {
              /* -oL will be by far the most common use of this utility,
                 but one could easily think -iL might have the same affect,
                 so disallow it as it could be confusing.  */
              error (0, 0, _("line buffering standard input is meaningless"));
              usage (EXIT_CANCELED);
            }

          જો (!streq (optarg, "L"))
            {
              ગણના strtol_error e = parse_size (optarg,
                                                &stdbuf[opt_fileno].size);
              જો (e != LONGINT_OK)
                xstrtol_fatal (e, oi, c, longopts, optarg);
            }

          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_CANCELED);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify at least 1 command.  */
  જો (argc < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  જો (! set_libstdbuf_options ())
    {
      error (0, 0, _("you must specify a buffering mode option"));
      usage (EXIT_CANCELED);
    }

  /* Try to preload libstdbuf first from the same path as
     stdbuf is running from.  */
  set_program_path (program_name);
  જો (!program_path)
    program_path = xstrdup (PKGLIBDIR);  /* Need to init to non-null.  */
  set_LD_PRELOAD ();
  મુક્ત (program_path);

  execvp (*argv, argv);

  પૂર્ણાંક exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  પાછા exit_status;
}
