/* group-list.c --Print a list of group IDs or names.
   Copyright (C) 1989-2026 Free Software Foundation, Inc.

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

/* Written by Arnold Robbins.
   Major rewrite by David MacKenzie, djm@gnu.ai.mit.edu.
   Extracted from id.c by James Youngman. */

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <pwd.h>
#अन्तर्भाव <grp.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "mgetgroups.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "group-list.h"


/* Print all of the distinct groups the user is in. */
बाह्य bool
print_गणः_list (वर्ण ध्रुव *username,
                  uid_t ruid, gid_t rgid, gid_t egid,
                  bool use_नामानि, वर्ण delim)
{
  bool ok = true;
  संरचना passwd *pwd = रिक्त;

  यदि (username)
    {
      pwd = getpwuid (ruid);
      यदि (pwd == रिक्त)
        ok = false;
    }

  यदि (!print_गणः (rgid, use_नामानि))
    ok = false;

  यदि (egid != rgid)
    {
      अक्षरं_लिख (delim);
      यदि (!print_गणः (egid, use_नामानि))
        ok = false;
    }

  {
    gid_t *groups;

    पूर्णाङ्क n_groups = xgetgroups (username, (pwd ? pwd->pw_gid : egid), &groups);
    यदि (n_groups < 0)
      {
        यदि (username)
          {
            error (0, errno, _("failed to get groups for user %s"),
                   quote (username));
          }
        अन्यथा
          {
            error (0, errno, _("failed to get groups for the current process"));
          }
        प्रत्यागम false;
      }

    प्रति (पूर्णाङ्क i = 0; i < n_groups; i++)
      यदि (groups[i] != rgid && groups[i] != egid)
        {
          अक्षरं_लिख (delim);
          यदि (!print_गणः (groups[i], use_नामानि))
            ok = false;
        }
    मुक्त (groups);
  }
  प्रत्यागम ok;
}

/* Print the name or value of group ID GID. */
बाह्य bool
print_गणः (gid_t gid, bool use_name)
{
  संरचना गणः *grp = रिक्त;
  bool ok = true;

  यदि (use_name)
    {
      grp = getgrgid (gid);
      यदि (grp == रिक्त)
        {
          यदि (TYPE_SIGNED (gid_t))
            {
              intmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %jd"), g);
            }
          अन्यथा
            {
              uintmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %ju"), g);
            }
          ok = false;
        }
    }

  यदि (grp)
    प्रारूप_लिख ("%s", grp->gr_name);
  अन्यथा
    प्रारूप_लिख ("%ju", (uintmax_t) gid);
  प्रत्यागम ok;
}
