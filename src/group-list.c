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
print_ಗುಂಪು_list (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *username,
                  uid_t ruid, gid_t rgid, gid_t egid,
                  bool use_ಹೆಸರುಗಳು, ಅಕ್ಷರ delim)
{
  bool ok = true;
  ರಚನೆ passwd *pwd = ಖಾಲಿ;

  ಆದರೆ (username)
    {
      pwd = getpwuid (ruid);
      ಆದರೆ (pwd == ಖಾಲಿ)
        ok = false;
    }

  ಆದರೆ (!print_ಗುಂಪು (rgid, use_ಹೆಸರುಗಳು))
    ok = false;

  ಆದರೆ (egid != rgid)
    {
      ಅಕ್ಷರ_ಬರೆ (delim);
      ಆದರೆ (!print_ಗುಂಪು (egid, use_ಹೆಸರುಗಳು))
        ok = false;
    }

  {
    gid_t *groups;

    ಪೂರ್ಣಾಂಕ n_groups = xgetgroups (username, (pwd ? pwd->pw_gid : egid), &groups);
    ಆದರೆ (n_groups < 0)
      {
        ಆದರೆ (username)
          {
            error (0, errno, _("failed to get groups for user %s"),
                   quote (username));
          }
        ಇಲ್ಲದಿದ್ದರೆ
          {
            error (0, errno, _("failed to get groups for the current process"));
          }
        ಹಿಂತಿರುಗಿ false;
      }

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_groups; i++)
      ಆದರೆ (groups[i] != rgid && groups[i] != egid)
        {
          ಅಕ್ಷರ_ಬರೆ (delim);
          ಆದರೆ (!print_ಗುಂಪು (groups[i], use_ಹೆಸರುಗಳು))
            ok = false;
        }
    ಬಿಡುಗಡೆ (groups);
  }
  ಹಿಂತಿರುಗಿ ok;
}

/* Print the name or value of group ID GID. */
ಬಾಹ್ಯ bool
print_ಗುಂಪು (gid_t gid, bool use_name)
{
  ರಚನೆ ಗುಂಪು *grp = ಖಾಲಿ;
  bool ok = true;

  ಆದರೆ (use_name)
    {
      grp = getgrgid (gid);
      ಆದರೆ (grp == ಖಾಲಿ)
        {
          ಆದರೆ (TYPE_SIGNED (gid_t))
            {
              intmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %jd"), g);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              uintmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %ju"), g);
            }
          ok = false;
        }
    }

  ಆದರೆ (grp)
    ಬರೆ ("%s", grp->gr_name);
  ಇಲ್ಲದಿದ್ದರೆ
    ಬರೆ ("%ju", (uintmax_t) gid);
  ಹಿಂತಿರುಗಿ ok;
}
