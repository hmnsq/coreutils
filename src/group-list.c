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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <pwd.h>
#ଅନ୍ତର୍ଭାଵ <grp.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "mgetgroups.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "group-list.h"


/* Print all of the distinct groups the user is in. */
ବାହ୍ୟ bool
print_group_list (ଅକ୍ଷର ଅଚଳ *username,
                  uid_t ruid, gid_t rgid, gid_t egid,
                  bool use_names, ଅକ୍ଷର delim)
{
  bool ok = true;
  ସଂରଚନା passwd *pwd = ଖାଲି;

  ଯଦି (username)
    {
      pwd = getpwuid (ruid);
      ଯଦି (pwd == ଖାଲି)
        ok = false;
    }

  ଯଦି (!print_group (rgid, use_names))
    ok = false;

  ଯଦି (egid != rgid)
    {
      putchar (delim);
      ଯଦି (!print_group (egid, use_names))
        ok = false;
    }

  {
    gid_t *groups;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_groups = xgetgroups (username, (pwd ? pwd->pw_gid : egid), &groups);
    ଯଦି (n_groups < 0)
      {
        ଯଦି (username)
          {
            error (0, errno, _("failed to get groups for user %s"),
                   quote (username));
          }
        ଅନ୍ୟଥା
          {
            error (0, errno, _("failed to get groups for the current process"));
          }
        ଫେରନ୍ତୁ false;
      }

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_groups; i++)
      ଯଦି (groups[i] != rgid && groups[i] != egid)
        {
          putchar (delim);
          ଯଦି (!print_group (groups[i], use_names))
            ok = false;
        }
    ମୁକ୍ତ (groups);
  }
  ଫେରନ୍ତୁ ok;
}

/* Print the name or value of group ID GID. */
ବାହ୍ୟ bool
print_group (gid_t gid, bool use_name)
{
  ସଂରଚନା group *grp = ଖାଲି;
  bool ok = true;

  ଯଦି (use_name)
    {
      grp = getgrgid (gid);
      ଯଦି (grp == ଖାଲି)
        {
          ଯଦି (TYPE_SIGNED (gid_t))
            {
              intmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %jd"), g);
            }
          ଅନ୍ୟଥା
            {
              uintmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %ju"), g);
            }
          ok = false;
        }
    }

  ଯଦି (grp)
    printf ("%s", grp->gr_name);
  ଅନ୍ୟଥା
    printf ("%ju", (uintmax_t) gid);
  ଫେରନ୍ତୁ ok;
}
