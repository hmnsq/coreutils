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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব "system.h"

#অন্তর্ভাব "alignalloc.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "backupfile.h"
#অন্তর্ভাব "buffer-lcm.h"
#অন্তর্ভাব "copy.h"
#অন্তর্ভাব "fadvise.h"
#অন্তর্ভাব "full-write.h"
#অন্তর্ভাব "ioblksize.h"

/* Result of infer_scantype when it returns LSEEK_SCANTYPE.  */
সংরচনা scan_inference
{
  /* The offset of the first data block, or -1 if the file has no data.  */
  off_t ext_start;

  /* The offset of the first hole.  */
  off_t hole_start;
};

/* Attempt to punch a hole to avoid any permanent
   speculative preallocation on file systems such as XFS.
   Return values as per fallocate(2) except ENOSYS etc. are ignored.  */
স্থির পূর্ণাঙ্ক
punch_hole (পূর্ণাঙ্ক fd, off_t offset, off_t length)
{
  পূর্ণাঙ্ক ret = 0;
/* +0 is to work around older <linux/fs.h> defining HAVE_FALLOCATE to empty.  */
#যদি HAVE_FALLOCATE + 0
# যদি defined FALLOC_FL_PUNCH_HOLE && defined FALLOC_FL_KEEP_SIZE
  ret = fallocate (fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
                   offset, length);
  যদি (ret < 0 && (is_ENOTSUP (errno) || errno == ENOSYS))
    ret = 0;
# endif
#endif
  প্রত্যাগম ret;
}

/* Create a hole at the end of the file with descriptor FD and name NAME.
   The hole is of size SIZE.  Assume FD is already at file end,
   and advance FD past the newly-created hole.
   Return the resulting position, or -1 on failure.  */
স্থির off_t
create_hole (পূর্ণাঙ্ক fd, বর্ণ ধ্রুব *name, off_t size)
{
  off_t file_end = lseek (fd, size, SEEK_CUR);

  যদি (file_end < 0)
    {
      error (0, errno, _("cannot lseek %s"), quoteaf (name));
      প্রত্যাগম -1;
    }

  /* Some file systems (like XFS) preallocate when write extending a file.
     I.e., a previous write() may have preallocated extra space
     that the seek above will not discard.  A subsequent write() could
     then make this allocation permanent.  */
  যদি (punch_hole (fd, file_end - size, size) < 0)
    {
      error (0, errno, _("error deallocating %s"), quoteaf (name));
      প্রত্যাগম -1;
    }

  প্রত্যাগম file_end;
}

/* Does ERR mean the copying operation is not supported or allowed for
   this file or process, even though the operation was invoked correctly?  */
