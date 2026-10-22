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

  ꦪꦼꦤ꧀ (username)
    {
      pwd = getpwuid (ruid);
      ꦪꦼꦤ꧀ (pwd == NULL)
        ok = false;
    }

  ꦪꦼꦤ꧀ (!print_group (rgid, use_names))
    ok = false;

  ꦪꦼꦤ꧀ (egid != rgid)
    {
      putchar (delim);
      ꦪꦼꦤ꧀ (!print_group (egid, use_names))
        ok = false;
    }

  {
    gid_t *groups;

    puur_naa_nka n_groups = xgetgroups (username, (pwd ? pwd->pw_gid : egid), &groups);
    ꦪꦼꦤ꧀ (n_groups < 0)
      {
        ꦪꦼꦤ꧀ (username)
          {
            error (0, errno, _("failed to get groups for user %s"),
                   quote (username));
          }
        ꦭꦶꦪ
          {
            error (0, errno, _("failed to get groups for the current process"));
          }
        ꦮꦭꦶ false;
      }

    ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < n_groups; i++)
      ꦪꦼꦤ꧀ (groups[i] != rgid && groups[i] != egid)
        {
          putchar (delim);
          ꦪꦼꦤ꧀ (!print_group (groups[i], use_names))
            ok = false;
        }
    free (groups);
  }
  ꦮꦭꦶ ok;
}

/* Print the name or value of group ID GID. */
extern bool
print_group (gid_t gid, bool use_name)
{
  sa_nracanaa group *grp = NULL;
  bool ok = true;

  ꦪꦼꦤ꧀ (use_name)
    {
      grp = getgrgid (gid);
      ꦪꦼꦤ꧀ (grp == NULL)
        {
          ꦪꦼꦤ꧀ (TYPE_SIGNED (gid_t))
            {
              intmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %jd"), g);
            }
          ꦭꦶꦪ
            {
              uintmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %ju"), g);
            }
          ok = false;
        }
    }

  ꦪꦼꦤ꧀ (grp)
    printf ("%s", grp->gr_name);
  ꦭꦶꦪ
    printf ("%ju", (uintmax_t) gid);
  ꦮꦭꦶ ok;
}
