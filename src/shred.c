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
#परिभाषय क्रमादेशः_NAME "shred"

#परिभाषय AUTHORS proper_name ("Colin Plumb")

#अन्तर्भाव <config.h>

#अन्तर्भाव <getopt.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#यदि defined __linux__ && HAVE_SYS_MTIO_H
# include <sys/mtio.h>
#यदि_अन्तः

#अन्तर्भाव "system.h"
#अन्तर्भाव "alignalloc.h"
#अन्तर्भाव "argmatch.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "xdectoint.h"
#अन्तर्भाव "fcntl--.h"
#अन्तर्भाव "gethrxtime.h"
#अन्तर्भाव "human.h"
#अन्तर्भाव "randint.h"
#अन्तर्भाव "randread.h"
#अन्तर्भाव "renameatu.h"
#अन्तर्भाव "stat-size.h"

/* Default number of times to overwrite.  */
गणना { DEFAULT_PASSES = 3 };

/* How many seconds to wait before checking whether to output another
   verbose output line.  */
गणना { VERBOSE_UPDATE = 5 };

/* Sector size and corresponding mask, for recovering after write failures.
   The size must be a power of 2.  */
गणना { SECTOR_आकार = 512 };
गणना { SECTOR_आवरणम् = SECTOR_आकार - 1 };
static_assert (0 < SECTOR_आकार && (SECTOR_आकार & SECTOR_आवरणम्) == 0);

गणना remove_विधानम्
{
  remove_none = 0,      /* the default: only wipe data.  */
  remove_unlink,        /* don't obfuscate name, just unlink.  */
  remove_wipe,          /* obfuscate name before unlink.  */
  remove_wipesync       /* obfuscate name, syncing each byte, before unlink.  */
};

स्थिर वर्ण ध्रुव *ध्रुव remove_तर्काः[] =
{
  "unlink", "wipe", "wipesync", रिक्त
};

स्थिर गणना remove_विधानम् ध्रुव remove_methods[] =
{
  remove_unlink, remove_wipe, remove_wipesync
};

संरचना Options
{
  bool force;		/* -f flag: chmod files if necessary */
  size_t n_iterations;	/* -n flag: Number of iterations */
  off_t आकार;		/* -s flag: size of file */
  गणना remove_विधानम् remove_file; /* -u flag: remove file after shredding */
  bool verbose;		/* -v flag: Print progress */
  bool exact;		/* -x flag: Do not round up file size */
  bool zero_fill;	/* -z flag: Add a final zero pass */
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  RANDOM_SOURCE_OPTION = CHAR_MAX + 1
};

