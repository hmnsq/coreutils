/* pwd - print current directory
   Copyright (C) 1994-2026 Free Software Foundation, Inc.

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

#شامل <config.h>
#شامل <getopt.h>
#شامل <stdio.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "quote.h"
#شامل "root-dev-ino.h"
#شامل "xgetcwd.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "pwd"

#تعریف_کرو AUTHORS proper_name ("Jim Meyering")

ساخت file_name
{
  idx_t n_alloc;
  حرف *buf COUNTED_BY (n_alloc);
  حرف *آغاز;
};

ساکن ساخت option ثابت longopts[] =
{
  {"logical", no_دلیل, تہی, 'L'},
  {"physical", no_دلیل, تہی, 'P'},
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
      لکھو (_("Usage: %s [OPTION]...\n"), پروگرام_name);
      fputs (_("\
Print the بھرا filename کا the current working directory.\n\
\n\
"), stdout);
      oputs (_("\
  -L, --logical\n\
         use PWD from environment, even اگر it شامل_ہے symlinks\n\
"));
      oputs (_("\
  -P, --physical\n\
         resolve all symlinks\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\n\
If no option is specified, -P is assumed.\n\
"), stdout);
      لکھو (USAGE_BUILTIN_WARNING, پروگرام_NAME);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

ساکن خالی
file_name_free (ساخت file_name *p)
{
  آزاد (p->buf);
  آزاد (p);
}

ساکن ساخت file_name *
file_name_init (خالی)
{
  ساخت file_name *p = xmalloc (حجم *p);

  /* Start with a buffer larger than likely file names, but beware of systems
     on which PATH_MAX is very large -- e.g., INT_MAX.  */
  صحیح init_alloc = 2 * MIN (PATH_MAX, 16 * 1024);
  p->n_alloc = init_alloc;

  p->buf = xmalloc (init_alloc);
  p->آغاز = p->buf + init_alloc - 1;
  p->آغاز[0] = '\0';
  واپس p;
}

/* Prepend the name S of length S_LEN, to the growing file_name, P.  */
ساکن خالی
file_name_prepend (ساخت file_name *p, حرف ثابت *s, size_t s_len)
{
  idx_t n_free = p->آغاز - p->buf;
  اگر (n_free < 1 + s_len)
    {
      /* Call xpalloc with NULL not p->buf, since with the latter
         we'd end up copying the data twice: once via realloc, then again
         to align it with the end of the new buffer.  By passing NULL we
         copy it only once.  */
      idx_t n_مستعمل = p->n_alloc - n_free;
      حرف *buf = xpalloc (تہی, &p->n_alloc, 1 + s_len - n_free, -1, 1);
      p->آغاز = حافظہ_نقل (buf + p->n_alloc - n_مستعمل, p->آغاز, n_مستعمل);
      آزاد (p->buf);
      p->buf = buf;
    }

  p->آغاز -= 1 + s_len;
  p->آغاز[0] = '/';
  حافظہ_نقل (p->آغاز + 1, s, s_len);
}

/* Return a string (malloc'd) consisting of N '/'-separated ".." components.  */
ساکن حرف *
nth_والد (size_t n)
{
  حرف *buf = xnmalloc (3, n);
  حرف *p = buf;

  برائے (size_t i = 0; i < n; i++)
    {
      حافظہ_نقل (p, "../", 3);
      p += 3;
    }
  p[-1] = '\0';
  واپس buf;
}

/* Determine the basename of the current directory, where DOT_SB is the
   result of lstat'ing "." and prepend that to the file name in *FILE_NAME.
   Find the directory entry in '..' that matches the dev/i-node of DOT_SB.
   Upon success, update *DOT_SB with stat information of '..', chdir to '..',
   and prepend "/basename" to FILE_NAME.
   Otherwise, exit with a diagnostic.
   PARENT_HEIGHT is the number of levels '..' is above the starting directory.
   The first time this function is called (from the initial directory),
   PARENT_HEIGHT is 1.  This is solely for diagnostics.
   Exit nonzero upon error.  */

ساکن خالی
ڈھونڈو_dir_اندراج (ساخت stat *dot_sb, ساخت file_name *file_name,
                size_t والد_اونچائی)
{
  DIR *dirp = opendir ("..");
  اگر (dirp == تہی)
    error (EXIT_FAILURE, errno, _("cannot open directory %s"),
           quote (nth_والد (والد_اونچائی)));

  صحیح fd = dirfd (dirp);
  اگر ((0 <= fd ? fchdir (fd) : chdir ("..")) < 0)
    error (EXIT_FAILURE, errno, _("failed to chdir to %s"),
           quote (nth_والد (والد_اونچائی)));

  ساخت stat والد_sb;
  اگر ((0 <= fd ? fstat (fd, &والد_sb) : stat (".", &والد_sb)) < 0)
    error (EXIT_FAILURE, errno, _("failed to stat %s"),
           quote (nth_والد (والد_اونچائی)));

  /* If parent and child directory are on different devices, then we
     can't rely on d_ino for useful i-node numbers; use lstat instead.  */
  bool use_lstat = (والد_sb.st_dev != dot_sb->st_dev);

  bool ملا = false;
  جب (true)
    {
      ساخت dirent ثابت *dp;

      errno = 0;
      اگر ((dp = readdir_ignoring_dot_and_dotdot (dirp)) == تہی)
        {
          اگر (errno)
            {
              /* Save/restore errno across closedir call.  */
              صحیح e = errno;
              closedir (dirp);
              errno = e;

              /* Arrange to give a diagnostic after exiting this loop.  */
              dirp = تہی;
            }
          توڑو;
        }

      ino_t ino = D_INO (dp);

      ساخت stat ent_sb;
      اگر (ino == NOT_AN_INODE_نمبر || use_lstat)
        {
          اگر (lstat (dp->d_name, &ent_sb) < 0)
            {
              /* Skip any entry we can't stat.  */
              جاری;
            }
          ino = ent_sb.st_ino;
        }

      اگر (ino != dot_sb->st_ino)
        جاری;

      /* If we're not crossing a device boundary, then a simple i-node
         match is enough.  */
      اگر ( ! use_lstat || ent_sb.st_dev == dot_sb->st_dev)
        {
          file_name_prepend (file_name, dp->d_name, _D_EXACT_NAMLEN (dp));
          ملا = true;
          توڑو;
        }
    }

  اگر (dirp == تہی || closedir (dirp) != 0)
    {
      /* Note that this diagnostic serves for both readdir
         and closedir failures.  */
      error (EXIT_FAILURE, errno, _("reading directory %s"),
             quote (nth_والد (والد_اونچائی)));
    }

  اگر ( ! ملا)
    error (EXIT_FAILURE, 0,
           _("couldn't find directory entry in %s with matching i-node"),
           quote (nth_والد (والد_اونچائی)));

  *dot_sb = والد_sb;
}

