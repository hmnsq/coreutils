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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "alignalloc.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "backupfile.h"
#ಅನ್ತರ್ಭಾವ "buffer-lcm.h"
#ಅನ್ತರ್ಭಾವ "copy.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "full-write.h"
#ಅನ್ತರ್ಭಾವ "ioblksize.h"

/* Result of infer_scantype when it returns LSEEK_SCANTYPE.  */
ರಚನೆ ಪರಿಶೀಲನೆ_inference
{
  /* The offset of the first data block, or -1 if the file has no data.  */
  off_t ext_ಆರಂಭ;

  /* The offset of the first hole.  */
  off_t hole_ಆರಂಭ;
};

/* Attempt to punch a hole to avoid any permanent
   speculative preallocation on file systems such as XFS.
   Return values as per fallocate(2) except ENOSYS etc. are ignored.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
punch_hole (ಪೂರ್ಣಾಂಕ fd, off_t ಸ್ಥಾನಾಂತರ, off_t ಉದ್ದಳತೆ)
{
  ಪೂರ್ಣಾಂಕ ret = 0;
/* +0 is to work around older <linux/fs.h> defining HAVE_FALLOCATE to empty.  */
#ಆದರೆ HAVE_FALLOCATE + 0
# ಆದರೆ defined FALLOC_FL_PUNCH_HOLE && defined FALLOC_FL_KEEP_ಗಾತ್ರ
  ret = fallocate (fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_ಗಾತ್ರ,
                   ಸ್ಥಾನಾಂತರ, ಉದ್ದಳತೆ);
  ಆದರೆ (ret < 0 && (is_ENOTSUP (errno) || errno == ENOSYS))
    ret = 0;
# endif
#ಆದರೆ_ಅಂತ್ಯ
  ಹಿಂತಿರುಗಿ ret;
}

/* Create a hole at the end of the file with descriptor FD and name NAME.
   The hole is of size SIZE.  Assume FD is already at file end,
   and advance FD past the newly-created hole.
   Return the resulting position, or -1 on failure.  */
ಸ್ಥಿರ off_t
create_hole (ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, off_t ಗಾತ್ರ_2)
{
  off_t file_ಕೊನೆ = lseek (fd, ಗಾತ್ರ_2, SEEK_CUR);

  ಆದರೆ (file_ಕೊನೆ < 0)
    {
      error (0, errno, _("cannot lseek %s"), quoteaf (name));
      ಹಿಂತಿರುಗಿ -1;
    }

  /* Some file systems (like XFS) preallocate when write extending a file.
     I.e., a previous write() may have preallocated extra space
     that the seek above will not discard.  A subsequent write() could
     then make this allocation permanent.  */
  ಆದರೆ (punch_hole (fd, file_ಕೊನೆ - ಗಾತ್ರ_2, ಗಾತ್ರ_2) < 0)
    {
      error (0, errno, _("error deallocating %s"), quoteaf (name));
      ಹಿಂತಿರುಗಿ -1;
    }

  ಹಿಂತಿರುಗಿ file_ಕೊನೆ;
}

/* Does ERR mean the copying operation is not supported or allowed for
   this file or process, even though the operation was invoked correctly?  */
