/* Copy data from one file to another.
   Copyright 1989-2026 Free Software Foundation, Inc.

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

/* Extracted from copy.c by Paul Eggert.  */

#ضمن <config.h>

#ضمن "system.h"

#ضمن "alignalloc.h"
#ضمن "assure.h"
#ضمن "backupfile.h"
#ضمن "buffer-lcm.h"
#ضمن "copy.h"
#ضمن "fadvise.h"
#ضمن "full-write.h"
#ضمن "ioblksize.h"

/* Result of infer_scantype when it returns LSEEK_SCANTYPE.  */
بنية جاچ_inference
{
  /* The offset of the first data block, or -1 if the file has no data.  */
  off_t ext_شروعات;

  /* The offset of the first hole.  */
  off_t hole_شروعات;
};

/* Attempt to punch a hole to avoid any permanent
   speculative preallocation on file systems such as XFS.
   Return values as per fallocate(2) except ENOSYS etc. are ignored.  */
ساكن صحيح
punch_hole (صحيح fd, off_t وٿي, off_t ڊيگهائي)
{
  صحيح ret = 0;
/* +0 is to work around older <linux/fs.h> defining HAVE_FALLOCATE to empty.  */
#إذا HAVE_FALLOCATE + 0
# إذا defined FALLOC_FL_PUNCH_HOLE && defined FALLOC_FL_KEEP_سائيز
  ret = fallocate (fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_سائيز,
                   وٿي, ڊيگهائي);
  إذا (ret < 0 && (is_ENOTSUP (errno) || errno == ENOSYS))
    ret = 0;
# endif
#جيڪڏهن_پورو
  أرجع ret;
}

/* Create a hole at the end of the file with descriptor FD and name NAME.
   The hole is of size SIZE.  Assume FD is already at file end,
   and advance FD past the newly-created hole.
   Return the resulting position, or -1 on failure.  */
ساكن off_t
create_hole (صحيح fd, حرف ثابت *name, off_t سائيز)
{
  off_t file_پڇاڙي = lseek (fd, سائيز, SEEK_CUR);

  إذا (file_پڇاڙي < 0)
    {
      error (0, errno, _("cannot lseek %s"), quoteaf (name));
      أرجع -1;
    }

  /* Some file systems (like XFS) preallocate when write extending a file.
     I.e., a previous write() may have preallocated extra space
     that the seek above will not discard.  A subsequent write() could
     then make this allocation permanent.  */
  إذا (punch_hole (fd, file_پڇاڙي - سائيز, سائيز) < 0)
    {
      error (0, errno, _("error deallocating %s"), quoteaf (name));
      أرجع -1;
    }

  أرجع file_پڇاڙي;
}

/* Does ERR mean the copying operation is not supported or allowed for
   this file or process, even though the operation was invoked correctly?  */
ساكن bool
is_CLONENOTSUP (صحيح err)
{
  أرجع err == ENOSYS || err == ENOTTY || is_ENOTSUP (err)
         || err == EINVAL || err == EBADF
         || err == EXDEV || err == ETXTBSY
         || err == EPERM || err == EACCES;
}

/* Copy SRC_FD to DEST_FD, using the buffer *ABUF of size BUF_SIZE
   for temporary storage, allocating the buffer lazily if *ABUF is null.
   For best results, *ABUF should be well-aligned.
   If ALLOW_REFLINK, possibly use copy_file_range to copy.
   SRC_NAME and DST_NAME are the source and destination's name.
   Copy no more than MAX_N_READ bytes.
   If HOLE_SIZE, look for holes in the input; *HOLE_SIZE contains
   the size of the current hole so far, and update *HOLE_SIZE
   at end to be the size of the hole at the end of the copy.
   Read and update *DEBUG as needed.
   If successful, return the number of bytes copied,
   otherwise diagnose the failure and return -1.  */
