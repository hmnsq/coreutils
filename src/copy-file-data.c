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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "alignalloc.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "backupfile.h"
#ଅନ୍ତର୍ଭାଵ "buffer-lcm.h"
#ଅନ୍ତର୍ଭାଵ "copy.h"
#ଅନ୍ତର୍ଭାଵ "fadvise.h"
#ଅନ୍ତର୍ଭାଵ "full-write.h"
#ଅନ୍ତର୍ଭାଵ "ioblksize.h"

/* Result of infer_scantype when it returns LSEEK_SCANTYPE.  */
ସଂରଚନା scan_inference
{
  /* The offset of the first data block, or -1 if the file has no data.  */
  off_t ext_start;

  /* The offset of the first hole.  */
  off_t hole_start;
};

/* Attempt to punch a hole to avoid any permanent
   speculative preallocation on file systems such as XFS.
   Return values as per fallocate(2) except ENOSYS etc. are ignored.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
punch_hole (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, off_t offset, off_t length)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
/* +0 is to work around older <linux/fs.h> defining HAVE_FALLOCATE to empty.  */
#ଯଦି HAVE_FALLOCATE + 0
# ଯଦି defined FALLOC_FL_PUNCH_HOLE && defined FALLOC_FL_KEEP_SIZE
  ret = fallocate (fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
                   offset, length);
  ଯଦି (ret < 0 && (is_ENOTSUP (errno) || errno == ENOSYS))
    ret = 0;
# endif
#endif
  ଫେରନ୍ତୁ ret;
}

/* Create a hole at the end of the file with descriptor FD and name NAME.
   The hole is of size SIZE.  Assume FD is already at file end,
   and advance FD past the newly-created hole.
   Return the resulting position, or -1 on failure.  */
ସ୍ଥିର off_t
create_hole (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର ଅଚଳ *name, off_t size)
{
  off_t file_end = lseek (fd, size, SEEK_CUR);

  ଯଦି (file_end < 0)
    {
      error (0, errno, _("cannot lseek %s"), quoteaf (name));
      ଫେରନ୍ତୁ -1;
    }

  /* Some file systems (like XFS) preallocate when write extending a file.
     I.e., a previous write() may have preallocated extra space
     that the seek above will not discard.  A subsequent write() could
     then make this allocation permanent.  */
  ଯଦି (punch_hole (fd, file_end - size, size) < 0)
    {
      error (0, errno, _("error deallocating %s"), quoteaf (name));
      ଫେରନ୍ତୁ -1;
    }

  ଫେରନ୍ତୁ file_end;
}

/* Does ERR mean the copying operation is not supported or allowed for
   this file or process, even though the operation was invoked correctly?  */
