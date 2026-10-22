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

#شامل <config.h>

#شامل "system.h"

#شامل "alignalloc.h"
#شامل "assure.h"
#شامل "backupfile.h"
#شامل "buffer-lcm.h"
#شامل "copy.h"
#شامل "fadvise.h"
#شامل "full-write.h"
#شامل "ioblksize.h"

/* Result of infer_scantype when it returns LSEEK_SCANTYPE.  */
ساخت جائزہ_inference
{
  /* The offset of the first data block, or -1 if the file has no data.  */
  off_t ext_آغاز;

  /* The offset of the first hole.  */
  off_t hole_آغاز;
};

/* Attempt to punch a hole to avoid any permanent
   speculative preallocation on file systems such as XFS.
   Return values as per fallocate(2) except ENOSYS etc. are ignored.  */
ساکن صحیح
punch_hole (صحیح fd, off_t انحراف, off_t لمبائی)
{
  صحیح ret = 0;
/* +0 is to work around older <linux/fs.h> defining HAVE_FALLOCATE to empty.  */
#اگر HAVE_FALLOCATE + 0
# اگر defined FALLOC_FL_PUNCH_HOLE && defined FALLOC_FL_KEEP_سائز
  ret = fallocate (fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_سائز,
                   انحراف, لمبائی);
  اگر (ret < 0 && (is_ENOTSUP (errno) || errno == ENOSYS))
    ret = 0;
# endif
#اگر_ختم
  واپس ret;
}

/* Create a hole at the end of the file with descriptor FD and name NAME.
   The hole is of size SIZE.  Assume FD is already at file end,
   and advance FD past the newly-created hole.
   Return the resulting position, or -1 on failure.  */
ساکن off_t
create_hole (صحیح fd, حرف ثابت *name, off_t سائز)
{
  off_t file_انتہا = lseek (fd, سائز, SEEK_CUR);

  اگر (file_انتہا < 0)
    {
      error (0, errno, _("cannot lseek %s"), quoteaf (name));
      واپس -1;
    }

  /* Some file systems (like XFS) preallocate when write extending a file.
     I.e., a previous write() may have preallocated extra space
     that the seek above will not discard.  A subsequent write() could
     then make this allocation permanent.  */
  اگر (punch_hole (fd, file_انتہا - سائز, سائز) < 0)
    {
      error (0, errno, _("error deallocating %s"), quoteaf (name));
      واپس -1;
    }

  واپس file_انتہا;
}

/* Does ERR mean the copying operation is not supported or allowed for
   this file or process, even though the operation was invoked correctly?  */