ساكن intmax_t
sparse_copy (صحيح src_fd, صحيح dest_fd, حرف **abuf, idx_t buf_سائيز,
             bool allow_reflink,
             حرف ثابت *src_name, حرف ثابت *dst_name,
             ڳڻپ_t max_n_read, off_t *hole_سائيز,
             بنية copy_debug *debug)
{
  ڳڻپ_t ڪل_n_read = 0;

  إذا (debug->sparse_detection == COPY_DEBUG_UNKNOWN)
    debug->sparse_detection = hole_سائيز ? COPY_DEBUG_YES : COPY_DEBUG_NO;
  وإلا إذا (hole_سائيز && debug->sparse_detection == COPY_DEBUG_EXTERNAL)
    debug->sparse_detection = COPY_DEBUG_EXTERNAL_اندروني;

  /* If not looking for holes, use copy_file_range if functional,
     but don't use if reflink disallowed as that may be implicit.  */
  إذا (!hole_سائيز && allow_reflink)
    طالما (0 < max_n_read)
      {
        /* Copy at most COPY_MAX bytes at a time; this is min
           (SSIZE_MAX, SIZE_MAX) truncated to a value that is
           surely aligned well.  */
        ssize_t copy_max = MIN (SSIZE_MAX, SIZE_MAX) >> 30 << 30;
        ssize_t n_copied = copy_file_range (src_fd, عدم, dest_fd, عدم,
                                            MIN (max_n_read, copy_max), 0);
        إذا (n_copied == 0)
          {
            /* copy_file_range incorrectly returns 0 when reading from
               the proc file system on the Linux kernel through at
               least 5.6.19 (2020), so fall back on 'read' if the
               input file seems empty.  */
            إذا (ڪل_n_read == 0)
              اكسر;
            debug->offload = COPY_DEBUG_YES;
            أرجع ڪل_n_read;
          }
        إذا (n_copied < 0)
          {
            debug->offload = errno;

            /* Don’t treat EFBIG as a reportable error from copy_file_range.
               If the input is at EOF and the output position is 2**63 - 1,
               copy_file_range (ifd, NULL, ofd, NULL, 2146435072, 0)
               incorrectly fails with EFBIG.  Problem observed on Ubuntu 25.10
               x86-64 with Linux kernel 6.17.0-8-generic #8-Ubuntu.  */
            إذا (errno == EFBIG)
              اكسر;

            /* Consider operation unsupported only if no data copied.
               For example, EPERM could occur if copy_file_range not enabled
               in seccomp filters, so retry with a standard copy.  EPERM can
               also occur for immutable files, but that would only be in the
               edge case where the file is made immutable after creating,
               in which case the (more accurate) error is still shown.  */
            إذا (ڪل_n_read == 0 && is_CLONENOTSUP (errno))
              اكسر;

            /* ENOENT was seen sometimes across CIFS shares, resulting in
               no data being copied, but subsequent standard copies succeed.  */
            إذا (ڪل_n_read == 0 && errno == ENOENT)
              اكسر;

            إذا (errno == EINTR)
              n_copied = 0;
            وإلا
              {
                error (0, errno, _("error copying %s to %s"),
                       quoteaf_n (0, src_name), quoteaf_n (1, dst_name));
                أرجع -1;
              }
          }
        debug->offload = COPY_DEBUG_YES;
        max_n_read -= n_copied;
        ڪل_n_read += n_copied;
      }
  وإلا
    debug->offload = COPY_DEBUG_AVOIDED;


  off_t psize = hole_سائيز ? *hole_سائيز : 0;
  bool make_hole = !!psize;

  طالما (0 < max_n_read)
    {
      إذا (!*abuf)
        *abuf = xalignalloc (getpagesize (), buf_سائيز);
      حرف *buf = *abuf;
      ssize_t n_read = اقرأ (src_fd, buf, MIN (max_n_read, buf_سائيز));
      إذا (n_read < 0)
        {
          إذا (errno == EINTR)
            استمر;
          error (0, errno, _("error reading %s"), quoteaf (src_name));
          أرجع -1;
        }
      إذا (n_read == 0)
        اكسر;
      max_n_read -= n_read;
      ڪل_n_read += n_read;

      /* If looking for holes, loop over the input buffer in chunks
         corresponding to the minimum hole size.  Otherwise, scan the
         whole buffer.  struct stat does not report the minimum hole
         size for a file, so use ST_NBLOCKSIZE which should be the
         minimum for all file systems on this platform.  */
      idx_t csize = hole_سائيز ? ST_NBLOCKSIZE : buf_سائيز;
      حرف *cbuf = buf;
      حرف *pbuf = buf;

      طالما (n_read)
        {
          bool prev_hole = make_hole;
          csize = MIN (csize, n_read);

          إذا (hole_سائيز)
            make_hole = is_nul (cbuf, csize);

          bool transition = (make_hole != prev_hole) && psize;
          bool آخري_chunk = n_read == csize && !make_hole;

          إذا (transition || آخري_chunk)
            {
              إذا (! transition)
                psize += csize;
              وإلا إذا (prev_hole)
                {
                  إذا (create_hole (dest_fd, dst_name, psize) < 0)
                    أرجع false;
                  pbuf = cbuf;
                  psize = csize;
                }

              إذا (!prev_hole || (transition && آخري_chunk))
                {
                  إذا (ڀريل_write (dest_fd, pbuf, psize) != psize)
                    {
                      error (0, errno, _("error writing %s"),
                             quoteaf (dst_name));
                      أرجع -1;
                    }
                  psize = !prev_hole && transition ? csize : 0;
                }
            }
          وإلا  /* Coalesce writes/seeks.  */
            {
              إذا (ckd_جوڙيو (&psize, psize, csize))
                {
                  error (0, 0, _("overflow reading %s"), quoteaf (src_name));
                  أرجع -1;
                }
            }

          n_read -= csize;
          cbuf += csize;
        }

      /* It's tempting to break early here upon a short read from
         a regular file.  That would save the final read syscall
         for each file.  Unfortunately that doesn't work for
         certain files in /proc or /sys with linux kernels.  */
    }

  إذا (hole_سائيز)
    *hole_سائيز = make_hole ? psize : 0;
  أرجع ڪل_n_read;
}

