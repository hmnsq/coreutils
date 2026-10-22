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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <pwd.h>
#অন্তর্ভাব <grp.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "mgetgroups.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "group-list.h"


/* Print all of the distinct groups the user is in. */
বাহ্য bool
print_group_list (বর্ণ ধ্রুব *username,
                  uid_t ruid, gid_t rgid, gid_t egid,
                  bool use_names, বর্ণ delim)
{
  bool ok = true;
  সংরচনা passwd *pwd = রিক্ত;

  যদি (username)
    {
      pwd = getpwuid (ruid);
      যদি (pwd == রিক্ত)
        ok = false;
    }

  যদি (!print_group (rgid, use_names))
    ok = false;

  যদি (egid != rgid)
    {
      putchar (delim);
      যদি (!print_group (egid, use_names))
        ok = false;
    }

  {
    gid_t *groups;

    পূর্ণাঙ্ক n_groups = xgetgroups (username, (pwd ? pwd->pw_gid : egid), &groups);
    যদি (n_groups < 0)
      {
        যদি (username)
          {
            error (0, errno, _("failed to get groups for user %s"),
                   quote (username));
          }
        অন্যথা
          {
            error (0, errno, _("failed to get groups for the current process"));
          }
        প্রত্যাগম false;
      }

    প্রতি (পূর্ণাঙ্ক i = 0; i < n_groups; i++)
      যদি (groups[i] != rgid && groups[i] != egid)
        {
          putchar (delim);
          যদি (!print_group (groups[i], use_names))
            ok = false;
        }
    মুক্ত (groups);
  }
  প্রত্যাগম ok;
}

/* Print the name or value of group ID GID. */
বাহ্য bool
print_group (gid_t gid, bool use_name)
{
  সংরচনা group *grp = রিক্ত;
  bool ok = true;

  যদি (use_name)
    {
      grp = getgrgid (gid);
      যদি (grp == রিক্ত)
        {
          যদি (TYPE_SIGNED (gid_t))
            {
              intmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %jd"), g);
            }
          অন্যথা
            {
              uintmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %ju"), g);
            }
          ok = false;
        }
    }

  যদি (grp)
    printf ("%s", grp->gr_name);
  অন্যথা
    printf ("%ju", (uintmax_t) gid);
  প্রত্যাগম ok;
}