स्थिर संरचना option ध्रुव long_opts[] =
{
  {"exact", no_तर्कः, रिक्त, 'x'},
  {"force", no_तर्कः, रिक्त, 'f'},
  {"iterations", required_तर्कः, रिक्त, 'n'},
  {"size", required_तर्कः, रिक्त, 's'},
  {"random-source", required_तर्कः, रिक्त, RANDOM_SOURCE_OPTION},
  {"remove", optional_तर्कः, रिक्त, 'u'},
  {"verbose", no_तर्कः, रिक्त, 'v'},
  {"zero", no_तर्कः, रिक्त, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("Usage: %s [OPTION]... FILE...\n"), क्रमादेशः_name);
      fputs (_("\
Overwrite the specified FILE(s) repeatedly, अन्तर् क्रमः गन्तव्यम् make it harder\n\
प्रति even very expensive hardware probing गन्तव्यम् recover the दत्त.\n\
"), stdout);
      fputs (_("\
\n\
If FILE is -, shred standard output.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --force\n\
         change permissions गन्तव्यम् allow writing यदि necessary\n\
"));
      oprintf (_("\
  -n, --iterations=N\n\
         overwrite N times instead सम्बन्धि the स्वभाव (%d)\n\
"), DEFAULT_PASSES);
      oputs (_("\
      --random-source=FILE\n\
         get random अष्टकानि from FILE\n\
"));
      oputs (_("\
  -s, --आकार=N\n\
         shred this many अष्टकानि (suffixes like K, M, G accepted)\n\
"));
      oputs (_("\
  -u\n\
         deallocate and remove file after overwriting\n\
"));
      oputs (_("\
      --remove[=HOW]\n\
         like -u but give control on HOW गन्तव्यम् delete;  See अधः\n\
"));
      oputs (_("\
  -v, --verbose\n\
         show details सम्बन्धि दत्त and metadata operations performed\n\
"));
      oputs (_("\
  -x, --exact\n\
         कुरु not आवर्तनम्_2 file sizes ऊर्ध्वम् गन्तव्यम् the अग्र पूर्णम् खण्डम्;\n\
         this is the स्वभाव प्रति non-regular files\n\
"));
      oputs (_("\
  -z, --zero\n\
         सङ्कलय a final overwrite with zeros गन्तव्यम् hide shredding\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      fputs (_("\
\n\
FILE will be skipped with a diagnostic सन्देशः_2 यदि it is a FIFO, socket, or\n\
terminal, since its दत्त does not reside on disk.\n\
"), stdout);
      fputs (_("\
\n\
Delete FILE(s) यदि --remove (-u) is specified.  The स्वभाव is not गन्तव्यम् remove\n\
the files because it is common गन्तव्यम् सञ्चालय on device files like /dev/hda,\n\
and those files usually should not be removed.\n\
The optional HOW parameter indicates how गन्तव्यम् remove a directory प्रविष्टिः:\n\
'unlink' => use a standard unlink call.\n\
'wipe' => also first obfuscate अष्टकानि अन्तर् the name.\n\
'wipesync' => also sync each obfuscated byte गन्तव्यम् the device.\n\
The स्वभाव बहुलकम्_2 is 'wipesync', but note it can be expensive.\n\
\n\
"), stdout);
      fputs (_("\
CAUTION: shred assumes the file system and hardware overwrite दत्त अन्तर् place.\n\
Although this is common, many platforms सञ्चालय otherwise.  Also, backups\n\
and mirrors may contain unremovable copies that will let a shredded file\n\
be recovered later.  See the GNU coreutils manual प्रति details.\n\
"), stdout);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/*
 * Determine if pattern type is periodic or not.
 */
स्थिर bool
periodic_आदर्शः (पूर्णाङ्क type)
{
  यदि (type <= 0)
    प्रत्यागम false;

  अचिह्नित वर्ण r[3];
  अचिह्नित पूर्णाङ्क द्वयङ्काः_2 = type & 0xfff;

  द्वयङ्काः_2 |= द्वयङ्काः_2 << 12;
  r[0] = (द्वयङ्काः_2 >> 4) & 255;
  r[1] = (द्वयङ्काः_2 >> 8) & 255;
  r[2] = द्वयङ्काः_2 & 255;

  प्रत्यागम (r[0] != r[1]) || (r[0] != r[2]);
}

/*
 * Fill a buffer with a fixed pattern.
 *
 * The buffer must be at least 3 bytes long, even if
 * size is less.  Larger sizes are filled exactly.
 */
स्थिर शून्य
fillpattern (पूर्णाङ्क type, अचिह्नित वर्ण *r, size_t आकार)
{
  size_t i;
  अचिह्नित पूर्णाङ्क द्वयङ्काः_2 = type & 0xfff;

  द्वयङ्काः_2 |= द्वयङ्काः_2 << 12;
  r[0] = (द्वयङ्काः_2 >> 4) & 255;
  r[1] = (द्वयङ्काः_2 >> 8) & 255;
  r[2] = द्वयङ्काः_2 & 255;
  प्रति (i = 3; i <= आकार / 2; i *= 2)
    स्मृतिं_प्रतिलिख (r + i, r, i);
  यदि (i < आकार)
    स्मृतिं_प्रतिलिख (r + i, r, आकार - i);

  /* Invert the first bit of every sector. */
  यदि (type & 0x1000)
    प्रति (i = 0; i < आकार; i += SECTOR_आकार)
      r[i] ^= 0x80;
}

/*
 * Generate a 6-character (+ nul) pass name string
 * FIXME: allow translation of "random".
 */
#परिभाषय PASS_NAME_आकार 7
स्थिर शून्य
passname (अचिह्नित वर्ण ध्रुव *दत्त, वर्ण name[PASS_NAME_आकार])
{
  यदि (दत्त)
    sprintf (name, "%02x%02x%02x", दत्त[0], दत्त[1], दत्त[2]);
  अन्यथा
    स्मृतिं_प्रतिलिख (name, "random", PASS_NAME_आकार);
}

/* Return true when it's ok to ignore an fsync or fdatasync
   failure that set errno to ERRNO_VAL.  */
स्थिर bool
ignorable_sync_errno (पूर्णाङ्क errno_val)
{
  प्रत्यागम (errno_val == EINVAL
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
स्थिर पूर्णाङ्क
dosync (पूर्णाङ्क fd, वर्ण ध्रुव *qname)
{
  पूर्णाङ्क err;

  यदि (fdatasync (fd) == 0)
    प्रत्यागम 0;
  err = errno;
  यदि ( ! ignorable_sync_errno (err))
    {
      error (0, err, _("%s: fdatasync failed"), qname);
      errno = err;
      प्रत्यागम -1;
    }

  यदि (fsync (fd) == 0)
    प्रत्यागम 0;
  err = errno;
  यदि ( ! ignorable_sync_errno (err))
    {
      error (0, err, _("%s: fsync failed"), qname);
      errno = err;
      प्रत्यागम -1;
    }

  sync ();
  प्रत्यागम 0;
}

/* Turn on or off direct I/O mode for file descriptor FD, if possible.
   Try to turn it on if ENABLE is true.  Otherwise, try to turn it off.  */
स्थिर शून्य
प्रत्यक्षम्_बहुलकम् (पूर्णाङ्क fd, bool enable)
{
  यदि (O_प्रत्यक्षम्)
    {
      पूर्णाङ्क fd_flags = fcntl (fd, F_GETFL);
      यदि (0 < fd_flags)
        {
          पूर्णाङ्क new_flags = (enable
                           ? (fd_flags | O_प्रत्यक्षम्)
                           : (fd_flags & ~O_प्रत्यक्षम्));
          यदि (new_flags != fd_flags)
            fcntl (fd, F_SETFL, new_flags);
        }
    }

#यदि HAVE_DIRECTIO && defined DIRECTIO_ON && defined DIRECTIO_OFF
  /* This is Solaris-specific.  */
  directio (fd, enable ? DIRECTIO_ON : DIRECTIO_OFF);
#यदि_अन्तः
}

/* Rewind FD; its status is ST.  */
स्थिर bool
dorewind (पूर्णाङ्क fd, संरचना stat ध्रुव *st)
{
  यदि (S_ISCHR (st->st_बहुलकम्))
    {
#यदि defined __linux__ && HAVE_SYS_MTIO_H
      /* In the Linux kernel, lseek does not work on tape devices; it
         returns a randomish value instead.  Try the low-level tape
         rewind operation first.  */
      संरचना mtop op;
      op.mt_op = MTREW;
      op.mt_गणनम् = 1;
      यदि (ioctl (fd, MTIOCTOP, &op) == 0)
        प्रत्यागम true;
#यदि_अन्तः
    }
  off_t विस्थापनम् = lseek (fd, 0, SEEK_SET);
  यदि (0 < विस्थापनम्)
    errno = EINVAL;
  प्रत्यागम विस्थापनम् == 0;
}

/* By convention, negative sizes represent unknown values.  */

स्थिर bool
ज्ञातम् (off_t आकार)
{
  प्रत्यागम 0 <= आकार;
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
स्थिर पूर्णाङ्क
dopass (पूर्णाङ्क fd, संरचना stat ध्रुव *st, वर्ण ध्रुव *qname, off_t *sizep,
        पूर्णाङ्क type, संरचना randread_source *s,
        अचिह्नित दीर्घ पूर्णाङ्क k, अचिह्नित दीर्घ पूर्णाङ्क n)
{
  off_t आकार = *sizep;
  off_t विस्थापनम्;                /* Current file position */
  xtime_t पूर्व IF_LINT ( = 0); /* Time we printed the previous update.  */
  xtime_t now = 0;             /* Current time */
  size_t lim;                  /* Amount of data to try writing */
  size_t soff;                 /* Offset into buffer for next write */
  ssize_t ssize;               /* Return value from write */

  /* Fill pattern buffer.  Aligning it to a page so we can do direct I/O.  */
  size_t page_आकार = getpagesize ();
#परिभाषय PERIODIC_OUTPUT_आकार (60 * 1024)
#परिभाषय NONPERIODIC_OUTPUT_आकार (64 * 1024)
  static_assert (PERIODIC_OUTPUT_आकार % 3 == 0);
  size_t output_आकार = periodic_आदर्शः (type)
                       ? PERIODIC_OUTPUT_आकार : NONPERIODIC_OUTPUT_आकार;
#परिभाषय FILLPATTERN_आकार (((output_आकार + 2) / 3) * 3) /* Multiple of 3 */
  अचिह्नित वर्ण *pbuf = xalignalloc (page_आकार, FILLPATTERN_आकार);

  वर्ण pass_string[PASS_NAME_आकार];	/* Name of current pass */
  bool write_error = false;
  bool other_error = false;

  /* Printable previous offset into the file */
  वर्ण previous_विस्थापनम्_buf[LONGEST_HUMAN_READABLE + 1];
  वर्ण ध्रुव *previous_human_विस्थापनम्;

  /* As a performance tweak, avoid direct I/O for small sizes,
     as it's just a performance rather than security consideration,
     and direct I/O can often be unsupported for small non aligned sizes.  */
  bool try_without_directio = 0 < आकार && आकार < output_आकार;
  यदि (! try_without_directio)
    प्रत्यक्षम्_बहुलकम् (fd, true);

  यदि (! dorewind (fd, st))
    {
      error (0, errno, _("%s: cannot rewind"), qname);
      other_error = true;
      गच्छ free_आदर्शः_mem;
    }

  /* Constant fill patterns need only be set up once. */
  यदि (type >= 0)
    {
      lim = ज्ञातम् (आकार) && आकार < FILLPATTERN_आकार ? आकार : FILLPATTERN_आकार;
      fillpattern (type, pbuf, lim);
      passname (pbuf, pass_string);
    }
  अन्यथा
    {
      passname (रिक्त, pass_string);
    }

  /* Set position if first status update */
  यदि (n)
    {
      error (0, 0, _("%s: pass %lu/%lu (%s)..."), qname, k, n, pass_string);
      पूर्व = gethrxtime ();
      previous_human_विस्थापनम् = "";
    }

  विस्थापनम् = 0;
  यावत् (true)
    {
      /* How much to write this time? */
      lim = output_आकार;
      यदि (ज्ञातम् (आकार) && आकार - विस्थापनम् < output_आकार)
        {
          यदि (आकार < विस्थापनम्)
            भङ्ग;
          lim = आकार - विस्थापनम्;
          यदि (!lim)
            भङ्ग;
        }
      यदि (type < 0)
        randread (s, pbuf, lim);
      /* Loop to retry partial writes. */
      प्रति (soff = 0; soff < lim; soff += ssize)
        {
          ssize = लिख (fd, pbuf + soff, lim - soff);
          यदि (ssize <= 0)
            {
              यदि (! ज्ञातम् (आकार) && (ssize == 0 || errno == ENOSPC))
                {
                  /* We have found the end of the file.  */
                  यदि (soff <= OFF_T_MAX - विस्थापनम्)
                    *sizep = आकार = विस्थापनम् + soff;
                  भङ्ग;
                }
              अन्यथा
                {
                  पूर्णाङ्क errnum = errno;

                  /* Retry without direct I/O since this may not be supported
                     at all on some (file) systems, or with the current size.
                     I.e., a specified --size that is not aligned, or when
                     dealing with slop at the end of a file with --exact.  */
                  यदि (! try_without_directio && errno == EINVAL)
                    {
                      प्रत्यक्षम्_बहुलकम् (fd, false);
                      ssize = 0;
                      try_without_directio = true;
                      अनुवर्त;
                    }
                  uintmax_t error_विस्थापनम् = विस्थापनम् + soff;
                  error (0, errnum, _("%s: error writing at offset %ju"),
                         qname, error_विस्थापनम्);

                  /* 'shred' is often used on bad media, before throwing it
                     out.  Thus, it shouldn't give up on bad blocks.  This
                     code works because lim is always a multiple of
                     SECTOR_SIZE, except at the end.  This size constraint
                     also enables direct I/O on some (file) systems.  */
                  static_assert (PERIODIC_OUTPUT_आकार % SECTOR_आकार == 0);
                  static_assert (NONPERIODIC_OUTPUT_आकार % SECTOR_आकार == 0);
                  यदि (errnum == EIO && ज्ञातम् (आकार)
                      && (soff | SECTOR_आवरणम्) < lim)
                    {
                      size_t soff1 = (soff | SECTOR_आवरणम्) + 1;
                      यदि (lseek (fd, विस्थापनम् + soff1, SEEK_SET) != -1)
                        {
                          /* Arrange to skip this block. */
                          ssize = soff1 - soff;
                          write_error = true;
                          अनुवर्त;
                        }
                      error (0, errno, _("%s: lseek failed"), qname);
                    }
                  other_error = true;
                  गच्छ free_आदर्शः_mem;
                }
            }
        }

      /* Okay, we have written "soff" bytes. */

      यदि (ckd_सङ्कलय (&विस्थापनम्, विस्थापनम्, soff))
        {
          error (0, 0, _("%s: file too large"), qname);
          other_error = true;
          गच्छ free_आदर्शः_mem;
        }

      bool समाप्तम् = विस्थापनम् == आकार;

      /* Time to print progress? */
      यदि (n && ((समाप्तम् && *previous_human_विस्थापनम्)
                || VERBOSE_UPDATE <= xtime_sec ((now = gethrxtime ()) - पूर्व)))
        {
          वर्ण विस्थापनम्_buf[LONGEST_HUMAN_READABLE + 1];
          वर्ण आकार_buf[LONGEST_HUMAN_READABLE + 1];
          पूर्णाङ्क human_प्रगतिः_opts = (human_autoscale | human_SI
                                     | human_आधारः_1024 | human_B);
          वर्ण ध्रुव *human_विस्थापनम्
            = human_readable (विस्थापनम्, विस्थापनम्_buf,
                              human_floor | human_प्रगतिः_opts, 1, 1);

          यदि (समाप्तम् || !streq (previous_human_विस्थापनम्, human_विस्थापनम्))
            {
              यदि (! ज्ञातम् (आकार))
                error (0, 0, _("%s: pass %lu/%lu (%s)...%s"),
                       qname, k, n, pass_string, human_विस्थापनम्);
              अन्यथा
                {
                  uintmax_t off = विस्थापनम्;
                  पूर्णाङ्क percent = (आकार == 0
                                 ? 100
                                 : (off <= UINTMAX_MAX / 100
                                    ? off * 100 / आकार
                                    : off / (आकार / 100)));
                  वर्ण ध्रुव *human_आकार
                    = human_readable (आकार, आकार_buf,
                                      human_ceiling | human_प्रगतिः_opts,
                                      1, 1);
                  यदि (समाप्तम्)
                    human_विस्थापनम् = human_आकार;
                  error (0, 0, _("%s: pass %lu/%lu (%s)...%s/%s %d%%"),
                         qname, k, n, pass_string, human_विस्थापनम्, human_आकार,
                         percent);
                }

              वाक्यं_प्रतिलिख (previous_विस्थापनम्_buf, human_विस्थापनम्);
              previous_human_विस्थापनम् = previous_विस्थापनम्_buf;
              पूर्व = now;

              /*
               * Force periodic syncs to keep displayed progress accurate
               * FIXME: Should these be present even if -v is not enabled,
               * to keep the buffer cache from filling with dirty pages?
               * It's a common problem with programs that do lots of writes,
               * like mkfs.
               */
              यदि (dosync (fd, qname) != 0)
                {
                  यदि (errno != EIO)
                    {
                      other_error = true;
                      गच्छ free_आदर्शः_mem;
                    }
                  write_error = true;
                }
            }
        }
    }

  /* Force what we just wrote to hit the media. */
  यदि (dosync (fd, qname) != 0)
    {
      यदि (errno != EIO)
        {
          other_error = true;
          गच्छ free_आदर्शः_mem;
        }
      write_error = true;
    }

free_आदर्शः_mem:
  alignfree (pbuf);

  प्रत्यागम other_error ? -1 : write_error;
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
स्थिर पूर्णाङ्क ध्रुव
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
  0				/*अन्तः*/
};

/*
 * Generate a random wiping pass pattern with num passes.
 * This is a two-stage process.  First, the passes to include
 * are chosen, and then they are shuffled into the desired
 * order.
 */
स्थिर शून्य
genpattern (पूर्णाङ्क *dest, size_t अङ्कः, संरचना randint_source *s)
{
  size_t randpasses;
  पूर्णाङ्क ध्रुव *p;
  पूर्णाङ्क *d;
  size_t n;
  size_t accum, शिखरम्, swap;
  पूर्णाङ्क k;

  यदि (!अङ्कः)
    प्रत्यागम;

  /* Stage 1: choose the passes to use */
  p = patterns;
  randpasses = 0;
  d = dest;			/* Destination for generated pass list */
  n = अङ्कः;			/* Passes remaining to fill */

  यावत् (true)
    {
      k = *p++;			/* Block descriptor word */
      यदि (!k)
        {			/* Loop back to the beginning */
          p = patterns;
        }
      अन्यथा यदि (k < 0)
        {			/* -k random passes */
          k = -k;
          यदि ((size_t) k >= n)
            {
              randpasses += n;
              भङ्ग;
            }
          randpasses += k;
          n -= k;
        }
      अन्यथा यदि ((size_t) k <= n)
        {			/* Full block of patterns */
          स्मृतिं_प्रतिलिख (d, p, k * परिमाण (पूर्णाङ्क));
          p += k;
          d += k;
          n -= k;
        }
      अन्यथा यदि (n < 2 || 3 * n < (size_t) k)
        {			/* Finish with random */
          randpasses += n;
          भङ्ग;
        }
      अन्यथा
        {			/* Pad out with n of the k available */
          कुरु
            {
              यदि (n == (size_t) k || randint_वृणु (s, k) < n)
                {
                  *d++ = *p;
                  n--;
                }
              p++;
              k--;
            }
          यावत् (n);
          भङ्ग;
        }
    }
  शिखरम् = अङ्कः - randpasses;	/* Top of initialized data */
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
  प्रति (n = 0; n < अङ्कः; n++)
    {
      यदि (accum <= randpasses)
        {
          accum += अङ्कः - 1;
          dest[शिखरम्++] = dest[n];
          dest[n] = -1;
        }
      अन्यथा
        {
          swap = n + randint_वृणु (s, शिखरम् - n);
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
स्थिर bool
do_wipefd (पूर्णाङ्क fd, वर्ण ध्रुव *qname, संरचना randint_source *s,
           संरचना Options ध्रुव *flags)
{
  संरचना stat st;
  off_t आकार;		/* Size to write, size to read */
  off_t i_आकार = 0;	/* For small files, initial size to overwrite inode */
  अचिह्नित दीर्घ पूर्णाङ्क n;	/* Number of passes for printing purposes */
  पूर्णाङ्क *passarray;
  bool ok = true;
  संरचना randread_source *rs;

  n = 0;		/* dopass takes n == 0 to mean "don't print progress" */
  यदि (flags->verbose)
    n = flags->n_iterations + flags->zero_fill;

  यदि (fstat (fd, &st))
    {
      error (0, errno, _("%s: fstat failed"), qname);
      प्रत्यागम false;
    }

  /* If we know that we can't possibly shred the file, give up now.
     Otherwise, we may go into an infinite loop writing data before we
     find that we can't rewind the device.  */
  यदि ((S_ISCHR (st.st_बहुलकम्) && isatty (fd))
      || S_ISFIFO (st.st_बहुलकम्)
      || S_ISSOCK (st.st_बहुलकम्))
    {
      error (0, 0, _("%s: invalid file type"), qname);
      प्रत्यागम false;
    }
  अन्यथा यदि (S_ISREG (st.st_बहुलकम्) && st.st_आकार < 0)
    {
      error (0, 0, _("%s: file has negative size"), qname);
      प्रत्यागम false;
    }

  /* Allocate pass array */
  passarray = xnmalloc (flags->n_iterations, परिमाण *passarray);

  आकार = flags->आकार;
  यदि (आकार == -1)
    {
      यदि (S_ISREG (st.st_बहुलकम्))
        {
          आकार = st.st_आकार;

          यदि (! flags->exact)
            {
              /* Round up to the nearest block size to clear slack space.  */
              off_t remainder = आकार % STP_BLKSIZE (&st);
              यदि (आकार && आकार < STP_BLKSIZE (&st))
                i_आकार = आकार;
              यदि (remainder != 0)
                {
                  off_t आकार_incr = STP_BLKSIZE (&st) - remainder;
                  आकार += MIN (आकार_incr, OFF_T_MAX - आकार);
                }
            }
        }
      अन्यथा
        {
          /* The behavior of lseek is unspecified, but in practice if
             it returns a positive number that's the size of this
             device.  */
          आकार = lseek (fd, 0, SEEK_END);
          यदि (आकार <= 0)
            {
              /* We are unable to determine the length, up front.
                 Let dopass do that as part of its first iteration.  */
              आकार = -1;
            }
        }
    }
  अन्यथा यदि (S_ISREG (st.st_बहुलकम्)
           && st.st_आकार < MIN (STP_BLKSIZE (&st), आकार))
    i_आकार = st.st_आकार;

  /* Schedule the passes in random order. */
  genpattern (passarray, flags->n_iterations, s);

  rs = randint_get_source (s);

  यावत् (true)
    {
      off_t pass_आकार;
      अचिह्नित दीर्घ पूर्णाङ्क pn = n;

      यदि (i_आकार)
        {
          pass_आकार = i_आकार;
          i_आकार = 0;
          pn = 0;
        }
      अन्यथा यदि (आकार)
        {
          pass_आकार = आकार;
          आकार = 0;
        }
      /* TODO: consider handling tail packing by
         writing the tail padding as a separate pass,
         (that would not rewind).  */
      अन्यथा
        भङ्ग;

      प्रति (size_t i = 0; i < flags->n_iterations + flags->zero_fill; i++)
        {
          पूर्णाङ्क err = 0;
          पूर्णाङ्क type = i < flags->n_iterations ? passarray[i] : 0;

          err = dopass (fd, &st, qname, &pass_आकार, type, rs, i + 1, pn);

          यदि (err)
            {
              ok = false;
              यदि (err < 0)
                गच्छ wipefd_बहिर्गमनम्;
            }
        }
    }

  /* Now deallocate the data.  The effect of ftruncate is specified
     on regular files and shared memory objects (also directories, but
     they are not possible here); don't worry about errors reported
     for other file types.  */

  यदि (flags->remove_file && ftruncate (fd, 0) != 0
      && (S_ISREG (st.st_बहुलकम्) || S_TYPEISSHM (&st)))
    {
      error (0, errno, _("%s: error truncating"), qname);
      ok = false;
      गच्छ wipefd_बहिर्गमनम्;
    }

wipefd_बहिर्गमनम्:
  मुक्त (passarray);
  प्रत्यागम ok;
}

/* A wrapper with a little more checking for fds on the command line */
स्थिर bool
wipefd (पूर्णाङ्क fd, वर्ण ध्रुव *qname, संरचना randint_source *s,
        संरचना Options ध्रुव *flags)
{
  पूर्णाङ्क fd_flags = fcntl (fd, F_GETFL);

  यदि (fd_flags < 0)
    {
      error (0, errno, _("%s: fcntl failed"), qname);
      प्रत्यागम false;
    }
  यदि (fd_flags & O_APPEND)
    {
      error (0, 0, _("%s: cannot shred append-only file descriptor"), qname);
      प्रत्यागम false;
    }
  प्रत्यागम do_wipefd (fd, qname, s, flags);
}

/* --- Name-wiping code --- */

/* Characters allowed in a file name - a safe universal set.  */
स्थिर वर्ण ध्रुव nameset[] =
"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_.";

/* Increment NAME (with LEN bytes).  NAME must be a big-endian base N
   number with the digits taken from nameset.  Return true if successful.
   Otherwise, (because NAME already has the greatest possible value)
   return false.  */

स्थिर bool
incname (वर्ण *name, size_t len)
{
  यावत् (len--)
    {
      वर्ण ध्रुव *p = strchr (nameset, name[len]);

      /* Given that NAME is composed of bytes from NAMESET,
         P will never be null here.  */

      /* If this character has a successor, use it.  */
      यदि (p[1])
        {
          name[len] = p[1];
          प्रत्यागम true;
        }

      /* Otherwise, set this digit to 0 and increment the prefix.  */
      name[len] = nameset[0];
    }

  प्रत्यागम false;
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
स्थिर bool
wipename (वर्ण *oldname, वर्ण ध्रुव *qoldname, संरचना Options ध्रुव *flags)
{
  वर्ण *newname = xstrdup (oldname);
  वर्ण *आधारः_2 = चरमः_component (newname);
  वर्ण *dir = dir_name (newname);
  वर्ण *qdir = xstrdup (quotef (dir));
  bool first = true;
  bool ok = true;
  पूर्णाङ्क dir_fd = -1;

  यदि (flags->remove_file == remove_wipesync)
    dir_fd = उद्घाटय (dir, O_RDONLY | O_DIRECTORY | O_NOCTTY | O_NONBLOCK);

  यदि (flags->verbose)
    error (0, 0, _("%s: removing"), qoldname);

  यदि (flags->remove_file != remove_unlink)
    प्रति (size_t len = आधारः_len (आधारः_2); len != 0; len--)
      {
        स्मृतिं_पूरय (आधारः_2, nameset[0], len);
        आधारः_2[len] = 0;
        bool rename_ok;
        यावत् (! (rename_ok = (renameatu (AT_FDCWD, oldname, AT_FDCWD, newname,
                                          RENAME_NOREPLACE)
                               == 0))
               && errno == EEXIST && incname (आधारः_2, len))
          अनुवर्त;
        यदि (rename_ok)
          {
            यदि (0 <= dir_fd && dosync (dir_fd, qdir) != 0)
              ok = false;
            यदि (flags->verbose)
              {
                /* People seem to understand this better than talking
                   about renaming OLDNAME.  NEWNAME doesn't need
                   quoting because we picked it.  OLDNAME needs to be
                   quoted only the first time.  */
                वर्ण ध्रुव *old = first ? qoldname : oldname;
                error (0, 0,
                       _("%s: renamed to %s"), old, newname);
                first = false;
              }
            स्मृतिं_प्रतिलिख (oldname + (आधारः_2 - newname), आधारः_2, len + 1);
          }
      }

  यदि (unlink (oldname) != 0)
    {
      error (0, errno, _("%s: failed to remove"), qoldname);
      ok = false;
    }
  अन्यथा यदि (flags->verbose)
    error (0, 0, _("%s: removed"), qoldname);
  यदि (0 <= dir_fd)
    {
      यदि (dosync (dir_fd, qdir) != 0)
        ok = false;
      यदि (close (dir_fd) != 0)
        {
          error (0, errno, _("%s: failed to close"), qdir);
          ok = false;
        }
    }
  मुक्त (newname);
  मुक्त (dir);
  मुक्त (qdir);
  प्रत्यागम ok;
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
स्थिर bool
wipefile (वर्ण *name, वर्ण ध्रुव *qname,
          संरचना randint_source *s, संरचना Options ध्रुव *flags)
{
  पूर्णाङ्क fd = उद्घाटय (name, O_WRONLY | O_NOCTTY | O_NONBLOCK | O_द्विमान);
  यदि (fd < 0
      && (errno == EACCES && flags->force)
      && chmod (name, S_IWUSR) == 0)
    fd = उद्घाटय (name, O_WRONLY | O_NOCTTY | O_NONBLOCK | O_द्विमान);
  यदि (fd < 0)
    {
      संरचना stat st;
      ध्रुव पूर्णाङ्क open_errno = errno;

      /* Try to give a more meaningful error message if we attempt to
         open a FIFO with no readers.  */
      यदि (open_errno == ENXIO && 0 <= stat (name, &st)
          && S_ISFIFO (st.st_बहुलकम्))
        error (0, 0, _("%s: invalid file type"), qname);
      अन्यथा
        error (0, open_errno, _("%s: failed to open for writing"), qname);
      प्रत्यागम false;
    }

  /* We used O_NONBLOCK to prevent blocking when opening a FIFO.
     Reset that here.  */
  पूर्णाङ्क fd_flags = fcntl (fd, F_GETFL);
  bool ok = 0 <= fd_flags && 0 <= fcntl (fd, F_SETFL, fd_flags & ~O_NONBLOCK);
  यदि (ok)
    ok = do_wipefd (fd, qname, s, flags);
  अन्यथा
    error (0, errno, _("couldn't reset non-blocking mode %s"), qname);

  यदि (close (fd) != 0)
    {
      error (0, errno, _("%s: failed to close"), qname);
      ok = false;
    }
  यदि (ok && flags->remove_file)
    ok = wipename (name, qname, flags);
  प्रत्यागम ok;
}


/* Buffers for random data.  */
स्थिर संरचना randint_source *randint_source;

/* Just on general principles, wipe buffers containing information
   that may be related to the possibly-pseudorandom values used during
   shredding.  */
स्थिर शून्य
clear_random_दत्त (शून्य)
{
  randint_all_free (randint_source);
}


पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  bool ok = true;
  संरचना Options flags = {0};
  वर्ण **file;
  पूर्णाङ्क n_files;
  पूर्णाङ्क c;
  वर्ण ध्रुव *random_source = रिक्त;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  flags.n_iterations = DEFAULT_PASSES;
  flags.आकार = -1;

  यावत् ((c = getopt_long (argc, argv, "fn:s:uvxz", long_opts, रिक्त)) != -1)
    {
      विकल्प (c)
        {
        पक्ष 'f':
          flags.force = true;
          भङ्ग;

        पक्ष 'n':
          flags.n_iterations = xdectoumax (optarg, 0,
                                           MIN (ULONG_MAX,
                                                SIZE_MAX / परिमाण (पूर्णाङ्क)), "",
                                           _("invalid number of passes"), 0);
          भङ्ग;

        पक्ष RANDOM_SOURCE_OPTION:
          यदि (random_source && !streq (random_source, optarg))
            error (EXIT_FAILURE, 0, _("multiple random sources specified"));
          random_source = optarg;
          भङ्ग;

        पक्ष 'u':
          यदि (optarg == रिक्त)
            flags.remove_file = remove_wipesync;
          अन्यथा
            flags.remove_file = XARGMATCH ("--remove", optarg,
                                           remove_तर्काः, remove_methods);
          भङ्ग;

        पक्ष 's':
          flags.आकार = xnumtoumax (optarg, 0, 0, OFF_T_MAX, "cbBkKMGTPEZYRQ0",
                                   _("invalid file size"), 0, 0);
          भङ्ग;

        पक्ष 'v':
          flags.verbose = true;
          भङ्ग;

        पक्ष 'x':
          flags.exact = true;
          भङ्ग;

        पक्ष 'z':
          flags.zero_fill = true;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  file = argv + optind;
  n_files = argc - optind;

  यदि (n_files == 0)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  randint_source = randint_all_new (random_source, SIZE_MAX);
  यदि (! randint_source)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_source ? random_source : "getrandom"));
  atexit (clear_random_दत्त);

  प्रति (पूर्णाङ्क i = 0; i < n_files; i++)
    {
      वर्ण *qname = xstrdup (quotef (file[i]));
      यदि (streq (file[i], "-"))
        {
          ok &= wipefd (STDOUT_FILENO, qname, randint_source, &flags);
        }
      अन्यथा
        {
          /* Plain filename - Note that this overwrites *argv! */
          ok &= wipefile (file[i], qname, randint_source, &flags);
        }
      मुक्त (qname);
    }

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
/*
 * vim:sw=2:sts=2:
 */
