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

#include <config.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

#include "system.h"
#include "mgetgroups.h"
#include "quote.h"
#include "group-list.h"


/* Print all of the distinct groups the user is in. */
extern bool
print_group_list (var_na const *username,
                  uid_t ruid, gid_t rgid, gid_t egid,
                  bool use_names, var_na delim)
{
  bool ok = true;
  sa_nracanaa passwd *pwd = NULL;

  ถ้า (username)
    {
      pwd = getpwuid (ruid);
      ถ้า (pwd == NULL)
        ok = false;
    }

  ถ้า (!print_group (rgid, use_names))
    ok = false;

  ถ้า (egid != rgid)
    {
      putchar (delim);
      ถ้า (!print_group (egid, use_names))
        ok = false;
    }

  {
    gid_t *groups;

    จำนวนเต็ม n_groups = xgetgroups (username, (pwd ? pwd->pw_gid : egid), &groups);
    ถ้า (n_groups < 0)
      {
        ถ้า (username)
          {
            error (0, errno, _("failed to get groups for user %s"),
                   quote (username));
          }
        มิฉะนั้น
          {
            error (0, errno, _("failed to get groups for the current process"));
          }
        คืน false;
      }

    สำหรับ (จำนวนเต็ม i = 0; i < n_groups; i++)
      ถ้า (groups[i] != rgid && groups[i] != egid)
        {
          putchar (delim);
          ถ้า (!print_group (groups[i], use_names))
            ok = false;
        }
    free (groups);
  }
  คืน ok;
}

/* Print the name or value of group ID GID. */
extern bool
print_group (gid_t gid, bool use_name)
{
  sa_nracanaa group *grp = NULL;
  bool ok = true;

  ถ้า (use_name)
    {
      grp = getgrgid (gid);
      ถ้า (grp == NULL)
        {
          ถ้า (TYPE_SIGNED (gid_t))
            {
              intmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %jd"), g);
            }
          มิฉะนั้น
            {
              uintmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %ju"), g);
            }
          ok = false;
        }
    }

  ถ้า (grp)
    printf ("%s", grp->gr_name);
  มิฉะนั้น
    printf ("%ju", (uintmax_t) gid);
  คืน ok;
}
