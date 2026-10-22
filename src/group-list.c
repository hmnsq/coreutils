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

  nếu (username)
    {
      pwd = getpwuid (ruid);
      nếu (pwd == NULL)
        ok = false;
    }

  nếu (!print_group (rgid, use_names))
    ok = false;

  nếu (egid != rgid)
    {
      putchar (delim);
      nếu (!print_group (egid, use_names))
        ok = false;
    }

  {
    gid_t *groups;

    số_nguyên n_groups = xgetgroups (username, (pwd ? pwd->pw_gid : egid), &groups);
    nếu (n_groups < 0)
      {
        nếu (username)
          {
            error (0, errno, _("failed to get groups for user %s"),
                   quote (username));
          }
        không_thì
          {
            error (0, errno, _("failed to get groups for the current process"));
          }
        trả_về false;
      }

    cho (số_nguyên i = 0; i < n_groups; i++)
      nếu (groups[i] != rgid && groups[i] != egid)
        {
          putchar (delim);
          nếu (!print_group (groups[i], use_names))
            ok = false;
        }
    free (groups);
  }
  trả_về ok;
}

/* Print the name or value of group ID GID. */
extern bool
print_group (gid_t gid, bool use_name)
{
  struct group *grp = NULL;
  bool ok = true;

  nếu (use_name)
    {
      grp = getgrgid (gid);
      nếu (grp == NULL)
        {
          nếu (TYPE_SIGNED (gid_t))
            {
              intmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %jd"), g);
            }
          không_thì
            {
              uintmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %ju"), g);
            }
          ok = false;
        }
    }

  nếu (grp)
    printf ("%s", grp->gr_name);
  không_thì
    printf ("%ju", (uintmax_t) gid);
  trả_về ok;
}
