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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <pwd.h>
#ಅನ್ತರ್ಭಾವ <grp.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "mgetgroups.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "group-list.h"


/* Print all of the distinct groups the user is in. */
ಬಾಹ್ಯ bool
print_group_list (ವರ್ಣ ಧ್ರುವ *username,
                  uid_t ruid, gid_t rgid, gid_t egid,
                  bool use_names, ವರ್ಣ delim)
{
  bool ok = true;
  ಸಂರಚನಾ passwd *pwd = ರಿಕ್ತ;

  ಯದಿ (username)
    {
      pwd = getpwuid (ruid);
      ಯದಿ (pwd == ರಿಕ್ತ)
        ok = false;
    }

  ಯದಿ (!print_group (rgid, use_names))
    ok = false;

  ಯದಿ (egid != rgid)
    {
      putchar (delim);
      ಯದಿ (!print_group (egid, use_names))
        ok = false;
    }

  {
    gid_t *groups;

    ಪೂರ್ಣಾಙ್ಕ n_groups = xgetgroups (username, (pwd ? pwd->pw_gid : egid), &groups);
    ಯದಿ (n_groups < 0)
      {
        ಯದಿ (username)
          {
            error (0, errno, _("failed to get groups for user %s"),
                   quote (username));
          }
        ಅನ್ಯಥಾ
          {
            error (0, errno, _("failed to get groups for the current process"));
          }
        ಪ್ರತ್ಯಾಗಮ false;
      }

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n_groups; i++)
      ಯದಿ (groups[i] != rgid && groups[i] != egid)
        {
          putchar (delim);
          ಯದಿ (!print_group (groups[i], use_names))
            ok = false;
        }
    ಮುಕ್ತ (groups);
  }
  ಪ್ರತ್ಯಾಗಮ ok;
}

/* Print the name or value of group ID GID. */
ಬಾಹ್ಯ bool
print_group (gid_t gid, bool use_name)
{
  ಸಂರಚನಾ group *grp = ರಿಕ್ತ;
  bool ok = true;

  ಯದಿ (use_name)
    {
      grp = getgrgid (gid);
      ಯದಿ (grp == ರಿಕ್ತ)
        {
          ಯದಿ (TYPE_SIGNED (gid_t))
            {
              intmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %jd"), g);
            }
          ಅನ್ಯಥಾ
            {
              uintmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %ju"), g);
            }
          ok = false;
        }
    }

  ಯದಿ (grp)
    printf ("%s", grp->gr_name);
  ಅನ್ಯಥಾ
    printf ("%ju", (uintmax_t) gid);
  ಪ್ರತ್ಯಾಗಮ ok;
}
