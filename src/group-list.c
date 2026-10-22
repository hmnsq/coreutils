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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <pwd.h>
#અન્તર્ભાવ <grp.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "mgetgroups.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "group-list.h"


/* Print all of the distinct groups the user is in. */
બાહ્ય bool
print_group_list (અક્ષર અચલ *username,
                  uid_t ruid, gid_t rgid, gid_t egid,
                  bool use_names, અક્ષર delim)
{
  bool ok = true;
  સંરચના passwd *pwd = ખાલી;

  જો (username)
    {
      pwd = getpwuid (ruid);
      જો (pwd == ખાલી)
        ok = false;
    }

  જો (!print_group (rgid, use_names))
    ok = false;

  જો (egid != rgid)
    {
      putchar (delim);
      જો (!print_group (egid, use_names))
        ok = false;
    }

  {
    gid_t *groups;

    પૂર્ણાંક n_groups = xgetgroups (username, (pwd ? pwd->pw_gid : egid), &groups);
    જો (n_groups < 0)
      {
        જો (username)
          {
            error (0, errno, _("failed to get groups for user %s"),
                   quote (username));
          }
        નહીંતર
          {
            error (0, errno, _("failed to get groups for the current process"));
          }
        પાછા false;
      }

    માટે (પૂર્ણાંક i = 0; i < n_groups; i++)
      જો (groups[i] != rgid && groups[i] != egid)
        {
          putchar (delim);
          જો (!print_group (groups[i], use_names))
            ok = false;
        }
    મુક્ત (groups);
  }
  પાછા ok;
}

/* Print the name or value of group ID GID. */
બાહ્ય bool
print_group (gid_t gid, bool use_name)
{
  સંરચના group *grp = ખાલી;
  bool ok = true;

  જો (use_name)
    {
      grp = getgrgid (gid);
      જો (grp == ખાલી)
        {
          જો (TYPE_SIGNED (gid_t))
            {
              intmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %jd"), g);
            }
          નહીંતર
            {
              uintmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %ju"), g);
            }
          ok = false;
        }
    }

  જો (grp)
    printf ("%s", grp->gr_name);
  નહીંતર
    printf ("%ju", (uintmax_t) gid);
  પાછા ok;
}
