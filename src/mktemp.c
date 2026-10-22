/* Create a temporary file or directory, safely.
   Copyright (C) 2007-2026 Free Software Foundation, Inc.

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

/* Written by Jim Meyering and Eric Blake.  */

#شامل <config.h>
#شامل <sys/types.h>
#شامل <getopt.h>

#شامل "system.h"

#شامل "close-stream.h"
#شامل "filenamecat.h"
#شامل "quote.h"
#شامل "tempname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "mktemp"

#تعریف_کرو AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Eric Blake")

ساکن حرف ثابت *default_template = "tmp.XXXXXXXXXX";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  SUFFIX_OPTION = CHAR_MAX + 1,
};

ساکن ساخت option ثابت longopts[] =
{
  {"directory", no_دلیل, تہی, 'd'},
  {"quiet", no_دلیل, تہی, 'q'},
  {"dry-run", no_دلیل, تہی, 'u'},
  {"suffix", required_دلیل, تہی, SUFFIX_OPTION},
  {"tmpdir", optional_دلیل, تہی, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]... [TEMPLATE]\n"), پروگرام_name);
      fputs (_("\
Create a temporary file or directory, safely, and print its name.\n\
TEMPLATE must contain at کم_سے_کم 3 consecutive 'X's اندر آخری component.\n\
If TEMPLATE is not specified, use tmp.XXXXXXXXXX, and --tmpdir is implied.\n\
"), stdout);
      fputs (_("\
Files are created u+rw, and directories u+rwx, minus umask restrictions.\n\
"), stdout);
      fputs ("\n", stdout);
      oputs (_("\
  -d, --directory\n\
         create a directory, not a file\n\
"));
      oputs (_("\
  -u, --dry-تسلسل_2\n\
         کرو not create anything; merely print a name (unsafe)\n\
"));
      oputs (_("\
  -q, --quiet\n\
         suppress diagnostics about file/dir-creation failure\n\
"));
      oputs (_("\
      --suffix=SUFF\n\
         append SUFF مقصد TEMPLATE; SUFF must not contain a slash.\n\
         This option is implied اگر TEMPLATE does not انتہا اندر X\n\
"));
      oputs (_("\
  -p DIR, --tmpdir[=DIR]\n\
         interpret TEMPLATE relative مقصد DIR;\n\
         اگر DIR is not specified, use $TMPDIR اگر سیٹ_2, ورنہ /tmp.\n\
         With this option, TEMPLATE must not be an absolute name;\n\
         unlike with -t, TEMPLATE may contain slashes,\n\
         but mktemp creates only the final component\n\
"));
      oputs (_("\
  -t\n\
         interpret TEMPLATE as a single file name component,\n\
         relative مقصد a directory: $TMPDIR, اگر سیٹ_2;\n\
         ورنہ the directory specified بذریعہ -p; ورنہ /tmp [deprecated]\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }

  باہر_نکلو (حالت_2);
}

ساکن size_t
گنتی_consecutive_X_s (حرف ثابت *s, size_t len)
{
  size_t n = 0;
  برائے ( ; len && s[len - 1] == 'X'; len--)
    ++n;
  واپس n;
}

ساکن صحیح
mkstemp_len (حرف *tmpl, size_t suff_len, size_t x_len, bool dry_تسلسل)
{
  واپس gen_tempname_len (tmpl, suff_len, 0, dry_تسلسل ? GT_NOCREATE : GT_FILE,
                           x_len);
}

ساکن صحیح
mkdtemp_len (حرف *tmpl, size_t suff_len, size_t x_len, bool dry_تسلسل)
{
  واپس gen_tempname_len (tmpl, suff_len, 0, dry_تسلسل ? GT_NOCREATE : GT_DIR,
                           x_len);
}

/* True if we have already closed standard output.  */
ساکن bool stdout_closed;

/* Avoid closing stdout twice.  Since we conditionally call
   close_stream (stdout) in order to decide whether to clean up a
   temporary file, the exit hook needs to know whether to do all of
   close_stdout or just the stderr half.  */
ساکن خالی
maybe_close_stdout (خالی)
{
  اگر (!stdout_closed)
    close_stdout ();
  ورنہ اگر (close_دھارا (stderr) != 0)
    _exit (EXIT_FAILURE);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  حرف ثابت *dest_dir;
  حرف ثابت *dest_dir_arg = تہی;
  bool suppress_file_err = false;
  صحیح c;
  حرف *سانچہ;
  حرف *suffix = تہی;
  bool use_dest_dir = false;
  bool deprecated_t_option = false;
  bool create_directory = false;
  bool dry_تسلسل = false;
  صحیح حالت_2 = EXIT_SUCCESS;
  size_t x_گنتی;
  size_t suffix_len;
  حرف *dest_name = تہی;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (maybe_close_stdout);

  جب ((c = getopt_long (argc, argv, "dp:qtuV", longopts, تہی)) != -1)
    {
      انتخاب (c)
        {
        حالت 'd':
          create_directory = true;
          توڑو;
        حالت 'p':
          dest_dir_arg = optarg;
          use_dest_dir = true;
          توڑو;
        حالت 'q':
          suppress_file_err = true;
          توڑو;
        حالت 't':
          use_dest_dir = true;
          deprecated_t_option = true;
          توڑو;
        حالت 'u':
          dry_تسلسل = true;
          توڑو;

        حالت SUFFIX_OPTION:
          suffix = optarg;
          توڑو;

        case_GETOPT_HELP_CHAR;

        حالت 'V': /* Undocumented alias, for compatibility with the original
                     mktemp program.  */
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  صحیح n_دلائل = argc - optind;
  اگر (2 <= n_دلائل)
    {
      error (0, 0, _("too many templates"));
      usage (EXIT_FAILURE);
    }

  اگر (n_دلائل == 0)
    {
      use_dest_dir = true;
      سانچہ = (حرف *) default_template;
    }
  ورنہ
    {
      سانچہ = argv[optind];
    }

  اگر (suffix)
    {
      size_t len = لڑی_لمبائی (سانچہ);
      اگر (!len || سانچہ[len - 1] != 'X')
        {
          error (EXIT_FAILURE, 0,
                 _("with --suffix, template %s must end in X"),
                 quote (سانچہ));
        }
      suffix_len = لڑی_لمبائی (suffix);
      dest_name = xcharalloc (len + suffix_len + 1);
      حافظہ_نقل (dest_name, سانچہ, len);
      حافظہ_نقل (dest_name + len, suffix, suffix_len + 1);
      سانچہ = dest_name;
      suffix = dest_name + len;
    }
  ورنہ
    {
      سانچہ = xstrdup (سانچہ);
      suffix = strrchr (سانچہ, 'X');
      اگر (!suffix)
        suffix = strchr (سانچہ, '\0');
      ورنہ
        suffix++;
      suffix_len = لڑی_لمبائی (suffix);
    }

  /* At this point, template is malloc'd, and suffix points into template.  */
  اگر (suffix_len && آخری_component (suffix) != suffix)
    {
      error (EXIT_FAILURE, 0,
             _("invalid suffix %s, contains directory separator"),
             quote (suffix));
    }
  x_گنتی = گنتی_consecutive_X_s (سانچہ, suffix - سانچہ);
  اگر (x_گنتی < 3)
    {
      /* when suffix was appended to template only diagnose the template.  */
      اگر (سانچہ == dest_name)
        سانچہ[suffix - سانچہ] = '\0';
      error (EXIT_FAILURE, 0, _("too few X's in template %s"),
             quote (سانچہ));
    }

  اگر (use_dest_dir)
    {
      اگر (deprecated_t_option)
        {
          حرف *env = getenv ("TMPDIR");
          اگر (env && *env)
            dest_dir = env;
          ورنہ اگر (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          ورنہ
            dest_dir = "/tmp";

          اگر (آخری_component (سانچہ) != سانچہ)
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s, contains directory separator"),
                   quote (سانچہ));
        }
      ورنہ
        {
          اگر (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          ورنہ
            {
              حرف *env = getenv ("TMPDIR");
              dest_dir = (env && *env ? env : "/tmp");
            }
          اگر (IS_ABSOLUTE_FILE_NAME (سانچہ))
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s; with --tmpdir,"
                     " it may not be absolute"),
                   quote (سانچہ));
        }

      dest_name = file_name_concat (dest_dir, سانچہ, تہی);
      آزاد (سانچہ);
      سانچہ = dest_name;
      /* Note that suffix is now invalid.  */
    }

  /* Make a copy to be used in case of diagnostic, since failing
     mkstemp may leave the buffer in an undefined state.  */
  dest_name = xstrdup (سانچہ);

  اگر (create_directory)
    {
      صحیح err = mkdtemp_len (dest_name, suffix_len, x_گنتی, dry_تسلسل);
      اگر (err != 0)
        {
          اگر (!suppress_file_err)
            error (0, errno, _("failed to create directory via template %s"),
                   quote (سانچہ));
          حالت_2 = EXIT_FAILURE;
        }
    }
  ورنہ
    {
      صحیح fd = mkstemp_len (dest_name, suffix_len, x_گنتی, dry_تسلسل);
      اگر (fd < 0 || (!dry_تسلسل && close (fd) != 0))
        {
          اگر (!suppress_file_err)
            error (0, errno, _("failed to create file via template %s"),
                   quote (سانچہ));
          حالت_2 = EXIT_FAILURE;
        }
    }

  اگر (حالت_2 == EXIT_SUCCESS)
    {
      puts (dest_name);
      /* If we created a file, but then failed to output the file
         name, we should clean up the mess before failing.  */
      اگر (!dry_تسلسل && ((stdout_closed = true), close_دھارا (stdout) != 0))
        {
          صحیح saved_errno = errno;
          (create_directory ? rmdir : unlink) (dest_name);
          اگر (!suppress_file_err)
            error (0, saved_errno, _("write error"));
          حالت_2 = EXIT_FAILURE;
        }
    }

  main_exit (حالت_2);
}
