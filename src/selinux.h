/* selinux - core functions for maintaining SELinux labeling
   Copyright (C) 2012-2026 Free Software Foundation, Inc.

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

/* Written by Daniel Walsh <dwalsh@redhat.com> */

#నిర్వచితం_కాకపోతే COREUTILS_SELINUX_H
# define COREUTILS_SELINUX_H

నిర్మాణం selabel_handle;

/* Return true if ERR corresponds to an unsupported request,
   or if there is no context or it's inaccessible.  */
స్థిర అంతఃవరుస bool
ignorable_ctx_err (పూర్ణాంకం err)
{
  తిరిగి err == ENOTSUP || err == ENODATA;
}

# ఒకవేళ HAVE_SELINUX_LABEL_H

బాహ్య bool
restorecon (నిర్మాణం selabel_handle *selabel_handle,
            అక్షరం స్థిరరాశి *మార్గం, bool recurse);
బాహ్య పూర్ణాంకం
defaultcon (నిర్మాణం selabel_handle *selabel_handle,
            అక్షరం స్థిరరాశి *మార్గం, mode_t బహుళకం_2);

# లేకపోతే

స్థిర అంతఃవరుస bool
restorecon (MAYBE_వాడని నిర్మాణం selabel_handle *selabel_handle,
            MAYBE_వాడని అక్షరం స్థిరరాశి *మార్గం, MAYBE_వాడని bool recurse)
{ errno = ENOTSUP; తిరిగి false; }

స్థిర అంతఃవరుస పూర్ణాంకం
defaultcon (MAYBE_వాడని నిర్మాణం selabel_handle *selabel_handle,
            MAYBE_వాడని అక్షరం స్థిరరాశి *మార్గం, MAYBE_వాడని mode_t బహుళకం_2)
{ errno = ENOTSUP; తిరిగి -1; }

# endif

#అయితే_ముగింపు
