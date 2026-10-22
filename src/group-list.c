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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <pwd.h>
#ضمن <grp.h>

#ضمن "system.h"
#ضمن "mgetgroups.h"
#ضمن "quote.h"
#ضمن "group-list.h"


/* Print all of the distinct groups the user is in. */
خارجي bool
print_group_list (حرف ثابت *username,
                  uid_t ruid, gid_t rgid, gid_t egid,
                  bool use_names, حرف delim)
{
  bool ok = true;
  بنية passwd *pwd = عدم;

  𐡄𐡍 (username)
    {
      pwd = getpwuid (ruid);
      𐡄𐡍 (pwd == عدم)
        ok = false;
    }

  𐡄𐡍 (!print_group (rgid, use_names))
    ok = false;

  𐡄𐡍 (egid != rgid)
    {
      putchar (delim);
      𐡄𐡍 (!print_group (egid, use_names))
        ok = false;
    }

  {
    gid_t *groups;

    صحيح n_groups = xgetgroups (username, (pwd ? pwd->pw_gid : egid), &groups);
    𐡄𐡍 (n_groups < 0)
      {
        𐡄𐡍 (username)
          {
            error (0, errno, _("failed to get groups for user %s"),
                   quote (username));
          }
        وإلا
          {
            error (0, errno, _("failed to get groups for the current process"));
          }
        أرجع false;
      }

    لكل (صحيح i = 0; i < n_groups; i++)
      𐡄𐡍 (groups[i] != rgid && groups[i] != egid)
        {
          putchar (delim);
          𐡄𐡍 (!print_group (groups[i], use_names))
            ok = false;
        }
    حرر (groups);
  }
  أرجع ok;
}

/* Print the name or value of group ID GID. */
خارجي bool
print_group (gid_t gid, bool use_name)
{
  بنية group *grp = عدم;
  bool ok = true;

  𐡄𐡍 (use_name)
    {
      grp = getgrgid (gid);
      𐡄𐡍 (grp == عدم)
        {
          𐡄𐡍 (TYPE_SIGNED (gid_t))
            {
              intmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %jd"), g);
            }
          وإلا
            {
              uintmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %ju"), g);
            }
          ok = false;
        }
    }

  𐡄𐡍 (grp)
    printf ("%s", grp->gr_name);
  وإلا
    printf ("%ju", (uintmax_t) gid);
  أرجع ok;
}
