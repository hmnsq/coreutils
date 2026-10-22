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
#تعریف_کرو پروگرام_NAME "shred"

#تعریف_کرو AUTHORS proper_name ("Colin Plumb")

#ضمن <config.h>

#ضمن <getopt.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#إذا defined __linux__ && HAVE_SYS_MTIO_H
# include <sys/mtio.h>
#جے_انت

#ضمن "system.h"
#ضمن "alignalloc.h"
#ضمن "argmatch.h"
#ضمن "assure.h"
#ضمن "xdectoint.h"
#ضمن "fcntl--.h"
#ضمن "gethrxtime.h"
#ضمن "human.h"
#ضمن "randint.h"
#ضمن "randread.h"
#ضمن "renameatu.h"
#ضمن "stat-size.h"

/* Default number of times to overwrite.  */
تعداد { DEFAULT_PASSES = 3 };

/* How many seconds to wait before checking whether to output another
   verbose output line.  */
تعداد { VERBOSE_UPDATE = 5 };

/* Sector size and corresponding mask, for recovering after write failures.
   The size must be a power of 2.  */
تعداد { SECTOR_ناپ = 512 };
تعداد { SECTOR_نقاب = SECTOR_ناپ - 1 };
static_assert (0 < SECTOR_ناپ && (SECTOR_ناپ & SECTOR_نقاب) == 0);

تعداد remove_طریقہ
{
  remove_none = 0,      /* the default: only wipe data.  */
  remove_unlink,        /* don't obfuscate name, just unlink.  */
  remove_wipe,          /* obfuscate name before unlink.  */
  remove_wipesync       /* obfuscate name, syncing each byte, before unlink.  */
};

ساكن حرف ثابت *ثابت remove_دلیلاں[] =
{
  "unlink", "wipe", "wipesync", عدم
};

ساكن تعداد remove_طریقہ ثابت remove_methods[] =
{
  remove_unlink, remove_wipe, remove_wipesync
};

