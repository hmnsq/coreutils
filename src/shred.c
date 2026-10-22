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
#define PROGRAM_NAME "shred"

#define AUTHORS proper_name ("Colin Plumb")

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#যদি defined __linux__ && HAVE_SYS_MTIO_H
# include <sys/mtio.h>
#endif

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "alignalloc.h"
#অন্তর্ভাব "argmatch.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "xdectoint.h"
#অন্তর্ভাব "fcntl--.h"
#অন্তর্ভাব "gethrxtime.h"
#অন্তর্ভাব "human.h"
#অন্তর্ভাব "randint.h"
#অন্তর্ভাব "randread.h"
#অন্তর্ভাব "renameatu.h"
#অন্তর্ভাব "stat-size.h"

/* Default number of times to overwrite.  */
গণনা { DEFAULT_PASSES = 3 };

/* How many seconds to wait before checking whether to output another
   verbose output line.  */
গণনা { VERBOSE_UPDATE = 5 };

/* Sector size and corresponding mask, for recovering after write failures.
   The size must be a power of 2.  */
গণনা { SECTOR_আকার = 512 };
গণনা { SECTOR_MASK = SECTOR_আকার - 1 };
static_assert (0 < SECTOR_আকার && (SECTOR_আকার & SECTOR_MASK) == 0);

গণনা remove_method
{
  remove_none = 0,      /* the default: only wipe data.  */
  remove_unlink,        /* don't obfuscate name, just unlink.  */
  remove_wipe,          /* obfuscate name before unlink.  */
  remove_wipesync       /* obfuscate name, syncing each byte, before unlink.  */
};

স্থির অক্ষর ধ্রুবক *ধ্রুবক remove_args[] =
{
  "unlink", "wipe", "wipesync", ফাঁকা
};

স্থির গণনা remove_method ধ্রুবক remove_methods[] =
{
  remove_unlink, remove_wipe, remove_wipesync
};

কাঠামো Options
{
  bool force;		/* -f flag: chmod files if necessary */
  size_t n_iterations;	/* -n flag: Number of iterations */
  off_t আকার_2;		/* -s flag: size of file */
  গণনা remove_method remove_file; /* -u flag: remove file after shredding */
  bool verbose;		/* -v flag: Print progress */
  bool exact;		/* -x flag: Do not round up file size */
  bool zero_fill;	/* -z flag: Add a final zero pass */
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  RANDOM_SOURCE_OPTION = CHAR_MAX + 1
};

