/* statx -> stat conversion functions for coreutils
   Copyright (C) 2019-2026 Free Software Foundation, Inc.

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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ COREUTILS_STATX_H
# define COREUTILS_STATX_H

# ಆದರೆ HAVE_STATX && defined STATX_INO
/* Much of the format printing requires a struct stat or timespec */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ timespec
statx_timestamp_ಗಮ್ಯ_timespec (ರಚನೆ statx_timestamp tsx)
{
  ರಚನೆ timespec ts;

  ts.tv_sec = tsx.tv_sec;
  ts.tv_nsec = tsx.tv_nsec;
  ಹಿಂತಿರುಗಿ ts;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
statx_ಗಮ್ಯ_stat (ರಚನೆ statx *stx, ರಚನೆ stat *stat)
{
  stat->st_dev = makedev (stx->stx_dev_major, stx->stx_dev_minor);
  stat->st_ino = stx->stx_ino;
  stat->st_ಬಹುಲಕ = stx->stx_ಬಹುಲಕ;
  stat->st_nlink = stx->stx_nlink;
  stat->st_uid = stx->stx_uid;
  stat->st_gid = stx->stx_gid;
  stat->st_rdev = makedev (stx->stx_rdev_major, stx->stx_rdev_minor);
  stat->st_ಗಾತ್ರ = stx->stx_ಗಾತ್ರ;
  stat->st_blksize = stx->stx_blksize;
/* define to avoid sc_prohibit_stat_st_blocks.  */
#  define SC_ST_ಖಂಡಗಳು st_ಖಂಡಗಳು
  stat->SC_ST_ಖಂಡಗಳು = stx->stx_ಖಂಡಗಳು;
  stat->st_atim = statx_timestamp_ಗಮ್ಯ_timespec (stx->stx_atime);
  stat->st_mtim = statx_timestamp_ಗಮ್ಯ_timespec (stx->stx_mtime);
  stat->st_ctim = statx_timestamp_ಗಮ್ಯ_timespec (stx->stx_ctime);
}
# endif /* HAVE_STATX && defined STATX_INO */
#ಆದರೆ_ಅಂತ್ಯ /* COREUTILS_STATX_H */
