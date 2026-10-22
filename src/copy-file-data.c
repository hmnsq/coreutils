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

#అన్తర్భావ <config.h>

#అన్తర్భావ "system.h"

#అన్తర్భావ "alignalloc.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "backupfile.h"
#అన్తర్భావ "buffer-lcm.h"
#అన్తర్భావ "copy.h"
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "full-write.h"
#అన్తర్భావ "ioblksize.h"

/* Result of infer_scantype when it returns LSEEK_SCANTYPE.  */
నిర్మాణం పరిశీలన_inference
{
  /* The offset of the first data block, or -1 if the file has no data.  */
  off_t ext_ప్రారంభం;

  /* The offset of the first hole.  */
  off_t hole_ప్రారంభం;
};

/* Attempt to punch a hole to avoid any permanent
   speculative preallocation on file systems such as XFS.
   Return values as per fallocate(2) except ENOSYS etc. are ignored.  */
స్థిర పూర్ణాంకం
punch_hole (పూర్ణాంకం fd, off_t స్థానభ్రంశం, off_t నిడివి)
{
  పూర్ణాంకం ret = 0;
/* +0 is to work around older <linux/fs.h> defining HAVE_FALLOCATE to empty.  */
#ఒకవేళ HAVE_FALLOCATE + 0
# ఒకవేళ defined FALLOC_FL_PUNCH_HOLE && defined FALLOC_FL_KEEP_కొలత
  ret = fallocate (fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_కొలత,
                   స్థానభ్రంశం, నిడివి);
  ఒకవేళ (ret < 0 && (is_ENOTSUP (errno) || errno == ENOSYS))
    ret = 0;
# endif
#అయితే_ముగింపు
  తిరిగి ret;
}

/* Create a hole at the end of the file with descriptor FD and name NAME.
   The hole is of size SIZE.  Assume FD is already at file end,
   and advance FD past the newly-created hole.
   Return the resulting position, or -1 on failure.  */
స్థిర off_t
create_hole (పూర్ణాంకం fd, అక్షరం స్థిరరాశి *name, off_t కొలత)
{
  off_t file_చివర = lseek (fd, కొలత, SEEK_CUR);

  ఒకవేళ (file_చివర < 0)
    {
      error (0, errno, _("cannot lseek %s"), quoteaf (name));
      తిరిగి -1;
    }

  /* Some file systems (like XFS) preallocate when write extending a file.
     I.e., a previous write() may have preallocated extra space
     that the seek above will not discard.  A subsequent write() could
     then make this allocation permanent.  */
  ఒకవేళ (punch_hole (fd, file_చివర - కొలత, కొలత) < 0)
    {
      error (0, errno, _("error deallocating %s"), quoteaf (name));
      తిరిగి -1;
    }

  తిరిగి file_చివర;
}

/* Does ERR mean the copying operation is not supported or allowed for
   this file or process, even though the operation was invoked correctly?  */
