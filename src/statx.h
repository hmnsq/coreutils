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

#यदि_अपरिभाषितम् COREUTILS_STATX_H
# define COREUTILS_STATX_H

# यदि HAVE_STATX && defined STATX_INO
/* Much of the format printing requires a struct stat or timespec */
स्थिर अन्तःस्थ संरचना timespec
statx_timestamp_गन्तव्यम्_timespec (संरचना statx_timestamp tsx)
{
  संरचना timespec ts;

  ts.tv_sec = tsx.tv_sec;
  ts.tv_nsec = tsx.tv_nsec;
  प्रत्यागम ts;
}

स्थिर अन्तःस्थ शून्य
statx_गन्तव्यम्_stat (संरचना statx *stx, संरचना stat *stat)
{
  stat->st_dev = makedev (stx->stx_dev_major, stx->stx_dev_minor);
  stat->st_ino = stx->stx_ino;
  stat->st_बहुलकम् = stx->stx_बहुलकम्;
  stat->st_nlink = stx->stx_nlink;
  stat->st_uid = stx->stx_uid;
  stat->st_gid = stx->stx_gid;
  stat->st_rdev = makedev (stx->stx_rdev_major, stx->stx_rdev_minor);
  stat->st_आकार = stx->stx_आकार;
  stat->st_blksize = stx->stx_blksize;
/* define to avoid sc_prohibit_stat_st_blocks.  */
#  define SC_ST_खण्डानि st_खण्डानि
  stat->SC_ST_खण्डानि = stx->stx_खण्डानि;
  stat->st_atim = statx_timestamp_गन्तव्यम्_timespec (stx->stx_atime);
  stat->st_mtim = statx_timestamp_गन्तव्यम्_timespec (stx->stx_mtime);
  stat->st_ctim = statx_timestamp_गन्तव्यम्_timespec (stx->stx_ctime);
}
# endif /* HAVE_STATX && defined STATX_INO */
#यदि_अन्तः /* COREUTILS_STATX_H */
