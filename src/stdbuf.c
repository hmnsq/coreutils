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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "filenamecat.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "xreadlink.h"
#అన్తర్భావ "xstrtol-error.h"
#అన్తర్భావ "xstrtol.h"
#అన్తర్భావ "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "stdbuf"
#నిర్వచించు LIB_NAME "libstdbuf.so" /* FIXME: don't hardcode  */

#నిర్వచించు AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

స్థిర అక్షరం *కార్యక్రమం_మార్గం;

స్థిర నిర్మాణం
{
  size_t కొలత;
  పూర్ణాంకం optc;
  అక్షరం *optarg;
} stdbuf[3];

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"input", required_వాదం, ఖాళీ, 'i'},
  {"output", required_వాదం, ఖాళీ, 'o'},
  {"error", required_వాదం, ఖాళీ, 'e'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* Set size to the value of STR, interpreted as a decimal integer,
   optionally multiplied by various values.
   Return LONGINT_OK on success, otherwise an error for use with
   xstrtol_fatal.

   This supports dd BLOCK size suffixes.
   Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats.  */
స్థిర గణన strtol_error
parse_కొలత (అక్షరం స్థిరరాశి *str, size_t *కొలత)
{
  uintmax_t tmp_కొలత;
  గణన strtol_error e = xstrtoumax (str, ఖాళీ, 10,
                                    &tmp_కొలత, "EGkKMPQRTYZ0");
  ఒకవేళ (e == LONGINT_OK && ckd_కూడు (కొలత, tmp_కొలత, 0))
    e = LONGINT_అధికప్రవాహం;
  తిరిగి e;
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s OPTION... COMMAND\n"), కార్యక్రమం_name);
      fputs (_("\
నిరంతరం COMMAND, with modified buffering operations కొరకు its standard streams.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --ఆదానం=బహుళకం   adjust standard ఆదానం ప్రవాహం buffering\n\
"));
      oputs (_("\
  -o, --output=బహుళకం  adjust standard output ప్రవాహం buffering\n\
"));
      oputs (_("\
  -e, --error=బహుళకం   adjust standard error ప్రవాహం buffering\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\n\
If బహుళకం is 'L' the corresponding ప్రవాహం will be పంక్తి buffered.\n\
This option is invalid with standard ఆదానం.\n"), stdout);
      fputs (_("\n\
If బహుళకం is '0' the corresponding ప్రవాహం will be unbuffered.\n\
"), stdout);
      fputs (_("\n\
Otherwise బహుళకం is a సంఖ్య which may be followed by one యొక్క the following:\n\
KB 1000, K 1024, MB 1000*1000, M 1024*1024, and so on కొరకు G,T,P,E,Z,Y,R,Q.\n\
ద్విమాన_2 prefixes can be వాడిన, too: KiB=K, MiB=M, and so on.\n\
లోపల_2 this స్థితి the corresponding ప్రవాహం will be fully buffered with the బఫరు\n\
కొలత సమితి_2 గమ్యం బహుళకం బైట్లు.\n\
"), stdout);
      fputs (_("\n\
NOTE: If COMMAND adjusts the buffering యొక్క its standard streams ('tee' does\n\
కొరకు example) then that will override corresponding changes by 'stdbuf'.\n\
Also some filters (like 'dd' and 'cat' etc.) don't use streams కొరకు I/O,\n\
and are thus unaffected by 'stdbuf' settings.\n\
"), stdout);
      emit_exec_స్థితి (కార్యక్రమం_NAME);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* argv[0] can be anything really, but generally it contains
   the path to the executable or just a name if it was executed
   using $PATH. In the latter case to get the path we can:
   search getenv("PATH"), readlink("/prof/self/exe"), getenv("_"),
   dladdr(), pstat_getpathname(), etc.  */

స్థిర శూన్య
సమితి_కార్యక్రమం_మార్గం (అక్షరం స్థిరరాశి *arg)
{
  ఒకవేళ (strchr (arg, '/'))        /* Use absolute or relative paths directly.  */
    {
      కార్యక్రమం_మార్గం = dir_name (arg);
    }
  లేకపోతే
    {
      అక్షరం *మార్గం = xreadlink ("/proc/self/exe");
      ఒకవేళ (మార్గం)
        కార్యక్రమం_మార్గం = dir_name (మార్గం);
      లేకపోతే ఒకవేళ ((మార్గం = getenv ("PATH")))
        {
          మార్గం = xstrdup (మార్గం);
          కొరకు (అక్షరం *dir = strtok (మార్గం, ":"); dir != ఖాళీ;
               dir = strtok (ఖాళీ, ":"))
            {
              అక్షరం *candidate = file_name_concat (dir, arg, ఖాళీ);
              ఒకవేళ (access (candidate, X_OK) == 0)
                {
                  కార్యక్రమం_మార్గం = dir_name (candidate);
                  విడుదల (candidate);
                  ఆపు;
                }
              విడుదల (candidate);
            }
        }
      విడుదల (మార్గం);
    }
}

స్థిర పూర్ణాంకం
optc_గమ్యం_fileno (పూర్ణాంకం c)
{
  పూర్ణాంకం ret = -1;

  ఎంపిక (c)
    {
    స్థితి 'e':
      ret = STDERR_FILENO;
      ఆపు;
    స్థితి 'i':
      ret = STDIN_FILENO;
      ఆపు;
    స్థితి 'o':
      ret = STDOUT_FILENO;
      ఆపు;
    అప్రమేయ:
      affirm (false);
    }

  తిరిగి ret;
}

స్థిర శూన్య
సమితి_LD_PRELOAD (శూన్య)
{
  పూర్ణాంకం ret;
#నిర్వచితమైతే __APPLE__
  అక్షరం స్థిరరాశి *preload_env = "DYLD_INSERT_LIBRARIES";
#లేదావేళ defined _AIX
  అక్షరం స్థిరరాశి *preload_env = (పరిమాణం (శూన్య *) < 8
                             ? "LDR_PRELOAD" : "LDR_PRELOAD64");
#లేకపోతే
  అక్షరం స్థిరరాశి *preload_env = "LD_PRELOAD";
#అయితే_ముగింపు
  అక్షరం *old_libs = getenv (preload_env);
  అక్షరం *LD_PRELOAD;

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
  అక్షరం స్థిరరాశి *స్థిరరాశి శోధన_మార్గం[] = {
    కార్యక్రమం_మార్గం,
    PKGLIBEXECDIR,
    ఖాళీ
  };

  అక్షరం స్థిరరాశి *స్థిరరాశి *మార్గం = శోధన_మార్గం;
  అక్షరం *libstdbuf;

  అయితే (true)
    {
      నిర్మాణం stat sb;

      ఒకవేళ (!**మార్గం)              /* system default  */
        {
          libstdbuf = xstrdup (LIB_NAME);
          ఆపు;
        }
      ret = asprintf (&libstdbuf, "%s/%s", *మార్గం, LIB_NAME);
      ఒకవేళ (ret < 0)
        xalloc_die ();
      ఒకవేళ (stat (libstdbuf, &sb) == 0)   /* file_exists  */
        ఆపు;
      విడుదల (libstdbuf);

      ++మార్గం;
      ఒకవేళ ( ! *మార్గం)
        error (EXIT_CANCELED, 0, _("failed to find %s"), quote (LIB_NAME));
    }

  /* FIXME: Do we need to support libstdbuf.dll, c:, '\' separators etc?  */

  ఒకవేళ (old_libs)
    ret = asprintf (&LD_PRELOAD, "%s=%s:%s", preload_env, old_libs, libstdbuf);
  లేకపోతే
    ret = asprintf (&LD_PRELOAD, "%s=%s", preload_env, libstdbuf);

  ఒకవేళ (ret < 0)
    xalloc_die ();

  విడుదల (libstdbuf);

  ret = putenv (LD_PRELOAD);
#నిర్వచితమైతే __APPLE__
  ఒకవేళ (ret == 0)
    ret = setenv ("DYLD_FORCE_FLAT_NAMESPACE", "y", 1);
#అయితే_ముగింపు

  ఒకవేళ (ret != 0)
    error (EXIT_CANCELED, errno,
           _("failed to update the environment with %s"),
           quote (LD_PRELOAD));
}

/* Populate environ with _STDBUF_I=$MODE _STDBUF_O=$MODE _STDBUF_E=$MODE.
   Return TRUE if any environment variables set.   */

స్థిర bool
సమితి_libstdbuf_options (శూన్య)
{
  bool env_సమితి = false;

  కొరకు (size_t i = 0; i < countof (stdbuf); i++)
    {
      ఒకవేళ (stdbuf[i].optarg)
        {
          అక్షరం *var;
          పూర్ణాంకం ret;

          ఒకవేళ (*stdbuf[i].optarg == 'L')
            ret = asprintf (&var, "%s%c=L", "_STDBUF_",
                            c_toupper (stdbuf[i].optc));
          లేకపోతే
            ret = asprintf (&var, "%s%c=%zu", "_STDBUF_",
                            c_toupper (stdbuf[i].optc),
                            stdbuf[i].కొలత);
          ఒకవేళ (ret < 0)
            xalloc_die ();

          ఒకవేళ (putenv (var) != 0)
            error (EXIT_CANCELED, errno,
                   _("failed to update the environment with %s"),
                   quote (var));

          env_సమితి = true;
        }
    }

  తిరిగి env_సమితి;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం c;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  అయితే ((c = getopt_long (argc, argv, "+i:o:e:", longopts, ఖాళీ)) != -1)
    {
      పూర్ణాంకం స్థిరరాశి oi = -1;
      పూర్ణాంకం opt_fileno;

      ఎంపిక (c)
        {
        /* Old McDonald had a farm ei...  */
        స్థితి 'e':
        స్థితి 'i':
        స్థితి 'o':
          opt_fileno = optc_గమ్యం_fileno (c);
          affirm (0 <= opt_fileno && opt_fileno < countof (stdbuf));
          stdbuf[opt_fileno].optc = c;
          అయితే (c_isspace (*optarg))
            optarg++;
          stdbuf[opt_fileno].optarg = optarg;
          ఒకవేళ (c == 'i' && *optarg == 'L')
            {
              /* -oL will be by far the most common use of this utility,
                 but one could easily think -iL might have the same affect,
                 so disallow it as it could be confusing.  */
              error (0, 0, _("line buffering standard input is meaningless"));
              usage (EXIT_CANCELED);
            }

          ఒకవేళ (!streq (optarg, "L"))
            {
              గణన strtol_error e = parse_కొలత (optarg,
                                                &stdbuf[opt_fileno].కొలత);
              ఒకవేళ (e != LONGINT_OK)
                xstrtol_fatal (e, oi, c, longopts, optarg);
            }

          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_CANCELED);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify at least 1 command.  */
  ఒకవేళ (argc < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  ఒకవేళ (! సమితి_libstdbuf_options ())
    {
      error (0, 0, _("you must specify a buffering mode option"));
      usage (EXIT_CANCELED);
    }

  /* Try to preload libstdbuf first from the same path as
     stdbuf is running from.  */
  సమితి_కార్యక్రమం_మార్గం (కార్యక్రమం_name);
  ఒకవేళ (!కార్యక్రమం_మార్గం)
    కార్యక్రమం_మార్గం = xstrdup (PKGLIBDIR);  /* Need to init to non-null.  */
  సమితి_LD_PRELOAD ();
  విడుదల (కార్యక్రమం_మార్గం);

  execvp (*argv, argv);

  పూర్ణాంకం exit_స్థితి = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  తిరిగి exit_స్థితి;
}