/* Write to FD.  Write data consisting of N_BYTES' worth of '\0's.
   Use the buffer *ABUF of size BUF_SIZE for temporary storage,
   allocating the buffer lazily if *ABUF is null.
   Return true if successful, false (setting errno) otherwise.  */
ساكن bool
write_zeros (صحيح fd, off_t n_بائيٽون, حرف **abuf, idx_t buf_سائيز)
{
  حرف *zeros = عدم;
  طالما (n_بائيٽون)
    {
      idx_t n = MIN (buf_سائيز, n_بائيٽون);
      إذا (!zeros)
        {
          إذا (!*abuf)
            *abuf = xalignalloc (getpagesize (), buf_سائيز);
          zeros = memset (*abuf, 0, n);
        }
      إذا (ڀريل_write (fd, zeros, n) != n)
        أرجع false;
      n_بائيٽون -= n;
    }

  أرجع true;
}

#جيڪڏهن_وصف_ٿيل SEEK_HOLE
/* Perform an efficient extent copy, if possible.  This avoids
   the overhead of detecting holes in hole-introducing/preserving
   copy, and thus makes copying sparse files much more efficient.
   Copy from SRC_FD to DEST_FD, using *ABUF (of size BUF_SIZE) for a buffer.
   Allocate *ABUF lazily if *ABUF is null.
   The input file was originally positioned at SRC_POS.
   Copy at most IBYTES.
   SCAN_INFERENCE points to the result of an input file scan inference.
   If SCAN_INFERENCE->ext_start is nonnegative, SRC_FD now positioned there;
   otherwise the file has no data extents and the file is now
   positioned at SRC_POS.
   SCAN_INFERENCE->hole_start is the position of the first hole.
   The file is of size SRC_TOTAL_SIZE.
   Use SPARSE_MODE to determine whether to create holes in the output.
   If ALLOW_REFLINK, possibly use copy_file_range to copy.
   SRC_NAME and DST_NAME are the input and output file names.
   Set *HOLE_SIZE to be the size of the hole at the end of the input;
   do not output the zeros in that hole.
   Read and update *DEBUG as needed.
   If successful, return the number of bytes copied,
   otherwise diagnose the failure and return -1.  */