স্থির bool
is_CLONENOTSUP (পূর্ণাঙ্ক err)
{
  প্রত্যাগম err == ENOSYS || err == ENOTTY || is_ENOTSUP (err)
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
স্থির intmax_t
sparse_copy (পূর্ণাঙ্ক src_fd, পূর্ণাঙ্ক dest_fd, বর্ণ **abuf, idx_t buf_size,
             bool allow_reflink,
             বর্ণ ধ্রুব *src_name, বর্ণ ধ্রুব *dst_name,
             গুনতি_t max_n_read, off_t *hole_size,
             সংরচনা copy_debug *debug)
{
  গুনতি_t total_n_read = 0;

  যদি (debug->sparse_detection == COPY_DEBUG_UNKNOWN)
    debug->sparse_detection = hole_size ? COPY_DEBUG_YES : COPY_DEBUG_NO;
  অন্যথা যদি (hole_size && debug->sparse_detection == COPY_DEBUG_EXTERNAL)
    debug->sparse_detection = COPY_DEBUG_EXTERNAL_INTERNAL;

  /* If not looking for holes, use copy_file_range if functional,
     but don't use if reflink disallowed as that may be implicit.  */
  যদি (!hole_size && allow_reflink)
    যাবত্ (0 < max_n_read)
      {
        /* Copy at most COPY_MAX bytes at a time; this is min
           (SSIZE_MAX, SIZE_MAX) truncated to a value that is
           surely aligned well.  */
        ssize_t copy_max = MIN (SSIZE_MAX, SIZE_MAX) >> 30 << 30;
        ssize_t n_copied = copy_file_range (src_fd, রিক্ত, dest_fd, রিক্ত,
                                            MIN (max_n_read, copy_max), 0);
        যদি (n_copied == 0)
          {
            /* copy_file_range incorrectly returns 0 when reading from
               the proc file system on the Linux kernel through at
               least 5.6.19 (2020), so fall back on 'read' if the
               input file seems empty.  */
            যদি (total_n_read == 0)
              ভঙ্গ;
            debug->offload = COPY_DEBUG_YES;
            প্রত্যাগম total_n_read;
          }
        যদি (n_copied < 0)
          {
            debug->offload = errno;

            /* Don’t treat EFBIG as a reportable error from copy_file_range.
               If the input is at EOF and the output position is 2**63 - 1,
               copy_file_range (ifd, NULL, ofd, NULL, 2146435072, 0)
               incorrectly fails with EFBIG.  Problem observed on Ubuntu 25.10
               x86-64 with Linux kernel 6.17.0-8-generic #8-Ubuntu.  */
            যদি (errno == EFBIG)
              ভঙ্গ;

            /* Consider operation unsupported only if no data copied.
               For example, EPERM could occur if copy_file_range not enabled
               in seccomp filters, so retry with a standard copy.  EPERM can
               also occur for immutable files, but that would only be in the
               edge case where the file is made immutable after creating,
               in which case the (more accurate) error is still shown.  */
            যদি (total_n_read == 0 && is_CLONENOTSUP (errno))
              ভঙ্গ;

            /* ENOENT was seen sometimes across CIFS shares, resulting in
               no data being copied, but subsequent standard copies succeed.  */
            যদি (total_n_read == 0 && errno == ENOENT)
              ভঙ্গ;

            যদি (errno == EINTR)
              n_copied = 0;
            অন্যথা
              {
                error (0, errno, _("error copying %s to %s"),
                       quoteaf_n (0, src_name), quoteaf_n (1, dst_name));
                প্রত্যাগম -1;
              }
          }
        debug->offload = COPY_DEBUG_YES;
        max_n_read -= n_copied;
        total_n_read += n_copied;
      }
  অন্যথা
    debug->offload = COPY_DEBUG_AVOIDED;


  off_t psize = hole_size ? *hole_size : 0;
  bool make_hole = !!psize;

  যাবত্ (0 < max_n_read)
    {
      যদি (!*abuf)
        *abuf = xalignalloc (getpagesize (), buf_size);
      বর্ণ *buf = *abuf;
      ssize_t n_read = পঠ (src_fd, buf, MIN (max_n_read, buf_size));
      যদি (n_read < 0)
        {
          যদি (errno == EINTR)
            অনুবর্ত;
          error (0, errno, _("error reading %s"), quoteaf (src_name));
          প্রত্যাগম -1;
        }
      যদি (n_read == 0)
        ভঙ্গ;
      max_n_read -= n_read;
      total_n_read += n_read;

      /* If looking for holes, loop over the input buffer in chunks
         corresponding to the minimum hole size.  Otherwise, scan the
         whole buffer.  struct stat does not report the minimum hole
         size for a file, so use ST_NBLOCKSIZE which should be the
         minimum for all file systems on this platform.  */
      idx_t csize = hole_size ? ST_NBLOCKSIZE : buf_size;
      বর্ণ *cbuf = buf;
      বর্ণ *pbuf = buf;

      যাবত্ (n_read)
        {
          bool prev_hole = make_hole;
          csize = MIN (csize, n_read);

          যদি (hole_size)
            make_hole = is_nul (cbuf, csize);

          bool transition = (make_hole != prev_hole) && psize;
          bool last_chunk = n_read == csize && !make_hole;

          যদি (transition || last_chunk)
            {
              যদি (! transition)
                psize += csize;
              অন্যথা যদি (prev_hole)
                {
                  যদি (create_hole (dest_fd, dst_name, psize) < 0)
                    প্রত্যাগম false;
                  pbuf = cbuf;
                  psize = csize;
                }

              যদি (!prev_hole || (transition && last_chunk))
                {
                  যদি (full_write (dest_fd, pbuf, psize) != psize)
                    {
                      error (0, errno, _("error writing %s"),
                             quoteaf (dst_name));
                      প্রত্যাগম -1;
                    }
                  psize = !prev_hole && transition ? csize : 0;
                }
            }
          অন্যথা  /* Coalesce writes/seeks.  */
            {
              যদি (ckd_add (&psize, psize, csize))
                {
                  error (0, 0, _("overflow reading %s"), quoteaf (src_name));
                  প্রত্যাগম -1;
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

  যদি (hole_size)
    *hole_size = make_hole ? psize : 0;
  প্রত্যাগম total_n_read;
}

/* Write to FD.  Write data consisting of N_BYTES' worth of '\0's.
   Use the buffer *ABUF of size BUF_SIZE for temporary storage,
   allocating the buffer lazily if *ABUF is null.
   Return true if successful, false (setting errno) otherwise.  */
স্থির bool
write_zeros (পূর্ণাঙ্ক fd, off_t n_bytes, বর্ণ **abuf, idx_t buf_size)
{
  বর্ণ *zeros = রিক্ত;
  যাবত্ (n_bytes)
    {
      idx_t n = MIN (buf_size, n_bytes);
      যদি (!zeros)
        {
          যদি (!*abuf)
            *abuf = xalignalloc (getpagesize (), buf_size);
          zeros = memset (*abuf, 0, n);
        }
      যদি (full_write (fd, zeros, n) != n)
        প্রত্যাগম false;
      n_bytes -= n;
    }

  প্রত্যাগম true;
}

#ifdef SEEK_HOLE
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

স্থির off_t
lseek_copy (পূর্ণাঙ্ক src_fd, পূর্ণাঙ্ক dest_fd, বর্ণ **abuf, idx_t buf_size,
            off_t src_pos, গুনতি_t ibytes,
            সংরচনা scan_inference ধ্রুব *scan_inference, off_t src_total_size,
            গণনা Sparse_type sparse_mode,
            bool allow_reflink,
            বর্ণ ধ্রুব *src_name, বর্ণ ধ্রুব *dst_name,
            off_t *hole_size, সংরচনা copy_debug *debug)
{
  off_t last_ext_start = src_pos;
  off_t last_ext_len = 0;
  off_t max_ipos = ckd_add (&max_ipos, src_pos, ibytes) ? OFF_T_MAX : max_ipos;

  /* From here on, SRC_TOTAL_SIZE is bounded above by MAX_IPOS.  */
  src_total_size = MIN (src_total_size, max_ipos);

  /* The input position after the most recent read of data,
     or SRC_POS if no data have been read.  */
  off_t ipos = src_pos;

  debug->sparse_detection = COPY_DEBUG_EXTERNAL;

  bool used_scan_inference = false;

  প্রতি (off_t ext_start = scan_inference->ext_start;
       0 <= ext_start && ext_start < max_ipos; )
    {
      off_t ext_end;
      যদি (ext_start == src_pos && ! used_scan_inference)
        {
          ext_end = scan_inference->hole_start;
          used_scan_inference = true;
        }
      অন্যথা
        ext_end = lseek (src_fd, ext_start, SEEK_HOLE);
      যদি (0 <= ext_end)
        ext_end = MIN (ext_end, max_ipos);
      অন্যথা
        {
          যদি (errno != ENXIO)
            গচ্ছ cannot_lseek;
          ext_end = src_total_size;
          যদি (ext_end <= ext_start)
            {
              /* The input file grew; get its current size.  */
              src_total_size = lseek (src_fd, 0, SEEK_END);
              যদি (src_total_size < 0)
                গচ্ছ cannot_lseek;
              src_total_size = MIN (src_total_size, max_ipos);

              /* If the input file shrank after growing, stop copying.  */
              যদি (src_total_size <= ext_start)
                ভঙ্গ;

              ext_end = src_total_size;
            }
        }
      /* If the input file must have grown, increase its measured size.  */
      যদি (src_total_size < ext_end)
        src_total_size = ext_end;

      যদি (lseek (src_fd, ext_start, SEEK_SET) < 0)
        গচ্ছ cannot_lseek;

      off_t ext_hole_size = ext_start - last_ext_start - last_ext_len;

      যদি (ext_hole_size)
        {
          যদি (sparse_mode == SPARSE_ALWAYS)
            *hole_size += ext_hole_size;
          অন্যথা যদি (sparse_mode != SPARSE_NEVER)
            {
              off_t epos = create_hole (dest_fd, dst_name, ext_hole_size);
              যদি (epos < 0)
                প্রত্যাগম epos;
            }
          অন্যথা
            {
              /* When not inducing holes and when there is a hole between
                 the end of the previous extent and the beginning of the
                 current one, write zeros to the destination file.  */
              যদি (! write_zeros (dest_fd, ext_hole_size, abuf, buf_size))
                {
                  error (0, errno, _("%s: write failed"),
                         quotef (dst_name));
                  প্রত্যাগম -1;
                }
            }
        }

      off_t ext_len = ext_end - ext_start;
      last_ext_start = ext_start;
      last_ext_len = ext_len;

      /* Copy this extent, looking for further opportunities to not
         bother to write zeros if --sparse=always, since SEEK_HOLE
         is conservative and may miss some holes.  */
      off_t n_read
        = sparse_copy (src_fd, dest_fd, abuf, buf_size,
                       allow_reflink, src_name, dst_name,
                       ext_len,
                       sparse_mode == SPARSE_ALWAYS ? hole_size : রিক্ত,
                       debug);
      যদি (n_read < 0)
        প্রত্যাগম -1;

      ipos = ext_start + n_read;
      যদি (n_read < ext_len)
        {
          /* The input file shrank.  */
          src_total_size = ipos;
          ভঙ্গ;
        }

      ext_start = lseek (src_fd, ipos, SEEK_DATA);
      যদি (ext_start < 0 && errno != ENXIO)
        গচ্ছ cannot_lseek;
    }

  *hole_size += src_total_size - (last_ext_start + last_ext_len);
  প্রত্যাগম src_total_size - src_pos;

 cannot_lseek:
  error (0, errno, _("cannot lseek %s"), quoteaf (src_name));
  প্রত্যাগম -1;
}
#endif

#ifndef HAVE_STRUCT_STAT_ST_BLOCKS
# define HAVE_STRUCT_STAT_ST_BLOCKS 0
#endif

/* Type of scan being done on the input when looking for sparseness.  */
গণনা scantype
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
স্থির গণনা scantype
infer_scantype (পূর্ণাঙ্ক fd, সংরচনা stat ধ্রুব *sb, off_t pos,
                সংরচনা scan_inference *scan_inference)
{
  /* Try SEEK_HOLE only if this heuristic
     suggests the file is sparse.  */
  যদি (! (HAVE_STRUCT_STAT_ST_BLOCKS
         && S_ISREG (sb->st_mode)
         && STP_NBLOCKS (sb) < sb->st_size / ST_NBLOCKSIZE))
    প্রত্যাগম PLAIN_SCANTYPE;

#ifdef SEEK_HOLE
  scan_inference->ext_start = lseek (fd, pos, SEEK_DATA);
  যদি (scan_inference->ext_start == pos)
    {
      scan_inference->hole_start = lseek (fd, pos, SEEK_HOLE);
      যদি (0 <= scan_inference->hole_start)
        {
          যদি (scan_inference->hole_start < sb->st_size)
            প্রত্যাগম LSEEK_SCANTYPE;

          /* Though the file may have holes, SEEK_DATA and SEEK_HOLE
             didn't find any.  This can happen with file systems
             that support SEEK_HOLE only trivially,
             such as squashfs in Linux kernel 6.17 and earlier.
             This can also happen due to transparent file compression,
             which can also indicate fewer than the usual number of blocks.  */

          যদি (lseek (fd, pos, SEEK_SET) < 0)
            প্রত্যাগম ERROR_SCANTYPE;

          /* we prefer to return PLAIN_SCANTYPE here so that copy offload
             continues to be used.  Falling through to ZERO_SCANTYPE would be
             less efficient in the compressed file case.  */
          প্রত্যাগম PLAIN_SCANTYPE;
        }
    }
  অন্যথা যদি (pos < scan_inference->ext_start || errno == ENXIO)
    {
      scan_inference->hole_start = pos;  /* Pacify -Wmaybe-uninitialized.  */
      প্রত্যাগম LSEEK_SCANTYPE;
    }
  অন্যথা যদি (errno != EINVAL && !is_ENOTSUP (errno))
    প্রত্যাগম ERROR_SCANTYPE;
#endif

  প্রত্যাগম ZERO_SCANTYPE;
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

বাহ্য intmax_t
copy_file_data (পূর্ণাঙ্ক ifd, সংরচনা stat ধ্রুব *ist, off_t ipos, বর্ণ ধ্রুব *iname,
                পূর্ণাঙ্ক ofd, সংরচনা stat ধ্রুব *ost, off_t opos, বর্ণ ধ্রুব *oname,
                গুনতি_t ibytes, সংরচনা cp_options ধ্রুব *x,
                সংরচনা copy_debug *debug)
{
  /* Choose a suitable buffer size; it may be adjusted later.  */
  idx_t buf_size = io_blksize (ost);

  /* Deal with sparse files.  */
  সংরচনা scan_inference scan_inference;
  গণনা scantype scantype = infer_scantype (ifd, ist, ipos, &scan_inference);
  যদি (scantype == ERROR_SCANTYPE)
    {
      error (0, errno, _("cannot lseek %s"), quoteaf (iname));
      প্রত্যাগম -1;
    }
  bool make_holes
    = (S_ISREG (ost->st_mode)
       && (x->sparse_mode == SPARSE_ALWAYS
           || (x->sparse_mode == SPARSE_AUTO
               && scantype != PLAIN_SCANTYPE)));

  /* If we _know_ we're going to read data sequentially into the process,
     i.e., --reflink or --sparse are not in auto mode,
     give that hint to the kernel so it can tune caching behavior.
     Also we don't bother calling fadvise for small copies,
     as it is not likely to help performance and might even hurt it.
     Also we only apply this hint for the whole file (0 length)
     as OpenZFS 2.2.2 at least will otherwise synchronously
     (decompress and) populate the cache when given a specific length.  */
  যদি (ipos == 0 && ibytes == গুনতি_MAX
      && (x->reflink_mode != REFLINK_AUTO || x->sparse_mode != SPARSE_AUTO))
    fdadvise (ifd, 0, 0, FADVISE_SEQUENTIAL);

  /* If not making a sparse file, try to use a more-efficient
     buffer size.  */
  যদি (! make_holes)
    {
      /* Compute the least common multiple of the input and output
         buffer sizes, adjusting for outlandish values.
         blcm is at most IDX_MAX - 1 so that buf_size cannot become 0 below.
         Note we read in multiples of the reported block size
         to support (unusual) devices that have this constraint.  */
      idx_t blcm_max = MIN (MIN (IDX_MAX - 1, SSIZE_MAX), SIZE_MAX);
      idx_t blcm = buffer_lcm (io_blksize (ist), buf_size,
                               blcm_max);

      /* Do not bother with a buffer larger than the input file, plus one
         byte to make sure the file has not grown while reading it.  */
      যদি (S_ISREG (ist->st_mode) && 0 <= ist->st_size
          && ist->st_size < buf_size)
        buf_size = ist->st_size + 1;

      /* However, stick with a block size that is a positive multiple of
         blcm, overriding the above adjustments.  Watch out for
         overflow.  */
      buf_size = ckd_add (&buf_size, buf_size, blcm - 1) ? IDX_MAX : buf_size;
      buf_size -= buf_size % blcm;
    }

  বর্ণ *buf = রিক্ত;
  intmax_t result;
  off_t hole_size = 0;

  যদি (scantype == LSEEK_SCANTYPE)
    {
#ifdef SEEK_HOLE
      result = lseek_copy (ifd, ofd, &buf, buf_size,
                           ipos, ibytes, &scan_inference, ist->st_size,
                           make_holes ? x->sparse_mode : SPARSE_NEVER,
                           x->reflink_mode != REFLINK_NEVER,
                           iname, oname, &hole_size, debug);
#অন্যথা
      affirm (false);
#endif
    }
  অন্যথা
    result = sparse_copy (ifd, ofd, &buf, buf_size,
                          x->reflink_mode != REFLINK_NEVER,
                          iname, oname, ibytes,
                          make_holes ? &hole_size : রিক্ত,
                          debug);

  যদি (0 <= result && 0 < hole_size)
    {
      off_t oend;
      যদি (ckd_add (&oend, opos, result)
          ? (errno = EOVERFLOW, true)
          : make_holes
          ? ftruncate (ofd, oend) < 0
          : !write_zeros (ofd, hole_size, &buf, buf_size))
        {
          error (0, errno, _("failed to extend %s"), quoteaf (oname));
          result = -1;
        }
      অন্যথা যদি (make_holes
               && punch_hole (ofd, oend - hole_size, hole_size) < 0)
        {
          error (0, errno, _("error deallocating %s"), quoteaf (oname));
          result = -1;
        }
    }

  alignfree (buf);
  প্রত্যাগম result;
}
