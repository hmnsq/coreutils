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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <pwd.h>
#شامل <grp.h>

#شامل "system.h"
#شامل "mgetgroups.h"
#شامل "quote.h"
#شامل "group-list.h"


/* Print all of the distinct groups the user is in. */
بیرونی bool
print_group_list (نویسه ثابت *username,
                  uid_t ruid, gid_t rgid, gid_t egid,
                  bool use_names, نویسه delim)
{
  bool ok = true;
  ساختار passwd *pwd = پوچ;

  اگر (username)
    {
      pwd = getpwuid (ruid);
      اگر (pwd == پوچ)
        ok = false;
    }

  اگر (!print_group (rgid, use_names))
    ok = false;

  اگر (egid != rgid)
    {
      putchar (delim);
      اگر (!print_group (egid, use_names))
        ok = false;
    }

  {
    gid_t *groups;

    صحیح n_groups = xgetgroups (username, (pwd ? pwd->pw_gid : egid), &groups);
    اگر (n_groups < 0)
      {
        اگر (username)
          {
            error (0, errno, _("failed to get groups for user %s"),
                   quote (username));
          }
        وگرنه
          {
            error (0, errno, _("failed to get groups for the current process"));
          }
        بازگشت false;
      }

    برای (صحیح i = 0; i < n_groups; i++)
      اگر (groups[i] != rgid && groups[i] != egid)
        {
          putchar (delim);
          اگر (!print_group (groups[i], use_names))
            ok = false;
        }
    آزاد (groups);
  }
  بازگشت ok;
}

/* Print the name or value of group ID GID. */
بیرونی bool
print_group (gid_t gid, bool use_name)
{
  ساختار group *grp = پوچ;
  bool ok = true;

  اگر (use_name)
    {
      grp = getgrgid (gid);
      اگر (grp == پوچ)
        {
          اگر (TYPE_SIGNED (gid_t))
            {
              intmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %jd"), g);
            }
          وگرنه
            {
              uintmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %ju"), g);
            }
          ok = false;
        }
    }

  اگر (grp)
    printf ("%s", grp->gr_name);
  وگرنه
    printf ("%ju", (uintmax_t) gid);
  بازگشت ok;
}