ساكن off_t
lseek_copy (صحيح src_fd, صحيح dest_fd, حرف **abuf, idx_t buf_سائيز,
            off_t src_pos, ڳڻپ_t ibytes,
            بنية جاچ_inference ثابت *جاچ_inference, off_t src_ڪل_سائيز,
            تعداد Sparse_type sparse_گهڻائي,
            bool allow_reflink,
            حرف ثابت *src_name, حرف ثابت *dst_name,
            off_t *hole_سائيز, بنية copy_debug *debug)
{
  off_t آخري_ext_شروعات = src_pos;
  off_t آخري_ext_len = 0;
  off_t max_ipos = ckd_جوڙيو (&max_ipos, src_pos, ibytes) ? OFF_T_MAX : max_ipos;

  /* From here on, SRC_TOTAL_SIZE is bounded above by MAX_IPOS.  */
  src_ڪل_سائيز = MIN (src_ڪل_سائيز, max_ipos);

  /* The input position after the most recent read of data,
     or SRC_POS if no data have been read.  */
  off_t ipos = src_pos;

  debug->sparse_detection = COPY_DEBUG_EXTERNAL;

  bool استعمال_ٿيل_جاچ_inference = false;

  لكل (off_t ext_شروعات = جاچ_inference->ext_شروعات;
       0 <= ext_شروعات && ext_شروعات < max_ipos; )
    {
      off_t ext_پڇاڙي;
      إذا (ext_شروعات == src_pos && ! استعمال_ٿيل_جاچ_inference)
        {
          ext_پڇاڙي = جاچ_inference->hole_شروعات;
          استعمال_ٿيل_جاچ_inference = true;
        }
      وإلا
        ext_پڇاڙي = lseek (src_fd, ext_شروعات, SEEK_HOLE);
      إذا (0 <= ext_پڇاڙي)
        ext_پڇاڙي = MIN (ext_پڇاڙي, max_ipos);
      وإلا
        {
          إذا (errno != ENXIO)
            اذهب cannot_lseek;
          ext_پڇاڙي = src_ڪل_سائيز;
          إذا (ext_پڇاڙي <= ext_شروعات)
            {
              /* The input file grew; get its current size.  */
              src_ڪل_سائيز = lseek (src_fd, 0, SEEK_END);
              إذا (src_ڪل_سائيز < 0)
                اذهب cannot_lseek;
              src_ڪل_سائيز = MIN (src_ڪل_سائيز, max_ipos);

              /* If the input file shrank after growing, stop copying.  */
              إذا (src_ڪل_سائيز <= ext_شروعات)
                اكسر;

              ext_پڇاڙي = src_ڪل_سائيز;
            }
        }
      /* If the input file must have grown, increase its measured size.  */
      إذا (src_ڪل_سائيز < ext_پڇاڙي)
        src_ڪل_سائيز = ext_پڇاڙي;

      إذا (lseek (src_fd, ext_شروعات, SEEK_SET) < 0)
        اذهب cannot_lseek;

      off_t ext_hole_سائيز = ext_شروعات - آخري_ext_شروعات - آخري_ext_len;

      إذا (ext_hole_سائيز)
        {
          إذا (sparse_گهڻائي == SPARSE_ALWAYS)
            *hole_سائيز += ext_hole_سائيز;
          وإلا إذا (sparse_گهڻائي != SPARSE_NEVER)
            {
              off_t epos = create_hole (dest_fd, dst_name, ext_hole_سائيز);
              إذا (epos < 0)
                أرجع epos;
            }
          وإلا
            {
              /* When not inducing holes and when there is a hole between
                 the end of the previous extent and the beginning of the
                 current one, write zeros to the destination file.  */
              إذا (! write_zeros (dest_fd, ext_hole_سائيز, abuf, buf_سائيز))
                {
                  error (0, errno, _("%s: write failed"),
                         quotef (dst_name));
                  أرجع -1;
                }
            }
        }

      off_t ext_len = ext_پڇاڙي - ext_شروعات;
      آخري_ext_شروعات = ext_شروعات;
      آخري_ext_len = ext_len;

      /* Copy this extent, looking for further opportunities to not
         bother to write zeros if --sparse=always, since SEEK_HOLE
         is conservative and may miss some holes.  */
      off_t n_read
        = sparse_copy (src_fd, dest_fd, abuf, buf_سائيز,
                       allow_reflink, src_name, dst_name,
                       ext_len,
                       sparse_گهڻائي == SPARSE_ALWAYS ? hole_سائيز : عدم,
                       debug);
      إذا (n_read < 0)
        أرجع -1;

      ipos = ext_شروعات + n_read;
      إذا (n_read < ext_len)
        {
          /* The input file shrank.  */
          src_ڪل_سائيز = ipos;
          اكسر;
        }

      ext_شروعات = lseek (src_fd, ipos, SEEK_معلومات);
      إذا (ext_شروعات < 0 && errno != ENXIO)
        اذهب cannot_lseek;
    }

  *hole_سائيز += src_ڪل_سائيز - (آخري_ext_شروعات + آخري_ext_len);
  أرجع src_ڪل_سائيز - src_pos;

 cannot_lseek:
  error (0, errno, _("cannot lseek %s"), quoteaf (src_name));
  أرجع -1;
}
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HAVE_STRUCT_STAT_ST_بلاڪون
# define HAVE_STRUCT_STAT_ST_بلاڪون 0
#جيڪڏهن_پورو