ساکن bool
is_CLONENOTSUP (صحیح err)
{
  واپس err == ENOSYS || err == ENOTTY || is_ENOTSUP (err)
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
ساکن intmax_t
sparse_copy (صحیح src_fd, صحیح dest_fd, حرف **abuf, idx_t buf_سائز,
             bool allow_reflink,
             حرف ثابت *src_name, حرف ثابت *dst_name,
             گنتی_t max_n_read, off_t *hole_سائز,
             ساخت copy_debug *debug)
{
  گنتی_t کل_n_read = 0;

  اگر (debug->sparse_detection == COPY_DEBUG_UNKNOWN)
    debug->sparse_detection = hole_سائز ? COPY_DEBUG_YES : COPY_DEBUG_NO;
  ورنہ اگر (hole_سائز && debug->sparse_detection == COPY_DEBUG_EXTERNAL)
    debug->sparse_detection = COPY_DEBUG_EXTERNAL_اندرونی;

  /* If not looking for holes, use copy_file_range if functional,
     but don't use if reflink disallowed as that may be implicit.  */
  اگر (!hole_سائز && allow_reflink)
    جب (0 < max_n_read)
      {
        /* Copy at most COPY_MAX bytes at a time; this is min
           (SSIZE_MAX, SIZE_MAX) truncated to a value that is
           surely aligned well.  */
        ssize_t copy_max = MIN (SSIZE_MAX, SIZE_MAX) >> 30 << 30;
        ssize_t n_copied = copy_file_range (src_fd, تہی, dest_fd, تہی,
                                            MIN (max_n_read, copy_max), 0);
        اگر (n_copied == 0)
          {
            /* copy_file_range incorrectly returns 0 when reading from
               the proc file system on the Linux kernel through at
               least 5.6.19 (2020), so fall back on 'read' if the
               input file seems empty.  */
            اگر (کل_n_read == 0)
              توڑو;
            debug->offload = COPY_DEBUG_YES;
            واپس کل_n_read;
          }
        اگر (n_copied < 0)
          {
            debug->offload = errno;

            /* Don’t treat EFBIG as a reportable error from copy_file_range.
               If the input is at EOF and the output position is 2**63 - 1,
               copy_file_range (ifd, NULL, ofd, NULL, 2146435072, 0)
               incorrectly fails with EFBIG.  Problem observed on Ubuntu 25.10
               x86-64 with Linux kernel 6.17.0-8-generic #8-Ubuntu.  */
            اگر (errno == EFBIG)
              توڑو;

            /* Consider operation unsupported only if no data copied.
               For example, EPERM could occur if copy_file_range not enabled
               in seccomp filters, so retry with a standard copy.  EPERM can
               also occur for immutable files, but that would only be in the
               edge case where the file is made immutable after creating,
               in which case the (more accurate) error is still shown.  */
            اگر (کل_n_read == 0 && is_CLONENOTSUP (errno))
              توڑو;

            /* ENOENT was seen sometimes across CIFS shares, resulting in
               no data being copied, but subsequent standard copies succeed.  */
            اگر (کل_n_read == 0 && errno == ENOENT)
              توڑو;

            اگر (errno == EINTR)
              n_copied = 0;
            ورنہ
              {
                error (0, errno, _("error copying %s to %s"),
                       quoteaf_n (0, src_name), quoteaf_n (1, dst_name));
                واپس -1;
              }
          }
        debug->offload = COPY_DEBUG_YES;
        max_n_read -= n_copied;
        کل_n_read += n_copied;
      }
  ورنہ
    debug->offload = COPY_DEBUG_AVOIDED;


  off_t psize = hole_سائز ? *hole_سائز : 0;
  bool make_hole = !!psize;

  جب (0 < max_n_read)
    {
      اگر (!*abuf)
        *abuf = xalignalloc (getpagesize (), buf_سائز);
      حرف *buf = *abuf;
      ssize_t n_read = اقرأ (src_fd, buf, MIN (max_n_read, buf_سائز));
      اگر (n_read < 0)
        {
          اگر (errno == EINTR)
            جاری;
          error (0, errno, _("error reading %s"), quoteaf (src_name));
          واپس -1;
        }
      اگر (n_read == 0)
        توڑو;
      max_n_read -= n_read;
      کل_n_read += n_read;

      /* If looking for holes, loop over the input buffer in chunks
         corresponding to the minimum hole size.  Otherwise, scan the
         whole buffer.  struct stat does not report the minimum hole
         size for a file, so use ST_NBLOCKSIZE which should be the
         minimum for all file systems on this platform.  */
      idx_t csize = hole_سائز ? ST_NBLOCKSIZE : buf_سائز;
      حرف *cbuf = buf;
      حرف *pbuf = buf;

      جب (n_read)
        {
          bool پچھلا_hole = make_hole;
          csize = MIN (csize, n_read);

          اگر (hole_سائز)
            make_hole = is_nul (cbuf, csize);

          bool transition = (make_hole != پچھلا_hole) && psize;
          bool آخری_chunk = n_read == csize && !make_hole;

          اگر (transition || آخری_chunk)
            {
              اگر (! transition)
                psize += csize;
              ورنہ اگر (پچھلا_hole)
                {
                  اگر (create_hole (dest_fd, dst_name, psize) < 0)
                    واپس false;
                  pbuf = cbuf;
                  psize = csize;
                }

              اگر (!پچھلا_hole || (transition && آخری_chunk))
                {
                  اگر (بھرا_write (dest_fd, pbuf, psize) != psize)
                    {
                      error (0, errno, _("error writing %s"),
                             quoteaf (dst_name));
                      واپس -1;
                    }
                  psize = !پچھلا_hole && transition ? csize : 0;
                }
            }
          ورنہ  /* Coalesce writes/seeks.  */
            {
              اگر (ckd_جوڑو (&psize, psize, csize))
                {
                  error (0, 0, _("overflow reading %s"), quoteaf (src_name));
                  واپس -1;
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

  اگر (hole_سائز)
    *hole_سائز = make_hole ? psize : 0;
  واپس کل_n_read;
}

/* Write to FD.  Write data consisting of N_BYTES' worth of '\0's.
   Use the buffer *ABUF of size BUF_SIZE for temporary storage,
   allocating the buffer lazily if *ABUF is null.
   Return true if successful, false (setting errno) otherwise.  */
ساکن bool
write_zeros (صحیح fd, off_t n_بائٹیں, حرف **abuf, idx_t buf_سائز)
{
  حرف *zeros = تہی;
  جب (n_بائٹیں)
    {
      idx_t n = MIN (buf_سائز, n_بائٹیں);
      اگر (!zeros)
        {
          اگر (!*abuf)
            *abuf = xalignalloc (getpagesize (), buf_سائز);
          zeros = memset (*abuf, 0, n);
        }
      اگر (بھرا_write (fd, zeros, n) != n)
        واپس false;
      n_بائٹیں -= n;
    }

  واپس true;
}

#اگر_تعریف_شدہ SEEK_HOLE
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

ساکن off_t
lseek_copy (صحیح src_fd, صحیح dest_fd, حرف **abuf, idx_t buf_سائز,
            off_t src_pos, گنتی_t ibytes,
            ساخت جائزہ_inference ثابت *جائزہ_inference, off_t src_کل_سائز,
            شمار Sparse_type sparse_منوال,
            bool allow_reflink,
            حرف ثابت *src_name, حرف ثابت *dst_name,
            off_t *hole_سائز, ساخت copy_debug *debug)
{
  off_t آخری_ext_آغاز = src_pos;
  off_t آخری_ext_len = 0;
  off_t max_ipos = ckd_جوڑو (&max_ipos, src_pos, ibytes) ? OFF_T_MAX : max_ipos;

  /* From here on, SRC_TOTAL_SIZE is bounded above by MAX_IPOS.  */
  src_کل_سائز = MIN (src_کل_سائز, max_ipos);

  /* The input position after the most recent read of data,
     or SRC_POS if no data have been read.  */
  off_t ipos = src_pos;

  debug->sparse_detection = COPY_DEBUG_EXTERNAL;

  bool مستعمل_جائزہ_inference = false;

  برائے (off_t ext_آغاز = جائزہ_inference->ext_آغاز;
       0 <= ext_آغاز && ext_آغاز < max_ipos; )
    {
      off_t ext_انتہا;
      اگر (ext_آغاز == src_pos && ! مستعمل_جائزہ_inference)
        {
          ext_انتہا = جائزہ_inference->hole_آغاز;
          مستعمل_جائزہ_inference = true;
        }
      ورنہ
        ext_انتہا = lseek (src_fd, ext_آغاز, SEEK_HOLE);
      اگر (0 <= ext_انتہا)
        ext_انتہا = MIN (ext_انتہا, max_ipos);
      ورنہ
        {
          اگر (errno != ENXIO)
            جاؤ cannot_lseek;
          ext_انتہا = src_کل_سائز;
          اگر (ext_انتہا <= ext_آغاز)
            {
              /* The input file grew; get its current size.  */
              src_کل_سائز = lseek (src_fd, 0, SEEK_END);
              اگر (src_کل_سائز < 0)
                جاؤ cannot_lseek;
              src_کل_سائز = MIN (src_کل_سائز, max_ipos);

              /* If the input file shrank after growing, stop copying.  */
              اگر (src_کل_سائز <= ext_آغاز)
                توڑو;

              ext_انتہا = src_کل_سائز;
            }
        }
      /* If the input file must have grown, increase its measured size.  */
      اگر (src_کل_سائز < ext_انتہا)
        src_کل_سائز = ext_انتہا;

      اگر (lseek (src_fd, ext_آغاز, SEEK_SET) < 0)
        جاؤ cannot_lseek;

      off_t ext_hole_سائز = ext_آغاز - آخری_ext_آغاز - آخری_ext_len;

      اگر (ext_hole_سائز)
        {
          اگر (sparse_منوال == SPARSE_ALWAYS)
            *hole_سائز += ext_hole_سائز;
          ورنہ اگر (sparse_منوال != SPARSE_NEVER)
            {
              off_t epos = create_hole (dest_fd, dst_name, ext_hole_سائز);
              اگر (epos < 0)
                واپس epos;
            }
          ورنہ
            {
              /* When not inducing holes and when there is a hole between
                 the end of the previous extent and the beginning of the
                 current one, write zeros to the destination file.  */
              اگر (! write_zeros (dest_fd, ext_hole_سائز, abuf, buf_سائز))
                {
                  error (0, errno, _("%s: write failed"),
                         quotef (dst_name));
                  واپس -1;
                }
            }
        }

      off_t ext_len = ext_انتہا - ext_آغاز;
      آخری_ext_آغاز = ext_آغاز;
      آخری_ext_len = ext_len;

      /* Copy this extent, looking for further opportunities to not
         bother to write zeros if --sparse=always, since SEEK_HOLE
         is conservative and may miss some holes.  */
      off_t n_read
        = sparse_copy (src_fd, dest_fd, abuf, buf_سائز,
                       allow_reflink, src_name, dst_name,
                       ext_len,
                       sparse_منوال == SPARSE_ALWAYS ? hole_سائز : تہی,
                       debug);
      اگر (n_read < 0)
        واپس -1;

      ipos = ext_آغاز + n_read;
      اگر (n_read < ext_len)
        {
          /* The input file shrank.  */
          src_کل_سائز = ipos;
          توڑو;
        }

      ext_آغاز = lseek (src_fd, ipos, SEEK_ڈیٹا);
      اگر (ext_آغاز < 0 && errno != ENXIO)
        جاؤ cannot_lseek;
    }

  *hole_سائز += src_کل_سائز - (آخری_ext_آغاز + آخری_ext_len);
  واپس src_کل_سائز - src_pos;

 cannot_lseek:
  error (0, errno, _("cannot lseek %s"), quoteaf (src_name));
  واپس -1;
}
#اگر_ختم

#اگر_غیر_تعریف_شدہ HAVE_STRUCT_STAT_ST_بلاکیں
# define HAVE_STRUCT_STAT_ST_بلاکیں 0
#اگر_ختم

/* Type of scan being done on the input when looking for sparseness.  */
شمار scantype
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
ساکن شمار scantype
infer_scantype (صحیح fd, ساخت stat ثابت *sb, off_t pos,
                ساخت جائزہ_inference *جائزہ_inference)
{
  /* Try SEEK_HOLE only if this heuristic
     suggests the file is sparse.  */
  اگر (! (HAVE_STRUCT_STAT_ST_بلاکیں
         && S_ISREG (sb->st_منوال)
         && STP_NBLOCKS (sb) < sb->st_سائز / ST_NBLOCKSIZE))
    واپس PLAIN_SCANTYPE;

#اگر_تعریف_شدہ SEEK_HOLE
  جائزہ_inference->ext_آغاز = lseek (fd, pos, SEEK_ڈیٹا);
  اگر (جائزہ_inference->ext_آغاز == pos)
    {
      جائزہ_inference->hole_آغاز = lseek (fd, pos, SEEK_HOLE);
      اگر (0 <= جائزہ_inference->hole_آغاز)
        {
          اگر (جائزہ_inference->hole_آغاز < sb->st_سائز)
            واپس LSEEK_SCANTYPE;

          /* Though the file may have holes, SEEK_DATA and SEEK_HOLE
             didn't find any.  This can happen with file systems
             that support SEEK_HOLE only trivially,
             such as squashfs in Linux kernel 6.17 and earlier.
             This can also happen due to transparent file compression,
             which can also indicate fewer than the usual number of blocks.  */

          اگر (lseek (fd, pos, SEEK_SET) < 0)
            واپس ERROR_SCANTYPE;

          /* we prefer to return PLAIN_SCANTYPE here so that copy offload
             continues to be used.  Falling through to ZERO_SCANTYPE would be
             less efficient in the compressed file case.  */
          واپس PLAIN_SCANTYPE;
        }
    }
  ورنہ اگر (pos < جائزہ_inference->ext_آغاز || errno == ENXIO)
    {
      جائزہ_inference->hole_آغاز = pos;  /* Pacify -Wmaybe-uninitialized.  */
      واپس LSEEK_SCANTYPE;
    }
  ورنہ اگر (errno != EINVAL && !is_ENOTSUP (errno))
    واپس ERROR_SCANTYPE;
#اگر_ختم

  واپس ZERO_SCANTYPE;
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

خارجی intmax_t
copy_file_ڈیٹا (صحیح ifd, ساخت stat ثابت *ist, off_t ipos, حرف ثابت *iname,
                صحیح ofd, ساخت stat ثابت *ost, off_t opos, حرف ثابت *oname,
                گنتی_t ibytes, ساخت cp_options ثابت *x,
                ساخت copy_debug *debug)
{
  /* Choose a suitable buffer size; it may be adjusted later.  */
  idx_t buf_سائز = io_blksize (ost);

  /* Deal with sparse files.  */
  ساخت جائزہ_inference جائزہ_inference;
  شمار scantype scantype = infer_scantype (ifd, ist, ipos, &جائزہ_inference);
  اگر (scantype == ERROR_SCANTYPE)
    {
      error (0, errno, _("cannot lseek %s"), quoteaf (iname));
      واپس -1;
    }
  bool make_holes
    = (S_ISREG (ost->st_منوال)
       && (x->sparse_منوال == SPARSE_ALWAYS
           || (x->sparse_منوال == SPARSE_AUTO
               && scantype != PLAIN_SCANTYPE)));

  /* If we _know_ we're going to read data sequentially into the process,
     i.e., --reflink or --sparse are not in auto mode,
     give that hint to the kernel so it can tune caching behavior.
     Also we don't bother calling fadvise for small copies,
     as it is not likely to help performance and might even hurt it.
     Also we only apply this hint for the whole file (0 length)
     as OpenZFS 2.2.2 at least will otherwise synchronously
     (decompress and) populate the cache when given a specific length.  */
  اگر (ipos == 0 && ibytes == گنتی_MAX
      && (x->reflink_منوال != REFLINK_AUTO || x->sparse_منوال != SPARSE_AUTO))
    fdadvise (ifd, 0, 0, FADVISE_SEQUENTIAL);

  /* If not making a sparse file, try to use a more-efficient
     buffer size.  */
  اگر (! make_holes)
    {
      /* Compute the least common multiple of the input and output
         buffer sizes, adjusting for outlandish values.
         blcm is at most IDX_MAX - 1 so that buf_size cannot become 0 below.
         Note we read in multiples of the reported block size
         to support (unusual) devices that have this constraint.  */
      idx_t blcm_max = MIN (MIN (IDX_MAX - 1, SSIZE_MAX), SIZE_MAX);
      idx_t blcm = بفر_lcm (io_blksize (ist), buf_سائز,
                               blcm_max);

      /* Do not bother with a buffer larger than the input file, plus one
         byte to make sure the file has not grown while reading it.  */
      اگر (S_ISREG (ist->st_منوال) && 0 <= ist->st_سائز
          && ist->st_سائز < buf_سائز)
        buf_سائز = ist->st_سائز + 1;

      /* However, stick with a block size that is a positive multiple of
         blcm, overriding the above adjustments.  Watch out for
         overflow.  */
      buf_سائز = ckd_جوڑو (&buf_سائز, buf_سائز, blcm - 1) ? IDX_MAX : buf_سائز;
      buf_سائز -= buf_سائز % blcm;
    }

  حرف *buf = تہی;
  intmax_t نتیجہ;
  off_t hole_سائز = 0;

  اگر (scantype == LSEEK_SCANTYPE)
    {
#اگر_تعریف_شدہ SEEK_HOLE
      نتیجہ = lseek_copy (ifd, ofd, &buf, buf_سائز,
                           ipos, ibytes, &جائزہ_inference, ist->st_سائز,
                           make_holes ? x->sparse_منوال : SPARSE_NEVER,
                           x->reflink_منوال != REFLINK_NEVER,
                           iname, oname, &hole_سائز, debug);
#ورنہ
      affirm (false);
#اگر_ختم
    }
  ورنہ
    نتیجہ = sparse_copy (ifd, ofd, &buf, buf_سائز,
                          x->reflink_منوال != REFLINK_NEVER,
                          iname, oname, ibytes,
                          make_holes ? &hole_سائز : تہی,
                          debug);

  اگر (0 <= نتیجہ && 0 < hole_سائز)
    {
      off_t oend;
      اگر (ckd_جوڑو (&oend, opos, نتیجہ)
          ? (errno = EOVERFLOW, true)
          : make_holes
          ? ftruncate (ofd, oend) < 0
          : !write_zeros (ofd, hole_سائز, &buf, buf_سائز))
        {
          error (0, errno, _("failed to extend %s"), quoteaf (oname));
          نتیجہ = -1;
        }
      ورنہ اگر (make_holes
               && punch_hole (ofd, oend - hole_سائز, hole_سائز) < 0)
        {
          error (0, errno, _("error deallocating %s"), quoteaf (oname));
          نتیجہ = -1;
        }
    }

  alignfree (buf);
  واپس نتیجہ;
}
