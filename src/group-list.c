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
print_group_list (char const *username,
                  uid_t ruid, gid_t rgid, gid_t egid,
                  bool use_names, char delim)
{
  bool ok = true;
  struct passwd *pwd = NULL;

  如果 (username)
    {
      pwd = getpwuid (ruid);
      如果 (pwd == NULL)
        ok = false;
    }

  如果 (!print_group (rgid, use_names))
    ok = false;

  如果 (egid != rgid)
    {
      putchar (delim);
      如果 (!print_group (egid, use_names))
        ok = false;
    }

  {
    gid_t *groups;

    整数 n_groups = xgetgroups (username, (pwd ? pwd->pw_gid : egid), &groups);
    如果 (n_groups < 0)
      {
        如果 (username)
          {
            error (0, errno, _("failed to get groups for user %s"),
                   quote (username));
          }
        否则
          {
            error (0, errno, _("failed to get groups for the current process"));
          }
        返回 false;
      }

    为 (整数 i = 0; i < n_groups; i++)
      如果 (groups[i] != rgid && groups[i] != egid)
        {
          putchar (delim);
          如果 (!print_group (groups[i], use_names))
            ok = false;
        }
    free (groups);
  }
  返回 ok;
}

/* Print the name or value of group ID GID. */
extern bool
print_group (gid_t gid, bool use_name)
{
  struct group *grp = NULL;
  bool ok = true;

  如果 (use_name)
    {
      grp = getgrgid (gid);
      如果 (grp == NULL)
        {
          如果 (TYPE_SIGNED (gid_t))
            {
              intmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %jd"), g);
            }
          否则
            {
              uintmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %ju"), g);
            }
          ok = false;
        }
    }

  如果 (grp)
    printf ("%s", grp->gr_name);
  否则
    printf ("%ju", (uintmax_t) gid);
  返回 ok;
}