ସ୍ଥିର bool
is_CLONENOTSUP (ପୂର୍ଣ୍ଣସଂଖ୍ୟା err)
{
  ଫେରନ୍ତୁ err == ENOSYS || err == ENOTTY || is_ENOTSUP (err)
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
ସ୍ଥିର intmax_t
sparse_copy (ପୂର୍ଣ୍ଣସଂଖ୍ୟା src_fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dest_fd, ଅକ୍ଷର **abuf, idx_t buf_size,
             bool allow_reflink,
             ଅକ୍ଷର ଅଚଳ *src_name, ଅକ୍ଷର ଅଚଳ *dst_name,
             ଗଣନା_t max_n_read, off_t *hole_size,
             ସଂରଚନା copy_debug *debug)
{
  ଗଣନା_t total_n_read = 0;

  ଯଦି (debug->sparse_detection == COPY_DEBUG_UNKNOWN)
    debug->sparse_detection = hole_size ? COPY_DEBUG_YES : COPY_DEBUG_NO;
  ଅନ୍ୟଥା ଯଦି (hole_size && debug->sparse_detection == COPY_DEBUG_EXTERNAL)
    debug->sparse_detection = COPY_DEBUG_EXTERNAL_INTERNAL;

  /* If not looking for holes, use copy_file_range if functional,
     but don't use if reflink disallowed as that may be implicit.  */
  ଯଦି (!hole_size && allow_reflink)
    ଯେତେବେଳେ (0 < max_n_read)
      {
        /* Copy at most COPY_MAX bytes at a time; this is min
           (SSIZE_MAX, SIZE_MAX) truncated to a value that is
           surely aligned well.  */
        ssize_t copy_max = MIN (SSIZE_MAX, SIZE_MAX) >> 30 << 30;
        ssize_t n_copied = copy_file_range (src_fd, ଖାଲି, dest_fd, ଖାଲି,
                                            MIN (max_n_read, copy_max), 0);
        ଯଦି (n_copied == 0)
          {
            /* copy_file_range incorrectly returns 0 when reading from
               the proc file system on the Linux kernel through at
               least 5.6.19 (2020), so fall back on 'read' if the
               input file seems empty.  */
            ଯଦି (total_n_read == 0)
              ଭାଙ୍ଗ;
            debug->offload = COPY_DEBUG_YES;
            ଫେରନ୍ତୁ total_n_read;
          }
        ଯଦି (n_copied < 0)
          {
            debug->offload = errno;

            /* Don’t treat EFBIG as a reportable error from copy_file_range.
               If the input is at EOF and the output position is 2**63 - 1,
               copy_file_range (ifd, NULL, ofd, NULL, 2146435072, 0)
               incorrectly fails with EFBIG.  Problem observed on Ubuntu 25.10
               x86-64 with Linux kernel 6.17.0-8-generic #8-Ubuntu.  */
            ଯଦି (errno == EFBIG)
              ଭାଙ୍ଗ;

            /* Consider operation unsupported only if no data copied.
               For example, EPERM could occur if copy_file_range not enabled
               in seccomp filters, so retry with a standard copy.  EPERM can
               also occur for immutable files, but that would only be in the
               edge case where the file is made immutable after creating,
               in which case the (more accurate) error is still shown.  */
            ଯଦି (total_n_read == 0 && is_CLONENOTSUP (errno))
              ଭାଙ୍ଗ;

            /* ENOENT was seen sometimes across CIFS shares, resulting in
               no data being copied, but subsequent standard copies succeed.  */
            ଯଦି (total_n_read == 0 && errno == ENOENT)
              ଭାଙ୍ଗ;

            ଯଦି (errno == EINTR)
              n_copied = 0;
            ଅନ୍ୟଥା
              {
                error (0, errno, _("error copying %s to %s"),
                       quoteaf_n (0, src_name), quoteaf_n (1, dst_name));
                ଫେରନ୍ତୁ -1;
              }
          }
        debug->offload = COPY_DEBUG_YES;
        max_n_read -= n_copied;
        total_n_read += n_copied;
      }
  ଅନ୍ୟଥା
    debug->offload = COPY_DEBUG_AVOIDED;


  off_t psize = hole_size ? *hole_size : 0;
  bool make_hole = !!psize;

  ଯେତେବେଳେ (0 < max_n_read)
    {
      ଯଦି (!*abuf)
        *abuf = xalignalloc (getpagesize (), buf_size);
      ଅକ୍ଷର *buf = *abuf;
      ssize_t n_read = ପଠ (src_fd, buf, MIN (max_n_read, buf_size));
      ଯଦି (n_read < 0)
        {
          ଯଦି (errno == EINTR)
            ଜାରି;
          error (0, errno, _("error reading %s"), quoteaf (src_name));
          ଫେରନ୍ତୁ -1;
        }
      ଯଦି (n_read == 0)
        ଭାଙ୍ଗ;
      max_n_read -= n_read;
      total_n_read += n_read;

      /* If looking for holes, loop over the input buffer in chunks
         corresponding to the minimum hole size.  Otherwise, scan the
         whole buffer.  struct stat does not report the minimum hole
         size for a file, so use ST_NBLOCKSIZE which should be the
         minimum for all file systems on this platform.  */
      idx_t csize = hole_size ? ST_NBLOCKSIZE : buf_size;
      ଅକ୍ଷର *cbuf = buf;
      ଅକ୍ଷର *pbuf = buf;

      ଯେତେବେଳେ (n_read)
        {
          bool prev_hole = make_hole;
          csize = MIN (csize, n_read);

          ଯଦି (hole_size)
            make_hole = is_nul (cbuf, csize);

          bool transition = (make_hole != prev_hole) && psize;
          bool last_chunk = n_read == csize && !make_hole;

          ଯଦି (transition || last_chunk)
            {
              ଯଦି (! transition)
                psize += csize;
              ଅନ୍ୟଥା ଯଦି (prev_hole)
                {
                  ଯଦି (create_hole (dest_fd, dst_name, psize) < 0)
                    ଫେରନ୍ତୁ false;
                  pbuf = cbuf;
                  psize = csize;
                }

              ଯଦି (!prev_hole || (transition && last_chunk))
                {
                  ଯଦି (full_write (dest_fd, pbuf, psize) != psize)
                    {
                      error (0, errno, _("error writing %s"),
                             quoteaf (dst_name));
                      ଫେରନ୍ତୁ -1;
                    }
                  psize = !prev_hole && transition ? csize : 0;
                }
            }
          ଅନ୍ୟଥା  /* Coalesce writes/seeks.  */
            {
              ଯଦି (ckd_add (&psize, psize, csize))
                {
                  error (0, 0, _("overflow reading %s"), quoteaf (src_name));
                  ଫେରନ୍ତୁ -1;
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

  ଯଦି (hole_size)
    *hole_size = make_hole ? psize : 0;
  ଫେରନ୍ତୁ total_n_read;
}

/* Write to FD.  Write data consisting of N_BYTES' worth of '\0's.
   Use the buffer *ABUF of size BUF_SIZE for temporary storage,
   allocating the buffer lazily if *ABUF is null.
   Return true if successful, false (setting errno) otherwise.  */
ସ୍ଥିର bool
write_zeros (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, off_t n_bytes, ଅକ୍ଷର **abuf, idx_t buf_size)
{
  ଅକ୍ଷର *zeros = ଖାଲି;
  ଯେତେବେଳେ (n_bytes)
    {
      idx_t n = MIN (buf_size, n_bytes);
      ଯଦି (!zeros)
        {
          ଯଦି (!*abuf)
            *abuf = xalignalloc (getpagesize (), buf_size);
          zeros = memset (*abuf, 0, n);
        }
      ଯଦି (full_write (fd, zeros, n) != n)
        ଫେରନ୍ତୁ false;
      n_bytes -= n;
    }

  ଫେରନ୍ତୁ true;
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

ସ୍ଥିର off_t
lseek_copy (ପୂର୍ଣ୍ଣସଂଖ୍ୟା src_fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dest_fd, ଅକ୍ଷର **abuf, idx_t buf_size,
            off_t src_pos, ଗଣନା_t ibytes,
            ସଂରଚନା scan_inference ଅଚଳ *scan_inference, off_t src_total_size,
            ଗଣନା Sparse_type sparse_mode,
            bool allow_reflink,
            ଅକ୍ଷର ଅଚଳ *src_name, ଅକ୍ଷର ଅଚଳ *dst_name,
            off_t *hole_size, ସଂରଚନା copy_debug *debug)
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

  ପାଇଁ (off_t ext_start = scan_inference->ext_start;
       0 <= ext_start && ext_start < max_ipos; )
    {
      off_t ext_end;
      ଯଦି (ext_start == src_pos && ! used_scan_inference)
        {
          ext_end = scan_inference->hole_start;
          used_scan_inference = true;
        }
      ଅନ୍ୟଥା
        ext_end = lseek (src_fd, ext_start, SEEK_HOLE);
      ଯଦି (0 <= ext_end)
        ext_end = MIN (ext_end, max_ipos);
      ଅନ୍ୟଥା
        {
          ଯଦି (errno != ENXIO)
            ଯାଅ cannot_lseek;
          ext_end = src_total_size;
          ଯଦି (ext_end <= ext_start)
            {
              /* The input file grew; get its current size.  */
              src_total_size = lseek (src_fd, 0, SEEK_END);
              ଯଦି (src_total_size < 0)
                ଯାଅ cannot_lseek;
              src_total_size = MIN (src_total_size, max_ipos);

              /* If the input file shrank after growing, stop copying.  */
              ଯଦି (src_total_size <= ext_start)
                ଭାଙ୍ଗ;

              ext_end = src_total_size;
            }
        }
      /* If the input file must have grown, increase its measured size.  */
      ଯଦି (src_total_size < ext_end)
        src_total_size = ext_end;

      ଯଦି (lseek (src_fd, ext_start, SEEK_SET) < 0)
        ଯାଅ cannot_lseek;

      off_t ext_hole_size = ext_start - last_ext_start - last_ext_len;

      ଯଦି (ext_hole_size)
        {
          ଯଦି (sparse_mode == SPARSE_ALWAYS)
            *hole_size += ext_hole_size;
          ଅନ୍ୟଥା ଯଦି (sparse_mode != SPARSE_NEVER)
            {
              off_t epos = create_hole (dest_fd, dst_name, ext_hole_size);
              ଯଦି (epos < 0)
                ଫେରନ୍ତୁ epos;
            }
          ଅନ୍ୟଥା
            {
              /* When not inducing holes and when there is a hole between
                 the end of the previous extent and the beginning of the
                 current one, write zeros to the destination file.  */
              ଯଦି (! write_zeros (dest_fd, ext_hole_size, abuf, buf_size))
                {
                  error (0, errno, _("%s: write failed"),
                         quotef (dst_name));
                  ଫେରନ୍ତୁ -1;
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
                       sparse_mode == SPARSE_ALWAYS ? hole_size : ଖାଲି,
                       debug);
      ଯଦି (n_read < 0)
        ଫେରନ୍ତୁ -1;

      ipos = ext_start + n_read;
      ଯଦି (n_read < ext_len)
        {
          /* The input file shrank.  */
          src_total_size = ipos;
          ଭାଙ୍ଗ;
        }

      ext_start = lseek (src_fd, ipos, SEEK_DATA);
      ଯଦି (ext_start < 0 && errno != ENXIO)
        ଯାଅ cannot_lseek;
    }

  *hole_size += src_total_size - (last_ext_start + last_ext_len);
  ଫେରନ୍ତୁ src_total_size - src_pos;

 cannot_lseek:
  error (0, errno, _("cannot lseek %s"), quoteaf (src_name));
  ଫେରନ୍ତୁ -1;
}
#endif

#ifndef HAVE_STRUCT_STAT_ST_BLOCKS
# define HAVE_STRUCT_STAT_ST_BLOCKS 0
#endif

/* Type of scan being done on the input when looking for sparseness.  */
ଗଣନା scantype
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
ସ୍ଥିର ଗଣନା scantype
infer_scantype (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ସଂରଚନା stat ଅଚଳ *sb, off_t pos,
                ସଂରଚନା scan_inference *scan_inference)
{
  /* Try SEEK_HOLE only if this heuristic
     suggests the file is sparse.  */
  ଯଦି (! (HAVE_STRUCT_STAT_ST_BLOCKS
         && S_ISREG (sb->st_mode)
         && STP_NBLOCKS (sb) < sb->st_size / ST_NBLOCKSIZE))
    ଫେରନ୍ତୁ PLAIN_SCANTYPE;

#ifdef SEEK_HOLE
  scan_inference->ext_start = lseek (fd, pos, SEEK_DATA);
  ଯଦି (scan_inference->ext_start == pos)
    {
      scan_inference->hole_start = lseek (fd, pos, SEEK_HOLE);
      ଯଦି (0 <= scan_inference->hole_start)
        {
          ଯଦି (scan_inference->hole_start < sb->st_size)
            ଫେରନ୍ତୁ LSEEK_SCANTYPE;

          /* Though the file may have holes, SEEK_DATA and SEEK_HOLE
             didn't find any.  This can happen with file systems
             that support SEEK_HOLE only trivially,
             such as squashfs in Linux kernel 6.17 and earlier.
             This can also happen due to transparent file compression,
             which can also indicate fewer than the usual number of blocks.  */

          ଯଦି (lseek (fd, pos, SEEK_SET) < 0)
            ଫେରନ୍ତୁ ERROR_SCANTYPE;

          /* we prefer to return PLAIN_SCANTYPE here so that copy offload
             continues to be used.  Falling through to ZERO_SCANTYPE would be
             less efficient in the compressed file case.  */
          ଫେରନ୍ତୁ PLAIN_SCANTYPE;
        }
    }
  ଅନ୍ୟଥା ଯଦି (pos < scan_inference->ext_start || errno == ENXIO)
    {
      scan_inference->hole_start = pos;  /* Pacify -Wmaybe-uninitialized.  */
      ଫେରନ୍ତୁ LSEEK_SCANTYPE;
    }
  ଅନ୍ୟଥା ଯଦି (errno != EINVAL && !is_ENOTSUP (errno))
    ଫେରନ୍ତୁ ERROR_SCANTYPE;
#endif

  ଫେରନ୍ତୁ ZERO_SCANTYPE;
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

ବାହ୍ୟ intmax_t
copy_file_data (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ifd, ସଂରଚନା stat ଅଚଳ *ist, off_t ipos, ଅକ୍ଷର ଅଚଳ *iname,
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା ofd, ସଂରଚନା stat ଅଚଳ *ost, off_t opos, ଅକ୍ଷର ଅଚଳ *oname,
                ଗଣନା_t ibytes, ସଂରଚନା cp_options ଅଚଳ *x,
                ସଂରଚନା copy_debug *debug)
{
  /* Choose a suitable buffer size; it may be adjusted later.  */
  idx_t buf_size = io_blksize (ost);

  /* Deal with sparse files.  */
  ସଂରଚନା scan_inference scan_inference;
  ଗଣନା scantype scantype = infer_scantype (ifd, ist, ipos, &scan_inference);
  ଯଦି (scantype == ERROR_SCANTYPE)
    {
      error (0, errno, _("cannot lseek %s"), quoteaf (iname));
      ଫେରନ୍ତୁ -1;
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
  ଯଦି (ipos == 0 && ibytes == ଗଣନା_MAX
      && (x->reflink_mode != REFLINK_AUTO || x->sparse_mode != SPARSE_AUTO))
    fdadvise (ifd, 0, 0, FADVISE_SEQUENTIAL);

  /* If not making a sparse file, try to use a more-efficient
     buffer size.  */
  ଯଦି (! make_holes)
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
      ଯଦି (S_ISREG (ist->st_mode) && 0 <= ist->st_size
          && ist->st_size < buf_size)
        buf_size = ist->st_size + 1;

      /* However, stick with a block size that is a positive multiple of
         blcm, overriding the above adjustments.  Watch out for
         overflow.  */
      buf_size = ckd_add (&buf_size, buf_size, blcm - 1) ? IDX_MAX : buf_size;
      buf_size -= buf_size % blcm;
    }

  ଅକ୍ଷର *buf = ଖାଲି;
  intmax_t result;
  off_t hole_size = 0;

  ଯଦି (scantype == LSEEK_SCANTYPE)
    {
#ifdef SEEK_HOLE
      result = lseek_copy (ifd, ofd, &buf, buf_size,
                           ipos, ibytes, &scan_inference, ist->st_size,
                           make_holes ? x->sparse_mode : SPARSE_NEVER,
                           x->reflink_mode != REFLINK_NEVER,
                           iname, oname, &hole_size, debug);
#ଅନ୍ୟଥା
      affirm (false);
#endif
    }
  ଅନ୍ୟଥା
    result = sparse_copy (ifd, ofd, &buf, buf_size,
                          x->reflink_mode != REFLINK_NEVER,
                          iname, oname, ibytes,
                          make_holes ? &hole_size : ଖାଲି,
                          debug);

  ଯଦି (0 <= result && 0 < hole_size)
    {
      off_t oend;
      ଯଦି (ckd_add (&oend, opos, result)
          ? (errno = EOVERFLOW, true)
          : make_holes
          ? ftruncate (ofd, oend) < 0
          : !write_zeros (ofd, hole_size, &buf, buf_size))
        {
          error (0, errno, _("failed to extend %s"), quoteaf (oname));
          result = -1;
        }
      ଅନ୍ୟଥା ଯଦି (make_holes
               && punch_hole (ofd, oend - hole_size, hole_size) < 0)
        {
          error (0, errno, _("error deallocating %s"), quoteaf (oname));
          result = -1;
        }
    }

  alignfree (buf);
  ଫେରନ୍ତୁ result;
}