/* Construct the full, absolute name of the current working
   directory and store it in *FILE_NAME.
   The getcwd function performs nearly the same task, but is typically
   unable to handle names longer than PATH_MAX.  This function has
   no such limitation.  However, this function *can* fail due to
   permission problems or a lack of memory, while GNU/Linux's getcwd
   function works regardless of restricted permissions on parent
   directories.  Upon failure, give a diagnostic and exit nonzero.

   Note: although this function is similar to getcwd, it has a fundamental
   difference in that it gives a diagnostic and exits upon failure.
   I would have liked a function that did not exit, and that could be
   used as a getcwd replacement.  Unfortunately, considering all of
   the information the caller would require in order to produce good
   diagnostics, it doesn't seem worth the added complexity.
   In any case, any getcwd replacement must *not* exceed the PATH_MAX
   limitation.  Otherwise, functions like 'chdir' would fail with
   ENAMETOOLONG.

   FIXME-maybe: if find_dir_entry fails due to permissions, try getcwd,
   in case the unreadable directory is close enough to the root that
   getcwd works from there.  */

ساکن خالی
robust_getcwd (ساخت file_name *file_name)
{
  size_t اونچائی = 1;
  ساخت dev_ino dev_ino_buf;
  ساخت dev_ino *جڑ_dev_ino = get_جڑ_dev_ino (&dev_ino_buf);

  اگر (جڑ_dev_ino == تہی)
    error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
           quoteaf ("/"));

  ساخت stat dot_sb;
  اگر (stat (".", &dot_sb) < 0)
    error (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf ("."));

  جب (true)
    {
      /* If we've reached the root, we're done.  */
      اگر (PSAME_INODE (&dot_sb, جڑ_dev_ino))
        توڑو;

      ڈھونڈو_dir_اندراج (&dot_sb, file_name, اونچائی++);
    }

  /* See if a leading slash is needed; file_name_prepend adds one.  */
  اگر (file_name->آغاز[0] == '\0')
    file_name_prepend (file_name, "", 0);
}


/* Return PWD from the environment if it is acceptable for 'pwd -L'
   output, otherwise NULL.  */
ساکن حرف ثابت *
logical_getcwd (خالی)
{
  حرف ثابت *wd = getenv ("PWD");

  /* Textual validation first.  */
  اگر (!wd || wd[0] != '/')
    واپس تہی;
  حرف ثابت *p = wd;
  جب ((p = لڑی_تلاش (p, "/.")))
    {
      اگر (!p[2] || p[2] == '/'
          || (p[2] == '.' && (!p[3] || p[3] == '/')))
        واپس تہی;
      p++;
    }

  /* System call validation.  */
  ساخت stat st1;
  ساخت stat st2;
  اگر (stat (wd, &st1) == 0 && stat (".", &st2) == 0 && psame_inode (&st1, &st2))
    واپس wd;
  واپس تہی;
}


صحیح
مین (صحیح argc, حرف **argv)
{
  /* POSIX requires a default of -L, but most scripts expect -P.
     Currently shells default to -L, while stand-alone
     pwd implementations default to -P.  */
  bool logical = (getenv ("POSIXLY_CORRECT") != تہی);

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب (true)
    {
      صحیح c = getopt_long (argc, argv, "LP", longopts, تہی);
      اگر (c == -1)
        توڑو;
      انتخاب (c)
        {
        حالت 'L':
          logical = true;
          توڑو;
        حالت 'P':
          logical = false;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  اگر (optind < argc)
    error (0, 0, _("ignoring non-option arguments"));

  اگر (logical)
    {
      حرف ثابت *wd = logical_getcwd ();
      اگر (wd)
        {
          puts (wd);
          واپس EXIT_SUCCESS;
        }
    }

  حرف *wd = xgetcwd ();
  اگر (wd != تہی)
    {
      puts (wd);
      آزاد (wd);
    }
  ورنہ
    {
      ساخت file_name *file_name = file_name_init ();
      robust_getcwd (file_name);
      puts (file_name->آغاز);
      file_name_free (file_name);
    }

  واپس EXIT_SUCCESS;
}
