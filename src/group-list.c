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

  အကယ်၍ (username)
    {
      pwd = getpwuid (ruid);
      အကယ်၍ (pwd == NULL)
        ok = false;
    }

  အကယ်၍ (!print_group (rgid, use_names))
    ok = false;

  အကယ်၍ (egid != rgid)
    {
      putchar (delim);
      အကယ်၍ (!print_group (egid, use_names))
        ok = false;
    }

  {
    gid_t *groups;

    ကိန်းပြည့် n_groups = xgetgroups (username, (pwd ? pwd->pw_gid : egid), &groups);
    အကယ်၍ (n_groups < 0)
      {
        အကယ်၍ (username)
          {
            error (0, errno, _("failed to get groups for user %s"),
                   quote (username));
          }
        မဟုတ်ပါက
          {
            error (0, errno, _("failed to get groups for the current process"));
          }
        ပြန်ပေး false;
      }

    အတွက် (ကိန်းပြည့် i = 0; i < n_groups; i++)
      အကယ်၍ (groups[i] != rgid && groups[i] != egid)
        {
          putchar (delim);
          အကယ်၍ (!print_group (groups[i], use_names))
            ok = false;
        }
    free (groups);
  }
  ပြန်ပေး ok;
}

/* Print the name or value of group ID GID. */
extern bool
print_group (gid_t gid, bool use_name)
{
  sa_nracanaa group *grp = NULL;
  bool ok = true;

  အကယ်၍ (use_name)
    {
      grp = getgrgid (gid);
      အကယ်၍ (grp == NULL)
        {
          အကယ်၍ (TYPE_SIGNED (gid_t))
            {
              intmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %jd"), g);
            }
          မဟုတ်ပါက
            {
              uintmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %ju"), g);
            }
          ok = false;
        }
    }

  အကယ်၍ (grp)
    printf ("%s", grp->gr_name);
  မဟုတ်ပါက
    printf ("%ju", (uintmax_t) gid);
  ပြန်ပေး ok;
}