స్థిర bool
is_CLONENOTSUP (పూర్ణాంకం err)
{
  తిరిగి err == ENOSYS || err == ENOTTY || is_ENOTSUP (err)
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
స్థిర intmax_t
sparse_copy (పూర్ణాంకం src_fd, పూర్ణాంకం dest_fd, అక్షరం **abuf, idx_t buf_కొలత,
             bool allow_reflink,
             అక్షరం స్థిరరాశి *src_name, అక్షరం స్థిరరాశి *dst_name,
             లెక్క_t max_n_read, off_t *hole_కొలత,
             నిర్మాణం copy_debug *debug)
{
  లెక్క_t మొత్తం_n_read = 0;

  ఒకవేళ (debug->sparse_detection == COPY_DEBUG_UNKNOWN)
    debug->sparse_detection = hole_కొలత ? COPY_DEBUG_YES : COPY_DEBUG_NO;
  లేకపోతే ఒకవేళ (hole_కొలత && debug->sparse_detection == COPY_DEBUG_EXTERNAL)
    debug->sparse_detection = COPY_DEBUG_EXTERNAL_అంతర్గత;

  /* If not looking for holes, use copy_file_range if functional,
     but don't use if reflink disallowed as that may be implicit.  */
  ఒకవేళ (!hole_కొలత && allow_reflink)
    అయితే (0 < max_n_read)
      {
        /* Copy at most COPY_MAX bytes at a time; this is min
           (SSIZE_MAX, SIZE_MAX) truncated to a value that is
           surely aligned well.  */
        ssize_t copy_max = MIN (SSIZE_MAX, SIZE_MAX) >> 30 << 30;
        ssize_t n_copied = copy_file_range (src_fd, ఖాళీ, dest_fd, ఖాళీ,
                                            MIN (max_n_read, copy_max), 0);
        ఒకవేళ (n_copied == 0)
          {
            /* copy_file_range incorrectly returns 0 when reading from
               the proc file system on the Linux kernel through at
               least 5.6.19 (2020), so fall back on 'read' if the
               input file seems empty.  */
            ఒకవేళ (మొత్తం_n_read == 0)
              ఆపు;
            debug->offload = COPY_DEBUG_YES;
            తిరిగి మొత్తం_n_read;
          }
        ఒకవేళ (n_copied < 0)
          {
            debug->offload = errno;

            /* Don’t treat EFBIG as a reportable error from copy_file_range.
               If the input is at EOF and the output position is 2**63 - 1,
               copy_file_range (ifd, NULL, ofd, NULL, 2146435072, 0)
               incorrectly fails with EFBIG.  Problem observed on Ubuntu 25.10
               x86-64 with Linux kernel 6.17.0-8-generic #8-Ubuntu.  */
            ఒకవేళ (errno == EFBIG)
              ఆపు;

            /* Consider operation unsupported only if no data copied.
               For example, EPERM could occur if copy_file_range not enabled
               in seccomp filters, so retry with a standard copy.  EPERM can
               also occur for immutable files, but that would only be in the
               edge case where the file is made immutable after creating,
               in which case the (more accurate) error is still shown.  */
            ఒకవేళ (మొత్తం_n_read == 0 && is_CLONENOTSUP (errno))
              ఆపు;

            /* ENOENT was seen sometimes across CIFS shares, resulting in
               no data being copied, but subsequent standard copies succeed.  */
            ఒకవేళ (మొత్తం_n_read == 0 && errno == ENOENT)
              ఆపు;

            ఒకవేళ (errno == EINTR)
              n_copied = 0;
            లేకపోతే
              {
                error (0, errno, _("error copying %s to %s"),
                       quoteaf_n (0, src_name), quoteaf_n (1, dst_name));
                తిరిగి -1;
              }
          }
        debug->offload = COPY_DEBUG_YES;
        max_n_read -= n_copied;
        మొత్తం_n_read += n_copied;
      }
  లేకపోతే
    debug->offload = COPY_DEBUG_AVOIDED;


  off_t psize = hole_కొలత ? *hole_కొలత : 0;
  bool make_hole = !!psize;

  అయితే (0 < max_n_read)
    {
      ఒకవేళ (!*abuf)
        *abuf = xalignalloc (getpagesize (), buf_కొలత);
      అక్షరం *buf = *abuf;
      ssize_t n_read = పఠ (src_fd, buf, MIN (max_n_read, buf_కొలత));
      ఒకవేళ (n_read < 0)
        {
          ఒకవేళ (errno == EINTR)
            కొనసాగు;
          error (0, errno, _("error reading %s"), quoteaf (src_name));
          తిరిగి -1;
        }
      ఒకవేళ (n_read == 0)
        ఆపు;
      max_n_read -= n_read;
      మొత్తం_n_read += n_read;

      /* If looking for holes, loop over the input buffer in chunks
         corresponding to the minimum hole size.  Otherwise, scan the
         whole buffer.  struct stat does not report the minimum hole
         size for a file, so use ST_NBLOCKSIZE which should be the
         minimum for all file systems on this platform.  */
      idx_t csize = hole_కొలత ? ST_NBLOCKSIZE : buf_కొలత;
      అక్షరం *cbuf = buf;
      అక్షరం *pbuf = buf;

      అయితే (n_read)
        {
          bool prev_hole = make_hole;
          csize = MIN (csize, n_read);

          ఒకవేళ (hole_కొలత)
            make_hole = is_nul (cbuf, csize);

          bool transition = (make_hole != prev_hole) && psize;
          bool చివరి_chunk = n_read == csize && !make_hole;

          ఒకవేళ (transition || చివరి_chunk)
            {
              ఒకవేళ (! transition)
                psize += csize;
              లేకపోతే ఒకవేళ (prev_hole)
                {
                  ఒకవేళ (create_hole (dest_fd, dst_name, psize) < 0)
                    తిరిగి false;
                  pbuf = cbuf;
                  psize = csize;
                }

              ఒకవేళ (!prev_hole || (transition && చివరి_chunk))
                {
                  ఒకవేళ (నిండిన_write (dest_fd, pbuf, psize) != psize)
                    {
                      error (0, errno, _("error writing %s"),
                             quoteaf (dst_name));
                      తిరిగి -1;
                    }
                  psize = !prev_hole && transition ? csize : 0;
                }
            }
          లేకపోతే  /* Coalesce writes/seeks.  */
            {
              ఒకవేళ (ckd_కూడు (&psize, psize, csize))
                {
                  error (0, 0, _("overflow reading %s"), quoteaf (src_name));
                  తిరిగి -1;
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

  ఒకవేళ (hole_కొలత)
    *hole_కొలత = make_hole ? psize : 0;
  తిరిగి మొత్తం_n_read;
}

/* Write to FD.  Write data consisting of N_BYTES' worth of '\0's.
   Use the buffer *ABUF of size BUF_SIZE for temporary storage,
   allocating the buffer lazily if *ABUF is null.
   Return true if successful, false (setting errno) otherwise.  */
స్థిర bool
write_zeros (పూర్ణాంకం fd, off_t n_బైట్లు, అక్షరం **abuf, idx_t buf_కొలత)
{
  అక్షరం *zeros = ఖాళీ;
  అయితే (n_బైట్లు)
    {
      idx_t n = MIN (buf_కొలత, n_బైట్లు);
      ఒకవేళ (!zeros)
        {
          ఒకవేళ (!*abuf)
            *abuf = xalignalloc (getpagesize (), buf_కొలత);
          zeros = memset (*abuf, 0, n);
        }
      ఒకవేళ (నిండిన_write (fd, zeros, n) != n)
        తిరిగి false;
      n_బైట్లు -= n;
    }

  తిరిగి true;
}

#నిర్వచితమైతే SEEK_HOLE
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

స్థిర off_t
lseek_copy (పూర్ణాంకం src_fd, పూర్ణాంకం dest_fd, అక్షరం **abuf, idx_t buf_కొలత,
            off_t src_pos, లెక్క_t ibytes,
            నిర్మాణం పరిశీలన_inference స్థిరరాశి *పరిశీలన_inference, off_t src_మొత్తం_కొలత,
            గణన Sparse_type sparse_బహుళకం,
            bool allow_reflink,
            అక్షరం స్థిరరాశి *src_name, అక్షరం స్థిరరాశి *dst_name,
            off_t *hole_కొలత, నిర్మాణం copy_debug *debug)
{
  off_t చివరి_ext_ప్రారంభం = src_pos;
  off_t చివరి_ext_len = 0;
  off_t max_ipos = ckd_కూడు (&max_ipos, src_pos, ibytes) ? OFF_T_MAX : max_ipos;

  /* From here on, SRC_TOTAL_SIZE is bounded above by MAX_IPOS.  */
  src_మొత్తం_కొలత = MIN (src_మొత్తం_కొలత, max_ipos);

  /* The input position after the most recent read of data,
     or SRC_POS if no data have been read.  */
  off_t ipos = src_pos;

  debug->sparse_detection = COPY_DEBUG_EXTERNAL;

  bool వాడిన_పరిశీలన_inference = false;

  కొరకు (off_t ext_ప్రారంభం = పరిశీలన_inference->ext_ప్రారంభం;
       0 <= ext_ప్రారంభం && ext_ప్రారంభం < max_ipos; )
    {
      off_t ext_చివర;
      ఒకవేళ (ext_ప్రారంభం == src_pos && ! వాడిన_పరిశీలన_inference)
        {
          ext_చివర = పరిశీలన_inference->hole_ప్రారంభం;
          వాడిన_పరిశీలన_inference = true;
        }
      లేకపోతే
        ext_చివర = lseek (src_fd, ext_ప్రారంభం, SEEK_HOLE);
      ఒకవేళ (0 <= ext_చివర)
        ext_చివర = MIN (ext_చివర, max_ipos);
      లేకపోతే
        {
          ఒకవేళ (errno != ENXIO)
            వెళ్ళు cannot_lseek;
          ext_చివర = src_మొత్తం_కొలత;
          ఒకవేళ (ext_చివర <= ext_ప్రారంభం)
            {
              /* The input file grew; get its current size.  */
              src_మొత్తం_కొలత = lseek (src_fd, 0, SEEK_END);
              ఒకవేళ (src_మొత్తం_కొలత < 0)
                వెళ్ళు cannot_lseek;
              src_మొత్తం_కొలత = MIN (src_మొత్తం_కొలత, max_ipos);

              /* If the input file shrank after growing, stop copying.  */
              ఒకవేళ (src_మొత్తం_కొలత <= ext_ప్రారంభం)
                ఆపు;

              ext_చివర = src_మొత్తం_కొలత;
            }
        }
      /* If the input file must have grown, increase its measured size.  */
      ఒకవేళ (src_మొత్తం_కొలత < ext_చివర)
        src_మొత్తం_కొలత = ext_చివర;

      ఒకవేళ (lseek (src_fd, ext_ప్రారంభం, SEEK_SET) < 0)
        వెళ్ళు cannot_lseek;

      off_t ext_hole_కొలత = ext_ప్రారంభం - చివరి_ext_ప్రారంభం - చివరి_ext_len;

      ఒకవేళ (ext_hole_కొలత)
        {
          ఒకవేళ (sparse_బహుళకం == SPARSE_ALWAYS)
            *hole_కొలత += ext_hole_కొలత;
          లేకపోతే ఒకవేళ (sparse_బహుళకం != SPARSE_NEVER)
            {
              off_t epos = create_hole (dest_fd, dst_name, ext_hole_కొలత);
              ఒకవేళ (epos < 0)
                తిరిగి epos;
            }
          లేకపోతే
            {
              /* When not inducing holes and when there is a hole between
                 the end of the previous extent and the beginning of the
                 current one, write zeros to the destination file.  */
              ఒకవేళ (! write_zeros (dest_fd, ext_hole_కొలత, abuf, buf_కొలత))
                {
                  error (0, errno, _("%s: write failed"),
                         quotef (dst_name));
                  తిరిగి -1;
                }
            }
        }

      off_t ext_len = ext_చివర - ext_ప్రారంభం;
      చివరి_ext_ప్రారంభం = ext_ప్రారంభం;
      చివరి_ext_len = ext_len;

      /* Copy this extent, looking for further opportunities to not
         bother to write zeros if --sparse=always, since SEEK_HOLE
         is conservative and may miss some holes.  */
      off_t n_read
        = sparse_copy (src_fd, dest_fd, abuf, buf_కొలత,
                       allow_reflink, src_name, dst_name,
                       ext_len,
                       sparse_బహుళకం == SPARSE_ALWAYS ? hole_కొలత : ఖాళీ,
                       debug);
      ఒకవేళ (n_read < 0)
        తిరిగి -1;

      ipos = ext_ప్రారంభం + n_read;
      ఒకవేళ (n_read < ext_len)
        {
          /* The input file shrank.  */
          src_మొత్తం_కొలత = ipos;
          ఆపు;
        }

      ext_ప్రారంభం = lseek (src_fd, ipos, SEEK_దత్తాంశం);
      ఒకవేళ (ext_ప్రారంభం < 0 && errno != ENXIO)
        వెళ్ళు cannot_lseek;
    }

  *hole_కొలత += src_మొత్తం_కొలత - (చివరి_ext_ప్రారంభం + చివరి_ext_len);
  తిరిగి src_మొత్తం_కొలత - src_pos;

 cannot_lseek:
  error (0, errno, _("cannot lseek %s"), quoteaf (src_name));
  తిరిగి -1;
}
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HAVE_STRUCT_STAT_ST_ఖండాలు
# define HAVE_STRUCT_STAT_ST_ఖండాలు 0
#అయితే_ముగింపు

/* Type of scan being done on the input when looking for sparseness.  */
గణన scantype
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
స్థిర గణన scantype
infer_scantype (పూర్ణాంకం fd, నిర్మాణం stat స్థిరరాశి *sb, off_t pos,
                నిర్మాణం పరిశీలన_inference *పరిశీలన_inference)
{
  /* Try SEEK_HOLE only if this heuristic
     suggests the file is sparse.  */
  ఒకవేళ (! (HAVE_STRUCT_STAT_ST_ఖండాలు
         && S_ISREG (sb->st_బహుళకం)
         && STP_NBLOCKS (sb) < sb->st_కొలత / ST_NBLOCKSIZE))
    తిరిగి PLAIN_SCANTYPE;

#నిర్వచితమైతే SEEK_HOLE
  పరిశీలన_inference->ext_ప్రారంభం = lseek (fd, pos, SEEK_దత్తాంశం);
  ఒకవేళ (పరిశీలన_inference->ext_ప్రారంభం == pos)
    {
      పరిశీలన_inference->hole_ప్రారంభం = lseek (fd, pos, SEEK_HOLE);
      ఒకవేళ (0 <= పరిశీలన_inference->hole_ప్రారంభం)
        {
          ఒకవేళ (పరిశీలన_inference->hole_ప్రారంభం < sb->st_కొలత)
            తిరిగి LSEEK_SCANTYPE;

          /* Though the file may have holes, SEEK_DATA and SEEK_HOLE
             didn't find any.  This can happen with file systems
             that support SEEK_HOLE only trivially,
             such as squashfs in Linux kernel 6.17 and earlier.
             This can also happen due to transparent file compression,
             which can also indicate fewer than the usual number of blocks.  */

          ఒకవేళ (lseek (fd, pos, SEEK_SET) < 0)
            తిరిగి ERROR_SCANTYPE;

          /* we prefer to return PLAIN_SCANTYPE here so that copy offload
             continues to be used.  Falling through to ZERO_SCANTYPE would be
             less efficient in the compressed file case.  */
          తిరిగి PLAIN_SCANTYPE;
        }
    }
  లేకపోతే ఒకవేళ (pos < పరిశీలన_inference->ext_ప్రారంభం || errno == ENXIO)
    {
      పరిశీలన_inference->hole_ప్రారంభం = pos;  /* Pacify -Wmaybe-uninitialized.  */
      తిరిగి LSEEK_SCANTYPE;
    }
  లేకపోతే ఒకవేళ (errno != EINVAL && !is_ENOTSUP (errno))
    తిరిగి ERROR_SCANTYPE;
#అయితే_ముగింపు

  తిరిగి ZERO_SCANTYPE;
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

బాహ్య intmax_t
copy_file_దత్తాంశం (పూర్ణాంకం ifd, నిర్మాణం stat స్థిరరాశి *ist, off_t ipos, అక్షరం స్థిరరాశి *iname,
                పూర్ణాంకం ofd, నిర్మాణం stat స్థిరరాశి *ost, off_t opos, అక్షరం స్థిరరాశి *oname,
                లెక్క_t ibytes, నిర్మాణం cp_options స్థిరరాశి *x,
                నిర్మాణం copy_debug *debug)
{
  /* Choose a suitable buffer size; it may be adjusted later.  */
  idx_t buf_కొలత = io_blksize (ost);

  /* Deal with sparse files.  */
  నిర్మాణం పరిశీలన_inference పరిశీలన_inference;
  గణన scantype scantype = infer_scantype (ifd, ist, ipos, &పరిశీలన_inference);
  ఒకవేళ (scantype == ERROR_SCANTYPE)
    {
      error (0, errno, _("cannot lseek %s"), quoteaf (iname));
      తిరిగి -1;
    }
  bool make_holes
    = (S_ISREG (ost->st_బహుళకం)
       && (x->sparse_బహుళకం == SPARSE_ALWAYS
           || (x->sparse_బహుళకం == SPARSE_AUTO
               && scantype != PLAIN_SCANTYPE)));

  /* If we _know_ we're going to read data sequentially into the process,
     i.e., --reflink or --sparse are not in auto mode,
     give that hint to the kernel so it can tune caching behavior.
     Also we don't bother calling fadvise for small copies,
     as it is not likely to help performance and might even hurt it.
     Also we only apply this hint for the whole file (0 length)
     as OpenZFS 2.2.2 at least will otherwise synchronously
     (decompress and) populate the cache when given a specific length.  */
  ఒకవేళ (ipos == 0 && ibytes == లెక్క_MAX
      && (x->reflink_బహుళకం != REFLINK_AUTO || x->sparse_బహుళకం != SPARSE_AUTO))
    fdadvise (ifd, 0, 0, FADVISE_SEQUENTIAL);

  /* If not making a sparse file, try to use a more-efficient
     buffer size.  */
  ఒకవేళ (! make_holes)
    {
      /* Compute the least common multiple of the input and output
         buffer sizes, adjusting for outlandish values.
         blcm is at most IDX_MAX - 1 so that buf_size cannot become 0 below.
         Note we read in multiples of the reported block size
         to support (unusual) devices that have this constraint.  */
      idx_t blcm_max = MIN (MIN (IDX_MAX - 1, SSIZE_MAX), SIZE_MAX);
      idx_t blcm = బఫరు_lcm (io_blksize (ist), buf_కొలత,
                               blcm_max);

      /* Do not bother with a buffer larger than the input file, plus one
         byte to make sure the file has not grown while reading it.  */
      ఒకవేళ (S_ISREG (ist->st_బహుళకం) && 0 <= ist->st_కొలత
          && ist->st_కొలత < buf_కొలత)
        buf_కొలత = ist->st_కొలత + 1;

      /* However, stick with a block size that is a positive multiple of
         blcm, overriding the above adjustments.  Watch out for
         overflow.  */
      buf_కొలత = ckd_కూడు (&buf_కొలత, buf_కొలత, blcm - 1) ? IDX_MAX : buf_కొలత;
      buf_కొలత -= buf_కొలత % blcm;
    }

  అక్షరం *buf = ఖాళీ;
  intmax_t ఫలితం;
  off_t hole_కొలత = 0;

  ఒకవేళ (scantype == LSEEK_SCANTYPE)
    {
#నిర్వచితమైతే SEEK_HOLE
      ఫలితం = lseek_copy (ifd, ofd, &buf, buf_కొలత,
                           ipos, ibytes, &పరిశీలన_inference, ist->st_కొలత,
                           make_holes ? x->sparse_బహుళకం : SPARSE_NEVER,
                           x->reflink_బహుళకం != REFLINK_NEVER,
                           iname, oname, &hole_కొలత, debug);
#లేకపోతే
      affirm (false);
#అయితే_ముగింపు
    }
  లేకపోతే
    ఫలితం = sparse_copy (ifd, ofd, &buf, buf_కొలత,
                          x->reflink_బహుళకం != REFLINK_NEVER,
                          iname, oname, ibytes,
                          make_holes ? &hole_కొలత : ఖాళీ,
                          debug);

  ఒకవేళ (0 <= ఫలితం && 0 < hole_కొలత)
    {
      off_t oend;
      ఒకవేళ (ckd_కూడు (&oend, opos, ఫలితం)
          ? (errno = EOVERFLOW, true)
          : make_holes
          ? ftruncate (ofd, oend) < 0
          : !write_zeros (ofd, hole_కొలత, &buf, buf_కొలత))
        {
          error (0, errno, _("failed to extend %s"), quoteaf (oname));
          ఫలితం = -1;
        }
      లేకపోతే ఒకవేళ (make_holes
               && punch_hole (ofd, oend - hole_కొలత, hole_కొలత) < 0)
        {
          error (0, errno, _("error deallocating %s"), quoteaf (oname));
          ఫలితం = -1;
        }
    }

  alignfree (buf);
  తిరిగి ఫలితం;
}
