/* Common definitions for splice and vmsplice.
   Copyright (C) 2026 Free Software Foundation, Inc.

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

#यदि_अपरिभाषितम् SPLICE_H
# define SPLICE_H 1

# यदि HAVE_VMSPLICE

/* splice() and vmsplice() were introduced at the same time,
   so assume splice() is available.  Note AIX also has a different
   splice() which is why we don't explicitly check for that function. */
#  define HAVE_SPLICE 1

/* Empirically determined pipe size for best throughput.
   Needs to be <= /proc/sys/fs/pipe-max-size  */
गणना { SPLICE_PIPE_आकार = 512 * 1024 };

स्थिर अन्तःस्थ idx_t
increase_pipe_आकार (पूर्णाङ्क fd)
{
  पूर्णाङ्क pipe_cap = 0;
#  यदि defined F_SETPIPE_SZ && defined F_GETPIPE_SZ
  यदि ((pipe_cap = fcntl (fd, F_SETPIPE_SZ, SPLICE_PIPE_आकार)) < 0)
    pipe_cap = fcntl (fd, F_GETPIPE_SZ);
#  endif
  यदि (pipe_cap <= 0)
    pipe_cap = 64 * 1024;
  प्रत्यागम pipe_cap;
}

# endif

#यदि_अन्तः