/* Type of scan being done on the input when looking for sparseness.  */
تعداد scantype
  {
   /* An error was found when determining scantype.  */
   ERROR_SCANTYPE,

   /* No fancy scanning; just read and write.  */
   PLAIN_SCANTYPE,

   /* Read and examine data looking for zero blocks; useful when
      attempting to create sparse output.  */
   ZERO_SCANTYPE,

   /* lseek information is available.  */
   LSEEK_SCANTYPE,
  };

/* Return how to scan a file with descriptor FD and stat buffer SB.
   The scan starts at POS.
   Set *SCAN_INFERENCE if returning LSEEK_SCANTYPE.  */
ساكن تعداد scantype
infer_scantype (صحيح fd, بنية stat ثابت *sb, off_t pos,
                بنية جاچ_inference *جاچ_inference)
{
  /* Try SEEK_HOLE only if this heuristic
     suggests the file is sparse.  */
  إذا (! (HAVE_STRUCT_STAT_ST_بلاڪون
         && S_ISREG (sb->st_گهڻائي)
         && STP_NBLOCKS (sb) < sb->st_سائيز / ST_NBLOCKSIZE))
    أرجع PLAIN_SCANTYPE;

#جيڪڏهن_وصف_ٿيل SEEK_HOLE
  جاچ_inference->ext_شروعات = lseek (fd, pos, SEEK_معلومات);
  إذا (جاچ_inference->ext_شروعات == pos)
    {
      جاچ_inference->hole_شروعات = lseek (fd, pos, SEEK_HOLE);
      إذا (0 <= جاچ_inference->hole_شروعات)
        {
          إذا (جاچ_inference->hole_شروعات < sb->st_سائيز)
            أرجع LSEEK_SCANTYPE;

          /* Though the file may have holes, SEEK_DATA and SEEK_HOLE
             didn't find any.  This can happen with file systems
             that support SEEK_HOLE only trivially,
             such as squashfs in Linux kernel 6.17 and earlier.
             This can also happen due to transparent file compression,
             which can also indicate fewer than the usual number of blocks.  */

          إذا (lseek (fd, pos, SEEK_SET) < 0)
            أرجع ERROR_SCANTYPE;

          /* we prefer to return PLAIN_SCANTYPE here so that copy offload
             continues to be used.  Falling through to ZERO_SCANTYPE would be
             less efficient in the compressed file case.  */
          أرجع PLAIN_SCANTYPE;
        }
    }
  وإلا إذا (pos < جاچ_inference->ext_شروعات || errno == ENXIO)
    {
      جاچ_inference->hole_شروعات = pos;  /* Pacify -Wmaybe-uninitialized.  */
      أرجع LSEEK_SCANTYPE;
    }
  وإلا إذا (errno != EINVAL && !is_ENOTSUP (errno))
    أرجع ERROR_SCANTYPE;
#جيڪڏهن_پورو

  أرجع ZERO_SCANTYPE;
}

/* Copy data from input file (descriptor IFD, status IST, initial file
   offset IPOS, and name INAME) to output file (OFD, OST, OPOS, ONAME).
   If IPOS and OPOS are negative, their values are not known, perhaps
   because the files are not seekable so their positions are irrelevant.
   Set the input and output file positions to unspecified values.
   Copy until IBYTES have been copied or until end of file;
   if IBYTES is COUNT_MAX that suffices to copy to end of file.
   Respect copy options X's sparse_mode and reflink_mode settings.
   Read and update *DEBUG as needed.
   If successful, return the number of bytes copied;
   otherwise, diagnose the error and return -1.
   If unsuccessful, the destination area's contents are unspecified.  */

