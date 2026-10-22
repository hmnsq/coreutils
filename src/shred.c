/* shred.c - overwrite files and devices to make it harder to recover data

   Copyright (C) 1999-2026 Free Software Foundation, Inc.
   Copyright (C) 1997, 1998, 1999 Colin Plumb.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.

   Written by Colin Plumb.  */

/*
 * Do a more secure overwrite of given files or devices, to make it harder
 * for even very expensive hardware probing to recover the data.
 *
 * Although this process is also known as "wiping", I prefer the longer
 * name both because I think it is more evocative of what is happening and
 * because a longer name conveys a more appropriate sense of deliberateness.
 *
 * For the theory behind this, see "Secure Deletion of Data from Magnetic
 * and Solid-State Memory", on line at
 * https://www.cs.auckland.ac.nz/~pgut001/pubs/secure_del.html
 *
 * Just for the record, reversing one or two passes of disk overwrite
 * is not terribly difficult with hardware help.  Hook up a good-quality
 * digitizing oscilloscope to the output of the head preamplifier and copy
 * the high-res digitized data to a computer for some off-line analysis.
 * Read the "current" data and average all the pulses together to get an
 * "average" pulse on the disk.  Subtract this average pulse from all of
 * the actual pulses and you can clearly see the "echo" of the previous
 * data on the disk.
 *
 * Real hard drives have to balance the cost of the media, the head,
 * and the read circuitry.  They use better-quality media than absolutely
 * necessary to limit the cost of the read circuitry.  By throwing that
 * assumption out, and the assumption that you want the data processed
 * as fast as the hard drive can spin, you can do better.
 *
 * If asked to wipe a file, this also unlinks it, renaming it in a
 * clever way to try to leave no trace of the original filename.
 *
 * This was inspired by a desire to improve on some code titled:
 * Wipe V1.0-- Overwrite and delete files.  S. 2/3/96
 * but I've rewritten everything here so completely that no trace of
 * the original remains.
 *
 * Thanks to:
 * Bob Jenkins, for his good RNG work and patience with the FSF copyright
 * paperwork.
 * Jim Meyering, for his work merging this into the GNU fileutils while
 * still letting me feel a sense of ownership and pride.  Getting me to
 * tolerate the GNU brace style was quite a feat of diplomacy.
 * Paul Eggert, for lots of useful discussion and code.  I disagree with
 * an awful lot of his suggestions, but they're disagreements worth having.
 *
 * Things to think about:
 * - Security: Is there any risk to the race
 *   between overwriting and unlinking a file?  Will it do anything
 *   drastically bad if told to attack a named pipe or socket?
 */

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "shred"

#నిర్వచించు AUTHORS proper_name ("Colin Plumb")

#అన్తర్భావ <config.h>

#అన్తర్భావ <getopt.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#ఒకవేళ defined __linux__ && HAVE_SYS_MTIO_H
# include <sys/mtio.h>
#అయితే_ముగింపు

#అన్తర్భావ "system.h"
#అన్తర్భావ "alignalloc.h"
#అన్తర్భావ "argmatch.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "xdectoint.h"
#అన్తర్భావ "fcntl--.h"
#అన్తర్భావ "gethrxtime.h"
#అన్తర్భావ "human.h"
#అన్తర్భావ "randint.h"
#అన్తర్భావ "randread.h"
#అన్తర్భావ "renameatu.h"
#అన్తర్భావ "stat-size.h"

/* Default number of times to overwrite.  */
గణన { DEFAULT_PASSES = 3 };

/* How many seconds to wait before checking whether to output another
   verbose output line.  */
గణన { VERBOSE_UPDATE = 5 };

/* Sector size and corresponding mask, for recovering after write failures.
   The size must be a power of 2.  */
గణన { SECTOR_కొలత = 512 };
గణన { SECTOR_ముసుగు = SECTOR_కొలత - 1 };
static_assert (0 < SECTOR_కొలత && (SECTOR_కొలత & SECTOR_ముసుగు) == 0);

గణన remove_పద్ధతి
{
  remove_none = 0,      /* the default: only wipe data.  */
  remove_unlink,        /* don't obfuscate name, just unlink.  */
  remove_wipe,          /* obfuscate name before unlink.  */
  remove_wipesync       /* obfuscate name, syncing each byte, before unlink.  */
};

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి remove_వాదనలు[] =
{
  "unlink", "wipe", "wipesync", ఖాళీ
};

స్థిర గణన remove_పద్ధతి స్థిరరాశి remove_methods[] =
{
  remove_unlink, remove_wipe, remove_wipesync
};

నిర్మాణం Options
{
  bool force;		/* -f flag: chmod files if necessary */
  size_t n_iterations;	/* -n flag: Number of iterations */
  off_t కొలత;		/* -s flag: size of file */
  గణన remove_పద్ధతి remove_file; /* -u flag: remove file after shredding */
  bool verbose;		/* -v flag: Print progress */
  bool exact;		/* -x flag: Do not round up file size */
  bool zero_fill;	/* -z flag: Add a final zero pass */
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  RANDOM_ఆధారం_OPTION = CHAR_MAX + 1
};

