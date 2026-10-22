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

#अन्तर्भाव <config.h>

#अन्तर्भाव "system.h"

#अन्तर्भाव "alignalloc.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "backupfile.h"
#अन्तर्भाव "buffer-lcm.h"
#अन्तर्भाव "copy.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "full-write.h"
#अन्तर्भाव "ioblksize.h"

/* Result of infer_scantype when it returns LSEEK_SCANTYPE.  */
संरचना अवलोकनम्_inference
{
  /* The offset of the first data block, or -1 if the file has no data.  */
  off_t ext_start;

  /* The offset of the first hole.  */
  off_t hole_start;
};

/* Attempt to punch a hole to avoid any permanent
   speculative preallocation on file systems such as XFS.
   Return values as per fallocate(2) except ENOSYS etc. are ignored.  */
स्थिर पूर्णाङ्क
punch_hole (पूर्णाङ्क fd, off_t विस्थापनम्, off_t दीर्घता)
{
  पूर्णाङ्क ret = 0;
/* +0 is to work around older <linux/fs.h> defining HAVE_FALLOCATE to empty.  */
#यदि HAVE_FALLOCATE + 0
# यदि defined FALLOC_FL_PUNCH_HOLE && defined FALLOC_FL_KEEP_आकार
  ret = fallocate (fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_आकार,
                   विस्थापनम्, दीर्घता);
  यदि (ret < 0 && (is_ENOTSUP (errno) || errno == ENOSYS))
    ret = 0;
# endif
#यदि_अन्तः
  प्रत्यागम ret;
}

/* Create a hole at the end of the file with descriptor FD and name NAME.
   The hole is of size SIZE.  Assume FD is already at file end,
   and advance FD past the newly-created hole.
   Return the resulting position, or -1 on failure.  */
स्थिर off_t
create_hole (पूर्णाङ्क fd, वर्ण ध्रुव *name, off_t आकार)
{
  off_t file_अन्तः = lseek (fd, आकार, SEEK_CUR);

  यदि (file_अन्तः < 0)
    {
      error (0, errno, _("cannot lseek %s"), quoteaf (name));
      प्रत्यागम -1;
    }

  /* Some file systems (like XFS) preallocate when write extending a file.
     I.e., a previous write() may have preallocated extra space
     that the seek above will not discard.  A subsequent write() could
     then make this allocation permanent.  */
  यदि (punch_hole (fd, file_अन्तः - आकार, आकार) < 0)
    {
      error (0, errno, _("error deallocating %s"), quoteaf (name));
      प्रत्यागम -1;
    }

  प्रत्यागम file_अन्तः;
}

/* Does ERR mean the copying operation is not supported or allowed for
   this file or process, even though the operation was invoked correctly?  */
