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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <pwd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <grp.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mgetgroups.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "group-list.h"


/* Print all of the distinct groups the user is in. */
ਬਾਹਰੀ bool
print_ਸਮੂਹ_list (ਅੱਖਰ ਅਚਲ *username,
                  uid_t ruid, gid_t rgid, gid_t egid,
                  bool use_ਨਾਂ, ਅੱਖਰ delim)
{
  bool ok = true;
  ਬਣਤਰ passwd *pwd = ਨਿਰਾ;

  ਜੇ (username)
    {
      pwd = getpwuid (ruid);
      ਜੇ (pwd == ਨਿਰਾ)
        ok = false;
    }

  ਜੇ (!print_ਸਮੂਹ (rgid, use_ਨਾਂ))
    ok = false;

  ਜੇ (egid != rgid)
    {
      ਅੱਖਰ_ਲਿਖੋ (delim);
      ਜੇ (!print_ਸਮੂਹ (egid, use_ਨਾਂ))
        ok = false;
    }

  {
    gid_t *groups;

    ਪੂਰਨ n_groups = xgetgroups (username, (pwd ? pwd->pw_gid : egid), &groups);
    ਜੇ (n_groups < 0)
      {
        ਜੇ (username)
          {
            error (0, errno, _("failed to get groups for user %s"),
                   quote (username));
          }
        ਨਹੀਂਤਾਂ
          {
            error (0, errno, _("failed to get groups for the current process"));
          }
        ਵਾਪਸ false;
      }

    ਲਈ (ਪੂਰਨ i = 0; i < n_groups; i++)
      ਜੇ (groups[i] != rgid && groups[i] != egid)
        {
          ਅੱਖਰ_ਲਿਖੋ (delim);
          ਜੇ (!print_ਸਮੂਹ (groups[i], use_ਨਾਂ))
            ok = false;
        }
    ਮੁਕਤ (groups);
  }
  ਵਾਪਸ ok;
}

/* Print the name or value of group ID GID. */
ਬਾਹਰੀ bool
print_ਸਮੂਹ (gid_t gid, bool use_name)
{
  ਬਣਤਰ ਸਮੂਹ *grp = ਨਿਰਾ;
  bool ok = true;

  ਜੇ (use_name)
    {
      grp = getgrgid (gid);
      ਜੇ (grp == ਨਿਰਾ)
        {
          ਜੇ (TYPE_SIGNED (gid_t))
            {
              intmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %jd"), g);
            }
          ਨਹੀਂਤਾਂ
            {
              uintmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %ju"), g);
            }
          ok = false;
        }
    }

  ਜੇ (grp)
    ਲਿਖੋ ("%s", grp->gr_name);
  ਨਹੀਂਤਾਂ
    ਲਿਖੋ ("%ju", (uintmax_t) gid);
  ਵਾਪਸ ok;
}