స్థిర నిర్మాణం option స్థిరరాశి long_opts[] =
{
  {"exact", no_వాదం, ఖాళీ, 'x'},
  {"force", no_వాదం, ఖాళీ, 'f'},
  {"iterations", required_వాదం, ఖాళీ, 'n'},
  {"size", required_వాదం, ఖాళీ, 's'},
  {"random-source", required_వాదం, ఖాళీ, RANDOM_ఆధారం_OPTION},
  {"remove", optional_వాదం, ఖాళీ, 'u'},
  {"verbose", no_వాదం, ఖాళీ, 'v'},
  {"zero", no_వాదం, ఖాళీ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]... FILE...\n"), కార్యక్రమం_name);
      fputs (_("\
Overwrite the specified FILE(s) repeatedly, లోపల క్రమం గమ్యం make it harder\n\
కొరకు even very expensive hardware probing గమ్యం recover the దత్తాంశం.\n\
"), stdout);
      fputs (_("\
\n\
If FILE is -, shred standard output.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --force\n\
         change permissions గమ్యం allow writing ఒకవేళ necessary\n\
"));
      oprintf (_("\
  -n, --iterations=N\n\
         overwrite N times instead యొక్క the అప్రమేయ (%d)\n\
"), DEFAULT_PASSES);
      oputs (_("\
      --random-ఆధారం_2=FILE\n\
         get random బైట్లు from FILE\n\
"));
      oputs (_("\
  -s, --కొలత=N\n\
         shred this many బైట్లు (suffixes like K, M, G accepted)\n\
"));
      oputs (_("\
  -u\n\
         deallocate and remove file after overwriting\n\
"));
      oputs (_("\
      --remove[=HOW]\n\
         like -u but give control on HOW గమ్యం delete;  See క్రింద\n\
"));
      oputs (_("\
  -v, --verbose\n\
         show details యొక్క దత్తాంశం and metadata operations performed\n\
"));
      oputs (_("\
  -x, --exact\n\
         చేయి not విడత_2 file sizes పైకి గమ్యం the తదుపరి నిండిన ఖండం;\n\
         this is the అప్రమేయ కొరకు non-regular files\n\
"));
      oputs (_("\
  -z, --zero\n\
         కూడు a final overwrite with zeros గమ్యం hide shredding\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
FILE will be skipped with a diagnostic సందేశం_2 ఒకవేళ it is a FIFO, socket, or\n\
terminal, since its దత్తాంశం does not reside on disk.\n\
"), stdout);
      fputs (_("\
\n\
Delete FILE(s) ఒకవేళ --remove (-u) is specified.  The అప్రమేయ is not గమ్యం remove\n\
the files because it is common గమ్యం నిర్వహించు on device files like /dev/hda,\n\
and those files usually should not be removed.\n\
The optional HOW parameter indicates how గమ్యం remove a directory నమోదు_2:\n\
'unlink' => use a standard unlink call.\n\
'wipe' => also మొదటి obfuscate బైట్లు లోపల the name.\n\
'wipesync' => also sync each obfuscated byte గమ్యం the device.\n\
The అప్రమేయ బహుళకం_2 is 'wipesync', but note it can be expensive.\n\
\n\
"), stdout);
      fputs (_("\
CAUTION: shred assumes the file system and hardware overwrite దత్తాంశం లోపల ఉంచు.\n\
Although this is common, many platforms నిర్వహించు otherwise.  Also, backups\n\
and mirrors may contain unremovable copies that will let a shredded file\n\
be recovered later.  See the GNU coreutils manual కొరకు details.\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/*
 * Determine if pattern type is periodic or not.
 */
స్థిర bool
periodic_నమూనా (పూర్ణాంకం type)
{
  ఒకవేళ (type <= 0)
    తిరిగి false;

  గుర్తులేని అక్షరం r[3];
  గుర్తులేని పూర్ణాంకం బిట్లు_2 = type & 0xfff;

  బిట్లు_2 |= బిట్లు_2 << 12;
  r[0] = (బిట్లు_2 >> 4) & 255;
  r[1] = (బిట్లు_2 >> 8) & 255;
  r[2] = బిట్లు_2 & 255;

  తిరిగి (r[0] != r[1]) || (r[0] != r[2]);
}

/*
 * Fill a buffer with a fixed pattern.
 *
 * The buffer must be at least 3 bytes long, even if
 * size is less.  Larger sizes are filled exactly.
 */
స్థిర శూన్య
fillpattern (పూర్ణాంకం type, గుర్తులేని అక్షరం *r, size_t కొలత)
{
  size_t i;
  గుర్తులేని పూర్ణాంకం బిట్లు_2 = type & 0xfff;

  బిట్లు_2 |= బిట్లు_2 << 12;
  r[0] = (బిట్లు_2 >> 4) & 255;
  r[1] = (బిట్లు_2 >> 8) & 255;
  r[2] = బిట్లు_2 & 255;
  కొరకు (i = 3; i <= కొలత / 2; i *= 2)
    జ్ఞాపకం_నకలు (r + i, r, i);
  ఒకవేళ (i < కొలత)
    జ్ఞాపకం_నకలు (r + i, r, కొలత - i);

  /* Invert the first bit of every sector. */
  ఒకవేళ (type & 0x1000)
    కొరకు (i = 0; i < కొలత; i += SECTOR_కొలత)
      r[i] ^= 0x80;
}

/*
 * Generate a 6-character (+ nul) pass name string
 * FIXME: allow translation of "random".
 */
#నిర్వచించు PASS_NAME_కొలత 7
స్థిర శూన్య
passname (గుర్తులేని అక్షరం స్థిరరాశి *దత్తాంశం, అక్షరం name[PASS_NAME_కొలత])
{
  ఒకవేళ (దత్తాంశం)
    sprintf (name, "%02x%02x%02x", దత్తాంశం[0], దత్తాంశం[1], దత్తాంశం[2]);
  లేకపోతే
    జ్ఞాపకం_నకలు (name, "random", PASS_NAME_కొలత);
}

/* Return true when it's ok to ignore an fsync or fdatasync
   failure that set errno to ERRNO_VAL.  */
స్థిర bool
ignorable_sync_errno (పూర్ణాంకం errno_val)
{
  తిరిగి (errno_val == EINVAL
          || errno_val == EBADF
          /* HP-UX does this */
          || errno_val == EISDIR);
}

/* Request that all data for FD be transferred to the corresponding
   storage device.  QNAME is the file name (quoted for colons).
   Report any errors found.  Return 0 on success, -1
   (setting errno) on failure.  It is not an error if fdatasync and/or
   fsync is not supported for this file, or if the file is not a
   writable file descriptor.  */
స్థిర పూర్ణాంకం
dosync (పూర్ణాంకం fd, అక్షరం స్థిరరాశి *qname)
{
  పూర్ణాంకం err;

  ఒకవేళ (fdatasync (fd) == 0)
    తిరిగి 0;
  err = errno;
  ఒకవేళ ( ! ignorable_sync_errno (err))
    {
      error (0, err, _("%s: fdatasync failed"), qname);
      errno = err;
      తిరిగి -1;
    }

  ఒకవేళ (fsync (fd) == 0)
    తిరిగి 0;
  err = errno;
  ఒకవేళ ( ! ignorable_sync_errno (err))
    {
      error (0, err, _("%s: fsync failed"), qname);
      errno = err;
      తిరిగి -1;
    }

  sync ();
  తిరిగి 0;
}

/* Turn on or off direct I/O mode for file descriptor FD, if possible.
   Try to turn it on if ENABLE is true.  Otherwise, try to turn it off.  */
స్థిర శూన్య
ప్రత్యక్ష_బహుళకం (పూర్ణాంకం fd, bool enable)
{
  ఒకవేళ (O_ప్రత్యక్ష)
    {
      పూర్ణాంకం fd_flags = fcntl (fd, F_GETFL);
      ఒకవేళ (0 < fd_flags)
        {
          పూర్ణాంకం new_flags = (enable
                           ? (fd_flags | O_ప్రత్యక్ష)
                           : (fd_flags & ~O_ప్రత్యక్ష));
          ఒకవేళ (new_flags != fd_flags)
            fcntl (fd, F_SETFL, new_flags);
        }
    }

#ఒకవేళ HAVE_DIRECTIO && defined DIRECTIO_ON && defined DIRECTIO_OFF
  /* This is Solaris-specific.  */
  directio (fd, enable ? DIRECTIO_ON : DIRECTIO_OFF);
#అయితే_ముగింపు
}

/* Rewind FD; its status is ST.  */
స్థిర bool
dorewind (పూర్ణాంకం fd, నిర్మాణం stat స్థిరరాశి *st)
{
  ఒకవేళ (S_ISCHR (st->st_బహుళకం))
    {
#ఒకవేళ defined __linux__ && HAVE_SYS_MTIO_H
      /* In the Linux kernel, lseek does not work on tape devices; it
         returns a randomish value instead.  Try the low-level tape
         rewind operation first.  */
      నిర్మాణం mtop op;
      op.mt_op = MTREW;
      op.mt_లెక్క = 1;
      ఒకవేళ (ioctl (fd, MTIOCTOP, &op) == 0)
        తిరిగి true;
#అయితే_ముగింపు
    }
  off_t స్థానభ్రంశం = lseek (fd, 0, SEEK_SET);
  ఒకవేళ (0 < స్థానభ్రంశం)
    errno = EINVAL;
  తిరిగి స్థానభ్రంశం == 0;
}

/* By convention, negative sizes represent unknown values.  */

స్థిర bool
తెలిసిన (off_t కొలత)
{
  తిరిగి 0 <= కొలత;
}

/*
 * Do pass number K of N, writing *SIZEP bytes of the given pattern TYPE
 * to the file descriptor FD.  K and N are passed in only for verbose
 * progress message purposes.  If N == 0, no progress messages are printed.
 *
 * If *SIZEP == -1, the size is unknown, and it will be filled in as soon
 * as writing fails with ENOSPC.
 *
 * Return 1 on write error, -1 on other error, 0 on success.
 */
స్థిర పూర్ణాంకం
dopass (పూర్ణాంకం fd, నిర్మాణం stat స్థిరరాశి *st, అక్షరం స్థిరరాశి *qname, off_t *sizep,
        పూర్ణాంకం type, నిర్మాణం randread_ఆధారం *s,
        గుర్తులేని దీర్ఘ పూర్ణాంకం k, గుర్తులేని దీర్ఘ పూర్ణాంకం n)
{
  off_t కొలత = *sizep;
  off_t స్థానభ్రంశం;                /* Current file position */
  xtime_t prev IF_LINT ( = 0); /* Time we printed the previous update.  */
  xtime_t now = 0;             /* Current time */
  size_t lim;                  /* Amount of data to try writing */
  size_t soff;                 /* Offset into buffer for next write */
  ssize_t ssize;               /* Return value from write */

  /* Fill pattern buffer.  Aligning it to a page so we can do direct I/O.  */
  size_t పుట_కొలత = getpagesize ();
#నిర్వచించు PERIODIC_OUTPUT_కొలత (60 * 1024)
#నిర్వచించు NONPERIODIC_OUTPUT_కొలత (64 * 1024)
  static_assert (PERIODIC_OUTPUT_కొలత % 3 == 0);
  size_t output_కొలత = periodic_నమూనా (type)
                       ? PERIODIC_OUTPUT_కొలత : NONPERIODIC_OUTPUT_కొలత;
#నిర్వచించు FILLPATTERN_కొలత (((output_కొలత + 2) / 3) * 3) /* Multiple of 3 */
  గుర్తులేని అక్షరం *pbuf = xalignalloc (పుట_కొలత, FILLPATTERN_కొలత);

  అక్షరం pass_string[PASS_NAME_కొలత];	/* Name of current pass */
  bool write_error = false;
  bool other_error = false;

  /* Printable previous offset into the file */
  అక్షరం మునుపటి_స్థానభ్రంశం_buf[LONGEST_HUMAN_READABLE + 1];
  అక్షరం స్థిరరాశి *మునుపటి_human_స్థానభ్రంశం;

  /* As a performance tweak, avoid direct I/O for small sizes,
     as it's just a performance rather than security consideration,
     and direct I/O can often be unsupported for small non aligned sizes.  */
  bool try_without_directio = 0 < కొలత && కొలత < output_కొలత;
  ఒకవేళ (! try_without_directio)
    ప్రత్యక్ష_బహుళకం (fd, true);

  ఒకవేళ (! dorewind (fd, st))
    {
      error (0, errno, _("%s: cannot rewind"), qname);
      other_error = true;
      వెళ్ళు free_నమూనా_mem;
    }

  /* Constant fill patterns need only be set up once. */
  ఒకవేళ (type >= 0)
    {
      lim = తెలిసిన (కొలత) && కొలత < FILLPATTERN_కొలత ? కొలత : FILLPATTERN_కొలత;
      fillpattern (type, pbuf, lim);
      passname (pbuf, pass_string);
    }
  లేకపోతే
    {
      passname (ఖాళీ, pass_string);
    }

  /* Set position if first status update */
  ఒకవేళ (n)
    {
      error (0, 0, _("%s: pass %lu/%lu (%s)..."), qname, k, n, pass_string);
      prev = gethrxtime ();
      మునుపటి_human_స్థానభ్రంశం = "";
    }

  స్థానభ్రంశం = 0;
  అయితే (true)
    {
      /* How much to write this time? */
      lim = output_కొలత;
      ఒకవేళ (తెలిసిన (కొలత) && కొలత - స్థానభ్రంశం < output_కొలత)
        {
          ఒకవేళ (కొలత < స్థానభ్రంశం)
            ఆపు;
          lim = కొలత - స్థానభ్రంశం;
          ఒకవేళ (!lim)
            ఆపు;
        }
      ఒకవేళ (type < 0)
        randread (s, pbuf, lim);
      /* Loop to retry partial writes. */
      కొరకు (soff = 0; soff < lim; soff += ssize)
        {
          ssize = లిఖ (fd, pbuf + soff, lim - soff);
          ఒకవేళ (ssize <= 0)
            {
              ఒకవేళ (! తెలిసిన (కొలత) && (ssize == 0 || errno == ENOSPC))
                {
                  /* We have found the end of the file.  */
                  ఒకవేళ (soff <= OFF_T_MAX - స్థానభ్రంశం)
                    *sizep = కొలత = స్థానభ్రంశం + soff;
                  ఆపు;
                }
              లేకపోతే
                {
                  పూర్ణాంకం errnum = errno;

                  /* Retry without direct I/O since this may not be supported
                     at all on some (file) systems, or with the current size.
                     I.e., a specified --size that is not aligned, or when
                     dealing with slop at the end of a file with --exact.  */
                  ఒకవేళ (! try_without_directio && errno == EINVAL)
                    {
                      ప్రత్యక్ష_బహుళకం (fd, false);
                      ssize = 0;
                      try_without_directio = true;
                      కొనసాగు;
                    }
                  uintmax_t error_స్థానభ్రంశం = స్థానభ్రంశం + soff;
                  error (0, errnum, _("%s: error writing at offset %ju"),
                         qname, error_స్థానభ్రంశం);

                  /* 'shred' is often used on bad media, before throwing it
                     out.  Thus, it shouldn't give up on bad blocks.  This
                     code works because lim is always a multiple of
                     SECTOR_SIZE, except at the end.  This size constraint
                     also enables direct I/O on some (file) systems.  */
                  static_assert (PERIODIC_OUTPUT_కొలత % SECTOR_కొలత == 0);
                  static_assert (NONPERIODIC_OUTPUT_కొలత % SECTOR_కొలత == 0);
                  ఒకవేళ (errnum == EIO && తెలిసిన (కొలత)
                      && (soff | SECTOR_ముసుగు) < lim)
                    {
                      size_t soff1 = (soff | SECTOR_ముసుగు) + 1;
                      ఒకవేళ (lseek (fd, స్థానభ్రంశం + soff1, SEEK_SET) != -1)
                        {
                          /* Arrange to skip this block. */
                          ssize = soff1 - soff;
                          write_error = true;
                          కొనసాగు;
                        }
                      error (0, errno, _("%s: lseek failed"), qname);
                    }
                  other_error = true;
                  వెళ్ళు free_నమూనా_mem;
                }
            }
        }

      /* Okay, we have written "soff" bytes. */

      ఒకవేళ (ckd_కూడు (&స్థానభ్రంశం, స్థానభ్రంశం, soff))
        {
          error (0, 0, _("%s: file too large"), qname);
          other_error = true;
          వెళ్ళు free_నమూనా_mem;
        }

      bool పూర్తి = స్థానభ్రంశం == కొలత;

      /* Time to print progress? */
      ఒకవేళ (n && ((పూర్తి && *మునుపటి_human_స్థానభ్రంశం)
                || VERBOSE_UPDATE <= xtime_sec ((now = gethrxtime ()) - prev)))
        {
          అక్షరం స్థానభ్రంశం_buf[LONGEST_HUMAN_READABLE + 1];
          అక్షరం కొలత_buf[LONGEST_HUMAN_READABLE + 1];
          పూర్ణాంకం human_పురోగతి_opts = (human_autoscale | human_SI
                                     | human_పునాది_1024 | human_B);
          అక్షరం స్థిరరాశి *human_స్థానభ్రంశం
            = human_readable (స్థానభ్రంశం, స్థానభ్రంశం_buf,
                              human_floor | human_పురోగతి_opts, 1, 1);

          ఒకవేళ (పూర్తి || !streq (మునుపటి_human_స్థానభ్రంశం, human_స్థానభ్రంశం))
            {
              ఒకవేళ (! తెలిసిన (కొలత))
                error (0, 0, _("%s: pass %lu/%lu (%s)...%s"),
                       qname, k, n, pass_string, human_స్థానభ్రంశం);
              లేకపోతే
                {
                  uintmax_t off = స్థానభ్రంశం;
                  పూర్ణాంకం percent = (కొలత == 0
                                 ? 100
                                 : (off <= UINTMAX_MAX / 100
                                    ? off * 100 / కొలత
                                    : off / (కొలత / 100)));
                  అక్షరం స్థిరరాశి *human_కొలత
                    = human_readable (కొలత, కొలత_buf,
                                      human_ceiling | human_పురోగతి_opts,
                                      1, 1);
                  ఒకవేళ (పూర్తి)
                    human_స్థానభ్రంశం = human_కొలత;
                  error (0, 0, _("%s: pass %lu/%lu (%s)...%s/%s %d%%"),
                         qname, k, n, pass_string, human_స్థానభ్రంశం, human_కొలత,
                         percent);
                }

              strcpy (మునుపటి_స్థానభ్రంశం_buf, human_స్థానభ్రంశం);
              మునుపటి_human_స్థానభ్రంశం = మునుపటి_స్థానభ్రంశం_buf;
              prev = now;

              /*
               * Force periodic syncs to keep displayed progress accurate
               * FIXME: Should these be present even if -v is not enabled,
               * to keep the buffer cache from filling with dirty pages?
               * It's a common problem with programs that do lots of writes,
               * like mkfs.
               */
              ఒకవేళ (dosync (fd, qname) != 0)
                {
                  ఒకవేళ (errno != EIO)
                    {
                      other_error = true;
                      వెళ్ళు free_నమూనా_mem;
                    }
                  write_error = true;
                }
            }
        }
    }

  /* Force what we just wrote to hit the media. */
  ఒకవేళ (dosync (fd, qname) != 0)
    {
      ఒకవేళ (errno != EIO)
        {
          other_error = true;
          వెళ్ళు free_నమూనా_mem;
        }
      write_error = true;
    }

free_నమూనా_mem:
  alignfree (pbuf);

  తిరిగి other_error ? -1 : write_error;
}

/*
 * The passes start and end with a random pass, and the passes in between
 * are done in random order.  The idea is to deprive someone trying to
 * reverse the process of knowledge of the overwrite patterns, so they
 * have the additional step of figuring out what was done to the device
 * before they can try to reverse or cancel it.
 *
 * First, all possible 1-bit patterns.  There are two of them.
 * Then, all possible 2-bit patterns.  There are four, but the two
 * which are also 1-bit patterns can be omitted.
 * Then, all possible 3-bit patterns.  Likewise, 8-2 = 6.
 * Then, all possible 4-bit patterns.  16-4 = 12.
 *
 * The basic passes are:
 * 1-bit: 0x000, 0xFFF
 * 2-bit: 0x555, 0xAAA
 * 3-bit: 0x249, 0x492, 0x924, 0x6DB, 0xB6D, 0xDB6 (+ 1-bit)
 *        100100100100         110110110110
 *           9   2   4            D   B   6
 * 4-bit: 0x111, 0x222, 0x333, 0x444, 0x666, 0x777,
 *        0x888, 0x999, 0xBBB, 0xCCC, 0xDDD, 0xEEE (+ 1-bit, 2-bit)
 * Adding three random passes at the beginning, middle and end
 * produces the default 25-pass structure.
 *
 * The next extension would be to 5-bit and 6-bit patterns.
 * There are 30 uncovered 5-bit patterns and 64-8-2 = 46 uncovered
 * 6-bit patterns, so they would increase the time required
 * significantly.  4-bit patterns are enough for most purposes.
 *
 * The main gotcha is that this would require a trickier encoding,
 * since lcm(2,3,4) = 12 bits is easy to fit into an int, but
 * lcm(2,3,4,5) = 60 bits is not.
 *
 * One extension that is included is to complement the first bit in each
 * 512-byte block, to alter the phase of the encoded data in the more
 * complex encodings.  This doesn't apply to MFM, so the 1-bit patterns
 * are considered part of the 3-bit ones and the 2-bit patterns are
 * considered part of the 4-bit patterns.
 *
 *
 * How does the generalization to variable numbers of passes work?
 *
 * Here's how...
 * Have an ordered list of groups of passes.  Each group is a set.
 * Take as many groups as will fit, plus a random subset of the
 * last partial group, and place them into the passes list.
 * Then shuffle the passes list into random order and use that.
 *
 * One extra detail: if we can't include a large enough fraction of the
 * last group to be interesting, then just substitute random passes.
 *
 * If you want more passes than the entire list of groups can
 * provide, just start repeating from the beginning of the list.
 */
స్థిర పూర్ణాంకం స్థిరరాశి
  patterns[] =
{
  -2,				/* 2 random passes */
  2, 0x000, 0xFFF,		/* 1-bit */
  2, 0x555, 0xAAA,		/* 2-bit */
  -1,				/* 1 random pass */
  6, 0x249, 0x492, 0x6DB, 0x924, 0xB6D, 0xDB6,	/* 3-bit */
  12, 0x111, 0x222, 0x333, 0x444, 0x666, 0x777,
  0x888, 0x999, 0xBBB, 0xCCC, 0xDDD, 0xEEE,	/* 4-bit */
  -1,				/* 1 random pass */
        /* The following patterns have the first bit per block flipped */
  8, 0x1000, 0x1249, 0x1492, 0x16DB, 0x1924, 0x1B6D, 0x1DB6, 0x1FFF,
  14, 0x1111, 0x1222, 0x1333, 0x1444, 0x1555, 0x1666, 0x1777,
  0x1888, 0x1999, 0x1AAA, 0x1BBB, 0x1CCC, 0x1DDD, 0x1EEE,
  -1,				/* 1 random pass */
  0				/*చివర*/
};

/*
 * Generate a random wiping pass pattern with num passes.
 * This is a two-stage process.  First, the passes to include
 * are chosen, and then they are shuffled into the desired
 * order.
 */
స్థిర శూన్య
genpattern (పూర్ణాంకం *dest, size_t అంకె, నిర్మాణం randint_ఆధారం *s)
{
  size_t randpasses;
  పూర్ణాంకం స్థిరరాశి *p;
  పూర్ణాంకం *d;
  size_t n;
  size_t accum, శిఖరం, మార్పిడి;
  పూర్ణాంకం k;

  ఒకవేళ (!అంకె)
    తిరిగి;

  /* Stage 1: choose the passes to use */
  p = patterns;
  randpasses = 0;
  d = dest;			/* Destination for generated pass list */
  n = అంకె;			/* Passes remaining to fill */

  అయితే (true)
    {
      k = *p++;			/* Block descriptor word */
      ఒకవేళ (!k)
        {			/* Loop back to the beginning */
          p = patterns;
        }
      లేకపోతే ఒకవేళ (k < 0)
        {			/* -k random passes */
          k = -k;
          ఒకవేళ ((size_t) k >= n)
            {
              randpasses += n;
              ఆపు;
            }
          randpasses += k;
          n -= k;
        }
      లేకపోతే ఒకవేళ ((size_t) k <= n)
        {			/* Full block of patterns */
          జ్ఞాపకం_నకలు (d, p, k * పరిమాణం (పూర్ణాంకం));
          p += k;
          d += k;
          n -= k;
        }
      లేకపోతే ఒకవేళ (n < 2 || 3 * n < (size_t) k)
        {			/* Finish with random */
          randpasses += n;
          ఆపు;
        }
      లేకపోతే
        {			/* Pad out with n of the k available */
          చేయి
            {
              ఒకవేళ (n == (size_t) k || randint_ఎంచుకో (s, k) < n)
                {
                  *d++ = *p;
                  n--;
                }
              p++;
              k--;
            }
          అయితే (n);
          ఆపు;
        }
    }
  శిఖరం = అంకె - randpasses;	/* Top of initialized data */
  /* affirm (d == dest + top); */

  /*
   * We now have fixed patterns in the dest buffer up to
   * "top", and we need to scramble them, with "randpasses"
   * random passes evenly spaced among them.
   *
   * We want one at the beginning, one at the end, and
   * evenly spaced in between.  To do this, we basically
   * use Bresenham's line draw (a.k.a DDA) algorithm
   * to draw a line with slope (randpasses-1)/(num-1).
   * (We use a positive accumulator and count down to
   * do this.)
   *
   * So for each desired output value, we do the following:
   * - If it should be a random pass, copy the pass type
   *   to top++, out of the way of the other passes, and
   *   set the current pass to -1 (random).
   * - If it should be a normal pattern pass, choose an
   *   entry at random between here and top-1 (inclusive)
   *   and swap the current entry with that one.
   */
  randpasses--;			/* To speed up later math */
  accum = randpasses;		/* Bresenham DDA accumulator */
  కొరకు (n = 0; n < అంకె; n++)
    {
      ఒకవేళ (accum <= randpasses)
        {
          accum += అంకె - 1;
          dest[శిఖరం++] = dest[n];
          dest[n] = -1;
        }
      లేకపోతే
        {
          మార్పిడి = n + randint_ఎంచుకో (s, శిఖరం - n);
          k = dest[n];
          dest[n] = dest[మార్పిడి];
          dest[మార్పిడి] = k;
        }
      accum -= randpasses;
    }
  /* affirm (top == num); */
}

/*
 * The core routine to actually do the work.  This overwrites the first
 * size bytes of the given fd.  Return true if successful.
 */
స్థిర bool
do_wipefd (పూర్ణాంకం fd, అక్షరం స్థిరరాశి *qname, నిర్మాణం randint_ఆధారం *s,
           నిర్మాణం Options స్థిరరాశి *flags)
{
  నిర్మాణం stat st;
  off_t కొలత;		/* Size to write, size to read */
  off_t i_కొలత = 0;	/* For small files, initial size to overwrite inode */
  గుర్తులేని దీర్ఘ పూర్ణాంకం n;	/* Number of passes for printing purposes */
  పూర్ణాంకం *passarray;
  bool ok = true;
  నిర్మాణం randread_ఆధారం *rs;

  n = 0;		/* dopass takes n == 0 to mean "don't print progress" */
  ఒకవేళ (flags->verbose)
    n = flags->n_iterations + flags->zero_fill;

  ఒకవేళ (fstat (fd, &st))
    {
      error (0, errno, _("%s: fstat failed"), qname);
      తిరిగి false;
    }

  /* If we know that we can't possibly shred the file, give up now.
     Otherwise, we may go into an infinite loop writing data before we
     find that we can't rewind the device.  */
  ఒకవేళ ((S_ISCHR (st.st_బహుళకం) && isatty (fd))
      || S_ISFIFO (st.st_బహుళకం)
      || S_ISSOCK (st.st_బహుళకం))
    {
      error (0, 0, _("%s: invalid file type"), qname);
      తిరిగి false;
    }
  లేకపోతే ఒకవేళ (S_ISREG (st.st_బహుళకం) && st.st_కొలత < 0)
    {
      error (0, 0, _("%s: file has negative size"), qname);
      తిరిగి false;
    }

  /* Allocate pass array */
  passarray = xnmalloc (flags->n_iterations, పరిమాణం *passarray);

  కొలత = flags->కొలత;
  ఒకవేళ (కొలత == -1)
    {
      ఒకవేళ (S_ISREG (st.st_బహుళకం))
        {
          కొలత = st.st_కొలత;

          ఒకవేళ (! flags->exact)
            {
              /* Round up to the nearest block size to clear slack space.  */
              off_t remainder = కొలత % STP_BLKSIZE (&st);
              ఒకవేళ (కొలత && కొలత < STP_BLKSIZE (&st))
                i_కొలత = కొలత;
              ఒకవేళ (remainder != 0)
                {
                  off_t కొలత_incr = STP_BLKSIZE (&st) - remainder;
                  కొలత += MIN (కొలత_incr, OFF_T_MAX - కొలత);
                }
            }
        }
      లేకపోతే
        {
          /* The behavior of lseek is unspecified, but in practice if
             it returns a positive number that's the size of this
             device.  */
          కొలత = lseek (fd, 0, SEEK_END);
          ఒకవేళ (కొలత <= 0)
            {
              /* We are unable to determine the length, up front.
                 Let dopass do that as part of its first iteration.  */
              కొలత = -1;
            }
        }
    }
  లేకపోతే ఒకవేళ (S_ISREG (st.st_బహుళకం)
           && st.st_కొలత < MIN (STP_BLKSIZE (&st), కొలత))
    i_కొలత = st.st_కొలత;

  /* Schedule the passes in random order. */
  genpattern (passarray, flags->n_iterations, s);

  rs = randint_get_ఆధారం (s);

  అయితే (true)
    {
      off_t pass_కొలత;
      గుర్తులేని దీర్ఘ పూర్ణాంకం pn = n;

      ఒకవేళ (i_కొలత)
        {
          pass_కొలత = i_కొలత;
          i_కొలత = 0;
          pn = 0;
        }
      లేకపోతే ఒకవేళ (కొలత)
        {
          pass_కొలత = కొలత;
          కొలత = 0;
        }
      /* TODO: consider handling tail packing by
         writing the tail padding as a separate pass,
         (that would not rewind).  */
      లేకపోతే
        ఆపు;

      కొరకు (size_t i = 0; i < flags->n_iterations + flags->zero_fill; i++)
        {
          పూర్ణాంకం err = 0;
          పూర్ణాంకం type = i < flags->n_iterations ? passarray[i] : 0;

          err = dopass (fd, &st, qname, &pass_కొలత, type, rs, i + 1, pn);

          ఒకవేళ (err)
            {
              ok = false;
              ఒకవేళ (err < 0)
                వెళ్ళు wipefd_బహిర్గతం;
            }
        }
    }

  /* Now deallocate the data.  The effect of ftruncate is specified
     on regular files and shared memory objects (also directories, but
     they are not possible here); don't worry about errors reported
     for other file types.  */

  ఒకవేళ (flags->remove_file && ftruncate (fd, 0) != 0
      && (S_ISREG (st.st_బహుళకం) || S_TYPEISSHM (&st)))
    {
      error (0, errno, _("%s: error truncating"), qname);
      ok = false;
      వెళ్ళు wipefd_బహిర్గతం;
    }

wipefd_బహిర్గతం:
  విడుదల (passarray);
  తిరిగి ok;
}

/* A wrapper with a little more checking for fds on the command line */
స్థిర bool
wipefd (పూర్ణాంకం fd, అక్షరం స్థిరరాశి *qname, నిర్మాణం randint_ఆధారం *s,
        నిర్మాణం Options స్థిరరాశి *flags)
{
  పూర్ణాంకం fd_flags = fcntl (fd, F_GETFL);

  ఒకవేళ (fd_flags < 0)
    {
      error (0, errno, _("%s: fcntl failed"), qname);
      తిరిగి false;
    }
  ఒకవేళ (fd_flags & O_APPEND)
    {
      error (0, 0, _("%s: cannot shred append-only file descriptor"), qname);
      తిరిగి false;
    }
  తిరిగి do_wipefd (fd, qname, s, flags);
}

/* --- Name-wiping code --- */

/* Characters allowed in a file name - a safe universal set.  */
స్థిర అక్షరం స్థిరరాశి nameset[] =
"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_.";

/* Increment NAME (with LEN bytes).  NAME must be a big-endian base N
   number with the digits taken from nameset.  Return true if successful.
   Otherwise, (because NAME already has the greatest possible value)
   return false.  */

స్థిర bool
incname (అక్షరం *name, size_t len)
{
  అయితే (len--)
    {
      అక్షరం స్థిరరాశి *p = strchr (nameset, name[len]);

      /* Given that NAME is composed of bytes from NAMESET,
         P will never be null here.  */

      /* If this character has a successor, use it.  */
      ఒకవేళ (p[1])
        {
          name[len] = p[1];
          తిరిగి true;
        }

      /* Otherwise, set this digit to 0 and increment the prefix.  */
      name[len] = nameset[0];
    }

  తిరిగి false;
}

/*
 * Repeatedly rename a file with shorter and shorter names,
 * to obliterate all traces of the file name (and length) on any system
 * that adds a trailing delimiter to on-device file names and reuses
 * the same directory slot.  Finally, unlink it.
 * The passed-in filename is modified in place to the new filename.
 * (Which is unlinked if this function succeeds, but is still present if
 * it fails for some reason.)
 *
 * The main loop is written carefully to not get stuck if all possible
 * names of a given length are occupied.  It counts down the length from
 * the original to 0.  While the length is non-zero, it tries to find an
 * unused file name of the given length.  It continues until either the
 * name is available and the rename succeeds, or it runs out of names
 * to try (incname wraps and returns 1).  Finally, it unlinks the file.
 *
 * The unlink is Unix-specific, as ANSI-standard remove has more
 * portability problems with C libraries making it "safe".  rename
 * is ANSI-standard.
 *
 * To force the directory data out, we try to open the directory and
 * invoke fdatasync and/or fsync on it.  This is non-standard, so don't
 * insist that it works: just fall back to a global sync in that case.
 * This is fairly significantly Unix-specific.  Of course, on any
 * file system with synchronous metadata updates, this is unnecessary.
 */
స్థిర bool
wipename (అక్షరం *oldname, అక్షరం స్థిరరాశి *qoldname, నిర్మాణం Options స్థిరరాశి *flags)
{
  అక్షరం *newname = xstrdup (oldname);
  అక్షరం *పునాది_2 = చివరి_component (newname);
  అక్షరం *dir = dir_name (newname);
  అక్షరం *qdir = xstrdup (quotef (dir));
  bool మొదటి = true;
  bool ok = true;
  పూర్ణాంకం dir_fd = -1;

  ఒకవేళ (flags->remove_file == remove_wipesync)
    dir_fd = open (dir, O_RDONLY | O_DIRECTORY | O_NOCTTY | O_NONBLOCK);

  ఒకవేళ (flags->verbose)
    error (0, 0, _("%s: removing"), qoldname);

  ఒకవేళ (flags->remove_file != remove_unlink)
    కొరకు (size_t len = పునాది_len (పునాది_2); len != 0; len--)
      {
        memset (పునాది_2, nameset[0], len);
        పునాది_2[len] = 0;
        bool rename_ok;
        అయితే (! (rename_ok = (renameatu (AT_FDCWD, oldname, AT_FDCWD, newname,
                                          RENAME_NOREPLACE)
                               == 0))
               && errno == EEXIST && incname (పునాది_2, len))
          కొనసాగు;
        ఒకవేళ (rename_ok)
          {
            ఒకవేళ (0 <= dir_fd && dosync (dir_fd, qdir) != 0)
              ok = false;
            ఒకవేళ (flags->verbose)
              {
                /* People seem to understand this better than talking
                   about renaming OLDNAME.  NEWNAME doesn't need
                   quoting because we picked it.  OLDNAME needs to be
                   quoted only the first time.  */
                అక్షరం స్థిరరాశి *old = మొదటి ? qoldname : oldname;
                error (0, 0,
                       _("%s: renamed to %s"), old, newname);
                మొదటి = false;
              }
            జ్ఞాపకం_నకలు (oldname + (పునాది_2 - newname), పునాది_2, len + 1);
          }
      }

  ఒకవేళ (unlink (oldname) != 0)
    {
      error (0, errno, _("%s: failed to remove"), qoldname);
      ok = false;
    }
  లేకపోతే ఒకవేళ (flags->verbose)
    error (0, 0, _("%s: removed"), qoldname);
  ఒకవేళ (0 <= dir_fd)
    {
      ఒకవేళ (dosync (dir_fd, qdir) != 0)
        ok = false;
      ఒకవేళ (close (dir_fd) != 0)
        {
          error (0, errno, _("%s: failed to close"), qdir);
          ok = false;
        }
    }
  విడుదల (newname);
  విడుదల (dir);
  విడుదల (qdir);
  తిరిగి ok;
}

/*
 * Finally, the function that actually takes a filename and grinds
 * it into hamburger.
 *
 * FIXME
 * Detail to note: since we do not restore errno to EACCES after
 * a failed chmod, we end up printing the error code from the chmod.
 * This is actually the error that stopped us from proceeding, so
 * it's arguably the right one, and in practice it'll be either EACCES
 * again or EPERM, which both give similar error messages.
 * Does anyone disagree?
 */
స్థిర bool
wipefile (అక్షరం *name, అక్షరం స్థిరరాశి *qname,
          నిర్మాణం randint_ఆధారం *s, నిర్మాణం Options స్థిరరాశి *flags)
{
  పూర్ణాంకం fd = open (name, O_WRONLY | O_NOCTTY | O_NONBLOCK | O_ద్విమాన);
  ఒకవేళ (fd < 0
      && (errno == EACCES && flags->force)
      && chmod (name, S_IWUSR) == 0)
    fd = open (name, O_WRONLY | O_NOCTTY | O_NONBLOCK | O_ద్విమాన);
  ఒకవేళ (fd < 0)
    {
      నిర్మాణం stat st;
      స్థిరరాశి పూర్ణాంకం open_errno = errno;

      /* Try to give a more meaningful error message if we attempt to
         open a FIFO with no readers.  */
      ఒకవేళ (open_errno == ENXIO && 0 <= stat (name, &st)
          && S_ISFIFO (st.st_బహుళకం))
        error (0, 0, _("%s: invalid file type"), qname);
      లేకపోతే
        error (0, open_errno, _("%s: failed to open for writing"), qname);
      తిరిగి false;
    }

  /* We used O_NONBLOCK to prevent blocking when opening a FIFO.
     Reset that here.  */
  పూర్ణాంకం fd_flags = fcntl (fd, F_GETFL);
  bool ok = 0 <= fd_flags && 0 <= fcntl (fd, F_SETFL, fd_flags & ~O_NONBLOCK);
  ఒకవేళ (ok)
    ok = do_wipefd (fd, qname, s, flags);
  లేకపోతే
    error (0, errno, _("couldn't reset non-blocking mode %s"), qname);

  ఒకవేళ (close (fd) != 0)
    {
      error (0, errno, _("%s: failed to close"), qname);
      ok = false;
    }
  ఒకవేళ (ok && flags->remove_file)
    ok = wipename (name, qname, flags);
  తిరిగి ok;
}


/* Buffers for random data.  */
స్థిర నిర్మాణం randint_ఆధారం *randint_ఆధారం;

/* Just on general principles, wipe buffers containing information
   that may be related to the possibly-pseudorandom values used during
   shredding.  */
స్థిర శూన్య
clear_random_దత్తాంశం (శూన్య)
{
  randint_all_free (randint_ఆధారం);
}


పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  bool ok = true;
  నిర్మాణం Options flags = {0};
  అక్షరం **file;
  పూర్ణాంకం n_files;
  పూర్ణాంకం c;
  అక్షరం స్థిరరాశి *random_ఆధారం = ఖాళీ;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  flags.n_iterations = DEFAULT_PASSES;
  flags.కొలత = -1;

  అయితే ((c = getopt_long (argc, argv, "fn:s:uvxz", long_opts, ఖాళీ)) != -1)
    {
      ఎంపిక (c)
        {
        స్థితి 'f':
          flags.force = true;
          ఆపు;

        స్థితి 'n':
          flags.n_iterations = xdectoumax (optarg, 0,
                                           MIN (ULONG_MAX,
                                                SIZE_MAX / పరిమాణం (పూర్ణాంకం)), "",
                                           _("invalid number of passes"), 0);
          ఆపు;

        స్థితి RANDOM_ఆధారం_OPTION:
          ఒకవేళ (random_ఆధారం && !streq (random_ఆధారం, optarg))
            error (EXIT_FAILURE, 0, _("multiple random sources specified"));
          random_ఆధారం = optarg;
          ఆపు;

        స్థితి 'u':
          ఒకవేళ (optarg == ఖాళీ)
            flags.remove_file = remove_wipesync;
          లేకపోతే
            flags.remove_file = XARGMATCH ("--remove", optarg,
                                           remove_వాదనలు, remove_methods);
          ఆపు;

        స్థితి 's':
          flags.కొలత = xnumtoumax (optarg, 0, 0, OFF_T_MAX, "cbBkKMGTPEZYRQ0",
                                   _("invalid file size"), 0, 0);
          ఆపు;

        స్థితి 'v':
          flags.verbose = true;
          ఆపు;

        స్థితి 'x':
          flags.exact = true;
          ఆపు;

        స్థితి 'z':
          flags.zero_fill = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  file = argv + optind;
  n_files = argc - optind;

  ఒకవేళ (n_files == 0)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  randint_ఆధారం = randint_all_new (random_ఆధారం, SIZE_MAX);
  ఒకవేళ (! randint_ఆధారం)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_ఆధారం ? random_ఆధారం : "getrandom"));
  atexit (clear_random_దత్తాంశం);

  కొరకు (పూర్ణాంకం i = 0; i < n_files; i++)
    {
      అక్షరం *qname = xstrdup (quotef (file[i]));
      ఒకవేళ (streq (file[i], "-"))
        {
          ok &= wipefd (STDOUT_FILENO, qname, randint_ఆధారం, &flags);
        }
      లేకపోతే
        {
          /* Plain filename - Note that this overwrites *argv! */
          ok &= wipefile (file[i], qname, randint_ఆధారం, &flags);
        }
      విడుదల (qname);
    }

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
/*
 * vim:sw=2:sts=2:
 */