स्थिर bool
is_CLONENOTSUP (पूर्णाङ्क err)
{
  प्रत्यागम err == ENOSYS || err == ENOTTY || is_ENOTSUP (err)
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
स्थिर intmax_t
sparse_copy (पूर्णाङ्क src_fd, पूर्णाङ्क dest_fd, वर्ण **abuf, idx_t buf_आकार,
             bool allow_reflink,
             वर्ण ध्रुव *src_name, वर्ण ध्रुव *dst_name,
             गणनम्_t max_n_read, off_t *hole_आकार,
             संरचना copy_debug *debug)
{
  गणनम्_t योग_n_read = 0;

  यदि (debug->sparse_detection == COPY_DEBUG_UNKNOWN)
    debug->sparse_detection = hole_आकार ? COPY_DEBUG_YES : COPY_DEBUG_NO;
  अन्यथा यदि (hole_आकार && debug->sparse_detection == COPY_DEBUG_EXTERNAL)
    debug->sparse_detection = COPY_DEBUG_EXTERNAL_आन्तरिकम्;

  /* If not looking for holes, use copy_file_range if functional,
     but don't use if reflink disallowed as that may be implicit.  */
  यदि (!hole_आकार && allow_reflink)
    यावत् (0 < max_n_read)
      {
        /* Copy at most COPY_MAX bytes at a time; this is min
           (SSIZE_MAX, SIZE_MAX) truncated to a value that is
           surely aligned well.  */
        ssize_t copy_max = MIN (SSIZE_MAX, SIZE_MAX) >> 30 << 30;
        ssize_t n_copied = copy_file_range (src_fd, रिक्त, dest_fd, रिक्त,
                                            MIN (max_n_read, copy_max), 0);
        यदि (n_copied == 0)
          {
            /* copy_file_range incorrectly returns 0 when reading from
               the proc file system on the Linux kernel through at
               least 5.6.19 (2020), so fall back on 'read' if the
               input file seems empty.  */
            यदि (योग_n_read == 0)
              भङ्ग;
            debug->offload = COPY_DEBUG_YES;
            प्रत्यागम योग_n_read;
          }
        यदि (n_copied < 0)
          {
            debug->offload = errno;

            /* Don’t treat EFBIG as a reportable error from copy_file_range.
               If the input is at EOF and the output position is 2**63 - 1,
               copy_file_range (ifd, NULL, ofd, NULL, 2146435072, 0)
               incorrectly fails with EFBIG.  Problem observed on Ubuntu 25.10
               x86-64 with Linux kernel 6.17.0-8-generic #8-Ubuntu.  */
            यदि (errno == EFBIG)
              भङ्ग;

            /* Consider operation unsupported only if no data copied.
               For example, EPERM could occur if copy_file_range not enabled
               in seccomp filters, so retry with a standard copy.  EPERM can
               also occur for immutable files, but that would only be in the
               edge case where the file is made immutable after creating,
               in which case the (more accurate) error is still shown.  */
            यदि (योग_n_read == 0 && is_CLONENOTSUP (errno))
              भङ्ग;

            /* ENOENT was seen sometimes across CIFS shares, resulting in
               no data being copied, but subsequent standard copies succeed.  */
            यदि (योग_n_read == 0 && errno == ENOENT)
              भङ्ग;

            यदि (errno == EINTR)
              n_copied = 0;
            अन्यथा
              {
                error (0, errno, _("error copying %s to %s"),
                       quoteaf_n (0, src_name), quoteaf_n (1, dst_name));
                प्रत्यागम -1;
              }
          }
        debug->offload = COPY_DEBUG_YES;
        max_n_read -= n_copied;
        योग_n_read += n_copied;
      }
  अन्यथा
    debug->offload = COPY_DEBUG_AVOIDED;


  off_t psize = hole_आकार ? *hole_आकार : 0;
  bool make_hole = !!psize;

  यावत् (0 < max_n_read)
    {
      यदि (!*abuf)
        *abuf = xalignalloc (getpagesize (), buf_आकार);
      वर्ण *buf = *abuf;
      ssize_t n_read = पठ (src_fd, buf, MIN (max_n_read, buf_आकार));
      यदि (n_read < 0)
        {
          यदि (errno == EINTR)
            अनुवर्त;
          error (0, errno, _("error reading %s"), quoteaf (src_name));
          प्रत्यागम -1;
        }
      यदि (n_read == 0)
        भङ्ग;
      max_n_read -= n_read;
      योग_n_read += n_read;

      /* If looking for holes, loop over the input buffer in chunks
         corresponding to the minimum hole size.  Otherwise, scan the
         whole buffer.  struct stat does not report the minimum hole
         size for a file, so use ST_NBLOCKSIZE which should be the
         minimum for all file systems on this platform.  */
      idx_t csize = hole_आकार ? ST_NBLOCKSIZE : buf_आकार;
      वर्ण *cbuf = buf;
      वर्ण *pbuf = buf;

      यावत् (n_read)
        {
          bool पूर्व_hole = make_hole;
          csize = MIN (csize, n_read);

          यदि (hole_आकार)
            make_hole = is_nul (cbuf, csize);

          bool transition = (make_hole != पूर्व_hole) && psize;
          bool चरमः_chunk = n_read == csize && !make_hole;

          यदि (transition || चरमः_chunk)
            {
              यदि (! transition)
                psize += csize;
              अन्यथा यदि (पूर्व_hole)
                {
                  यदि (create_hole (dest_fd, dst_name, psize) < 0)
                    प्रत्यागम false;
                  pbuf = cbuf;
                  psize = csize;
                }

              यदि (!पूर्व_hole || (transition && चरमः_chunk))
                {
                  यदि (पूर्णम्_write (dest_fd, pbuf, psize) != psize)
                    {
                      error (0, errno, _("error writing %s"),
                             quoteaf (dst_name));
                      प्रत्यागम -1;
                    }
                  psize = !पूर्व_hole && transition ? csize : 0;
                }
            }
          अन्यथा  /* Coalesce writes/seeks.  */
            {
              यदि (ckd_सङ्कलय (&psize, psize, csize))
                {
                  error (0, 0, _("overflow reading %s"), quoteaf (src_name));
                  प्रत्यागम -1;
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

  यदि (hole_आकार)
    *hole_आकार = make_hole ? psize : 0;
  प्रत्यागम योग_n_read;
}

/* Write to FD.  Write data consisting of N_BYTES' worth of '\0's.
   Use the buffer *ABUF of size BUF_SIZE for temporary storage,
   allocating the buffer lazily if *ABUF is null.
   Return true if successful, false (setting errno) otherwise.  */
स्थिर bool
write_zeros (पूर्णाङ्क fd, off_t n_अष्टकानि, वर्ण **abuf, idx_t buf_आकार)
{
  वर्ण *zeros = रिक्त;
  यावत् (n_अष्टकानि)
    {
      idx_t n = MIN (buf_आकार, n_अष्टकानि);
      यदि (!zeros)
        {
          यदि (!*abuf)
            *abuf = xalignalloc (getpagesize (), buf_आकार);
          zeros = स्मृतिं_पूरय (*abuf, 0, n);
        }
      यदि (पूर्णम्_write (fd, zeros, n) != n)
        प्रत्यागम false;
      n_अष्टकानि -= n;
    }

  प्रत्यागम true;
}

#यदि_परिभाषितम् SEEK_HOLE
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

स्थिर off_t
lseek_copy (पूर्णाङ्क src_fd, पूर्णाङ्क dest_fd, वर्ण **abuf, idx_t buf_आकार,
            off_t src_pos, गणनम्_t ibytes,
            संरचना अवलोकनम्_inference ध्रुव *अवलोकनम्_inference, off_t src_योग_आकार,
            गणना Sparse_type sparse_बहुलकम्,
            bool allow_reflink,
            वर्ण ध्रुव *src_name, वर्ण ध्रुव *dst_name,
            off_t *hole_आकार, संरचना copy_debug *debug)
{
  off_t चरमः_ext_start = src_pos;
  off_t चरमः_ext_len = 0;
  off_t max_ipos = ckd_सङ्कलय (&max_ipos, src_pos, ibytes) ? OFF_T_MAX : max_ipos;

  /* From here on, SRC_TOTAL_SIZE is bounded above by MAX_IPOS.  */
  src_योग_आकार = MIN (src_योग_आकार, max_ipos);

  /* The input position after the most recent read of data,
     or SRC_POS if no data have been read.  */
  off_t ipos = src_pos;

  debug->sparse_detection = COPY_DEBUG_EXTERNAL;

  bool प्रयुक्तम्_अवलोकनम्_inference = false;

  प्रति (off_t ext_start = अवलोकनम्_inference->ext_start;
       0 <= ext_start && ext_start < max_ipos; )
    {
      off_t ext_अन्तः;
      यदि (ext_start == src_pos && ! प्रयुक्तम्_अवलोकनम्_inference)
        {
          ext_अन्तः = अवलोकनम्_inference->hole_start;
          प्रयुक्तम्_अवलोकनम्_inference = true;
        }
      अन्यथा
        ext_अन्तः = lseek (src_fd, ext_start, SEEK_HOLE);
      यदि (0 <= ext_अन्तः)
        ext_अन्तः = MIN (ext_अन्तः, max_ipos);
      अन्यथा
        {
          यदि (errno != ENXIO)
            गच्छ cannot_lseek;
          ext_अन्तः = src_योग_आकार;
          यदि (ext_अन्तः <= ext_start)
            {
              /* The input file grew; get its current size.  */
              src_योग_आकार = lseek (src_fd, 0, SEEK_END);
              यदि (src_योग_आकार < 0)
                गच्छ cannot_lseek;
              src_योग_आकार = MIN (src_योग_आकार, max_ipos);

              /* If the input file shrank after growing, stop copying.  */
              यदि (src_योग_आकार <= ext_start)
                भङ्ग;

              ext_अन्तः = src_योग_आकार;
            }
        }
      /* If the input file must have grown, increase its measured size.  */
      यदि (src_योग_आकार < ext_अन्तः)
        src_योग_आकार = ext_अन्तः;

      यदि (lseek (src_fd, ext_start, SEEK_SET) < 0)
        गच्छ cannot_lseek;

      off_t ext_hole_आकार = ext_start - चरमः_ext_start - चरमः_ext_len;

      यदि (ext_hole_आकार)
        {
          यदि (sparse_बहुलकम् == SPARSE_ALWAYS)
            *hole_आकार += ext_hole_आकार;
          अन्यथा यदि (sparse_बहुलकम् != SPARSE_NEVER)
            {
              off_t epos = create_hole (dest_fd, dst_name, ext_hole_आकार);
              यदि (epos < 0)
                प्रत्यागम epos;
            }
          अन्यथा
            {
              /* When not inducing holes and when there is a hole between
                 the end of the previous extent and the beginning of the
                 current one, write zeros to the destination file.  */
              यदि (! write_zeros (dest_fd, ext_hole_आकार, abuf, buf_आकार))
                {
                  error (0, errno, _("%s: write failed"),
                         quotef (dst_name));
                  प्रत्यागम -1;
                }
            }
        }

      off_t ext_len = ext_अन्तः - ext_start;
      चरमः_ext_start = ext_start;
      चरमः_ext_len = ext_len;

      /* Copy this extent, looking for further opportunities to not
         bother to write zeros if --sparse=always, since SEEK_HOLE
         is conservative and may miss some holes.  */
      off_t n_read
        = sparse_copy (src_fd, dest_fd, abuf, buf_आकार,
                       allow_reflink, src_name, dst_name,
                       ext_len,
                       sparse_बहुलकम् == SPARSE_ALWAYS ? hole_आकार : रिक्त,
                       debug);
      यदि (n_read < 0)
        प्रत्यागम -1;

      ipos = ext_start + n_read;
      यदि (n_read < ext_len)
        {
          /* The input file shrank.  */
          src_योग_आकार = ipos;
          भङ्ग;
        }

      ext_start = lseek (src_fd, ipos, SEEK_दत्त);
      यदि (ext_start < 0 && errno != ENXIO)
        गच्छ cannot_lseek;
    }

  *hole_आकार += src_योग_आकार - (चरमः_ext_start + चरमः_ext_len);
  प्रत्यागम src_योग_आकार - src_pos;

 cannot_lseek:
  error (0, errno, _("cannot lseek %s"), quoteaf (src_name));
  प्रत्यागम -1;
}
#यदि_अन्तः

#यदि_अपरिभाषितम् HAVE_STRUCT_STAT_ST_खण्डानि
# define HAVE_STRUCT_STAT_ST_खण्डानि 0
#यदि_अन्तः

/* Type of scan being done on the input when looking for sparseness.  */
गणना scantype
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
स्थिर गणना scantype
infer_scantype (पूर्णाङ्क fd, संरचना stat ध्रुव *sb, off_t pos,
                संरचना अवलोकनम्_inference *अवलोकनम्_inference)
{
  /* Try SEEK_HOLE only if this heuristic
     suggests the file is sparse.  */
  यदि (! (HAVE_STRUCT_STAT_ST_खण्डानि
         && S_ISREG (sb->st_बहुलकम्)
         && STP_NBLOCKS (sb) < sb->st_आकार / ST_NBLOCKSIZE))
    प्रत्यागम PLAIN_SCANTYPE;

#यदि_परिभाषितम् SEEK_HOLE
  अवलोकनम्_inference->ext_start = lseek (fd, pos, SEEK_दत्त);
  यदि (अवलोकनम्_inference->ext_start == pos)
    {
      अवलोकनम्_inference->hole_start = lseek (fd, pos, SEEK_HOLE);
      यदि (0 <= अवलोकनम्_inference->hole_start)
        {
          यदि (अवलोकनम्_inference->hole_start < sb->st_आकार)
            प्रत्यागम LSEEK_SCANTYPE;

          /* Though the file may have holes, SEEK_DATA and SEEK_HOLE
             didn't find any.  This can happen with file systems
             that support SEEK_HOLE only trivially,
             such as squashfs in Linux kernel 6.17 and earlier.
             This can also happen due to transparent file compression,
             which can also indicate fewer than the usual number of blocks.  */

          यदि (lseek (fd, pos, SEEK_SET) < 0)
            प्रत्यागम ERROR_SCANTYPE;

          /* we prefer to return PLAIN_SCANTYPE here so that copy offload
             continues to be used.  Falling through to ZERO_SCANTYPE would be
             less efficient in the compressed file case.  */
          प्रत्यागम PLAIN_SCANTYPE;
        }
    }
  अन्यथा यदि (pos < अवलोकनम्_inference->ext_start || errno == ENXIO)
    {
      अवलोकनम्_inference->hole_start = pos;  /* Pacify -Wmaybe-uninitialized.  */
      प्रत्यागम LSEEK_SCANTYPE;
    }
  अन्यथा यदि (errno != EINVAL && !is_ENOTSUP (errno))
    प्रत्यागम ERROR_SCANTYPE;
#यदि_अन्तः

  प्रत्यागम ZERO_SCANTYPE;
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

बाह्य intmax_t
copy_file_दत्त (पूर्णाङ्क ifd, संरचना stat ध्रुव *ist, off_t ipos, वर्ण ध्रुव *iname,
                पूर्णाङ्क ofd, संरचना stat ध्रुव *ost, off_t opos, वर्ण ध्रुव *oname,
                गणनम्_t ibytes, संरचना cp_options ध्रुव *x,
                संरचना copy_debug *debug)
{
  /* Choose a suitable buffer size; it may be adjusted later.  */
  idx_t buf_आकार = io_blksize (ost);

  /* Deal with sparse files.  */
  संरचना अवलोकनम्_inference अवलोकनम्_inference;
  गणना scantype scantype = infer_scantype (ifd, ist, ipos, &अवलोकनम्_inference);
  यदि (scantype == ERROR_SCANTYPE)
    {
      error (0, errno, _("cannot lseek %s"), quoteaf (iname));
      प्रत्यागम -1;
    }
  bool make_holes
    = (S_ISREG (ost->st_बहुलकम्)
       && (x->sparse_बहुलकम् == SPARSE_ALWAYS
           || (x->sparse_बहुलकम् == SPARSE_AUTO
               && scantype != PLAIN_SCANTYPE)));

  /* If we _know_ we're going to read data sequentially into the process,
     i.e., --reflink or --sparse are not in auto mode,
     give that hint to the kernel so it can tune caching behavior.
     Also we don't bother calling fadvise for small copies,
     as it is not likely to help performance and might even hurt it.
     Also we only apply this hint for the whole file (0 length)
     as OpenZFS 2.2.2 at least will otherwise synchronously
     (decompress and) populate the cache when given a specific length.  */
  यदि (ipos == 0 && ibytes == गणनम्_MAX
      && (x->reflink_बहुलकम् != REFLINK_AUTO || x->sparse_बहुलकम् != SPARSE_AUTO))
    fdadvise (ifd, 0, 0, FADVISE_SEQUENTIAL);

  /* If not making a sparse file, try to use a more-efficient
     buffer size.  */
  यदि (! make_holes)
    {
      /* Compute the least common multiple of the input and output
         buffer sizes, adjusting for outlandish values.
         blcm is at most IDX_MAX - 1 so that buf_size cannot become 0 below.
         Note we read in multiples of the reported block size
         to support (unusual) devices that have this constraint.  */
      idx_t blcm_max = MIN (MIN (IDX_MAX - 1, SSIZE_MAX), SIZE_MAX);
      idx_t blcm = बफर_lcm (io_blksize (ist), buf_आकार,
                               blcm_max);

      /* Do not bother with a buffer larger than the input file, plus one
         byte to make sure the file has not grown while reading it.  */
      यदि (S_ISREG (ist->st_बहुलकम्) && 0 <= ist->st_आकार
          && ist->st_आकार < buf_आकार)
        buf_आकार = ist->st_आकार + 1;

      /* However, stick with a block size that is a positive multiple of
         blcm, overriding the above adjustments.  Watch out for
         overflow.  */
      buf_आकार = ckd_सङ्कलय (&buf_आकार, buf_आकार, blcm - 1) ? IDX_MAX : buf_आकार;
      buf_आकार -= buf_आकार % blcm;
    }

  वर्ण *buf = रिक्त;
  intmax_t परिणाम;
  off_t hole_आकार = 0;

  यदि (scantype == LSEEK_SCANTYPE)
    {
#यदि_परिभाषितम् SEEK_HOLE
      परिणाम = lseek_copy (ifd, ofd, &buf, buf_आकार,
                           ipos, ibytes, &अवलोकनम्_inference, ist->st_आकार,
                           make_holes ? x->sparse_बहुलकम् : SPARSE_NEVER,
                           x->reflink_बहुलकम् != REFLINK_NEVER,
                           iname, oname, &hole_आकार, debug);
#अन्यथा
      affirm (false);
#यदि_अन्तः
    }
  अन्यथा
    परिणाम = sparse_copy (ifd, ofd, &buf, buf_आकार,
                          x->reflink_बहुलकम् != REFLINK_NEVER,
                          iname, oname, ibytes,
                          make_holes ? &hole_आकार : रिक्त,
                          debug);

  यदि (0 <= परिणाम && 0 < hole_आकार)
    {
      off_t oend;
      यदि (ckd_सङ्कलय (&oend, opos, परिणाम)
          ? (errno = EOVERFLOW, true)
          : make_holes
          ? ftruncate (ofd, oend) < 0
          : !write_zeros (ofd, hole_आकार, &buf, buf_आकार))
        {
          error (0, errno, _("failed to extend %s"), quoteaf (oname));
          परिणाम = -1;
        }
      अन्यथा यदि (make_holes
               && punch_hole (ofd, oend - hole_आकार, hole_आकार) < 0)
        {
          error (0, errno, _("error deallocating %s"), quoteaf (oname));
          परिणाम = -1;
        }
    }

  alignfree (buf);
  प्रत्यागम परिणाम;
}