ಸ್ಥಿರ bool
is_CLONENOTSUP (ಪೂರ್ಣಾಂಕ err)
{
  ಹಿಂತಿರುಗಿ err == ENOSYS || err == ENOTTY || is_ENOTSUP (err)
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
ಸ್ಥಿರ intmax_t
sparse_copy (ಪೂರ್ಣಾಂಕ src_fd, ಪೂರ್ಣಾಂಕ dest_fd, ಅಕ್ಷರ **abuf, idx_t buf_ಗಾತ್ರ,
             bool allow_reflink,
             ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src_name, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_name,
             ಎಣಿಕೆ_t max_n_read, off_t *hole_ಗಾತ್ರ,
             ರಚನೆ copy_debug *debug)
{
  ಎಣಿಕೆ_t ಒಟ್ಟು_n_read = 0;

  ಆದರೆ (debug->sparse_detection == COPY_DEBUG_UNKNOWN)
    debug->sparse_detection = hole_ಗಾತ್ರ ? COPY_DEBUG_YES : COPY_DEBUG_NO;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (hole_ಗಾತ್ರ && debug->sparse_detection == COPY_DEBUG_EXTERNAL)
    debug->sparse_detection = COPY_DEBUG_EXTERNAL_ಆಂತರಿಕ;

  /* If not looking for holes, use copy_file_range if functional,
     but don't use if reflink disallowed as that may be implicit.  */
  ಆದರೆ (!hole_ಗಾತ್ರ && allow_reflink)
    ಆಗ (0 < max_n_read)
      {
        /* Copy at most COPY_MAX bytes at a time; this is min
           (SSIZE_MAX, SIZE_MAX) truncated to a value that is
           surely aligned well.  */
        ssize_t copy_max = MIN (SSIZE_MAX, SIZE_MAX) >> 30 << 30;
        ssize_t n_copied = copy_file_range (src_fd, ಖಾಲಿ, dest_fd, ಖಾಲಿ,
                                            MIN (max_n_read, copy_max), 0);
        ಆದರೆ (n_copied == 0)
          {
            /* copy_file_range incorrectly returns 0 when reading from
               the proc file system on the Linux kernel through at
               least 5.6.19 (2020), so fall back on 'read' if the
               input file seems empty.  */
            ಆದರೆ (ಒಟ್ಟು_n_read == 0)
              ನಿಲ್ಲಿಸು;
            debug->offload = COPY_DEBUG_YES;
            ಹಿಂತಿರುಗಿ ಒಟ್ಟು_n_read;
          }
        ಆದರೆ (n_copied < 0)
          {
            debug->offload = errno;

            /* Don’t treat EFBIG as a reportable error from copy_file_range.
               If the input is at EOF and the output position is 2**63 - 1,
               copy_file_range (ifd, NULL, ofd, NULL, 2146435072, 0)
               incorrectly fails with EFBIG.  Problem observed on Ubuntu 25.10
               x86-64 with Linux kernel 6.17.0-8-generic #8-Ubuntu.  */
            ಆದರೆ (errno == EFBIG)
              ನಿಲ್ಲಿಸು;

            /* Consider operation unsupported only if no data copied.
               For example, EPERM could occur if copy_file_range not enabled
               in seccomp filters, so retry with a standard copy.  EPERM can
               also occur for immutable files, but that would only be in the
               edge case where the file is made immutable after creating,
               in which case the (more accurate) error is still shown.  */
            ಆದರೆ (ಒಟ್ಟು_n_read == 0 && is_CLONENOTSUP (errno))
              ನಿಲ್ಲಿಸು;

            /* ENOENT was seen sometimes across CIFS shares, resulting in
               no data being copied, but subsequent standard copies succeed.  */
            ಆದರೆ (ಒಟ್ಟು_n_read == 0 && errno == ENOENT)
              ನಿಲ್ಲಿಸು;

            ಆದರೆ (errno == EINTR)
              n_copied = 0;
            ಇಲ್ಲದಿದ್ದರೆ
              {
                error (0, errno, _("error copying %s to %s"),
                       quoteaf_n (0, src_name), quoteaf_n (1, dst_name));
                ಹಿಂತಿರುಗಿ -1;
              }
          }
        debug->offload = COPY_DEBUG_YES;
        max_n_read -= n_copied;
        ಒಟ್ಟು_n_read += n_copied;
      }
  ಇಲ್ಲದಿದ್ದರೆ
    debug->offload = COPY_DEBUG_AVOIDED;


  off_t psize = hole_ಗಾತ್ರ ? *hole_ಗಾತ್ರ : 0;
  bool make_hole = !!psize;

  ಆಗ (0 < max_n_read)
    {
      ಆದರೆ (!*abuf)
        *abuf = xalignalloc (getpagesize (), buf_ಗಾತ್ರ);
      ಅಕ್ಷರ *buf = *abuf;
      ssize_t n_read = ಪಠ (src_fd, buf, MIN (max_n_read, buf_ಗಾತ್ರ));
      ಆದರೆ (n_read < 0)
        {
          ಆದರೆ (errno == EINTR)
            ಮುಂದುವರಿ;
          error (0, errno, _("error reading %s"), quoteaf (src_name));
          ಹಿಂತಿರುಗಿ -1;
        }
      ಆದರೆ (n_read == 0)
        ನಿಲ್ಲಿಸು;
      max_n_read -= n_read;
      ಒಟ್ಟು_n_read += n_read;

      /* If looking for holes, loop over the input buffer in chunks
         corresponding to the minimum hole size.  Otherwise, scan the
         whole buffer.  struct stat does not report the minimum hole
         size for a file, so use ST_NBLOCKSIZE which should be the
         minimum for all file systems on this platform.  */
      idx_t csize = hole_ಗಾತ್ರ ? ST_NBLOCKSIZE : buf_ಗಾತ್ರ;
      ಅಕ್ಷರ *cbuf = buf;
      ಅಕ್ಷರ *pbuf = buf;

      ಆಗ (n_read)
        {
          bool prev_hole = make_hole;
          csize = MIN (csize, n_read);

          ಆದರೆ (hole_ಗಾತ್ರ)
            make_hole = is_nul (cbuf, csize);

          bool transition = (make_hole != prev_hole) && psize;
          bool ಕೊನೆಯ_chunk = n_read == csize && !make_hole;

          ಆದರೆ (transition || ಕೊನೆಯ_chunk)
            {
              ಆದರೆ (! transition)
                psize += csize;
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (prev_hole)
                {
                  ಆದರೆ (create_hole (dest_fd, dst_name, psize) < 0)
                    ಹಿಂತಿರುಗಿ false;
                  pbuf = cbuf;
                  psize = csize;
                }

              ಆದರೆ (!prev_hole || (transition && ಕೊನೆಯ_chunk))
                {
                  ಆದರೆ (ತುಂಬಿದ_write (dest_fd, pbuf, psize) != psize)
                    {
                      error (0, errno, _("error writing %s"),
                             quoteaf (dst_name));
                      ಹಿಂತಿರುಗಿ -1;
                    }
                  psize = !prev_hole && transition ? csize : 0;
                }
            }
          ಇಲ್ಲದಿದ್ದರೆ  /* Coalesce writes/seeks.  */
            {
              ಆದರೆ (ckd_ಕೂಡಿಸು (&psize, psize, csize))
                {
                  error (0, 0, _("overflow reading %s"), quoteaf (src_name));
                  ಹಿಂತಿರುಗಿ -1;
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

  ಆದರೆ (hole_ಗಾತ್ರ)
    *hole_ಗಾತ್ರ = make_hole ? psize : 0;
  ಹಿಂತಿರುಗಿ ಒಟ್ಟು_n_read;
}

/* Write to FD.  Write data consisting of N_BYTES' worth of '\0's.
   Use the buffer *ABUF of size BUF_SIZE for temporary storage,
   allocating the buffer lazily if *ABUF is null.
   Return true if successful, false (setting errno) otherwise.  */
ಸ್ಥಿರ bool
write_zeros (ಪೂರ್ಣಾಂಕ fd, off_t n_ಬೈಟುಗಳು, ಅಕ್ಷರ **abuf, idx_t buf_ಗಾತ್ರ)
{
  ಅಕ್ಷರ *zeros = ಖಾಲಿ;
  ಆಗ (n_ಬೈಟುಗಳು)
    {
      idx_t n = MIN (buf_ಗಾತ್ರ, n_ಬೈಟುಗಳು);
      ಆದರೆ (!zeros)
        {
          ಆದರೆ (!*abuf)
            *abuf = xalignalloc (getpagesize (), buf_ಗಾತ್ರ);
          zeros = memset (*abuf, 0, n);
        }
      ಆದರೆ (ತುಂಬಿದ_write (fd, zeros, n) != n)
        ಹಿಂತಿರುಗಿ false;
      n_ಬೈಟುಗಳು -= n;
    }

  ಹಿಂತಿರುಗಿ true;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SEEK_HOLE
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

ಸ್ಥಿರ off_t
lseek_copy (ಪೂರ್ಣಾಂಕ src_fd, ಪೂರ್ಣಾಂಕ dest_fd, ಅಕ್ಷರ **abuf, idx_t buf_ಗಾತ್ರ,
            off_t src_pos, ಎಣಿಕೆ_t ibytes,
            ರಚನೆ ಪರಿಶೀಲನೆ_inference ಸ್ಥಿರಾಂಕ *ಪರಿಶೀಲನೆ_inference, off_t src_ಒಟ್ಟು_ಗಾತ್ರ,
            ಎಣಿಕೆ Sparse_type sparse_ಬಹುಲಕ,
            bool allow_reflink,
            ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src_name, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_name,
            off_t *hole_ಗಾತ್ರ, ರಚನೆ copy_debug *debug)
{
  off_t ಕೊನೆಯ_ext_ಆರಂಭ = src_pos;
  off_t ಕೊನೆಯ_ext_len = 0;
  off_t max_ipos = ckd_ಕೂಡಿಸು (&max_ipos, src_pos, ibytes) ? OFF_T_MAX : max_ipos;

  /* From here on, SRC_TOTAL_SIZE is bounded above by MAX_IPOS.  */
  src_ಒಟ್ಟು_ಗಾತ್ರ = MIN (src_ಒಟ್ಟು_ಗಾತ್ರ, max_ipos);

  /* The input position after the most recent read of data,
     or SRC_POS if no data have been read.  */
  off_t ipos = src_pos;

  debug->sparse_detection = COPY_DEBUG_EXTERNAL;

  bool ಬಳಸಿದ_ಪರಿಶೀಲನೆ_inference = false;

  ಗಾಗಿ (off_t ext_ಆರಂಭ = ಪರಿಶೀಲನೆ_inference->ext_ಆರಂಭ;
       0 <= ext_ಆರಂಭ && ext_ಆರಂಭ < max_ipos; )
    {
      off_t ext_ಕೊನೆ;
      ಆದರೆ (ext_ಆರಂಭ == src_pos && ! ಬಳಸಿದ_ಪರಿಶೀಲನೆ_inference)
        {
          ext_ಕೊನೆ = ಪರಿಶೀಲನೆ_inference->hole_ಆರಂಭ;
          ಬಳಸಿದ_ಪರಿಶೀಲನೆ_inference = true;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        ext_ಕೊನೆ = lseek (src_fd, ext_ಆರಂಭ, SEEK_HOLE);
      ಆದರೆ (0 <= ext_ಕೊನೆ)
        ext_ಕೊನೆ = MIN (ext_ಕೊನೆ, max_ipos);
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (errno != ENXIO)
            ಹೋಗು cannot_lseek;
          ext_ಕೊನೆ = src_ಒಟ್ಟು_ಗಾತ್ರ;
          ಆದರೆ (ext_ಕೊನೆ <= ext_ಆರಂಭ)
            {
              /* The input file grew; get its current size.  */
              src_ಒಟ್ಟು_ಗಾತ್ರ = lseek (src_fd, 0, SEEK_END);
              ಆದರೆ (src_ಒಟ್ಟು_ಗಾತ್ರ < 0)
                ಹೋಗು cannot_lseek;
              src_ಒಟ್ಟು_ಗಾತ್ರ = MIN (src_ಒಟ್ಟು_ಗಾತ್ರ, max_ipos);

              /* If the input file shrank after growing, stop copying.  */
              ಆದರೆ (src_ಒಟ್ಟು_ಗಾತ್ರ <= ext_ಆರಂಭ)
                ನಿಲ್ಲಿಸು;

              ext_ಕೊನೆ = src_ಒಟ್ಟು_ಗಾತ್ರ;
            }
        }
      /* If the input file must have grown, increase its measured size.  */
      ಆದರೆ (src_ಒಟ್ಟು_ಗಾತ್ರ < ext_ಕೊನೆ)
        src_ಒಟ್ಟು_ಗಾತ್ರ = ext_ಕೊನೆ;

      ಆದರೆ (lseek (src_fd, ext_ಆರಂಭ, SEEK_SET) < 0)
        ಹೋಗು cannot_lseek;

      off_t ext_hole_ಗಾತ್ರ = ext_ಆರಂಭ - ಕೊನೆಯ_ext_ಆರಂಭ - ಕೊನೆಯ_ext_len;

      ಆದರೆ (ext_hole_ಗಾತ್ರ)
        {
          ಆದರೆ (sparse_ಬಹುಲಕ == SPARSE_ALWAYS)
            *hole_ಗಾತ್ರ += ext_hole_ಗಾತ್ರ;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sparse_ಬಹುಲಕ != SPARSE_NEVER)
            {
              off_t epos = create_hole (dest_fd, dst_name, ext_hole_ಗಾತ್ರ);
              ಆದರೆ (epos < 0)
                ಹಿಂತಿರುಗಿ epos;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* When not inducing holes and when there is a hole between
                 the end of the previous extent and the beginning of the
                 current one, write zeros to the destination file.  */
              ಆದರೆ (! write_zeros (dest_fd, ext_hole_ಗಾತ್ರ, abuf, buf_ಗಾತ್ರ))
                {
                  error (0, errno, _("%s: write failed"),
                         quotef (dst_name));
                  ಹಿಂತಿರುಗಿ -1;
                }
            }
        }

      off_t ext_len = ext_ಕೊನೆ - ext_ಆರಂಭ;
      ಕೊನೆಯ_ext_ಆರಂಭ = ext_ಆರಂಭ;
      ಕೊನೆಯ_ext_len = ext_len;

      /* Copy this extent, looking for further opportunities to not
         bother to write zeros if --sparse=always, since SEEK_HOLE
         is conservative and may miss some holes.  */
      off_t n_read
        = sparse_copy (src_fd, dest_fd, abuf, buf_ಗಾತ್ರ,
                       allow_reflink, src_name, dst_name,
                       ext_len,
                       sparse_ಬಹುಲಕ == SPARSE_ALWAYS ? hole_ಗಾತ್ರ : ಖಾಲಿ,
                       debug);
      ಆದರೆ (n_read < 0)
        ಹಿಂತಿರುಗಿ -1;

      ipos = ext_ಆರಂಭ + n_read;
      ಆದರೆ (n_read < ext_len)
        {
          /* The input file shrank.  */
          src_ಒಟ್ಟು_ಗಾತ್ರ = ipos;
          ನಿಲ್ಲಿಸು;
        }

      ext_ಆರಂಭ = lseek (src_fd, ipos, SEEK_ದತ್ತಾಂಶ);
      ಆದರೆ (ext_ಆರಂಭ < 0 && errno != ENXIO)
        ಹೋಗು cannot_lseek;
    }

  *hole_ಗಾತ್ರ += src_ಒಟ್ಟು_ಗಾತ್ರ - (ಕೊನೆಯ_ext_ಆರಂಭ + ಕೊನೆಯ_ext_len);
  ಹಿಂತಿರುಗಿ src_ಒಟ್ಟು_ಗಾತ್ರ - src_pos;

 cannot_lseek:
  error (0, errno, _("cannot lseek %s"), quoteaf (src_name));
  ಹಿಂತಿರುಗಿ -1;
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_STRUCT_STAT_ST_ಖಂಡಗಳು
# define HAVE_STRUCT_STAT_ST_ಖಂಡಗಳು 0
#ಆದರೆ_ಅಂತ್ಯ

/* Type of scan being done on the input when looking for sparseness.  */
ಎಣಿಕೆ scantype
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
ಸ್ಥಿರ ಎಣಿಕೆ scantype
infer_scantype (ಪೂರ್ಣಾಂಕ fd, ರಚನೆ stat ಸ್ಥಿರಾಂಕ *sb, off_t pos,
                ರಚನೆ ಪರಿಶೀಲನೆ_inference *ಪರಿಶೀಲನೆ_inference)
{
  /* Try SEEK_HOLE only if this heuristic
     suggests the file is sparse.  */
  ಆದರೆ (! (HAVE_STRUCT_STAT_ST_ಖಂಡಗಳು
         && S_ISREG (sb->st_ಬಹುಲಕ)
         && STP_NBLOCKS (sb) < sb->st_ಗಾತ್ರ / ST_NBLOCKSIZE))
    ಹಿಂತಿರುಗಿ PLAIN_SCANTYPE;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SEEK_HOLE
  ಪರಿಶೀಲನೆ_inference->ext_ಆರಂಭ = lseek (fd, pos, SEEK_ದತ್ತಾಂಶ);
  ಆದರೆ (ಪರಿಶೀಲನೆ_inference->ext_ಆರಂಭ == pos)
    {
      ಪರಿಶೀಲನೆ_inference->hole_ಆರಂಭ = lseek (fd, pos, SEEK_HOLE);
      ಆದರೆ (0 <= ಪರಿಶೀಲನೆ_inference->hole_ಆರಂಭ)
        {
          ಆದರೆ (ಪರಿಶೀಲನೆ_inference->hole_ಆರಂಭ < sb->st_ಗಾತ್ರ)
            ಹಿಂತಿರುಗಿ LSEEK_SCANTYPE;

          /* Though the file may have holes, SEEK_DATA and SEEK_HOLE
             didn't find any.  This can happen with file systems
             that support SEEK_HOLE only trivially,
             such as squashfs in Linux kernel 6.17 and earlier.
             This can also happen due to transparent file compression,
             which can also indicate fewer than the usual number of blocks.  */

          ಆದರೆ (lseek (fd, pos, SEEK_SET) < 0)
            ಹಿಂತಿರುಗಿ ERROR_SCANTYPE;

          /* we prefer to return PLAIN_SCANTYPE here so that copy offload
             continues to be used.  Falling through to ZERO_SCANTYPE would be
             less efficient in the compressed file case.  */
          ಹಿಂತಿರುಗಿ PLAIN_SCANTYPE;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (pos < ಪರಿಶೀಲನೆ_inference->ext_ಆರಂಭ || errno == ENXIO)
    {
      ಪರಿಶೀಲನೆ_inference->hole_ಆರಂಭ = pos;  /* Pacify -Wmaybe-uninitialized.  */
      ಹಿಂತಿರುಗಿ LSEEK_SCANTYPE;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (errno != EINVAL && !is_ENOTSUP (errno))
    ಹಿಂತಿರುಗಿ ERROR_SCANTYPE;
#ಆದರೆ_ಅಂತ್ಯ

  ಹಿಂತಿರುಗಿ ZERO_SCANTYPE;
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

ಬಾಹ್ಯ intmax_t
copy_file_ದತ್ತಾಂಶ (ಪೂರ್ಣಾಂಕ ifd, ರಚನೆ stat ಸ್ಥಿರಾಂಕ *ist, off_t ipos, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *iname,
                ಪೂರ್ಣಾಂಕ ofd, ರಚನೆ stat ಸ್ಥಿರಾಂಕ *ost, off_t opos, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *oname,
                ಎಣಿಕೆ_t ibytes, ರಚನೆ cp_options ಸ್ಥಿರಾಂಕ *x,
                ರಚನೆ copy_debug *debug)
{
  /* Choose a suitable buffer size; it may be adjusted later.  */
  idx_t buf_ಗಾತ್ರ = io_blksize (ost);

  /* Deal with sparse files.  */
  ರಚನೆ ಪರಿಶೀಲನೆ_inference ಪರಿಶೀಲನೆ_inference;
  ಎಣಿಕೆ scantype scantype = infer_scantype (ifd, ist, ipos, &ಪರಿಶೀಲನೆ_inference);
  ಆದರೆ (scantype == ERROR_SCANTYPE)
    {
      error (0, errno, _("cannot lseek %s"), quoteaf (iname));
      ಹಿಂತಿರುಗಿ -1;
    }
  bool make_holes
    = (S_ISREG (ost->st_ಬಹುಲಕ)
       && (x->sparse_ಬಹುಲಕ == SPARSE_ALWAYS
           || (x->sparse_ಬಹುಲಕ == SPARSE_AUTO
               && scantype != PLAIN_SCANTYPE)));

  /* If we _know_ we're going to read data sequentially into the process,
     i.e., --reflink or --sparse are not in auto mode,
     give that hint to the kernel so it can tune caching behavior.
     Also we don't bother calling fadvise for small copies,
     as it is not likely to help performance and might even hurt it.
     Also we only apply this hint for the whole file (0 length)
     as OpenZFS 2.2.2 at least will otherwise synchronously
     (decompress and) populate the cache when given a specific length.  */
  ಆದರೆ (ipos == 0 && ibytes == ಎಣಿಕೆ_MAX
      && (x->reflink_ಬಹುಲಕ != REFLINK_AUTO || x->sparse_ಬಹುಲಕ != SPARSE_AUTO))
    fdadvise (ifd, 0, 0, FADVISE_SEQUENTIAL);

  /* If not making a sparse file, try to use a more-efficient
     buffer size.  */
  ಆದರೆ (! make_holes)
    {
      /* Compute the least common multiple of the input and output
         buffer sizes, adjusting for outlandish values.
         blcm is at most IDX_MAX - 1 so that buf_size cannot become 0 below.
         Note we read in multiples of the reported block size
         to support (unusual) devices that have this constraint.  */
      idx_t blcm_max = MIN (MIN (IDX_MAX - 1, SSIZE_MAX), SIZE_MAX);
      idx_t blcm = ಬಫರ್_lcm (io_blksize (ist), buf_ಗಾತ್ರ,
                               blcm_max);

      /* Do not bother with a buffer larger than the input file, plus one
         byte to make sure the file has not grown while reading it.  */
      ಆದರೆ (S_ISREG (ist->st_ಬಹುಲಕ) && 0 <= ist->st_ಗಾತ್ರ
          && ist->st_ಗಾತ್ರ < buf_ಗಾತ್ರ)
        buf_ಗಾತ್ರ = ist->st_ಗಾತ್ರ + 1;

      /* However, stick with a block size that is a positive multiple of
         blcm, overriding the above adjustments.  Watch out for
         overflow.  */
      buf_ಗಾತ್ರ = ckd_ಕೂಡಿಸು (&buf_ಗಾತ್ರ, buf_ಗಾತ್ರ, blcm - 1) ? IDX_MAX : buf_ಗಾತ್ರ;
      buf_ಗಾತ್ರ -= buf_ಗಾತ್ರ % blcm;
    }

  ಅಕ್ಷರ *buf = ಖಾಲಿ;
  intmax_t ಫಲಿತಾಂಶ;
  off_t hole_ಗಾತ್ರ = 0;

  ಆದರೆ (scantype == LSEEK_SCANTYPE)
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SEEK_HOLE
      ಫಲಿತಾಂಶ = lseek_copy (ifd, ofd, &buf, buf_ಗಾತ್ರ,
                           ipos, ibytes, &ಪರಿಶೀಲನೆ_inference, ist->st_ಗಾತ್ರ,
                           make_holes ? x->sparse_ಬಹುಲಕ : SPARSE_NEVER,
                           x->reflink_ಬಹುಲಕ != REFLINK_NEVER,
                           iname, oname, &hole_ಗಾತ್ರ, debug);
#ಇಲ್ಲದಿದ್ದರೆ
      affirm (false);
#ಆದರೆ_ಅಂತ್ಯ
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಫಲಿತಾಂಶ = sparse_copy (ifd, ofd, &buf, buf_ಗಾತ್ರ,
                          x->reflink_ಬಹುಲಕ != REFLINK_NEVER,
                          iname, oname, ibytes,
                          make_holes ? &hole_ಗಾತ್ರ : ಖಾಲಿ,
                          debug);

  ಆದರೆ (0 <= ಫಲಿತಾಂಶ && 0 < hole_ಗಾತ್ರ)
    {
      off_t oend;
      ಆದರೆ (ckd_ಕೂಡಿಸು (&oend, opos, ಫಲಿತಾಂಶ)
          ? (errno = EOVERFLOW, true)
          : make_holes
          ? ftruncate (ofd, oend) < 0
          : !write_zeros (ofd, hole_ಗಾತ್ರ, &buf, buf_ಗಾತ್ರ))
        {
          error (0, errno, _("failed to extend %s"), quoteaf (oname));
          ಫಲಿತಾಂಶ = -1;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (make_holes
               && punch_hole (ofd, oend - hole_ಗಾತ್ರ, hole_ಗಾತ್ರ) < 0)
        {
          error (0, errno, _("error deallocating %s"), quoteaf (oname));
          ಫಲಿತಾಂಶ = -1;
        }
    }

  alignfree (buf);
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}
