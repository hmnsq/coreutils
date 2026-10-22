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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "alignalloc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "backupfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "buffer-lcm.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "copy.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "full-write.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ioblksize.h"

/* Result of infer_scantype when it returns LSEEK_SCANTYPE.  */
ਬਣਤਰ ਜਾਂਚ_inference
{
  /* The offset of the first data block, or -1 if the file has no data.  */
  off_t ext_ਸ਼ੁਰੂ;

  /* The offset of the first hole.  */
  off_t hole_ਸ਼ੁਰੂ;
};

/* Attempt to punch a hole to avoid any permanent
   speculative preallocation on file systems such as XFS.
   Return values as per fallocate(2) except ENOSYS etc. are ignored.  */
ਸਥਿਰ ਪੂਰਨ
punch_hole (ਪੂਰਨ fd, off_t ਵਿੱਥ, off_t ਲੰਮਾਈ)
{
  ਪੂਰਨ ret = 0;
/* +0 is to work around older <linux/fs.h> defining HAVE_FALLOCATE to empty.  */
#ਜੇ HAVE_FALLOCATE + 0
# ਜੇ defined FALLOC_FL_PUNCH_HOLE && defined FALLOC_FL_KEEP_ਮਾਪ
  ret = fallocate (fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_ਮਾਪ,
                   ਵਿੱਥ, ਲੰਮਾਈ);
  ਜੇ (ret < 0 && (is_ENOTSUP (errno) || errno == ENOSYS))
    ret = 0;
# endif
#ਜੇ_ਅੰਤ
  ਵਾਪਸ ret;
}

/* Create a hole at the end of the file with descriptor FD and name NAME.
   The hole is of size SIZE.  Assume FD is already at file end,
   and advance FD past the newly-created hole.
   Return the resulting position, or -1 on failure.  */
ਸਥਿਰ off_t
create_hole (ਪੂਰਨ fd, ਅੱਖਰ ਅਚਲ *name, off_t ਮਾਪ)
{
  off_t file_ਸਿਰਾ = lseek (fd, ਮਾਪ, SEEK_CUR);

  ਜੇ (file_ਸਿਰਾ < 0)
    {
      error (0, errno, _("cannot lseek %s"), quoteaf (name));
      ਵਾਪਸ -1;
    }

  /* Some file systems (like XFS) preallocate when write extending a file.
     I.e., a previous write() may have preallocated extra space
     that the seek above will not discard.  A subsequent write() could
     then make this allocation permanent.  */
  ਜੇ (punch_hole (fd, file_ਸਿਰਾ - ਮਾਪ, ਮਾਪ) < 0)
    {
      error (0, errno, _("error deallocating %s"), quoteaf (name));
      ਵਾਪਸ -1;
    }

  ਵਾਪਸ file_ਸਿਰਾ;
}

/* Does ERR mean the copying operation is not supported or allowed for
   this file or process, even though the operation was invoked correctly?  */
