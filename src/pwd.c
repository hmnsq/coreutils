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

#समावेश <config.h>
#समावेश <getopt.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>

#समावेश "system.h"
#समावेश "quote.h"
#समावेश "root-dev-ino.h"
#समावेश "xgetcwd.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "pwd"

#घोषणा AUTHORS proper_name ("Jim Meyering")

काष्ठा file_name
{
  idx_t n_alloc;
  अक्षर *buf COUNTED_BY (n_alloc);
  अक्षर *start;
};

अटल काष्ठा option स्थिर longopts[] =
{
  {"logical", no_argument, शून्य, 'L'},
  {"physical", no_argument, शून्य, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]...\n"), program_name);
      ख_माला_दो (_("\
Print the full filename of the current working directory.\n\
\n\
"), मानक_निकास);
      oputs (_("\
  -L, --logical\n\
         use PWD from environment, even अगर it contains symlinks\n\
"));
      oputs (_("\
  -P, --physical\n\
         resolve all symlinks\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\n\
If no option is specified, -P is assumed.\n\
"), मानक_निकास);
      म_लिखो (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

अटल व्योम
file_name_free (काष्ठा file_name *p)
{
  मुक्त (p->buf);
  मुक्त (p);
}

अटल काष्ठा file_name *
file_name_init (व्योम)
{
  काष्ठा file_name *p = xmalloc (माप *p);

  /* Start with a buffer larger than likely file names, but beware of systems
     on which PATH_MAX is very large -- e.g., INT_MAX.  */
  पूर्णांक init_alloc = 2 * MIN (PATH_MAX, 16 * 1024);
  p->n_alloc = init_alloc;

  p->buf = xmalloc (init_alloc);
  p->start = p->buf + init_alloc - 1;
  p->start[0] = '\0';
  वापस p;
}

/* Prepend the name S of length S_LEN, to the growing file_name, P.  */
अटल व्योम
file_name_prepend (काष्ठा file_name *p, अक्षर स्थिर *s, माप_प्रकार s_len)
{
  idx_t n_free = p->start - p->buf;
  अगर (n_free < 1 + s_len)
    {
      /* Call xpalloc with NULL not p->buf, since with the latter
         we'd end up copying the data twice: once via realloc, then again
         to align it with the end of the new buffer.  By passing NULL we
         copy it only once.  */
      idx_t n_used = p->n_alloc - n_free;
      अक्षर *buf = xpalloc (शून्य, &p->n_alloc, 1 + s_len - n_free, -1, 1);
      p->start = स_नकल (buf + p->n_alloc - n_used, p->start, n_used);
      मुक्त (p->buf);
      p->buf = buf;
    }

  p->start -= 1 + s_len;
  p->start[0] = '/';
  स_नकल (p->start + 1, s, s_len);
}

/* Return a string (malloc'd) consisting of N '/'-separated ".." components.  */
अटल अक्षर *
nth_parent (माप_प्रकार n)
{
  अक्षर *buf = xnmalloc (3, n);
  अक्षर *p = buf;

  क्रम (माप_प्रकार i = 0; i < n; i++)
    {
      स_नकल (p, "../", 3);
      p += 3;
    }
  p[-1] = '\0';
  वापस buf;
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

अटल व्योम
find_dir_entry (काष्ठा stat *dot_sb, काष्ठा file_name *file_name,
                माप_प्रकार parent_height)
{
  सूची *dirp = सूची_खोलो ("..");
  अगर (dirp == शून्य)
    error (निकास_त्रुटि, त्रुटि_सं, _("cannot open directory %s"),
           quote (nth_parent (parent_height)));

  पूर्णांक fd = dirfd (dirp);
  अगर ((0 <= fd ? fchdir (fd) : स_बदलो ("..")) < 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("failed to chdir to %s"),
           quote (nth_parent (parent_height)));

  काष्ठा stat parent_sb;
  अगर ((0 <= fd ? ख_स्थिति (fd, &parent_sb) : stat (".", &parent_sb)) < 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("failed to stat %s"),
           quote (nth_parent (parent_height)));

  /* If parent and child directory are on different devices, then we
     can't rely on d_ino for useful i-node numbers; use lstat instead.  */
  bool use_lstat = (parent_sb.st_dev != dot_sb->st_dev);

  bool found = false;
  जबतक (true)
    {
      काष्ठा dirent स्थिर *dp;

      त्रुटि_सं = 0;
      अगर ((dp = readdir_ignoring_dot_and_dotdot (dirp)) == शून्य)
        {
          अगर (त्रुटि_सं)
            {
              /* Save/restore errno across closedir call.  */
              पूर्णांक e = त्रुटि_सं;
              बंद_सूची (dirp);
              त्रुटि_सं = e;

              /* Arrange to give a diagnostic after exiting this loop.  */
              dirp = शून्य;
            }
          अवरोध;
        }

      ino_t ino = D_INO (dp);

      काष्ठा stat ent_sb;
      अगर (ino == NOT_AN_INODE_संख्या || use_lstat)
        {
          अगर (lstat (dp->d_name, &ent_sb) < 0)
            {
              /* Skip any entry we can't stat.  */
              जारी;
            }
          ino = ent_sb.st_ino;
        }

      अगर (ino != dot_sb->st_ino)
        जारी;

      /* If we're not crossing a device boundary, then a simple i-node
         match is enough.  */
      अगर ( ! use_lstat || ent_sb.st_dev == dot_sb->st_dev)
        {
          file_name_prepend (file_name, dp->d_name, _D_EXACT_NAMLEN (dp));
          found = true;
          अवरोध;
        }
    }

  अगर (dirp == शून्य || बंद_सूची (dirp) != 0)
    {
      /* Note that this diagnostic serves for both readdir
         and closedir failures.  */
      error (निकास_त्रुटि, त्रुटि_सं, _("reading directory %s"),
             quote (nth_parent (parent_height)));
    }

  अगर ( ! found)
    error (निकास_त्रुटि, 0,
           _("couldn't find directory entry in %s with matching i-node"),
           quote (nth_parent (parent_height)));

  *dot_sb = parent_sb;
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

अटल व्योम
robust_getcwd (काष्ठा file_name *file_name)
{
  माप_प्रकार height = 1;
  काष्ठा dev_ino dev_ino_buf;
  काष्ठा dev_ino *मूल_dev_ino = get_मूल_dev_ino (&dev_ino_buf);

  अगर (मूल_dev_ino == शून्य)
    error (निकास_त्रुटि, त्रुटि_सं, _("failed to get attributes of %s"),
           quoteaf ("/"));

  काष्ठा stat dot_sb;
  अगर (stat (".", &dot_sb) < 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("failed to stat %s"), quoteaf ("."));

  जबतक (true)
    {
      /* If we've reached the root, we're done.  */
      अगर (PSAME_INODE (&dot_sb, मूल_dev_ino))
        अवरोध;

      find_dir_entry (&dot_sb, file_name, height++);
    }

  /* See if a leading slash is needed; file_name_prepend adds one.  */
  अगर (file_name->start[0] == '\0')
    file_name_prepend (file_name, "", 0);
}


/* Return PWD from the environment if it is acceptable for 'pwd -L'
   output, otherwise NULL.  */
अटल अक्षर स्थिर *
logical_getcwd (व्योम)
{
  अक्षर स्थिर *wd = दो_पर्या ("PWD");

  /* Textual validation first.  */
  अगर (!wd || wd[0] != '/')
    वापस शून्य;
  अक्षर स्थिर *p = wd;
  जबतक ((p = म_माला (p, "/.")))
    {
      अगर (!p[2] || p[2] == '/'
          || (p[2] == '.' && (!p[3] || p[3] == '/')))
        वापस शून्य;
      p++;
    }

  /* System call validation.  */
  काष्ठा stat st1;
  काष्ठा stat st2;
  अगर (stat (wd, &st1) == 0 && stat (".", &st2) == 0 && psame_inode (&st1, &st2))
    वापस wd;
  वापस शून्य;
}


पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  /* POSIX requires a default of -L, but most scripts expect -P.
     Currently shells default to -L, while stand-alone
     pwd implementations default to -P.  */
  bool logical = (दो_पर्या ("POSIXLY_CORRECT") != शून्य);

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक (true)
    {
      पूर्णांक c = getopt_long (argc, argv, "LP", longopts, शून्य);
      अगर (c == -1)
        अवरोध;
      चयन (c)
        {
        हाल 'L':
          logical = true;
          अवरोध;
        हाल 'P':
          logical = false;
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
        }
    }

  अगर (optind < argc)
    error (0, 0, _("ignoring non-option arguments"));

  अगर (logical)
    {
      अक्षर स्थिर *wd = logical_getcwd ();
      अगर (wd)
        {
          माला_दो (wd);
          वापस निकास_सफल;
        }
    }

  अक्षर *wd = xgetcwd ();
  अगर (wd != शून्य)
    {
      माला_दो (wd);
      मुक्त (wd);
    }
  अन्यथा
    {
      काष्ठा file_name *file_name = file_name_init ();
      robust_getcwd (file_name);
      माला_दो (file_name->start);
      file_name_free (file_name);
    }

  वापस निकास_सफल;
}
