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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <pwd.h>
#அந்தர்பாவ <grp.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "mgetgroups.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "group-list.h"


/* Print all of the distinct groups the user is in. */
வெளி bool
print_group_list (எழுத்து மாறிலி *username,
                  uid_t ruid, gid_t rgid, gid_t egid,
                  bool use_names, எழுத்து delim)
{
  bool ok = true;
  கட்டமைப்பு passwd *pwd = வெற்று;

  எனில் (username)
    {
      pwd = getpwuid (ruid);
      எனில் (pwd == வெற்று)
        ok = false;
    }

  எனில் (!print_group (rgid, use_names))
    ok = false;

  எனில் (egid != rgid)
    {
      putchar (delim);
      எனில் (!print_group (egid, use_names))
        ok = false;
    }

  {
    gid_t *groups;

    முழு n_groups = xgetgroups (username, (pwd ? pwd->pw_gid : egid), &groups);
    எனில் (n_groups < 0)
      {
        எனில் (username)
          {
            error (0, errno, _("failed to get groups for user %s"),
                   quote (username));
          }
        இல்லையெனில்
          {
            error (0, errno, _("failed to get groups for the current process"));
          }
        திரும்பு false;
      }

    க்கு (முழு i = 0; i < n_groups; i++)
      எனில் (groups[i] != rgid && groups[i] != egid)
        {
          putchar (delim);
          எனில் (!print_group (groups[i], use_names))
            ok = false;
        }
    விடுவி (groups);
  }
  திரும்பு ok;
}

/* Print the name or value of group ID GID. */
வெளி bool
print_group (gid_t gid, bool use_name)
{
  கட்டமைப்பு group *grp = வெற்று;
  bool ok = true;

  எனில் (use_name)
    {
      grp = getgrgid (gid);
      எனில் (grp == வெற்று)
        {
          எனில் (TYPE_SIGNED (gid_t))
            {
              intmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %jd"), g);
            }
          இல்லையெனில்
            {
              uintmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %ju"), g);
            }
          ok = false;
        }
    }

  எனில் (grp)
    printf ("%s", grp->gr_name);
  இல்லையெனில்
    printf ("%ju", (uintmax_t) gid);
  திரும்பு ok;
}
