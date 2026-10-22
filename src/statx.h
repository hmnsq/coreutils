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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ COREUTILS_STATX_H
# define COREUTILS_STATX_H

# ਜੇ HAVE_STATX && defined STATX_INO
/* Much of the format printing requires a struct stat or timespec */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ timespec
statx_timestamp_ਮੰਜ਼ਿਲ_timespec (ਬਣਤਰ statx_timestamp tsx)
{
  ਬਣਤਰ timespec ts;

  ts.tv_sec = tsx.tv_sec;
  ts.tv_nsec = tsx.tv_nsec;
  ਵਾਪਸ ts;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
statx_ਮੰਜ਼ਿਲ_stat (ਬਣਤਰ statx *stx, ਬਣਤਰ stat *stat)
{
  stat->st_dev = makedev (stx->stx_dev_major, stx->stx_dev_minor);
  stat->st_ino = stx->stx_ino;
  stat->st_ਬਹੁਲਕ = stx->stx_ਬਹੁਲਕ;
  stat->st_nlink = stx->stx_nlink;
  stat->st_uid = stx->stx_uid;
  stat->st_gid = stx->stx_gid;
  stat->st_rdev = makedev (stx->stx_rdev_major, stx->stx_rdev_minor);
  stat->st_ਮਾਪ = stx->stx_ਮਾਪ;
  stat->st_blksize = stx->stx_blksize;
/* define to avoid sc_prohibit_stat_st_blocks.  */
#  define SC_ST_ਖੰਡਾਂ st_ਖੰਡਾਂ
  stat->SC_ST_ਖੰਡਾਂ = stx->stx_ਖੰਡਾਂ;
  stat->st_atim = statx_timestamp_ਮੰਜ਼ਿਲ_timespec (stx->stx_atime);
  stat->st_mtim = statx_timestamp_ਮੰਜ਼ਿਲ_timespec (stx->stx_mtime);
  stat->st_ctim = statx_timestamp_ਮੰਜ਼ਿਲ_timespec (stx->stx_ctime);
}
# endif /* HAVE_STATX && defined STATX_INO */
#ਜੇ_ਅੰਤ /* COREUTILS_STATX_H */