ਸਥਿਰ bool
is_CLONENOTSUP (ਪੂਰਨ err)
{
  ਵਾਪਸ err == ENOSYS || err == ENOTTY || is_ENOTSUP (err)
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
ਸਥਿਰ intmax_t
sparse_copy (ਪੂਰਨ src_fd, ਪੂਰਨ dest_fd, ਅੱਖਰ **abuf, idx_t buf_ਮਾਪ,
             bool allow_reflink,
             ਅੱਖਰ ਅਚਲ *src_name, ਅੱਖਰ ਅਚਲ *dst_name,
             ਗਿਣਤੀ_t max_n_read, off_t *hole_ਮਾਪ,
             ਬਣਤਰ copy_debug *debug)
{
  ਗਿਣਤੀ_t ਕੁੱਲ_n_read = 0;

  ਜੇ (debug->sparse_detection == COPY_DEBUG_UNKNOWN)
    debug->sparse_detection = hole_ਮਾਪ ? COPY_DEBUG_YES : COPY_DEBUG_NO;
  ਨਹੀਂਤਾਂ ਜੇ (hole_ਮਾਪ && debug->sparse_detection == COPY_DEBUG_EXTERNAL)
    debug->sparse_detection = COPY_DEBUG_EXTERNAL_ਅੰਦਰੂਨੀ;

  /* If not looking for holes, use copy_file_range if functional,
     but don't use if reflink disallowed as that may be implicit.  */
  ਜੇ (!hole_ਮਾਪ && allow_reflink)
    ਜਦੋਂ (0 < max_n_read)
      {
        /* Copy at most COPY_MAX bytes at a time; this is min
           (SSIZE_MAX, SIZE_MAX) truncated to a value that is
           surely aligned well.  */
        ssize_t copy_max = MIN (SSIZE_MAX, SIZE_MAX) >> 30 << 30;
        ssize_t n_copied = copy_file_range (src_fd, ਨਿਰਾ, dest_fd, ਨਿਰਾ,
                                            MIN (max_n_read, copy_max), 0);
        ਜੇ (n_copied == 0)
          {
            /* copy_file_range incorrectly returns 0 when reading from
               the proc file system on the Linux kernel through at
               least 5.6.19 (2020), so fall back on 'read' if the
               input file seems empty.  */
            ਜੇ (ਕੁੱਲ_n_read == 0)
              ਰੋਕੋ;
            debug->offload = COPY_DEBUG_YES;
            ਵਾਪਸ ਕੁੱਲ_n_read;
          }
        ਜੇ (n_copied < 0)
          {
            debug->offload = errno;

            /* Don’t treat EFBIG as a reportable error from copy_file_range.
               If the input is at EOF and the output position is 2**63 - 1,
               copy_file_range (ifd, NULL, ofd, NULL, 2146435072, 0)
               incorrectly fails with EFBIG.  Problem observed on Ubuntu 25.10
               x86-64 with Linux kernel 6.17.0-8-generic #8-Ubuntu.  */
            ਜੇ (errno == EFBIG)
              ਰੋਕੋ;

            /* Consider operation unsupported only if no data copied.
               For example, EPERM could occur if copy_file_range not enabled
               in seccomp filters, so retry with a standard copy.  EPERM can
               also occur for immutable files, but that would only be in the
               edge case where the file is made immutable after creating,
               in which case the (more accurate) error is still shown.  */
            ਜੇ (ਕੁੱਲ_n_read == 0 && is_CLONENOTSUP (errno))
              ਰੋਕੋ;

            /* ENOENT was seen sometimes across CIFS shares, resulting in
               no data being copied, but subsequent standard copies succeed.  */
            ਜੇ (ਕੁੱਲ_n_read == 0 && errno == ENOENT)
              ਰੋਕੋ;

            ਜੇ (errno == EINTR)
              n_copied = 0;
            ਨਹੀਂਤਾਂ
              {
                error (0, errno, _("error copying %s to %s"),
                       quoteaf_n (0, src_name), quoteaf_n (1, dst_name));
                ਵਾਪਸ -1;
              }
          }
        debug->offload = COPY_DEBUG_YES;
        max_n_read -= n_copied;
        ਕੁੱਲ_n_read += n_copied;
      }
  ਨਹੀਂਤਾਂ
    debug->offload = COPY_DEBUG_AVOIDED;


  off_t psize = hole_ਮਾਪ ? *hole_ਮਾਪ : 0;
  bool make_hole = !!psize;

  ਜਦੋਂ (0 < max_n_read)
    {
      ਜੇ (!*abuf)
        *abuf = xalignalloc (getpagesize (), buf_ਮਾਪ);
      ਅੱਖਰ *buf = *abuf;
      ssize_t n_read = ਪਠ (src_fd, buf, MIN (max_n_read, buf_ਮਾਪ));
      ਜੇ (n_read < 0)
        {
          ਜੇ (errno == EINTR)
            ਜਾਰੀ;
          error (0, errno, _("error reading %s"), quoteaf (src_name));
          ਵਾਪਸ -1;
        }
      ਜੇ (n_read == 0)
        ਰੋਕੋ;
      max_n_read -= n_read;
      ਕੁੱਲ_n_read += n_read;

      /* If looking for holes, loop over the input buffer in chunks
         corresponding to the minimum hole size.  Otherwise, scan the
         whole buffer.  struct stat does not report the minimum hole
         size for a file, so use ST_NBLOCKSIZE which should be the
         minimum for all file systems on this platform.  */
      idx_t csize = hole_ਮਾਪ ? ST_NBLOCKSIZE : buf_ਮਾਪ;
      ਅੱਖਰ *cbuf = buf;
      ਅੱਖਰ *pbuf = buf;

      ਜਦੋਂ (n_read)
        {
          bool prev_hole = make_hole;
          csize = MIN (csize, n_read);

          ਜੇ (hole_ਮਾਪ)
            make_hole = is_nul (cbuf, csize);

          bool transition = (make_hole != prev_hole) && psize;
          bool ਆਖਰੀ_chunk = n_read == csize && !make_hole;

          ਜੇ (transition || ਆਖਰੀ_chunk)
            {
              ਜੇ (! transition)
                psize += csize;
              ਨਹੀਂਤਾਂ ਜੇ (prev_hole)
                {
                  ਜੇ (create_hole (dest_fd, dst_name, psize) < 0)
                    ਵਾਪਸ false;
                  pbuf = cbuf;
                  psize = csize;
                }

              ਜੇ (!prev_hole || (transition && ਆਖਰੀ_chunk))
                {
                  ਜੇ (ਭਰਿਆ_write (dest_fd, pbuf, psize) != psize)
                    {
                      error (0, errno, _("error writing %s"),
                             quoteaf (dst_name));
                      ਵਾਪਸ -1;
                    }
                  psize = !prev_hole && transition ? csize : 0;
                }
            }
          ਨਹੀਂਤਾਂ  /* Coalesce writes/seeks.  */
            {
              ਜੇ (ckd_ਜੋੜੋ (&psize, psize, csize))
                {
                  error (0, 0, _("overflow reading %s"), quoteaf (src_name));
                  ਵਾਪਸ -1;
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

  ਜੇ (hole_ਮਾਪ)
    *hole_ਮਾਪ = make_hole ? psize : 0;
  ਵਾਪਸ ਕੁੱਲ_n_read;
}

/* Write to FD.  Write data consisting of N_BYTES' worth of '\0's.
   Use the buffer *ABUF of size BUF_SIZE for temporary storage,
   allocating the buffer lazily if *ABUF is null.
   Return true if successful, false (setting errno) otherwise.  */
ਸਥਿਰ bool
write_zeros (ਪੂਰਨ fd, off_t n_ਬਾਈਟਾਂ, ਅੱਖਰ **abuf, idx_t buf_ਮਾਪ)
{
  ਅੱਖਰ *zeros = ਨਿਰਾ;
  ਜਦੋਂ (n_ਬਾਈਟਾਂ)
    {
      idx_t n = MIN (buf_ਮਾਪ, n_ਬਾਈਟਾਂ);
      ਜੇ (!zeros)
        {
          ਜੇ (!*abuf)
            *abuf = xalignalloc (getpagesize (), buf_ਮਾਪ);
          zeros = memset (*abuf, 0, n);
        }
      ਜੇ (ਭਰਿਆ_write (fd, zeros, n) != n)
        ਵਾਪਸ false;
      n_ਬਾਈਟਾਂ -= n;
    }

  ਵਾਪਸ true;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SEEK_HOLE
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

ਸਥਿਰ off_t
lseek_copy (ਪੂਰਨ src_fd, ਪੂਰਨ dest_fd, ਅੱਖਰ **abuf, idx_t buf_ਮਾਪ,
            off_t src_pos, ਗਿਣਤੀ_t ibytes,
            ਬਣਤਰ ਜਾਂਚ_inference ਅਚਲ *ਜਾਂਚ_inference, off_t src_ਕੁੱਲ_ਮਾਪ,
            ਗਿਣਤੀ Sparse_type sparse_ਬਹੁਲਕ,
            bool allow_reflink,
            ਅੱਖਰ ਅਚਲ *src_name, ਅੱਖਰ ਅਚਲ *dst_name,
            off_t *hole_ਮਾਪ, ਬਣਤਰ copy_debug *debug)
{
  off_t ਆਖਰੀ_ext_ਸ਼ੁਰੂ = src_pos;
  off_t ਆਖਰੀ_ext_len = 0;
  off_t max_ipos = ckd_ਜੋੜੋ (&max_ipos, src_pos, ibytes) ? OFF_T_MAX : max_ipos;

  /* From here on, SRC_TOTAL_SIZE is bounded above by MAX_IPOS.  */
  src_ਕੁੱਲ_ਮਾਪ = MIN (src_ਕੁੱਲ_ਮਾਪ, max_ipos);

  /* The input position after the most recent read of data,
     or SRC_POS if no data have been read.  */
  off_t ipos = src_pos;

  debug->sparse_detection = COPY_DEBUG_EXTERNAL;

  bool ਵਰਤਿਆ_ਜਾਂਚ_inference = false;

  ਲਈ (off_t ext_ਸ਼ੁਰੂ = ਜਾਂਚ_inference->ext_ਸ਼ੁਰੂ;
       0 <= ext_ਸ਼ੁਰੂ && ext_ਸ਼ੁਰੂ < max_ipos; )
    {
      off_t ext_ਸਿਰਾ;
      ਜੇ (ext_ਸ਼ੁਰੂ == src_pos && ! ਵਰਤਿਆ_ਜਾਂਚ_inference)
        {
          ext_ਸਿਰਾ = ਜਾਂਚ_inference->hole_ਸ਼ੁਰੂ;
          ਵਰਤਿਆ_ਜਾਂਚ_inference = true;
        }
      ਨਹੀਂਤਾਂ
        ext_ਸਿਰਾ = lseek (src_fd, ext_ਸ਼ੁਰੂ, SEEK_HOLE);
      ਜੇ (0 <= ext_ਸਿਰਾ)
        ext_ਸਿਰਾ = MIN (ext_ਸਿਰਾ, max_ipos);
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (errno != ENXIO)
            ਜਾਓ cannot_lseek;
          ext_ਸਿਰਾ = src_ਕੁੱਲ_ਮਾਪ;
          ਜੇ (ext_ਸਿਰਾ <= ext_ਸ਼ੁਰੂ)
            {
              /* The input file grew; get its current size.  */
              src_ਕੁੱਲ_ਮਾਪ = lseek (src_fd, 0, SEEK_END);
              ਜੇ (src_ਕੁੱਲ_ਮਾਪ < 0)
                ਜਾਓ cannot_lseek;
              src_ਕੁੱਲ_ਮਾਪ = MIN (src_ਕੁੱਲ_ਮਾਪ, max_ipos);

              /* If the input file shrank after growing, stop copying.  */
              ਜੇ (src_ਕੁੱਲ_ਮਾਪ <= ext_ਸ਼ੁਰੂ)
                ਰੋਕੋ;

              ext_ਸਿਰਾ = src_ਕੁੱਲ_ਮਾਪ;
            }
        }
      /* If the input file must have grown, increase its measured size.  */
      ਜੇ (src_ਕੁੱਲ_ਮਾਪ < ext_ਸਿਰਾ)
        src_ਕੁੱਲ_ਮਾਪ = ext_ਸਿਰਾ;

      ਜੇ (lseek (src_fd, ext_ਸ਼ੁਰੂ, SEEK_SET) < 0)
        ਜਾਓ cannot_lseek;

      off_t ext_hole_ਮਾਪ = ext_ਸ਼ੁਰੂ - ਆਖਰੀ_ext_ਸ਼ੁਰੂ - ਆਖਰੀ_ext_len;

      ਜੇ (ext_hole_ਮਾਪ)
        {
          ਜੇ (sparse_ਬਹੁਲਕ == SPARSE_ALWAYS)
            *hole_ਮਾਪ += ext_hole_ਮਾਪ;
          ਨਹੀਂਤਾਂ ਜੇ (sparse_ਬਹੁਲਕ != SPARSE_NEVER)
            {
              off_t epos = create_hole (dest_fd, dst_name, ext_hole_ਮਾਪ);
              ਜੇ (epos < 0)
                ਵਾਪਸ epos;
            }
          ਨਹੀਂਤਾਂ
            {
              /* When not inducing holes and when there is a hole between
                 the end of the previous extent and the beginning of the
                 current one, write zeros to the destination file.  */
              ਜੇ (! write_zeros (dest_fd, ext_hole_ਮਾਪ, abuf, buf_ਮਾਪ))
                {
                  error (0, errno, _("%s: write failed"),
                         quotef (dst_name));
                  ਵਾਪਸ -1;
                }
            }
        }

      off_t ext_len = ext_ਸਿਰਾ - ext_ਸ਼ੁਰੂ;
      ਆਖਰੀ_ext_ਸ਼ੁਰੂ = ext_ਸ਼ੁਰੂ;
      ਆਖਰੀ_ext_len = ext_len;

      /* Copy this extent, looking for further opportunities to not
         bother to write zeros if --sparse=always, since SEEK_HOLE
         is conservative and may miss some holes.  */
      off_t n_read
        = sparse_copy (src_fd, dest_fd, abuf, buf_ਮਾਪ,
                       allow_reflink, src_name, dst_name,
                       ext_len,
                       sparse_ਬਹੁਲਕ == SPARSE_ALWAYS ? hole_ਮਾਪ : ਨਿਰਾ,
                       debug);
      ਜੇ (n_read < 0)
        ਵਾਪਸ -1;

      ipos = ext_ਸ਼ੁਰੂ + n_read;
      ਜੇ (n_read < ext_len)
        {
          /* The input file shrank.  */
          src_ਕੁੱਲ_ਮਾਪ = ipos;
          ਰੋਕੋ;
        }

      ext_ਸ਼ੁਰੂ = lseek (src_fd, ipos, SEEK_ਅੰਕੜੇ);
      ਜੇ (ext_ਸ਼ੁਰੂ < 0 && errno != ENXIO)
        ਜਾਓ cannot_lseek;
    }

  *hole_ਮਾਪ += src_ਕੁੱਲ_ਮਾਪ - (ਆਖਰੀ_ext_ਸ਼ੁਰੂ + ਆਖਰੀ_ext_len);
  ਵਾਪਸ src_ਕੁੱਲ_ਮਾਪ - src_pos;

 cannot_lseek:
  error (0, errno, _("cannot lseek %s"), quoteaf (src_name));
  ਵਾਪਸ -1;
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_STRUCT_STAT_ST_ਖੰਡਾਂ
# define HAVE_STRUCT_STAT_ST_ਖੰਡਾਂ 0
#ਜੇ_ਅੰਤ

/* Type of scan being done on the input when looking for sparseness.  */
ਗਿਣਤੀ scantype
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
ਸਥਿਰ ਗਿਣਤੀ scantype
infer_scantype (ਪੂਰਨ fd, ਬਣਤਰ stat ਅਚਲ *sb, off_t pos,
                ਬਣਤਰ ਜਾਂਚ_inference *ਜਾਂਚ_inference)
{
  /* Try SEEK_HOLE only if this heuristic
     suggests the file is sparse.  */
  ਜੇ (! (HAVE_STRUCT_STAT_ST_ਖੰਡਾਂ
         && S_ISREG (sb->st_ਬਹੁਲਕ)
         && STP_NBLOCKS (sb) < sb->st_ਮਾਪ / ST_NBLOCKSIZE))
    ਵਾਪਸ PLAIN_SCANTYPE;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SEEK_HOLE
  ਜਾਂਚ_inference->ext_ਸ਼ੁਰੂ = lseek (fd, pos, SEEK_ਅੰਕੜੇ);
  ਜੇ (ਜਾਂਚ_inference->ext_ਸ਼ੁਰੂ == pos)
    {
      ਜਾਂਚ_inference->hole_ਸ਼ੁਰੂ = lseek (fd, pos, SEEK_HOLE);
      ਜੇ (0 <= ਜਾਂਚ_inference->hole_ਸ਼ੁਰੂ)
        {
          ਜੇ (ਜਾਂਚ_inference->hole_ਸ਼ੁਰੂ < sb->st_ਮਾਪ)
            ਵਾਪਸ LSEEK_SCANTYPE;

          /* Though the file may have holes, SEEK_DATA and SEEK_HOLE
             didn't find any.  This can happen with file systems
             that support SEEK_HOLE only trivially,
             such as squashfs in Linux kernel 6.17 and earlier.
             This can also happen due to transparent file compression,
             which can also indicate fewer than the usual number of blocks.  */

          ਜੇ (lseek (fd, pos, SEEK_SET) < 0)
            ਵਾਪਸ ERROR_SCANTYPE;

          /* we prefer to return PLAIN_SCANTYPE here so that copy offload
             continues to be used.  Falling through to ZERO_SCANTYPE would be
             less efficient in the compressed file case.  */
          ਵਾਪਸ PLAIN_SCANTYPE;
        }
    }
  ਨਹੀਂਤਾਂ ਜੇ (pos < ਜਾਂਚ_inference->ext_ਸ਼ੁਰੂ || errno == ENXIO)
    {
      ਜਾਂਚ_inference->hole_ਸ਼ੁਰੂ = pos;  /* Pacify -Wmaybe-uninitialized.  */
      ਵਾਪਸ LSEEK_SCANTYPE;
    }
  ਨਹੀਂਤਾਂ ਜੇ (errno != EINVAL && !is_ENOTSUP (errno))
    ਵਾਪਸ ERROR_SCANTYPE;
#ਜੇ_ਅੰਤ

  ਵਾਪਸ ZERO_SCANTYPE;
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

ਬਾਹਰੀ intmax_t
copy_file_ਅੰਕੜੇ (ਪੂਰਨ ifd, ਬਣਤਰ stat ਅਚਲ *ist, off_t ipos, ਅੱਖਰ ਅਚਲ *iname,
                ਪੂਰਨ ofd, ਬਣਤਰ stat ਅਚਲ *ost, off_t opos, ਅੱਖਰ ਅਚਲ *oname,
                ਗਿਣਤੀ_t ibytes, ਬਣਤਰ cp_options ਅਚਲ *x,
                ਬਣਤਰ copy_debug *debug)
{
  /* Choose a suitable buffer size; it may be adjusted later.  */
  idx_t buf_ਮਾਪ = io_blksize (ost);

  /* Deal with sparse files.  */
  ਬਣਤਰ ਜਾਂਚ_inference ਜਾਂਚ_inference;
  ਗਿਣਤੀ scantype scantype = infer_scantype (ifd, ist, ipos, &ਜਾਂਚ_inference);
  ਜੇ (scantype == ERROR_SCANTYPE)
    {
      error (0, errno, _("cannot lseek %s"), quoteaf (iname));
      ਵਾਪਸ -1;
    }
  bool make_holes
    = (S_ISREG (ost->st_ਬਹੁਲਕ)
       && (x->sparse_ਬਹੁਲਕ == SPARSE_ALWAYS
           || (x->sparse_ਬਹੁਲਕ == SPARSE_AUTO
               && scantype != PLAIN_SCANTYPE)));

  /* If we _know_ we're going to read data sequentially into the process,
     i.e., --reflink or --sparse are not in auto mode,
     give that hint to the kernel so it can tune caching behavior.
     Also we don't bother calling fadvise for small copies,
     as it is not likely to help performance and might even hurt it.
     Also we only apply this hint for the whole file (0 length)
     as OpenZFS 2.2.2 at least will otherwise synchronously
     (decompress and) populate the cache when given a specific length.  */
  ਜੇ (ipos == 0 && ibytes == ਗਿਣਤੀ_MAX
      && (x->reflink_ਬਹੁਲਕ != REFLINK_AUTO || x->sparse_ਬਹੁਲਕ != SPARSE_AUTO))
    fdadvise (ifd, 0, 0, FADVISE_SEQUENTIAL);

  /* If not making a sparse file, try to use a more-efficient
     buffer size.  */
  ਜੇ (! make_holes)
    {
      /* Compute the least common multiple of the input and output
         buffer sizes, adjusting for outlandish values.
         blcm is at most IDX_MAX - 1 so that buf_size cannot become 0 below.
         Note we read in multiples of the reported block size
         to support (unusual) devices that have this constraint.  */
      idx_t blcm_max = MIN (MIN (IDX_MAX - 1, SSIZE_MAX), SIZE_MAX);
      idx_t blcm = ਬਫਰ_lcm (io_blksize (ist), buf_ਮਾਪ,
                               blcm_max);

      /* Do not bother with a buffer larger than the input file, plus one
         byte to make sure the file has not grown while reading it.  */
      ਜੇ (S_ISREG (ist->st_ਬਹੁਲਕ) && 0 <= ist->st_ਮਾਪ
          && ist->st_ਮਾਪ < buf_ਮਾਪ)
        buf_ਮਾਪ = ist->st_ਮਾਪ + 1;

      /* However, stick with a block size that is a positive multiple of
         blcm, overriding the above adjustments.  Watch out for
         overflow.  */
      buf_ਮਾਪ = ckd_ਜੋੜੋ (&buf_ਮਾਪ, buf_ਮਾਪ, blcm - 1) ? IDX_MAX : buf_ਮਾਪ;
      buf_ਮਾਪ -= buf_ਮਾਪ % blcm;
    }

  ਅੱਖਰ *buf = ਨਿਰਾ;
  intmax_t ਨਤੀਜਾ;
  off_t hole_ਮਾਪ = 0;

  ਜੇ (scantype == LSEEK_SCANTYPE)
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SEEK_HOLE
      ਨਤੀਜਾ = lseek_copy (ifd, ofd, &buf, buf_ਮਾਪ,
                           ipos, ibytes, &ਜਾਂਚ_inference, ist->st_ਮਾਪ,
                           make_holes ? x->sparse_ਬਹੁਲਕ : SPARSE_NEVER,
                           x->reflink_ਬਹੁਲਕ != REFLINK_NEVER,
                           iname, oname, &hole_ਮਾਪ, debug);
#ਨਹੀਂਤਾਂ
      affirm (false);
#ਜੇ_ਅੰਤ
    }
  ਨਹੀਂਤਾਂ
    ਨਤੀਜਾ = sparse_copy (ifd, ofd, &buf, buf_ਮਾਪ,
                          x->reflink_ਬਹੁਲਕ != REFLINK_NEVER,
                          iname, oname, ibytes,
                          make_holes ? &hole_ਮਾਪ : ਨਿਰਾ,
                          debug);

  ਜੇ (0 <= ਨਤੀਜਾ && 0 < hole_ਮਾਪ)
    {
      off_t oend;
      ਜੇ (ckd_ਜੋੜੋ (&oend, opos, ਨਤੀਜਾ)
          ? (errno = EOVERFLOW, true)
          : make_holes
          ? ftruncate (ofd, oend) < 0
          : !write_zeros (ofd, hole_ਮਾਪ, &buf, buf_ਮਾਪ))
        {
          error (0, errno, _("failed to extend %s"), quoteaf (oname));
          ਨਤੀਜਾ = -1;
        }
      ਨਹੀਂਤਾਂ ਜੇ (make_holes
               && punch_hole (ofd, oend - hole_ਮਾਪ, hole_ਮਾਪ) < 0)
        {
          error (0, errno, _("error deallocating %s"), quoteaf (oname));
          ਨਤੀਜਾ = -1;
        }
    }

  alignfree (buf);
  ਵਾਪਸ ਨਤੀਜਾ;
}