بنية Options
{
  bool force;		/* -f flag: chmod files if necessary */
  size_t n_iterations;	/* -n flag: Number of iterations */
  off_t ناپ;		/* -s flag: size of file */
  تعداد remove_طریقہ remove_file; /* -u flag: remove file after shredding */
  bool verbose;		/* -v flag: Print progress */
  bool exact;		/* -x flag: Do not round up file size */
  bool zero_fill;	/* -z flag: Add a final zero pass */
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  RANDOM_سروت_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت long_opts[] =
{
  {"exact", no_دلیل, عدم, 'x'},
  {"force", no_دلیل, عدم, 'f'},
  {"iterations", required_دلیل, عدم, 'n'},
  {"size", required_دلیل, عدم, 's'},
  {"random-source", required_دلیل, عدم, RANDOM_سروت_OPTION},
  {"remove", optional_دلیل, عدم, 'u'},
  {"verbose", no_دلیل, عدم, 'v'},
  {"zero", no_دلیل, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("Usage: %s [OPTION]... FILE...\n"), پروگرام_name);
      fputs (_("\
Overwrite the specified FILE(s) repeatedly, وچ ترتیب منزل make it harder\n\
لكل even very expensive hardware probing منزل recover the انکڑے.\n\
"), stdout);
      fputs (_("\
\n\
If FILE is -, shred standard output.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --force\n\
         change permissions منزل allow writing إذا necessary\n\
"));
      oprintf (_("\
  -n, --iterations=N\n\
         overwrite N times instead دا the افتراضي (%d)\n\
"), DEFAULT_PASSES);
      oputs (_("\
      --random-سروت_2=FILE\n\
         get random بائٹاں from FILE\n\
"));
      oputs (_("\
  -s, --ناپ=N\n\
         shred this many بائٹاں (suffixes like K, M, G accepted)\n\
"));
      oputs (_("\
  -u\n\
         deallocate and remove file after overwriting\n\
"));
      oputs (_("\
      --remove[=HOW]\n\
         like -u but give control on HOW منزل delete;  See ہیٹھاں\n\
"));
      oputs (_("\
  -v, --verbose\n\
         show details دا انکڑے and metadata operations performed\n\
"));
      oputs (_("\
  -x, --exact\n\
         افعل not گیڑا_2 file sizes اتے منزل the اگلا بھریا بلاک;\n\
         this is the افتراضي لكل non-regular files\n\
"));
      oputs (_("\
  -z, --zero\n\
         جوڑو a final overwrite with zeros منزل hide shredding\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
FILE will be skipped with a diagnostic سنیہا_2 إذا it is a FIFO, socket, or\n\
terminal, since its انکڑے does not reside on disk.\n\
"), stdout);
      fputs (_("\
\n\
Delete FILE(s) إذا --remove (-u) is specified.  The افتراضي is not منزل remove\n\
the files because it is common منزل چلاؤ on device files like /dev/hda,\n\
and those files usually should not be removed.\n\
The optional HOW parameter indicates how منزل remove a directory اندراج:\n\
'unlink' => use a standard unlink call.\n\
'wipe' => also پہلا obfuscate بائٹاں وچ the name.\n\
'wipesync' => also sync each obfuscated byte منزل the device.\n\
The افتراضي منوال_2 is 'wipesync', but note it can be expensive.\n\
\n\
"), stdout);
      fputs (_("\
CAUTION: shred assumes the file system and hardware overwrite انکڑے وچ رکھو.\n\
Although this is common, many platforms چلاؤ otherwise.  Also, backups\n\
and mirrors may contain unremovable copies that will let a shredded file\n\
be recovered later.  See the GNU coreutils manual لكل details.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

/*
 * Determine if pattern type is periodic or not.
 */
ساكن bool
periodic_نمونہ (صحيح type)
{
  إذا (type <= 0)
    أرجع false;

  بنانشانی حرف r[3];
  بنانشانی صحيح بٹ_2 = type & 0xfff;

  بٹ_2 |= بٹ_2 << 12;
  r[0] = (بٹ_2 >> 4) & 255;
  r[1] = (بٹ_2 >> 8) & 255;
  r[2] = بٹ_2 & 255;

  أرجع (r[0] != r[1]) || (r[0] != r[2]);
}

/*
 * Fill a buffer with a fixed pattern.
 *
 * The buffer must be at least 3 bytes long, even if
 * size is less.  Larger sizes are filled exactly.
 */
ساكن فراغ
fillpattern (صحيح type, بنانشانی حرف *r, size_t ناپ)
{
  size_t i;
  بنانشانی صحيح بٹ_2 = type & 0xfff;

  بٹ_2 |= بٹ_2 << 12;
  r[0] = (بٹ_2 >> 4) & 255;
  r[1] = (بٹ_2 >> 8) & 255;
  r[2] = بٹ_2 & 255;
  لكل (i = 3; i <= ناپ / 2; i *= 2)
    حافظہ_نقل (r + i, r, i);
  إذا (i < ناپ)
    حافظہ_نقل (r + i, r, ناپ - i);

  /* Invert the first bit of every sector. */
  إذا (type & 0x1000)
    لكل (i = 0; i < ناپ; i += SECTOR_ناپ)
      r[i] ^= 0x80;
}

/*
 * Generate a 6-character (+ nul) pass name string
 * FIXME: allow translation of "random".
 */
#تعریف_کرو PASS_NAME_ناپ 7
ساكن فراغ
passname (بنانشانی حرف ثابت *انکڑے, حرف name[PASS_NAME_ناپ])
{
  إذا (انکڑے)
    sprintf (name, "%02x%02x%02x", انکڑے[0], انکڑے[1], انکڑے[2]);
  وإلا
    حافظہ_نقل (name, "random", PASS_NAME_ناپ);
}

/* Return true when it's ok to ignore an fsync or fdatasync
   failure that set errno to ERRNO_VAL.  */
ساكن bool
ignorable_sync_errno (صحيح errno_val)
{
  أرجع (errno_val == EINVAL
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
ساكن صحيح
dosync (صحيح fd, حرف ثابت *qname)
{
  صحيح err;

  إذا (fdatasync (fd) == 0)
    أرجع 0;
  err = errno;
  إذا ( ! ignorable_sync_errno (err))
    {
      error (0, err, _("%s: fdatasync failed"), qname);
      errno = err;
      أرجع -1;
    }

  إذا (fsync (fd) == 0)
    أرجع 0;
  err = errno;
  إذا ( ! ignorable_sync_errno (err))
    {
      error (0, err, _("%s: fsync failed"), qname);
      errno = err;
      أرجع -1;
    }

  sync ();
  أرجع 0;
}

/* Turn on or off direct I/O mode for file descriptor FD, if possible.
   Try to turn it on if ENABLE is true.  Otherwise, try to turn it off.  */
ساكن فراغ
سدھا_منوال (صحيح fd, bool enable)
{
  إذا (O_سدھا)
    {
      صحيح fd_flags = fcntl (fd, F_GETFL);
      إذا (0 < fd_flags)
        {
          صحيح new_flags = (enable
                           ? (fd_flags | O_سدھا)
                           : (fd_flags & ~O_سدھا));
          إذا (new_flags != fd_flags)
            fcntl (fd, F_SETFL, new_flags);
        }
    }

#إذا HAVE_DIRECTIO && defined DIRECTIO_ON && defined DIRECTIO_OFF
  /* This is Solaris-specific.  */
  directio (fd, enable ? DIRECTIO_ON : DIRECTIO_OFF);
#جے_انت
}

/* Rewind FD; its status is ST.  */
ساكن bool
dorewind (صحيح fd, بنية stat ثابت *st)
{
  إذا (S_ISCHR (st->st_منوال))
    {
#إذا defined __linux__ && HAVE_SYS_MTIO_H
      /* In the Linux kernel, lseek does not work on tape devices; it
         returns a randomish value instead.  Try the low-level tape
         rewind operation first.  */
      بنية mtop op;
      op.mt_op = MTREW;
      op.mt_گنتی = 1;
      إذا (ioctl (fd, MTIOCTOP, &op) == 0)
        أرجع true;
#جے_انت
    }
  off_t وتھ = lseek (fd, 0, SEEK_SET);
  إذا (0 < وتھ)
    errno = EINVAL;
  أرجع وتھ == 0;
}

/* By convention, negative sizes represent unknown values.  */

ساكن bool
معلوم (off_t ناپ)
{
  أرجع 0 <= ناپ;
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
ساكن صحيح
dopass (صحيح fd, بنية stat ثابت *st, حرف ثابت *qname, off_t *sizep,
        صحيح type, بنية randread_سروت *s,
        بنانشانی لما صحيح k, بنانشانی لما صحيح n)
{
  off_t ناپ = *sizep;
  off_t وتھ;                /* Current file position */
  xtime_t prev IF_LINT ( = 0); /* Time we printed the previous update.  */
  xtime_t now = 0;             /* Current time */
  size_t lim;                  /* Amount of data to try writing */
  size_t soff;                 /* Offset into buffer for next write */
  ssize_t ssize;               /* Return value from write */

  /* Fill pattern buffer.  Aligning it to a page so we can do direct I/O.  */
  size_t ورقہ_ناپ = getpagesize ();
#تعریف_کرو PERIODIC_OUTPUT_ناپ (60 * 1024)
#تعریف_کرو NONPERIODIC_OUTPUT_ناپ (64 * 1024)
  static_assert (PERIODIC_OUTPUT_ناپ % 3 == 0);
  size_t output_ناپ = periodic_نمونہ (type)
                       ? PERIODIC_OUTPUT_ناپ : NONPERIODIC_OUTPUT_ناپ;
#تعریف_کرو FILLPATTERN_ناپ (((output_ناپ + 2) / 3) * 3) /* Multiple of 3 */
  بنانشانی حرف *pbuf = xalignalloc (ورقہ_ناپ, FILLPATTERN_ناپ);

  حرف pass_string[PASS_NAME_ناپ];	/* Name of current pass */
  bool write_error = false;
  bool other_error = false;

  /* Printable previous offset into the file */
  حرف پچھلا_وتھ_buf[LONGEST_HUMAN_READABLE + 1];
  حرف ثابت *پچھلا_human_وتھ;

  /* As a performance tweak, avoid direct I/O for small sizes,
     as it's just a performance rather than security consideration,
     and direct I/O can often be unsupported for small non aligned sizes.  */
  bool try_without_directio = 0 < ناپ && ناپ < output_ناپ;
  إذا (! try_without_directio)
    سدھا_منوال (fd, true);

  إذا (! dorewind (fd, st))
    {
      error (0, errno, _("%s: cannot rewind"), qname);
      other_error = true;
      اذهب free_نمونہ_mem;
    }

  /* Constant fill patterns need only be set up once. */
  إذا (type >= 0)
    {
      lim = معلوم (ناپ) && ناپ < FILLPATTERN_ناپ ? ناپ : FILLPATTERN_ناپ;
      fillpattern (type, pbuf, lim);
      passname (pbuf, pass_string);
    }
  وإلا
    {
      passname (عدم, pass_string);
    }

  /* Set position if first status update */
  إذا (n)
    {
      error (0, 0, _("%s: pass %lu/%lu (%s)..."), qname, k, n, pass_string);
      prev = gethrxtime ();
      پچھلا_human_وتھ = "";
    }

  وتھ = 0;
  طالما (true)
    {
      /* How much to write this time? */
      lim = output_ناپ;
      إذا (معلوم (ناپ) && ناپ - وتھ < output_ناپ)
        {
          إذا (ناپ < وتھ)
            اكسر;
          lim = ناپ - وتھ;
          إذا (!lim)
            اكسر;
        }
      إذا (type < 0)
        randread (s, pbuf, lim);
      /* Loop to retry partial writes. */
      لكل (soff = 0; soff < lim; soff += ssize)
        {
          ssize = اكتب (fd, pbuf + soff, lim - soff);
          إذا (ssize <= 0)
            {
              إذا (! معلوم (ناپ) && (ssize == 0 || errno == ENOSPC))
                {
                  /* We have found the end of the file.  */
                  إذا (soff <= OFF_T_MAX - وتھ)
                    *sizep = ناپ = وتھ + soff;
                  اكسر;
                }
              وإلا
                {
                  صحيح errnum = errno;

                  /* Retry without direct I/O since this may not be supported
                     at all on some (file) systems, or with the current size.
                     I.e., a specified --size that is not aligned, or when
                     dealing with slop at the end of a file with --exact.  */
                  إذا (! try_without_directio && errno == EINVAL)
                    {
                      سدھا_منوال (fd, false);
                      ssize = 0;
                      try_without_directio = true;
                      استمر;
                    }
                  uintmax_t error_وتھ = وتھ + soff;
                  error (0, errnum, _("%s: error writing at offset %ju"),
                         qname, error_وتھ);

                  /* 'shred' is often used on bad media, before throwing it
                     out.  Thus, it shouldn't give up on bad blocks.  This
                     code works because lim is always a multiple of
                     SECTOR_SIZE, except at the end.  This size constraint
                     also enables direct I/O on some (file) systems.  */
                  static_assert (PERIODIC_OUTPUT_ناپ % SECTOR_ناپ == 0);
                  static_assert (NONPERIODIC_OUTPUT_ناپ % SECTOR_ناپ == 0);
                  إذا (errnum == EIO && معلوم (ناپ)
                      && (soff | SECTOR_نقاب) < lim)
                    {
                      size_t soff1 = (soff | SECTOR_نقاب) + 1;
                      إذا (lseek (fd, وتھ + soff1, SEEK_SET) != -1)
                        {
                          /* Arrange to skip this block. */
                          ssize = soff1 - soff;
                          write_error = true;
                          استمر;
                        }
                      error (0, errno, _("%s: lseek failed"), qname);
                    }
                  other_error = true;
                  اذهب free_نمونہ_mem;
                }
            }
        }

      /* Okay, we have written "soff" bytes. */

      إذا (ckd_جوڑو (&وتھ, وتھ, soff))
        {
          error (0, 0, _("%s: file too large"), qname);
          other_error = true;
          اذهب free_نمونہ_mem;
        }

      bool پورا = وتھ == ناپ;

      /* Time to print progress? */
      إذا (n && ((پورا && *پچھلا_human_وتھ)
                || VERBOSE_UPDATE <= xtime_sec ((now = gethrxtime ()) - prev)))
        {
          حرف وتھ_buf[LONGEST_HUMAN_READABLE + 1];
          حرف ناپ_buf[LONGEST_HUMAN_READABLE + 1];
          صحيح human_ترقی_opts = (human_autoscale | human_SI
                                     | human_ادھار_1024 | human_B);
          حرف ثابت *human_وتھ
            = human_readable (وتھ, وتھ_buf,
                              human_floor | human_ترقی_opts, 1, 1);

          إذا (پورا || !streq (پچھلا_human_وتھ, human_وتھ))
            {
              إذا (! معلوم (ناپ))
                error (0, 0, _("%s: pass %lu/%lu (%s)...%s"),
                       qname, k, n, pass_string, human_وتھ);
              وإلا
                {
                  uintmax_t off = وتھ;
                  صحيح percent = (ناپ == 0
                                 ? 100
                                 : (off <= UINTMAX_MAX / 100
                                    ? off * 100 / ناپ
                                    : off / (ناپ / 100)));
                  حرف ثابت *human_ناپ
                    = human_readable (ناپ, ناپ_buf,
                                      human_ceiling | human_ترقی_opts,
                                      1, 1);
                  إذا (پورا)
                    human_وتھ = human_ناپ;
                  error (0, 0, _("%s: pass %lu/%lu (%s)...%s/%s %d%%"),
                         qname, k, n, pass_string, human_وتھ, human_ناپ,
                         percent);
                }

              strcpy (پچھلا_وتھ_buf, human_وتھ);
              پچھلا_human_وتھ = پچھلا_وتھ_buf;
              prev = now;

              /*
               * Force periodic syncs to keep displayed progress accurate
               * FIXME: Should these be present even if -v is not enabled,
               * to keep the buffer cache from filling with dirty pages?
               * It's a common problem with programs that do lots of writes,
               * like mkfs.
               */
              إذا (dosync (fd, qname) != 0)
                {
                  إذا (errno != EIO)
                    {
                      other_error = true;
                      اذهب free_نمونہ_mem;
                    }
                  write_error = true;
                }
            }
        }
    }

  /* Force what we just wrote to hit the media. */
  إذا (dosync (fd, qname) != 0)
    {
      إذا (errno != EIO)
        {
          other_error = true;
          اذهب free_نمونہ_mem;
        }
      write_error = true;
    }

free_نمونہ_mem:
  alignfree (pbuf);

  أرجع other_error ? -1 : write_error;
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
ساكن صحيح ثابت
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
  0				/*سرا*/
};

/*
 * Generate a random wiping pass pattern with num passes.
 * This is a two-stage process.  First, the passes to include
 * are chosen, and then they are shuffled into the desired
 * order.
 */
ساكن فراغ
genpattern (صحيح *dest, size_t عدد, بنية randint_سروت *s)
{
  size_t randpasses;
  صحيح ثابت *p;
  صحيح *d;
  size_t n;
  size_t accum, چوٹی, وٹاندرا;
  صحيح k;

  إذا (!عدد)
    أرجع;

  /* Stage 1: choose the passes to use */
  p = patterns;
  randpasses = 0;
  d = dest;			/* Destination for generated pass list */
  n = عدد;			/* Passes remaining to fill */

  طالما (true)
    {
      k = *p++;			/* Block descriptor word */
      إذا (!k)
        {			/* Loop back to the beginning */
          p = patterns;
        }
      وإلا إذا (k < 0)
        {			/* -k random passes */
          k = -k;
          إذا ((size_t) k >= n)
            {
              randpasses += n;
              اكسر;
            }
          randpasses += k;
          n -= k;
        }
      وإلا إذا ((size_t) k <= n)
        {			/* Full block of patterns */
          حافظہ_نقل (d, p, k * حجم (صحيح));
          p += k;
          d += k;
          n -= k;
        }
      وإلا إذا (n < 2 || 3 * n < (size_t) k)
        {			/* Finish with random */
          randpasses += n;
          اكسر;
        }
      وإلا
        {			/* Pad out with n of the k available */
          افعل
            {
              إذا (n == (size_t) k || randint_چنو (s, k) < n)
                {
                  *d++ = *p;
                  n--;
                }
              p++;
              k--;
            }
          طالما (n);
          اكسر;
        }
    }
  چوٹی = عدد - randpasses;	/* Top of initialized data */
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
  لكل (n = 0; n < عدد; n++)
    {
      إذا (accum <= randpasses)
        {
          accum += عدد - 1;
          dest[چوٹی++] = dest[n];
          dest[n] = -1;
        }
      وإلا
        {
          وٹاندرا = n + randint_چنو (s, چوٹی - n);
          k = dest[n];
          dest[n] = dest[وٹاندرا];
          dest[وٹاندرا] = k;
        }
      accum -= randpasses;
    }
  /* affirm (top == num); */
}

/*
 * The core routine to actually do the work.  This overwrites the first
 * size bytes of the given fd.  Return true if successful.
 */
ساكن bool
do_wipefd (صحيح fd, حرف ثابت *qname, بنية randint_سروت *s,
           بنية Options ثابت *flags)
{
  بنية stat st;
  off_t ناپ;		/* Size to write, size to read */
  off_t i_ناپ = 0;	/* For small files, initial size to overwrite inode */
  بنانشانی لما صحيح n;	/* Number of passes for printing purposes */
  صحيح *passarray;
  bool ok = true;
  بنية randread_سروت *rs;

  n = 0;		/* dopass takes n == 0 to mean "don't print progress" */
  إذا (flags->verbose)
    n = flags->n_iterations + flags->zero_fill;

  إذا (fstat (fd, &st))
    {
      error (0, errno, _("%s: fstat failed"), qname);
      أرجع false;
    }

  /* If we know that we can't possibly shred the file, give up now.
     Otherwise, we may go into an infinite loop writing data before we
     find that we can't rewind the device.  */
  إذا ((S_ISCHR (st.st_منوال) && isatty (fd))
      || S_ISFIFO (st.st_منوال)
      || S_ISSOCK (st.st_منوال))
    {
      error (0, 0, _("%s: invalid file type"), qname);
      أرجع false;
    }
  وإلا إذا (S_ISREG (st.st_منوال) && st.st_ناپ < 0)
    {
      error (0, 0, _("%s: file has negative size"), qname);
      أرجع false;
    }

  /* Allocate pass array */
  passarray = xnmalloc (flags->n_iterations, حجم *passarray);

  ناپ = flags->ناپ;
  إذا (ناپ == -1)
    {
      إذا (S_ISREG (st.st_منوال))
        {
          ناپ = st.st_ناپ;

          إذا (! flags->exact)
            {
              /* Round up to the nearest block size to clear slack space.  */
              off_t remainder = ناپ % STP_BLKSIZE (&st);
              إذا (ناپ && ناپ < STP_BLKSIZE (&st))
                i_ناپ = ناپ;
              إذا (remainder != 0)
                {
                  off_t ناپ_incr = STP_BLKSIZE (&st) - remainder;
                  ناپ += MIN (ناپ_incr, OFF_T_MAX - ناپ);
                }
            }
        }
      وإلا
        {
          /* The behavior of lseek is unspecified, but in practice if
             it returns a positive number that's the size of this
             device.  */
          ناپ = lseek (fd, 0, SEEK_END);
          إذا (ناپ <= 0)
            {
              /* We are unable to determine the length, up front.
                 Let dopass do that as part of its first iteration.  */
              ناپ = -1;
            }
        }
    }
  وإلا إذا (S_ISREG (st.st_منوال)
           && st.st_ناپ < MIN (STP_BLKSIZE (&st), ناپ))
    i_ناپ = st.st_ناپ;

  /* Schedule the passes in random order. */
  genpattern (passarray, flags->n_iterations, s);

  rs = randint_get_سروت (s);

  طالما (true)
    {
      off_t pass_ناپ;
      بنانشانی لما صحيح pn = n;

      إذا (i_ناپ)
        {
          pass_ناپ = i_ناپ;
          i_ناپ = 0;
          pn = 0;
        }
      وإلا إذا (ناپ)
        {
          pass_ناپ = ناپ;
          ناپ = 0;
        }
      /* TODO: consider handling tail packing by
         writing the tail padding as a separate pass,
         (that would not rewind).  */
      وإلا
        اكسر;

      لكل (size_t i = 0; i < flags->n_iterations + flags->zero_fill; i++)
        {
          صحيح err = 0;
          صحيح type = i < flags->n_iterations ? passarray[i] : 0;

          err = dopass (fd, &st, qname, &pass_ناپ, type, rs, i + 1, pn);

          إذا (err)
            {
              ok = false;
              إذا (err < 0)
                اذهب wipefd_باہرلا;
            }
        }
    }

  /* Now deallocate the data.  The effect of ftruncate is specified
     on regular files and shared memory objects (also directories, but
     they are not possible here); don't worry about errors reported
     for other file types.  */

  إذا (flags->remove_file && ftruncate (fd, 0) != 0
      && (S_ISREG (st.st_منوال) || S_TYPEISSHM (&st)))
    {
      error (0, errno, _("%s: error truncating"), qname);
      ok = false;
      اذهب wipefd_باہرلا;
    }

wipefd_باہرلا:
  حرر (passarray);
  أرجع ok;
}

/* A wrapper with a little more checking for fds on the command line */
ساكن bool
wipefd (صحيح fd, حرف ثابت *qname, بنية randint_سروت *s,
        بنية Options ثابت *flags)
{
  صحيح fd_flags = fcntl (fd, F_GETFL);

  إذا (fd_flags < 0)
    {
      error (0, errno, _("%s: fcntl failed"), qname);
      أرجع false;
    }
  إذا (fd_flags & O_APPEND)
    {
      error (0, 0, _("%s: cannot shred append-only file descriptor"), qname);
      أرجع false;
    }
  أرجع do_wipefd (fd, qname, s, flags);
}

/* --- Name-wiping code --- */

/* Characters allowed in a file name - a safe universal set.  */
ساكن حرف ثابت nameset[] =
"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_.";

/* Increment NAME (with LEN bytes).  NAME must be a big-endian base N
   number with the digits taken from nameset.  Return true if successful.
   Otherwise, (because NAME already has the greatest possible value)
   return false.  */

ساكن bool
incname (حرف *name, size_t len)
{
  طالما (len--)
    {
      حرف ثابت *p = strchr (nameset, name[len]);

      /* Given that NAME is composed of bytes from NAMESET,
         P will never be null here.  */

      /* If this character has a successor, use it.  */
      إذا (p[1])
        {
          name[len] = p[1];
          أرجع true;
        }

      /* Otherwise, set this digit to 0 and increment the prefix.  */
      name[len] = nameset[0];
    }

  أرجع false;
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
ساكن bool
wipename (حرف *oldname, حرف ثابت *qoldname, بنية Options ثابت *flags)
{
  حرف *newname = xstrdup (oldname);
  حرف *ادھار_2 = آخری_component (newname);
  حرف *dir = dir_name (newname);
  حرف *qdir = xstrdup (quotef (dir));
  bool پہلا = true;
  bool ok = true;
  صحيح dir_fd = -1;

  إذا (flags->remove_file == remove_wipesync)
    dir_fd = open (dir, O_RDONLY | O_DIRECTORY | O_NOCTTY | O_NONBLOCK);

  إذا (flags->verbose)
    error (0, 0, _("%s: removing"), qoldname);

  إذا (flags->remove_file != remove_unlink)
    لكل (size_t len = ادھار_len (ادھار_2); len != 0; len--)
      {
        memset (ادھار_2, nameset[0], len);
        ادھار_2[len] = 0;
        bool rename_ok;
        طالما (! (rename_ok = (renameatu (AT_FDCWD, oldname, AT_FDCWD, newname,
                                          RENAME_NOREPLACE)
                               == 0))
               && errno == EEXIST && incname (ادھار_2, len))
          استمر;
        إذا (rename_ok)
          {
            إذا (0 <= dir_fd && dosync (dir_fd, qdir) != 0)
              ok = false;
            إذا (flags->verbose)
              {
                /* People seem to understand this better than talking
                   about renaming OLDNAME.  NEWNAME doesn't need
                   quoting because we picked it.  OLDNAME needs to be
                   quoted only the first time.  */
                حرف ثابت *old = پہلا ? qoldname : oldname;
                error (0, 0,
                       _("%s: renamed to %s"), old, newname);
                پہلا = false;
              }
            حافظہ_نقل (oldname + (ادھار_2 - newname), ادھار_2, len + 1);
          }
      }

  إذا (unlink (oldname) != 0)
    {
      error (0, errno, _("%s: failed to remove"), qoldname);
      ok = false;
    }
  وإلا إذا (flags->verbose)
    error (0, 0, _("%s: removed"), qoldname);
  إذا (0 <= dir_fd)
    {
      إذا (dosync (dir_fd, qdir) != 0)
        ok = false;
      إذا (close (dir_fd) != 0)
        {
          error (0, errno, _("%s: failed to close"), qdir);
          ok = false;
        }
    }
  حرر (newname);
  حرر (dir);
  حرر (qdir);
  أرجع ok;
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
ساكن bool
wipefile (حرف *name, حرف ثابت *qname,
          بنية randint_سروت *s, بنية Options ثابت *flags)
{
  صحيح fd = open (name, O_WRONLY | O_NOCTTY | O_NONBLOCK | O_ثنائی);
  إذا (fd < 0
      && (errno == EACCES && flags->force)
      && chmod (name, S_IWUSR) == 0)
    fd = open (name, O_WRONLY | O_NOCTTY | O_NONBLOCK | O_ثنائی);
  إذا (fd < 0)
    {
      بنية stat st;
      ثابت صحيح open_errno = errno;

      /* Try to give a more meaningful error message if we attempt to
         open a FIFO with no readers.  */
      إذا (open_errno == ENXIO && 0 <= stat (name, &st)
          && S_ISFIFO (st.st_منوال))
        error (0, 0, _("%s: invalid file type"), qname);
      وإلا
        error (0, open_errno, _("%s: failed to open for writing"), qname);
      أرجع false;
    }

  /* We used O_NONBLOCK to prevent blocking when opening a FIFO.
     Reset that here.  */
  صحيح fd_flags = fcntl (fd, F_GETFL);
  bool ok = 0 <= fd_flags && 0 <= fcntl (fd, F_SETFL, fd_flags & ~O_NONBLOCK);
  إذا (ok)
    ok = do_wipefd (fd, qname, s, flags);
  وإلا
    error (0, errno, _("couldn't reset non-blocking mode %s"), qname);

  إذا (close (fd) != 0)
    {
      error (0, errno, _("%s: failed to close"), qname);
      ok = false;
    }
  إذا (ok && flags->remove_file)
    ok = wipename (name, qname, flags);
  أرجع ok;
}


/* Buffers for random data.  */
ساكن بنية randint_سروت *randint_سروت;

/* Just on general principles, wipe buffers containing information
   that may be related to the possibly-pseudorandom values used during
   shredding.  */
ساكن فراغ
clear_random_انکڑے (فراغ)
{
  randint_all_free (randint_سروت);
}


صحيح
مکھ (صحيح argc, حرف **argv)
{
  bool ok = true;
  بنية Options flags = {0};
  حرف **file;
  صحيح n_files;
  صحيح c;
  حرف ثابت *random_سروت = عدم;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  flags.n_iterations = DEFAULT_PASSES;
  flags.ناپ = -1;

  طالما ((c = getopt_long (argc, argv, "fn:s:uvxz", long_opts, عدم)) != -1)
    {
      بدل (c)
        {
        حالة 'f':
          flags.force = true;
          اكسر;

        حالة 'n':
          flags.n_iterations = xdectoumax (optarg, 0,
                                           MIN (ULONG_MAX,
                                                SIZE_MAX / حجم (صحيح)), "",
                                           _("invalid number of passes"), 0);
          اكسر;

        حالة RANDOM_سروت_OPTION:
          إذا (random_سروت && !streq (random_سروت, optarg))
            error (EXIT_FAILURE, 0, _("multiple random sources specified"));
          random_سروت = optarg;
          اكسر;

        حالة 'u':
          إذا (optarg == عدم)
            flags.remove_file = remove_wipesync;
          وإلا
            flags.remove_file = XARGMATCH ("--remove", optarg,
                                           remove_دلیلاں, remove_methods);
          اكسر;

        حالة 's':
          flags.ناپ = xnumtoumax (optarg, 0, 0, OFF_T_MAX, "cbBkKMGTPEZYRQ0",
                                   _("invalid file size"), 0, 0);
          اكسر;

        حالة 'v':
          flags.verbose = true;
          اكسر;

        حالة 'x':
          flags.exact = true;
          اكسر;

        حالة 'z':
          flags.zero_fill = true;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  file = argv + optind;
  n_files = argc - optind;

  إذا (n_files == 0)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  randint_سروت = randint_all_new (random_سروت, SIZE_MAX);
  إذا (! randint_سروت)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_سروت ? random_سروت : "getrandom"));
  atexit (clear_random_انکڑے);

  لكل (صحيح i = 0; i < n_files; i++)
    {
      حرف *qname = xstrdup (quotef (file[i]));
      إذا (streq (file[i], "-"))
        {
          ok &= wipefd (STDOUT_FILENO, qname, randint_سروت, &flags);
        }
      وإلا
        {
          /* Plain filename - Note that this overwrites *argv! */
          ok &= wipefile (file[i], qname, randint_سروت, &flags);
        }
      حرر (qname);
    }

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
/*
 * vim:sw=2:sts=2:
 */