خارجي intmax_t
copy_file_معلومات (صحيح ifd, بنية stat ثابت *ist, off_t ipos, حرف ثابت *iname,
                صحيح ofd, بنية stat ثابت *ost, off_t opos, حرف ثابت *oname,
                ڳڻپ_t ibytes, بنية cp_options ثابت *x,
                بنية copy_debug *debug)
{
  /* Choose a suitable buffer size; it may be adjusted later.  */
  idx_t buf_سائيز = io_blksize (ost);

  /* Deal with sparse files.  */
  بنية جاچ_inference جاچ_inference;
  تعداد scantype scantype = infer_scantype (ifd, ist, ipos, &جاچ_inference);
  إذا (scantype == ERROR_SCANTYPE)
    {
      error (0, errno, _("cannot lseek %s"), quoteaf (iname));
      أرجع -1;
    }
  bool make_holes
    = (S_ISREG (ost->st_گهڻائي)
       && (x->sparse_گهڻائي == SPARSE_ALWAYS
           || (x->sparse_گهڻائي == SPARSE_AUTO
               && scantype != PLAIN_SCANTYPE)));

  /* If we _know_ we're going to read data sequentially into the process,
     i.e., --reflink or --sparse are not in auto mode,
     give that hint to the kernel so it can tune caching behavior.
     Also we don't bother calling fadvise for small copies,
     as it is not likely to help performance and might even hurt it.
     Also we only apply this hint for the whole file (0 length)
     as OpenZFS 2.2.2 at least will otherwise synchronously
     (decompress and) populate the cache when given a specific length.  */
  إذا (ipos == 0 && ibytes == ڳڻپ_MAX
      && (x->reflink_گهڻائي != REFLINK_AUTO || x->sparse_گهڻائي != SPARSE_AUTO))
    fdadvise (ifd, 0, 0, FADVISE_SEQUENTIAL);

  /* If not making a sparse file, try to use a more-efficient
     buffer size.  */
  إذا (! make_holes)
    {
      /* Compute the least common multiple of the input and output
         buffer sizes, adjusting for outlandish values.
         blcm is at most IDX_MAX - 1 so that buf_size cannot become 0 below.
         Note we read in multiples of the reported block size
         to support (unusual) devices that have this constraint.  */
      idx_t blcm_max = MIN (MIN (IDX_MAX - 1, SSIZE_MAX), SIZE_MAX);
      idx_t blcm = بفر_lcm (io_blksize (ist), buf_سائيز,
                               blcm_max);

      /* Do not bother with a buffer larger than the input file, plus one
         byte to make sure the file has not grown while reading it.  */
      إذا (S_ISREG (ist->st_گهڻائي) && 0 <= ist->st_سائيز
          && ist->st_سائيز < buf_سائيز)
        buf_سائيز = ist->st_سائيز + 1;

      /* However, stick with a block size that is a positive multiple of
         blcm, overriding the above adjustments.  Watch out for
         overflow.  */
      buf_سائيز = ckd_جوڙيو (&buf_سائيز, buf_سائيز, blcm - 1) ? IDX_MAX : buf_سائيز;
      buf_سائيز -= buf_سائيز % blcm;
    }

  حرف *buf = عدم;
  intmax_t نتيجو;
  off_t hole_سائيز = 0;

  إذا (scantype == LSEEK_SCANTYPE)
    {
#جيڪڏهن_وصف_ٿيل SEEK_HOLE
      نتيجو = lseek_copy (ifd, ofd, &buf, buf_سائيز,
                           ipos, ibytes, &جاچ_inference, ist->st_سائيز,
                           make_holes ? x->sparse_گهڻائي : SPARSE_NEVER,
                           x->reflink_گهڻائي != REFLINK_NEVER,
                           iname, oname, &hole_سائيز, debug);
#وإلا
      affirm (false);
#جيڪڏهن_پورو
    }
  وإلا
    نتيجو = sparse_copy (ifd, ofd, &buf, buf_سائيز,
                          x->reflink_گهڻائي != REFLINK_NEVER,
                          iname, oname, ibytes,
                          make_holes ? &hole_سائيز : عدم,
                          debug);

  إذا (0 <= نتيجو && 0 < hole_سائيز)
    {
      off_t oend;
      إذا (ckd_جوڙيو (&oend, opos, نتيجو)
          ? (errno = EOVERFLOW, true)
          : make_holes
          ? ftruncate (ofd, oend) < 0
          : !write_zeros (ofd, hole_سائيز, &buf, buf_سائيز))
        {
          error (0, errno, _("failed to extend %s"), quoteaf (oname));
          نتيجو = -1;
        }
      وإلا إذا (make_holes
               && punch_hole (ofd, oend - hole_سائيز, hole_سائيز) < 0)
        {
          error (0, errno, _("error deallocating %s"), quoteaf (oname));
          نتيجو = -1;
        }
    }

  alignfree (buf);
  أرجع نتيجو;
}
