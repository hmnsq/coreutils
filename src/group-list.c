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
print_group_list (अक्षर स्थिरांक *username,
                  uid_t ruid, gid_t rgid, gid_t egid,
                  bool use_names, अक्षर delim)
{
  bool ok = true;
  संरचना passwd *pwd = रिक्त;

  जर (username)
    {
      pwd = getpwuid (ruid);
      जर (pwd == रिक्त)
        ok = false;
    }

  जर (!print_group (rgid, use_names))
    ok = false;

  जर (egid != rgid)
    {
      अक्षर_दो (delim);
      जर (!print_group (egid, use_names))
        ok = false;
    }

  {
    gid_t *groups;

    पूर्णांक n_groups = xgetgroups (username, (pwd ? pwd->pw_gid : egid), &groups);
    जर (n_groups < 0)
      {
        जर (username)
          {
            error (0, त्रुटि_सं, _("failed to get groups for user %s"),
                   quote (username));
          }
        अन्यथा
          {
            error (0, त्रुटि_सं, _("failed to get groups for the current process"));
          }
        परत false;
      }

    साठी (पूर्णांक i = 0; i < n_groups; i++)
      जर (groups[i] != rgid && groups[i] != egid)
        {
          अक्षर_दो (delim);
          जर (!print_group (groups[i], use_names))
            ok = false;
        }
    मुक्त (groups);
  }
  परत ok;
}

/* Print the name or value of group ID GID. */
बाह्य bool
print_group (gid_t gid, bool use_name)
{
  संरचना group *grp = रिक्त;
  bool ok = true;

  जर (use_name)
    {
      grp = getgrgid (gid);
      जर (grp == रिक्त)
        {
          जर (TYPE_SIGNED (gid_t))
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

  जर (grp)
    म_लिखो ("%s", grp->gr_name);
  अन्यथा
    म_लिखो ("%ju", (uintmax_t) gid);
  परत ok;
}