স্থির কাঠামো option ধ্রুবক long_opts[] =
{
  {"exact", no_argument, ফাঁকা, 'x'},
  {"force", no_argument, ফাঁকা, 'f'},
  {"iterations", required_argument, ফাঁকা, 'n'},
  {"size", required_argument, ফাঁকা, 's'},
  {"random-source", required_argument, ফাঁকা, RANDOM_SOURCE_OPTION},
  {"remove", optional_argument, ফাঁকা, 'u'},
  {"verbose", no_argument, ফাঁকা, 'v'},
  {"zero", no_argument, ফাঁকা, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      fputs (_("\
Overwrite the specified FILE(s) repeatedly, in order to make it harder\n\
জন্য even very expensive hardware probing to recover the তথ্য.\n\
"), stdout);
      fputs (_("\
\n\
If FILE is -, shred standard output.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --force\n\
         change permissions to allow writing যদি necessary\n\
"));
      oprintf (_("\
  -n, --iterations=N\n\
         overwrite N times instead of the পূর্বনির্ধারিত (%d)\n\
"), DEFAULT_PASSES);
      oputs (_("\
      --random-source=FILE\n\
         get random bytes from FILE\n\
"));
      oputs (_("\
  -s, --আকার_2=N\n\
         shred this many bytes (suffixes like K, M, G accepted)\n\
"));
      oputs (_("\
  -u\n\
         deallocate and remove file after overwriting\n\
"));
      oputs (_("\
      --remove[=HOW]\n\
         like -u but give control on HOW to delete;  See below\n\
"));
      oputs (_("\
  -v, --verbose\n\
         show details of তথ্য and metadata operations performed\n\
"));
      oputs (_("\
  -x, --exact\n\
         করো not round file sizes up to the পরবর্তী full block;\n\
         this is the পূর্বনির্ধারিত জন্য non-regular files\n\
"));
      oputs (_("\
  -z, --zero\n\
         add a final overwrite with zeros to hide shredding\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
FILE will be skipped with a diagnostic message যদি it is a FIFO, socket, or\n\
terminal, since its তথ্য does not reside on disk.\n\
"), stdout);
      fputs (_("\
\n\
Delete FILE(s) যদি --remove (-u) is specified.  The পূর্বনির্ধারিত is not to remove\n\
the files because it is common to operate on device files like /dev/hda,\n\
and those files usually should not be removed.\n\
The optional HOW parameter indicates how to remove a directory entry:\n\
'unlink' => use a standard unlink call.\n\
'wipe' => also first obfuscate bytes in the name.\n\
'wipesync' => also sync each obfuscated byte to the device.\n\
The পূর্বনির্ধারিত mode is 'wipesync', but note it can be expensive.\n\
\n\
"), stdout);
      fputs (_("\
CAUTION: shred assumes the file system and hardware overwrite তথ্য in place.\n\
Although this is common, many platforms operate otherwise.  Also, backups\n\
and mirrors may contain unremovable copies that will let a shredded file\n\
be recovered later.  See the GNU coreutils manual জন্য details.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/*
 * Determine if pattern type is periodic or not.
 */
স্থির bool
periodic_pattern (পূর্ণসংখ্যা type)
{
  যদি (type <= 0)
    ফেরত false;

  unsigned অক্ষর r[3];
  unsigned পূর্ণসংখ্যা bits = type & 0xfff;

  bits |= bits << 12;
  r[0] = (bits >> 4) & 255;
  r[1] = (bits >> 8) & 255;
  r[2] = bits & 255;

  ফেরত (r[0] != r[1]) || (r[0] != r[2]);
}

/*
 * Fill a buffer with a fixed pattern.
 *
 * The buffer must be at least 3 bytes long, even if
 * size is less.  Larger sizes are filled exactly.
 */
স্থির শূন্য
fillpattern (পূর্ণসংখ্যা type, unsigned অক্ষর *r, size_t আকার_2)
{
  size_t i;
  unsigned পূর্ণসংখ্যা bits = type & 0xfff;

  bits |= bits << 12;
  r[0] = (bits >> 4) & 255;
  r[1] = (bits >> 8) & 255;
  r[2] = bits & 255;
  জন্য (i = 3; i <= আকার_2 / 2; i *= 2)
    memcpy (r + i, r, i);
  যদি (i < আকার_2)
    memcpy (r + i, r, আকার_2 - i);

  /* Invert the first bit of every sector. */
  যদি (type & 0x1000)
    জন্য (i = 0; i < আকার_2; i += SECTOR_আকার)
      r[i] ^= 0x80;
}

/*
 * Generate a 6-character (+ nul) pass name string
 * FIXME: allow translation of "random".
 */
#define PASS_NAME_আকার 7
স্থির শূন্য
passname (unsigned অক্ষর ধ্রুবক *তথ্য, অক্ষর name[PASS_NAME_আকার])
{
  যদি (তথ্য)
    sprintf (name, "%02x%02x%02x", তথ্য[0], তথ্য[1], তথ্য[2]);
  অন্যথা
    memcpy (name, "random", PASS_NAME_আকার);
}

/* Return true when it's ok to ignore an fsync or fdatasync
   failure that set errno to ERRNO_VAL.  */
স্থির bool
ignorable_sync_errno (পূর্ণসংখ্যা errno_val)
{
  ফেরত (errno_val == EINVAL
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
স্থির পূর্ণসংখ্যা
dosync (পূর্ণসংখ্যা fd, অক্ষর ধ্রুবক *qname)
{
  পূর্ণসংখ্যা err;

  যদি (fdatasync (fd) == 0)
    ফেরত 0;
  err = errno;
  যদি ( ! ignorable_sync_errno (err))
    {
      error (0, err, _("%s: fdatasync failed"), qname);
      errno = err;
      ফেরত -1;
    }

  যদি (fsync (fd) == 0)
    ফেরত 0;
  err = errno;
  যদি ( ! ignorable_sync_errno (err))
    {
      error (0, err, _("%s: fsync failed"), qname);
      errno = err;
      ফেরত -1;
    }

  sync ();
  ফেরত 0;
}

/* Turn on or off direct I/O mode for file descriptor FD, if possible.
   Try to turn it on if ENABLE is true.  Otherwise, try to turn it off.  */
স্থির শূন্য
direct_mode (পূর্ণসংখ্যা fd, bool enable)
{
  যদি (O_DIRECT)
    {
      পূর্ণসংখ্যা fd_flags = fcntl (fd, F_GETFL);
      যদি (0 < fd_flags)
        {
          পূর্ণসংখ্যা new_flags = (enable
                           ? (fd_flags | O_DIRECT)
                           : (fd_flags & ~O_DIRECT));
          যদি (new_flags != fd_flags)
            fcntl (fd, F_SETFL, new_flags);
        }
    }

#যদি HAVE_DIRECTIO && defined DIRECTIO_ON && defined DIRECTIO_OFF
  /* This is Solaris-specific.  */
  directio (fd, enable ? DIRECTIO_ON : DIRECTIO_OFF);
#endif
}

/* Rewind FD; its status is ST.  */
স্থির bool
dorewind (পূর্ণসংখ্যা fd, কাঠামো stat ধ্রুবক *st)
{
  যদি (S_ISCHR (st->st_mode))
    {
#যদি defined __linux__ && HAVE_SYS_MTIO_H
      /* In the Linux kernel, lseek does not work on tape devices; it
         returns a randomish value instead.  Try the low-level tape
         rewind operation first.  */
      কাঠামো mtop op;
      op.mt_op = MTREW;
      op.mt_গুনতি = 1;
      যদি (ioctl (fd, MTIOCTOP, &op) == 0)
        ফেরত true;
#endif
    }
  off_t offset = lseek (fd, 0, SEEK_SET);
  যদি (0 < offset)
    errno = EINVAL;
  ফেরত offset == 0;
}

/* By convention, negative sizes represent unknown values.  */

স্থির bool
known (off_t আকার_2)
{
  ফেরত 0 <= আকার_2;
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
স্থির পূর্ণসংখ্যা
dopass (পূর্ণসংখ্যা fd, কাঠামো stat ধ্রুবক *st, অক্ষর ধ্রুবক *qname, off_t *sizep,
        পূর্ণসংখ্যা type, কাঠামো randread_source *s,
        unsigned long পূর্ণসংখ্যা k, unsigned long পূর্ণসংখ্যা n)
{
  off_t আকার_2 = *sizep;
  off_t offset;                /* Current file position */
  xtime_t পূর্ব IF_LINT ( = 0); /* Time we printed the previous update.  */
  xtime_t now = 0;             /* Current time */
  size_t lim;                  /* Amount of data to try writing */
  size_t soff;                 /* Offset into buffer for next write */
  ssize_t ssize;               /* Return value from write */

  /* Fill pattern buffer.  Aligning it to a page so we can do direct I/O.  */
  size_t page_আকার = getpagesize ();
#define PERIODIC_OUTPUT_আকার (60 * 1024)
#define NONPERIODIC_OUTPUT_আকার (64 * 1024)
  static_assert (PERIODIC_OUTPUT_আকার % 3 == 0);
  size_t output_আকার = periodic_pattern (type)
                       ? PERIODIC_OUTPUT_আকার : NONPERIODIC_OUTPUT_আকার;
#define FILLPATTERN_আকার (((output_আকার + 2) / 3) * 3) /* Multiple of 3 */
  unsigned অক্ষর *pbuf = xalignalloc (page_আকার, FILLPATTERN_আকার);

  অক্ষর pass_string[PASS_NAME_আকার];	/* Name of current pass */
  bool write_error = false;
  bool other_error = false;

  /* Printable previous offset into the file */
  অক্ষর previous_offset_buf[LONGEST_HUMAN_READABLE + 1];
  অক্ষর ধ্রুবক *previous_human_offset;

  /* As a performance tweak, avoid direct I/O for small sizes,
     as it's just a performance rather than security consideration,
     and direct I/O can often be unsupported for small non aligned sizes.  */
  bool try_without_directio = 0 < আকার_2 && আকার_2 < output_আকার;
  যদি (! try_without_directio)
    direct_mode (fd, true);

  যদি (! dorewind (fd, st))
    {
      error (0, errno, _("%s: cannot rewind"), qname);
      other_error = true;
      যাও free_pattern_mem;
    }

  /* Constant fill patterns need only be set up once. */
  যদি (type >= 0)
    {
      lim = known (আকার_2) && আকার_2 < FILLPATTERN_আকার ? আকার_2 : FILLPATTERN_আকার;
      fillpattern (type, pbuf, lim);
      passname (pbuf, pass_string);
    }
  অন্যথা
    {
      passname (ফাঁকা, pass_string);
    }

  /* Set position if first status update */
  যদি (n)
    {
      error (0, 0, _("%s: pass %lu/%lu (%s)..."), qname, k, n, pass_string);
      পূর্ব = gethrxtime ();
      previous_human_offset = "";
    }

  offset = 0;
  যখন (true)
    {
      /* How much to write this time? */
      lim = output_আকার;
      যদি (known (আকার_2) && আকার_2 - offset < output_আকার)
        {
          যদি (আকার_2 < offset)
            বিরতি;
          lim = আকার_2 - offset;
          যদি (!lim)
            বিরতি;
        }
      যদি (type < 0)
        randread (s, pbuf, lim);
      /* Loop to retry partial writes. */
      জন্য (soff = 0; soff < lim; soff += ssize)
        {
          ssize = লিখ (fd, pbuf + soff, lim - soff);
          যদি (ssize <= 0)
            {
              যদি (! known (আকার_2) && (ssize == 0 || errno == ENOSPC))
                {
                  /* We have found the end of the file.  */
                  যদি (soff <= OFF_T_MAX - offset)
                    *sizep = আকার_2 = offset + soff;
                  বিরতি;
                }
              অন্যথা
                {
                  পূর্ণসংখ্যা errnum = errno;

                  /* Retry without direct I/O since this may not be supported
                     at all on some (file) systems, or with the current size.
                     I.e., a specified --size that is not aligned, or when
                     dealing with slop at the end of a file with --exact.  */
                  যদি (! try_without_directio && errno == EINVAL)
                    {
                      direct_mode (fd, false);
                      ssize = 0;
                      try_without_directio = true;
                      চালু;
                    }
                  uintmax_t error_offset = offset + soff;
                  error (0, errnum, _("%s: error writing at offset %ju"),
                         qname, error_offset);

                  /* 'shred' is often used on bad media, before throwing it
                     out.  Thus, it shouldn't give up on bad blocks.  This
                     code works because lim is always a multiple of
                     SECTOR_SIZE, except at the end.  This size constraint
                     also enables direct I/O on some (file) systems.  */
                  static_assert (PERIODIC_OUTPUT_আকার % SECTOR_আকার == 0);
                  static_assert (NONPERIODIC_OUTPUT_আকার % SECTOR_আকার == 0);
                  যদি (errnum == EIO && known (আকার_2)
                      && (soff | SECTOR_MASK) < lim)
                    {
                      size_t soff1 = (soff | SECTOR_MASK) + 1;
                      যদি (lseek (fd, offset + soff1, SEEK_SET) != -1)
                        {
                          /* Arrange to skip this block. */
                          ssize = soff1 - soff;
                          write_error = true;
                          চালু;
                        }
                      error (0, errno, _("%s: lseek failed"), qname);
                    }
                  other_error = true;
                  যাও free_pattern_mem;
                }
            }
        }

      /* Okay, we have written "soff" bytes. */

      যদি (ckd_add (&offset, offset, soff))
        {
          error (0, 0, _("%s: file too large"), qname);
          other_error = true;
          যাও free_pattern_mem;
        }

      bool done = offset == আকার_2;

      /* Time to print progress? */
      যদি (n && ((done && *previous_human_offset)
                || VERBOSE_UPDATE <= xtime_sec ((now = gethrxtime ()) - পূর্ব)))
        {
          অক্ষর offset_buf[LONGEST_HUMAN_READABLE + 1];
          অক্ষর আকার_buf[LONGEST_HUMAN_READABLE + 1];
          পূর্ণসংখ্যা human_progress_opts = (human_autoscale | human_SI
                                     | human_base_1024 | human_B);
          অক্ষর ধ্রুবক *human_offset
            = human_readable (offset, offset_buf,
                              human_floor | human_progress_opts, 1, 1);

          যদি (done || !streq (previous_human_offset, human_offset))
            {
              যদি (! known (আকার_2))
                error (0, 0, _("%s: pass %lu/%lu (%s)...%s"),
                       qname, k, n, pass_string, human_offset);
              অন্যথা
                {
                  uintmax_t off = offset;
                  পূর্ণসংখ্যা percent = (আকার_2 == 0
                                 ? 100
                                 : (off <= UINTMAX_MAX / 100
                                    ? off * 100 / আকার_2
                                    : off / (আকার_2 / 100)));
                  অক্ষর ধ্রুবক *human_আকার
                    = human_readable (আকার_2, আকার_buf,
                                      human_ceiling | human_progress_opts,
                                      1, 1);
                  যদি (done)
                    human_offset = human_আকার;
                  error (0, 0, _("%s: pass %lu/%lu (%s)...%s/%s %d%%"),
                         qname, k, n, pass_string, human_offset, human_আকার,
                         percent);
                }

              strcpy (previous_offset_buf, human_offset);
              previous_human_offset = previous_offset_buf;
              পূর্ব = now;

              /*
               * Force periodic syncs to keep displayed progress accurate
               * FIXME: Should these be present even if -v is not enabled,
               * to keep the buffer cache from filling with dirty pages?
               * It's a common problem with programs that do lots of writes,
               * like mkfs.
               */
              যদি (dosync (fd, qname) != 0)
                {
                  যদি (errno != EIO)
                    {
                      other_error = true;
                      যাও free_pattern_mem;
                    }
                  write_error = true;
                }
            }
        }
    }

  /* Force what we just wrote to hit the media. */
  যদি (dosync (fd, qname) != 0)
    {
      যদি (errno != EIO)
        {
          other_error = true;
          যাও free_pattern_mem;
        }
      write_error = true;
    }

free_pattern_mem:
  alignfree (pbuf);

  ফেরত other_error ? -1 : write_error;
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
স্থির পূর্ণসংখ্যা ধ্রুবক
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
  0				/* End */
};

/*
 * Generate a random wiping pass pattern with num passes.
 * This is a two-stage process.  First, the passes to include
 * are chosen, and then they are shuffled into the desired
 * order.
 */
স্থির শূন্য
genpattern (পূর্ণসংখ্যা *dest, size_t অঙ্ক, কাঠামো randint_source *s)
{
  size_t randpasses;
  পূর্ণসংখ্যা ধ্রুবক *p;
  পূর্ণসংখ্যা *d;
  size_t n;
  size_t accum, শীর্ষ_2, swap;
  পূর্ণসংখ্যা k;

  যদি (!অঙ্ক)
    ফেরত;

  /* Stage 1: choose the passes to use */
  p = patterns;
  randpasses = 0;
  d = dest;			/* Destination for generated pass list */
  n = অঙ্ক;			/* Passes remaining to fill */

  যখন (true)
    {
      k = *p++;			/* Block descriptor word */
      যদি (!k)
        {			/* Loop back to the beginning */
          p = patterns;
        }
      অন্যথা যদি (k < 0)
        {			/* -k random passes */
          k = -k;
          যদি ((size_t) k >= n)
            {
              randpasses += n;
              বিরতি;
            }
          randpasses += k;
          n -= k;
        }
      অন্যথা যদি ((size_t) k <= n)
        {			/* Full block of patterns */
          memcpy (d, p, k * আকার (পূর্ণসংখ্যা));
          p += k;
          d += k;
          n -= k;
        }
      অন্যথা যদি (n < 2 || 3 * n < (size_t) k)
        {			/* Finish with random */
          randpasses += n;
          বিরতি;
        }
      অন্যথা
        {			/* Pad out with n of the k available */
          করো
            {
              যদি (n == (size_t) k || randint_choose (s, k) < n)
                {
                  *d++ = *p;
                  n--;
                }
              p++;
              k--;
            }
          যখন (n);
          বিরতি;
        }
    }
  শীর্ষ_2 = অঙ্ক - randpasses;	/* Top of initialized data */
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
  জন্য (n = 0; n < অঙ্ক; n++)
    {
      যদি (accum <= randpasses)
        {
          accum += অঙ্ক - 1;
          dest[শীর্ষ_2++] = dest[n];
          dest[n] = -1;
        }
      অন্যথা
        {
          swap = n + randint_choose (s, শীর্ষ_2 - n);
          k = dest[n];
          dest[n] = dest[swap];
          dest[swap] = k;
        }
      accum -= randpasses;
    }
  /* affirm (top == num); */
}

/*
 * The core routine to actually do the work.  This overwrites the first
 * size bytes of the given fd.  Return true if successful.
 */
স্থির bool
do_wipefd (পূর্ণসংখ্যা fd, অক্ষর ধ্রুবক *qname, কাঠামো randint_source *s,
           কাঠামো Options ধ্রুবক *flags)
{
  কাঠামো stat st;
  off_t আকার_2;		/* Size to write, size to read */
  off_t i_আকার = 0;	/* For small files, initial size to overwrite inode */
  unsigned long পূর্ণসংখ্যা n;	/* Number of passes for printing purposes */
  পূর্ণসংখ্যা *passarray;
  bool ok = true;
  কাঠামো randread_source *rs;

  n = 0;		/* dopass takes n == 0 to mean "don't print progress" */
  যদি (flags->verbose)
    n = flags->n_iterations + flags->zero_fill;

  যদি (fstat (fd, &st))
    {
      error (0, errno, _("%s: fstat failed"), qname);
      ফেরত false;
    }

  /* If we know that we can't possibly shred the file, give up now.
     Otherwise, we may go into an infinite loop writing data before we
     find that we can't rewind the device.  */
  যদি ((S_ISCHR (st.st_mode) && isatty (fd))
      || S_ISFIFO (st.st_mode)
      || S_ISSOCK (st.st_mode))
    {
      error (0, 0, _("%s: invalid file type"), qname);
      ফেরত false;
    }
  অন্যথা যদি (S_ISREG (st.st_mode) && st.st_আকার < 0)
    {
      error (0, 0, _("%s: file has negative size"), qname);
      ফেরত false;
    }

  /* Allocate pass array */
  passarray = xnmalloc (flags->n_iterations, আকার *passarray);

  আকার_2 = flags->আকার_2;
  যদি (আকার_2 == -1)
    {
      যদি (S_ISREG (st.st_mode))
        {
          আকার_2 = st.st_আকার;

          যদি (! flags->exact)
            {
              /* Round up to the nearest block size to clear slack space.  */
              off_t remainder = আকার_2 % STP_BLKSIZE (&st);
              যদি (আকার_2 && আকার_2 < STP_BLKSIZE (&st))
                i_আকার = আকার_2;
              যদি (remainder != 0)
                {
                  off_t আকার_incr = STP_BLKSIZE (&st) - remainder;
                  আকার_2 += MIN (আকার_incr, OFF_T_MAX - আকার_2);
                }
            }
        }
      অন্যথা
        {
          /* The behavior of lseek is unspecified, but in practice if
             it returns a positive number that's the size of this
             device.  */
          আকার_2 = lseek (fd, 0, SEEK_END);
          যদি (আকার_2 <= 0)
            {
              /* We are unable to determine the length, up front.
                 Let dopass do that as part of its first iteration.  */
              আকার_2 = -1;
            }
        }
    }
  অন্যথা যদি (S_ISREG (st.st_mode)
           && st.st_আকার < MIN (STP_BLKSIZE (&st), আকার_2))
    i_আকার = st.st_আকার;

  /* Schedule the passes in random order. */
  genpattern (passarray, flags->n_iterations, s);

  rs = randint_get_source (s);

  যখন (true)
    {
      off_t pass_আকার;
      unsigned long পূর্ণসংখ্যা pn = n;

      যদি (i_আকার)
        {
          pass_আকার = i_আকার;
          i_আকার = 0;
          pn = 0;
        }
      অন্যথা যদি (আকার_2)
        {
          pass_আকার = আকার_2;
          আকার_2 = 0;
        }
      /* TODO: consider handling tail packing by
         writing the tail padding as a separate pass,
         (that would not rewind).  */
      অন্যথা
        বিরতি;

      জন্য (size_t i = 0; i < flags->n_iterations + flags->zero_fill; i++)
        {
          পূর্ণসংখ্যা err = 0;
          পূর্ণসংখ্যা type = i < flags->n_iterations ? passarray[i] : 0;

          err = dopass (fd, &st, qname, &pass_আকার, type, rs, i + 1, pn);

          যদি (err)
            {
              ok = false;
              যদি (err < 0)
                যাও wipefd_out;
            }
        }
    }

  /* Now deallocate the data.  The effect of ftruncate is specified
     on regular files and shared memory objects (also directories, but
     they are not possible here); don't worry about errors reported
     for other file types.  */

  যদি (flags->remove_file && ftruncate (fd, 0) != 0
      && (S_ISREG (st.st_mode) || S_TYPEISSHM (&st)))
    {
      error (0, errno, _("%s: error truncating"), qname);
      ok = false;
      যাও wipefd_out;
    }

wipefd_out:
  মুক্ত (passarray);
  ফেরত ok;
}

/* A wrapper with a little more checking for fds on the command line */
স্থির bool
wipefd (পূর্ণসংখ্যা fd, অক্ষর ধ্রুবক *qname, কাঠামো randint_source *s,
        কাঠামো Options ধ্রুবক *flags)
{
  পূর্ণসংখ্যা fd_flags = fcntl (fd, F_GETFL);

  যদি (fd_flags < 0)
    {
      error (0, errno, _("%s: fcntl failed"), qname);
      ফেরত false;
    }
  যদি (fd_flags & O_APPEND)
    {
      error (0, 0, _("%s: cannot shred append-only file descriptor"), qname);
      ফেরত false;
    }
  ফেরত do_wipefd (fd, qname, s, flags);
}

/* --- Name-wiping code --- */

/* Characters allowed in a file name - a safe universal set.  */
স্থির অক্ষর ধ্রুবক nameset[] =
"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_.";

/* Increment NAME (with LEN bytes).  NAME must be a big-endian base N
   number with the digits taken from nameset.  Return true if successful.
   Otherwise, (because NAME already has the greatest possible value)
   return false.  */

স্থির bool
incname (অক্ষর *name, size_t len)
{
  যখন (len--)
    {
      অক্ষর ধ্রুবক *p = strchr (nameset, name[len]);

      /* Given that NAME is composed of bytes from NAMESET,
         P will never be null here.  */

      /* If this character has a successor, use it.  */
      যদি (p[1])
        {
          name[len] = p[1];
          ফেরত true;
        }

      /* Otherwise, set this digit to 0 and increment the prefix.  */
      name[len] = nameset[0];
    }

  ফেরত false;
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
স্থির bool
wipename (অক্ষর *oldname, অক্ষর ধ্রুবক *qoldname, কাঠামো Options ধ্রুবক *flags)
{
  অক্ষর *newname = xstrdup (oldname);
  অক্ষর *base = last_component (newname);
  অক্ষর *dir = dir_name (newname);
  অক্ষর *qdir = xstrdup (quotef (dir));
  bool first = true;
  bool ok = true;
  পূর্ণসংখ্যা dir_fd = -1;

  যদি (flags->remove_file == remove_wipesync)
    dir_fd = open (dir, O_RDONLY | O_DIRECTORY | O_NOCTTY | O_NONBLOCK);

  যদি (flags->verbose)
    error (0, 0, _("%s: removing"), qoldname);

  যদি (flags->remove_file != remove_unlink)
    জন্য (size_t len = base_len (base); len != 0; len--)
      {
        memset (base, nameset[0], len);
        base[len] = 0;
        bool rename_ok;
        যখন (! (rename_ok = (renameatu (AT_FDCWD, oldname, AT_FDCWD, newname,
                                          RENAME_NOREPLACE)
                               == 0))
               && errno == EEXIST && incname (base, len))
          চালু;
        যদি (rename_ok)
          {
            যদি (0 <= dir_fd && dosync (dir_fd, qdir) != 0)
              ok = false;
            যদি (flags->verbose)
              {
                /* People seem to understand this better than talking
                   about renaming OLDNAME.  NEWNAME doesn't need
                   quoting because we picked it.  OLDNAME needs to be
                   quoted only the first time.  */
                অক্ষর ধ্রুবক *old = first ? qoldname : oldname;
                error (0, 0,
                       _("%s: renamed to %s"), old, newname);
                first = false;
              }
            memcpy (oldname + (base - newname), base, len + 1);
          }
      }

  যদি (unlink (oldname) != 0)
    {
      error (0, errno, _("%s: failed to remove"), qoldname);
      ok = false;
    }
  অন্যথা যদি (flags->verbose)
    error (0, 0, _("%s: removed"), qoldname);
  যদি (0 <= dir_fd)
    {
      যদি (dosync (dir_fd, qdir) != 0)
        ok = false;
      যদি (close (dir_fd) != 0)
        {
          error (0, errno, _("%s: failed to close"), qdir);
          ok = false;
        }
    }
  মুক্ত (newname);
  মুক্ত (dir);
  মুক্ত (qdir);
  ফেরত ok;
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
স্থির bool
wipefile (অক্ষর *name, অক্ষর ধ্রুবক *qname,
          কাঠামো randint_source *s, কাঠামো Options ধ্রুবক *flags)
{
  পূর্ণসংখ্যা fd = open (name, O_WRONLY | O_NOCTTY | O_NONBLOCK | O_দ্বিমিক);
  যদি (fd < 0
      && (errno == EACCES && flags->force)
      && chmod (name, S_IWUSR) == 0)
    fd = open (name, O_WRONLY | O_NOCTTY | O_NONBLOCK | O_দ্বিমিক);
  যদি (fd < 0)
    {
      কাঠামো stat st;
      ধ্রুবক পূর্ণসংখ্যা open_errno = errno;

      /* Try to give a more meaningful error message if we attempt to
         open a FIFO with no readers.  */
      যদি (open_errno == ENXIO && 0 <= stat (name, &st)
          && S_ISFIFO (st.st_mode))
        error (0, 0, _("%s: invalid file type"), qname);
      অন্যথা
        error (0, open_errno, _("%s: failed to open for writing"), qname);
      ফেরত false;
    }

  /* We used O_NONBLOCK to prevent blocking when opening a FIFO.
     Reset that here.  */
  পূর্ণসংখ্যা fd_flags = fcntl (fd, F_GETFL);
  bool ok = 0 <= fd_flags && 0 <= fcntl (fd, F_SETFL, fd_flags & ~O_NONBLOCK);
  যদি (ok)
    ok = do_wipefd (fd, qname, s, flags);
  অন্যথা
    error (0, errno, _("couldn't reset non-blocking mode %s"), qname);

  যদি (close (fd) != 0)
    {
      error (0, errno, _("%s: failed to close"), qname);
      ok = false;
    }
  যদি (ok && flags->remove_file)
    ok = wipename (name, qname, flags);
  ফেরত ok;
}


/* Buffers for random data.  */
স্থির কাঠামো randint_source *randint_source;

/* Just on general principles, wipe buffers containing information
   that may be related to the possibly-pseudorandom values used during
   shredding.  */
স্থির শূন্য
clear_random_তথ্য (শূন্য)
{
  randint_all_free (randint_source);
}


পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  bool ok = true;
  কাঠামো Options flags = {0};
  অক্ষর **file;
  পূর্ণসংখ্যা n_files;
  পূর্ণসংখ্যা c;
  অক্ষর ধ্রুবক *random_source = ফাঁকা;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  flags.n_iterations = DEFAULT_PASSES;
  flags.আকার_2 = -1;

  যখন ((c = getopt_long (argc, argv, "fn:s:uvxz", long_opts, ফাঁকা)) != -1)
    {
      নির্বাচন (c)
        {
        ক্ষেত্র 'f':
          flags.force = true;
          বিরতি;

        ক্ষেত্র 'n':
          flags.n_iterations = xdectoumax (optarg, 0,
                                           MIN (ULONG_MAX,
                                                SIZE_MAX / আকার (পূর্ণসংখ্যা)), "",
                                           _("invalid number of passes"), 0);
          বিরতি;

        ক্ষেত্র RANDOM_SOURCE_OPTION:
          যদি (random_source && !streq (random_source, optarg))
            error (EXIT_FAILURE, 0, _("multiple random sources specified"));
          random_source = optarg;
          বিরতি;

        ক্ষেত্র 'u':
          যদি (optarg == ফাঁকা)
            flags.remove_file = remove_wipesync;
          অন্যথা
            flags.remove_file = XARGMATCH ("--remove", optarg,
                                           remove_args, remove_methods);
          বিরতি;

        ক্ষেত্র 's':
          flags.আকার_2 = xnumtoumax (optarg, 0, 0, OFF_T_MAX, "cbBkKMGTPEZYRQ0",
                                   _("invalid file size"), 0, 0);
          বিরতি;

        ক্ষেত্র 'v':
          flags.verbose = true;
          বিরতি;

        ক্ষেত্র 'x':
          flags.exact = true;
          বিরতি;

        ক্ষেত্র 'z':
          flags.zero_fill = true;
          বিরতি;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  file = argv + optind;
  n_files = argc - optind;

  যদি (n_files == 0)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  randint_source = randint_all_new (random_source, SIZE_MAX);
  যদি (! randint_source)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_source ? random_source : "getrandom"));
  atexit (clear_random_তথ্য);

  জন্য (পূর্ণসংখ্যা i = 0; i < n_files; i++)
    {
      অক্ষর *qname = xstrdup (quotef (file[i]));
      যদি (streq (file[i], "-"))
        {
          ok &= wipefd (STDOUT_FILENO, qname, randint_source, &flags);
        }
      অন্যথা
        {
          /* Plain filename - Note that this overwrites *argv! */
          ok &= wipefile (file[i], qname, randint_source, &flags);
        }
      মুক্ত (qname);
    }

  ফেরত ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
/*
 * vim:sw=2:sts=2:
 */
