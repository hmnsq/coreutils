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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>

#समावेश "system.h"
#समावेश "assure.h"
#समावेश "filenamecat.h"
#समावेश "quote.h"
#समावेश "xreadlink.h"
#समावेश "xstrtol-error.h"
#समावेश "xstrtol.h"
#समावेश "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "stdbuf"
#घोषणा LIB_NAME "libstdbuf.so" /* FIXME: don't hardcode  */

#घोषणा AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

अटल अक्षर *program_path;

अटल काष्ठा
{
  माप_प्रकार आकार;
  पूर्णांक optc;
  अक्षर *optarg;
} stdbuf[3];

अटल काष्ठा option स्थिर longopts[] =
{
  {"input", required_argument, शून्य, 'i'},
  {"output", required_argument, शून्य, 'o'},
  {"error", required_argument, शून्य, 'e'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

/* Set size to the value of STR, interpreted as a decimal integer,
   optionally multiplied by various values.
   Return LONGINT_OK on success, otherwise an error for use with
   xstrtol_fatal.

   This supports dd BLOCK size suffixes.
   Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats.  */
अटल क्रमागत strtol_error
parse_आकार (अक्षर स्थिर *str, माप_प्रकार *आकार)
{
  uintmax_t tmp_आकार;
  क्रमागत strtol_error e = xstrtoumax (str, शून्य, 10,
                                    &tmp_आकार, "EGkKMPQRTYZ0");
  अगर (e == LONGINT_OK && ckd_add (आकार, tmp_आकार, 0))
    e = LONGINT_OVERFLOW;
  वापस e;
}

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s OPTION... COMMAND\n"), program_name);
      ख_माला_दो (_("\
Run COMMAND, with modified buffering operations क्रम its standard streams.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --निवेश=MODE   adjust standard निवेश stream buffering\n\
"));
      oputs (_("\
  -o, --output=MODE  adjust standard output stream buffering\n\
"));
      oputs (_("\
  -e, --error=MODE   adjust standard error stream buffering\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\n\
If MODE is 'L' the corresponding stream will be पंक्ति buffered.\n\
This option is invalid with standard निवेश.\n"), मानक_निकास);
      ख_माला_दो (_("\n\
If MODE is '0' the corresponding stream will be unbuffered.\n\
"), मानक_निकास);
      ख_माला_दो (_("\n\
Otherwise MODE is a संख्या which may be followed by one of the following:\n\
KB 1000, K 1024, MB 1000*1000, M 1024*1024, and so on क्रम G,T,P,E,Z,Y,R,Q.\n\
द्विआधारी_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
In this हाल the corresponding stream will be fully buffered with the बफर\n\
आकार set to MODE bytes.\n\
"), मानक_निकास);
      ख_माला_दो (_("\n\
NOTE: If COMMAND adjusts the buffering of its standard streams ('tee' does\n\
क्रम example) then that will override corresponding changes by 'stdbuf'.\n\
Also some filters (like 'dd' and 'cat' etc.) don't use streams क्रम I/O,\n\
and are thus unaffected by 'stdbuf' settings.\n\
"), मानक_निकास);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* argv[0] can be anything really, but generally it contains
   the path to the executable or just a name if it was executed
   using $PATH. In the latter case to get the path we can:
   search getenv("PATH"), readlink("/prof/self/exe"), getenv("_"),
   dladdr(), pstat_getpathname(), etc.  */

अटल व्योम
set_program_path (अक्षर स्थिर *arg)
{
  अगर (म_अक्षर (arg, '/'))        /* Use absolute or relative paths directly.  */
    {
      program_path = dir_name (arg);
    }
  अन्यथा
    {
      अक्षर *path = xreadlink ("/proc/self/exe");
      अगर (path)
        program_path = dir_name (path);
      अन्यथा अगर ((path = दो_पर्या ("PATH")))
        {
          path = xstrdup (path);
          क्रम (अक्षर *dir = म_मोहर (path, ":"); dir != शून्य;
               dir = म_मोहर (शून्य, ":"))
            {
              अक्षर *candidate = file_name_concat (dir, arg, शून्य);
              अगर (access (candidate, X_OK) == 0)
                {
                  program_path = dir_name (candidate);
                  मुक्त (candidate);
                  अवरोध;
                }
              मुक्त (candidate);
            }
        }
      मुक्त (path);
    }
}

अटल पूर्णांक
optc_to_fileno (पूर्णांक c)
{
  पूर्णांक ret = -1;

  चयन (c)
    {
    हाल 'e':
      ret = STDERR_FILENO;
      अवरोध;
    हाल 'i':
      ret = STDIN_FILENO;
      अवरोध;
    हाल 'o':
      ret = STDOUT_FILENO;
      अवरोध;
    शेष:
      affirm (false);
    }

  वापस ret;
}

अटल व्योम
set_LD_PRELOAD (व्योम)
{
  पूर्णांक ret;
#अगर_घोषित __APPLE__
  अक्षर स्थिर *preload_env = "DYLD_INSERT_LIBRARIES";
#या_अगर defined _AIX
  अक्षर स्थिर *preload_env = (माप (व्योम *) < 8
                             ? "LDR_PRELOAD" : "LDR_PRELOAD64");
#अन्यथा
  अक्षर स्थिर *preload_env = "LD_PRELOAD";
#पूर्ण_अगर
  अक्षर *old_libs = दो_पर्या (preload_env);
  अक्षर *LD_PRELOAD;

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
  अक्षर स्थिर *स्थिर खोज_path[] = {
    program_path,
    PKGLIBEXECDIR,
    शून्य
  };

  अक्षर स्थिर *स्थिर *path = खोज_path;
  अक्षर *libstdbuf;

  जबतक (true)
    {
      काष्ठा stat sb;

      अगर (!**path)              /* system default  */
        {
          libstdbuf = xstrdup (LIB_NAME);
          अवरोध;
        }
      ret = asprintf (&libstdbuf, "%s/%s", *path, LIB_NAME);
      अगर (ret < 0)
        xalloc_die ();
      अगर (stat (libstdbuf, &sb) == 0)   /* file_exists  */
        अवरोध;
      मुक्त (libstdbuf);

      ++path;
      अगर ( ! *path)
        error (EXIT_CANCELED, 0, _("failed to find %s"), quote (LIB_NAME));
    }

  /* FIXME: Do we need to support libstdbuf.dll, c:, '\' separators etc?  */

  अगर (old_libs)
    ret = asprintf (&LD_PRELOAD, "%s=%s:%s", preload_env, old_libs, libstdbuf);
  अन्यथा
    ret = asprintf (&LD_PRELOAD, "%s=%s", preload_env, libstdbuf);

  अगर (ret < 0)
    xalloc_die ();

  मुक्त (libstdbuf);

  ret = putenv (LD_PRELOAD);
#अगर_घोषित __APPLE__
  अगर (ret == 0)
    ret = setenv ("DYLD_FORCE_FLAT_NAMESPACE", "y", 1);
#पूर्ण_अगर

  अगर (ret != 0)
    error (EXIT_CANCELED, त्रुटि_सं,
           _("failed to update the environment with %s"),
           quote (LD_PRELOAD));
}

/* Populate environ with _STDBUF_I=$MODE _STDBUF_O=$MODE _STDBUF_E=$MODE.
   Return TRUE if any environment variables set.   */

अटल bool
set_libstdbuf_options (व्योम)
{
  bool env_set = false;

  क्रम (माप_प्रकार i = 0; i < countof (stdbuf); i++)
    {
      अगर (stdbuf[i].optarg)
        {
          अक्षर *var;
          पूर्णांक ret;

          अगर (*stdbuf[i].optarg == 'L')
            ret = asprintf (&var, "%s%c=L", "_STDBUF_",
                            c_toupper (stdbuf[i].optc));
          अन्यथा
            ret = asprintf (&var, "%s%c=%zu", "_STDBUF_",
                            c_toupper (stdbuf[i].optc),
                            stdbuf[i].आकार);
          अगर (ret < 0)
            xalloc_die ();

          अगर (putenv (var) != 0)
            error (EXIT_CANCELED, त्रुटि_सं,
                   _("failed to update the environment with %s"),
                   quote (var));

          env_set = true;
        }
    }

  वापस env_set;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  निकास_पर (close_stdout);

  जबतक ((c = getopt_long (argc, argv, "+i:o:e:", longopts, शून्य)) != -1)
    {
      पूर्णांक स्थिर oi = -1;
      पूर्णांक opt_fileno;

      चयन (c)
        {
        /* Old McDonald had a farm ei...  */
        हाल 'e':
        हाल 'i':
        हाल 'o':
          opt_fileno = optc_to_fileno (c);
          affirm (0 <= opt_fileno && opt_fileno < countof (stdbuf));
          stdbuf[opt_fileno].optc = c;
          जबतक (c_isspace (*optarg))
            optarg++;
          stdbuf[opt_fileno].optarg = optarg;
          अगर (c == 'i' && *optarg == 'L')
            {
              /* -oL will be by far the most common use of this utility,
                 but one could easily think -iL might have the same affect,
                 so disallow it as it could be confusing.  */
              error (0, 0, _("line buffering standard input is meaningless"));
              usage (EXIT_CANCELED);
            }

          अगर (!streq (optarg, "L"))
            {
              क्रमागत strtol_error e = parse_आकार (optarg,
                                                &stdbuf[opt_fileno].आकार);
              अगर (e != LONGINT_OK)
                xstrtol_fatal (e, oi, c, longopts, optarg);
            }

          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (EXIT_CANCELED);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify at least 1 command.  */
  अगर (argc < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  अगर (! set_libstdbuf_options ())
    {
      error (0, 0, _("you must specify a buffering mode option"));
      usage (EXIT_CANCELED);
    }

  /* Try to preload libstdbuf first from the same path as
     stdbuf is running from.  */
  set_program_path (program_name);
  अगर (!program_path)
    program_path = xstrdup (PKGLIBDIR);  /* Need to init to non-null.  */
  set_LD_PRELOAD ();
  मुक्त (program_path);

  execvp (*argv, argv);

  पूर्णांक exit_status = त्रुटि_सं == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, त्रुटि_सं, _("failed to run command %s"), quote (argv[0]));
  वापस exit_status;
}
