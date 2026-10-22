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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <pwd.h>
#అన్తర్భావ <grp.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "mgetgroups.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "group-list.h"


/* Print all of the distinct groups the user is in. */
బాహ్య bool
print_సమూహం_list (అక్షరం స్థిరరాశి *username,
                  uid_t ruid, gid_t rgid, gid_t egid,
                  bool use_పేర్లు, అక్షరం delim)
{
  bool ok = true;
  నిర్మాణం passwd *pwd = ఖాళీ;

  ఒకవేళ (username)
    {
      pwd = getpwuid (ruid);
      ఒకవేళ (pwd == ఖాళీ)
        ok = false;
    }

  ఒకవేళ (!print_సమూహం (rgid, use_పేర్లు))
    ok = false;

  ఒకవేళ (egid != rgid)
    {
      అక్షరం_రాయి (delim);
      ఒకవేళ (!print_సమూహం (egid, use_పేర్లు))
        ok = false;
    }

  {
    gid_t *groups;

    పూర్ణాంకం n_groups = xgetgroups (username, (pwd ? pwd->pw_gid : egid), &groups);
    ఒకవేళ (n_groups < 0)
      {
        ఒకవేళ (username)
          {
            error (0, errno, _("failed to get groups for user %s"),
                   quote (username));
          }
        లేకపోతే
          {
            error (0, errno, _("failed to get groups for the current process"));
          }
        తిరిగి false;
      }

    కొరకు (పూర్ణాంకం i = 0; i < n_groups; i++)
      ఒకవేళ (groups[i] != rgid && groups[i] != egid)
        {
          అక్షరం_రాయి (delim);
          ఒకవేళ (!print_సమూహం (groups[i], use_పేర్లు))
            ok = false;
        }
    విడుదల (groups);
  }
  తిరిగి ok;
}

/* Print the name or value of group ID GID. */
బాహ్య bool
print_సమూహం (gid_t gid, bool use_name)
{
  నిర్మాణం సమూహం *grp = ఖాళీ;
  bool ok = true;

  ఒకవేళ (use_name)
    {
      grp = getgrgid (gid);
      ఒకవేళ (grp == ఖాళీ)
        {
          ఒకవేళ (TYPE_SIGNED (gid_t))
            {
              intmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %jd"), g);
            }
          లేకపోతే
            {
              uintmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %ju"), g);
            }
          ok = false;
        }
    }

  ఒకవేళ (grp)
    రాయి ("%s", grp->gr_name);
  లేకపోతే
    రాయి ("%ju", (uintmax_t) gid);
  తిరిగి ok;
}
