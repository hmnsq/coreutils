/* od -- dump files in octal and other formats
   Copyright (C) 1992-2026 Free Software Foundation, Inc.

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

/* Written by Jim Meyering.  */

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <endian.h>
#ಅನ್ತರ್ಭಾವ <float.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "argmatch.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "ftoastr.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "stat-size.h"
#ಅನ್ತರ್ಭಾವ "xbinary-io.h"
#ಅನ್ತರ್ಭಾವ "xprintf.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"
#ಅನ್ತರ್ಭಾವ "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "od"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("Jim Meyering")

/* The default number of input bytes per output line.  */
#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ 16

#ಆದರೆ FLOAT16_SUPPORTED
  /* Available since clang 6 (2018), and gcc 7 (2017).  */
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ _Float16 float16;
#ಇಲ್ಲದಿದ್ದರೆ
# define FLOAT16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದಶಮಾಂಶ float16;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ BF16_SUPPORTED
  /* Available since clang 11 (2020), and gcc 13 (2023). */
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __bf16 bfloat16;
#ಇಲ್ಲದಿದ್ದರೆ
# define BF16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದಶಮಾಂಶ bfloat16;
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ ಗಾತ್ರ_spec
  {
    NO_ಗಾತ್ರ = 0,
    CHAR,
    SHORT = CHAR + (UCHAR_MAX < USHRT_MAX),
    INT = SHORT + (USHRT_MAX < UINT_MAX),
    LONG = INT + (UINT_MAX < ULONG_MAX),
    LONG_LONG = LONG + (ULONG_MAX < ULLONG_MAX),
    INTMAX = LONG_LONG + (ULLONG_MAX < UINTMAX_MAX),
    FLOAT_HALF, /* Used only if (FLOAT16_SUPPORTED || BF16_SUPPORTED).  */
    FLOAT_SINGLE = FLOAT_HALF + (FLOAT16_SUPPORTED || BF16_SUPPORTED),
    FLOAT_DOUBLE = FLOAT_SINGLE + (FLT_MANT_DIG < DBL_MANT_DIG
                                   || FLT_MAX_EXP < DBL_MAX_EXP),
    FLOAT_LONG_DOUBLE = FLOAT_DOUBLE + (DBL_MANT_DIG < LDBL_MANT_DIG
                                        || DBL_MAX_EXP < LDBL_MAX_EXP),
    N_ಗಾತ್ರ_SPECS
  };

ಎಣಿಕೆ output_format
  {
    SIGNED_DECIMAL,
    UNSIGNED_DECIMAL,
    OCTAL,
    HEXADECIMAL,
    FLOATING_ಬಿಂದು,
    HFLOATING_ಬಿಂದು,
    BFLOATING_ಬಿಂದು,
    NAMED_CHARACTER,
    CHARACTER
  };

ಎಣಿಕೆ { MAX_INTEGRAL_TYPE_ಅಗಲ = UINTMAX_ಅಗಲ };

/* The maximum number of bytes needed for a format string, including
   the trailing nul.  Each format string expects a variable amount of
   padding (guaranteed to be at least 1 plus the field width), then an
   element that will be formatted in the field.  */
ಎಣಿಕೆ
  {
    FMT_ಬೈಟುಗಳು_ALLOCATED =
      ಗಾತ್ರ "%*.%dlld" - ಗಾತ್ರ "%d" + INT_STRLEN_BOUND (ಪೂರ್ಣಾಂಕ) + 1
  };

/* Ensure that our choice for FMT_BYTES_ALLOCATED is reasonable.  */
static_assert (MAX_INTEGRAL_TYPE_ಅಗಲ <= 3 * 99);

/* The type of a print function.  FIELDS is the number of fields per
   line, BLANK is the number of fields to leave blank.  WIDTH is width
   of one field, excluding leading space, and PAD is total pad to
   divide among FIELDS.  PAD is at least as large as FIELDS.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*print_function_type)
  (idx_t fields, idx_t blank, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *ದತ್ತಾಂಶ,
   ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fmt, ಪೂರ್ಣಾಂಕ ಅಗಲ, idx_t pad);

/* Each output format specification (from '-t spec' or from
   old-style options) is represented by one of these structures.  */
ರಚನೆ tspec
  {
    ಎಣಿಕೆ output_format fmt;
    ಎಣಿಕೆ ಗಾತ್ರ_spec ಗಾತ್ರ_2; /* Type of input object.  */
    print_function_type print_function;
    ಅಕ್ಷರ fmt_string[FMT_ಬೈಟುಗಳು_ALLOCATED]; /* Of the style "%*d".  */
    bool hexl_ಬಹುಲಕ_trailer;
    ಪೂರ್ಣಾಂಕ ಕ್ಷೇತ್ರ_ಅಗಲ; /* Minimum width of a field, excluding leading space.  */
    idx_t pad_ಅಗಲ; /* Total padding to be divided among fields.  */
  };

/* Convert enum size_spec to the size of the named type.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ_ಬೈಟುಗಳು[] =
{
  -1,
  ಗಾತ್ರ (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ),
#ಆದರೆ UCHAR_MAX < USHRT_MAX
  ಗಾತ್ರ (ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ ಪೂರ್ಣಾಂಕ),
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ USHRT_MAX < UINT_MAX
  ಗಾತ್ರ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ),
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ UINT_MAX < ULONG_MAX
  ಗಾತ್ರ (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ),
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ ULONG_MAX < ULLONG_MAX
  ಗಾತ್ರ (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ),
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ ULLONG_MAX < UINTMAX_MAX
  ಗಾತ್ರ (uintmax_t),
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ BF16_SUPPORTED
  ಗಾತ್ರ (bfloat16),
#ಅಥವಾಆದರೆ FLOAT16_SUPPORTED
  ಗಾತ್ರ (float16),
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
  ಗಾತ್ರ (ದಶಮಾಂಶ),
#ಆದರೆ_ಅಂತ್ಯ
  ಗಾತ್ರ (ದ್ವಿಗುಣ),
#ಆದರೆ DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
  ಗಾತ್ರ (ದೀರ್ಘ ದ್ವಿಗುಣ),
#ಆದರೆ_ಅಂತ್ಯ
};

/* Ensure that for each member of 'enum size_spec' there is an
   initializer in the width_bytes array.  */
static_assert (countof (ಅಗಲ_ಬೈಟುಗಳು) == N_ಗಾತ್ರ_SPECS);

/* Names for some non-printing characters.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ charname[33][4] =
{
  "nul", "soh", "stx", "etx", "eot", "enq", "ack", "bel",
  "bs", "ht", "nl", "vt", "ff", "cr", "so", "si",
  "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb",
  "can", "em", "sub", "esc", "fs", "gs", "rs", "us",
  "sp"
};

/* Address base (8, 10 or 16).  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ವಿಳಾಸ_ತಳ = 8;

/* The number of octal digits required to represent the largest
   address value.  */
ಎಣಿಕೆ { MAX_ವಿಳಾಸ_ಉದ್ದಳತೆ = ((INTMAX_ಅಗಲ - 1) / 3
                             + ((INTMAX_ಅಗಲ - 1) % 3 != 0)) };

/* Width of a normal address.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ವಿಳಾಸ_pad_len = 7;

/* Minimum length when detecting --strings.  */
ಸ್ಥಿರ idx_t string_min;

/* True when in --strings mode.  */
ಸ್ಥಿರ bool flag_dump_ಅಕ್ಷರಸರಣಿಗಳು;

/* True if we should recognize the older non-option arguments
   that specified at most one file and optional arguments specifying
   offset and pseudo-start address.  */
ಸ್ಥಿರ bool traditional;

/* True if an old-style 'pseudo-address' was specified.  */
ಸ್ಥಿರ bool flag_pseudo_ಆರಂಭ;

/* The difference between the old-style pseudo starting address and
   the number of bytes to skip.  */
ಸ್ಥಿರ intmax_t pseudo_ಸ್ಥಾನಾಂತರ;

/* Function that accepts an address and an optional following char,
   and prints the address and char to stdout.  */
ಸ್ಥಿರ ಶೂನ್ಯ format_ವಿಳಾಸ_std (intmax_t, ಅಕ್ಷರ);
ಸ್ಥಿರ ಶೂನ್ಯ (*format_ವಿಳಾಸ) (intmax_t, ಅಕ್ಷರ) = format_ವಿಳಾಸ_std;

/* The number of input bytes to skip before formatting and writing.  */
ಸ್ಥಿರ intmax_t n_ಬೈಟುಗಳು_ಗಮ್ಯ_skip;

/* The offset of the first byte after the last byte to be formatted.
   If negative, there is no limit.  */
ಸ್ಥಿರ intmax_t ಕೊನೆ_ಸ್ಥಾನಾಂತರ = -1;

/* When true and two or more consecutive blocks are equal, format
   only the first block and output an asterisk alone on the following
   line to indicate that identical blocks have been elided.  */
ಸ್ಥಿರ bool abbreviate_duplicate_ಖಂಡಗಳು = true;

/* An array of specs describing how to format each input block.  */
ಸ್ಥಿರ ರಚನೆ tspec *spec;

/* The number of format specs.  */
ಸ್ಥಿರ idx_t n_specs;

/* The allocated length of SPEC.  */
ಸ್ಥಿರ idx_t n_specs_allocated;

/* The number of input bytes formatted per output line.  It must be
   a multiple of the least common multiple of the sizes associated with
   the specified output types.  It should be as large as possible, but
   no larger than 16 -- unless specified with the -w option.  */
ಸ್ಥಿರ idx_t ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ;

/* Human-readable representation of *file_list (for error messages).
   It differs from file_list[-1] only when file_list[-1] is "-".  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಒಳಹರಿವು_filename;

/* A null-terminated list of the file-arguments from the command line.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ *file_list;

/* Initializer for file_list if no file-arguments
   were specified on the command line.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ default_file_list[] = {"-", ಖಾಲಿ};

/* The input stream associated with the current file.  */
ಸ್ಥಿರ FILE *ಒಳ_ಹರಿವು;

/* If true, at least one of the files we read was standard input.  */
ಸ್ಥಿರ bool have_read_stdin;

/* Map the size in bytes to a type identifier.
   When two types have the same machine layout:
     - Prefer unsigned int to other types, as its format is shorter.
     - Prefer unsigned long to higher-ranked types, as it is older.
     - Prefer uintmax_t to unsigned long long int; this wins if %lld
       does not work but %jd does (e.g., MS-Windows).  */
ಸ್ಥಿರ ಎಣಿಕೆ ಗಾತ್ರ_spec ಸ್ಥಿರಾಂಕ integral_type_ಗಾತ್ರ[] =
  {
#ಆದರೆ UCHAR_MAX < USHRT_MAX
    [ಗಾತ್ರ (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)] = CHAR,
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ USHRT_MAX < UINT_MAX
    [ಗಾತ್ರ (ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ ಪೂರ್ಣಾಂಕ)] = SHORT,
#ಆದರೆ_ಅಂತ್ಯ
    [ಗಾತ್ರ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)] = INT,
#ಆದರೆ UINT_MAX < ULONG_MAX
    [ಗಾತ್ರ (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)] = LONG,
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ ULONG_MAX < ULLONG_MAX && ULLONG_MAX < UINTMAX_MAX
    [ಗಾತ್ರ (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)] = LONG_LONG,
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ ULONG_MAX < UINTMAX_MAX
    [ಗಾತ್ರ (uintmax_t)] = INTMAX,
#ಆದರೆ_ಅಂತ್ಯ
  };

/* Map the size in bytes to a floating type identifier.
   When two types have the same machine layout:
     - Prefer double to the other types, as its format is shorter.  */
ಸ್ಥಿರ ಎಣಿಕೆ ಗಾತ್ರ_spec ಸ್ಥಿರಾಂಕ fp_type_ಗಾತ್ರ[] =
  {
#ಆದರೆ FLOAT16_SUPPORTED
    [ಗಾತ್ರ (float16)] = FLOAT_HALF,
#ಅಥವಾಆದರೆ BF16_SUPPORTED
    [ಗಾತ್ರ (bfloat16)] = FLOAT_HALF,
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
    [ಗಾತ್ರ (ದಶಮಾಂಶ)] = FLOAT_SINGLE,
#ಆದರೆ_ಅಂತ್ಯ
    [ಗಾತ್ರ (ದ್ವಿಗುಣ)] = FLOAT_DOUBLE,
#ಆದರೆ DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
    [ಗಾತ್ರ (ದೀರ್ಘ ದ್ವಿಗುಣ)] = FLOAT_LONG_DOUBLE,
#ಆದರೆ_ಅಂತ್ಯ
  };

/* Use native endianness by default.  */
ಸ್ಥಿರ bool ಒಳಹರಿವು_ಅದಲುಬದಲು;

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ short_options[] = "A:aBbcDdeFfHhIij:LlN:OoS:st:vw::Xx";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  TRADITIONAL_OPTION = CHAR_MAX + 1,
  ENDIAN_OPTION,
};

ಎಣಿಕೆ endian_type
{
  endian_little,
  endian_big
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ endian_ವಾದಗಳು[] =
{
  "little", "big", ಖಾಲಿ
};

ಸ್ಥಿರ ಎಣಿಕೆ endian_type ಸ್ಥಿರಾಂಕ endian_types[] =
{
  endian_little, endian_big
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"skip-bytes", required_ವಾದ, ಖಾಲಿ, 'j'},
  {"address-radix", required_ವಾದ, ಖಾಲಿ, 'A'},
  {"read-bytes", required_ವಾದ, ಖಾಲಿ, 'N'},
  {"format", required_ವಾದ, ಖಾಲಿ, 't'},
  {"output-duplicates", no_ವಾದ, ಖಾಲಿ, 'v'},
  {"strings", optional_ವಾದ, ಖಾಲಿ, 'S'},
  {"traditional", no_ವಾದ, ಖಾಲಿ, TRADITIONAL_OPTION},
  {"width", optional_ವಾದ, ಖಾಲಿ, 'w'},
  {"endian", required_ವಾದ, ಖಾಲಿ, ENDIAN_OPTION },

  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [-abcdfilosx]... [FILE] [[+]ಸ್ಥಾನಾಂತರ_2[.][b]]\n\
  or:  %s --traditional [OPTION]... [FILE] [[+]ಸ್ಥಾನಾಂತರ_2[.][b] [+][LABEL][.][b]]\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\n\
Write an unambiguous representation, octal ಬೈಟುಗಳು by ಪೂರ್ವನಿಯೋಜಿತ,\n\
ದ FILE ಗಮ್ಯ standard output.  With ಇನ್ನಷ್ಟು than one FILE ವಾದ,\n\
concatenate them ಒಳ the listed ಕ್ರಮ ಗಮ್ಯ form the ಒಳಹರಿವು.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
If ಮೊದಲ and ಎರಡನೇ call formats both apply, the ಎರಡನೇ format is assumed\n\
ಆದರೆ the ಕೊನೆಯ ಕಾರ್ಯಾಂಶ_2 begins with + or (ಆದರೆ there are 2 operands) a digit.\n\
An ಸ್ಥಾನಾಂತರ_2 ಕಾರ್ಯಾಂಶ_2 means -j ಸ್ಥಾನಾಂತರ_2.  LABEL is the pseudo-ವಿಳಾಸ\n\
at ಮೊದಲ byte printed, incremented when dump is progressing.\n\
For ಸ್ಥಾನಾಂತರ_2 and LABEL, a 0x or 0X ಪೂರ್ವಪ್ರತ್ಯಯ indicates hexadecimal;\n\
suffixes may be . ಗಾಗಿ octal and b ಗಾಗಿ multiply by 512.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --ವಿಳಾಸ-radix=RADIX\n\
         output format ಗಾಗಿ file offsets;\n\
         RADIX is one ದ [doxn], ಗಾಗಿ Decimal, Octal, Hex or None\n\
"));
      oputs (_("\
      --endian={big|little}\n\
         ಅದಲುಬದಲು ಒಳಹರಿವು ಬೈಟುಗಳು according the specified ಕ್ರಮ\n\
"));
      oputs (_("\
  -j, --skip-ಬೈಟುಗಳು=ಬೈಟುಗಳು_2\n\
         skip ಬೈಟುಗಳು_2 ಒಳಹರಿವು ಬೈಟುಗಳು ಮೊದಲ\n\
"));
      oputs (_("\
  -N, --ಪಠ-ಬೈಟುಗಳು=ಬೈಟುಗಳು_2\n\
         ಎಲ್ಲೆ dump ಗಮ್ಯ ಬೈಟುಗಳು_2 ಒಳಹರಿವು ಬೈಟುಗಳು\n\
"));
      oputs (_("\
  -S ಬೈಟುಗಳು_2, --ಅಕ್ಷರಸರಣಿಗಳು[=ಬೈಟುಗಳು_2]\n\
         show only NUL terminated ಅಕ್ಷರಸರಣಿಗಳು\n\
         ದ at ಕನಿಷ್ಠ ಬೈಟುಗಳು_2 (ಪೂರ್ವನಿಯೋಜಿತ 3) printable characters\n\
"));
      oputs (_("\
  -t, --format=TYPE\n\
         select output format or formats\n\
"));
      oputs (_("\
  -v, --output-duplicates\n\
         ಮಾಡು not use * ಗಮ್ಯ mark ಸಾಲು suppression\n\
"));
      oputs (_("\
  -w[ಬೈಟುಗಳು_2], --ಅಗಲ[=ಬೈಟುಗಳು_2]\n\
         output ಬೈಟುಗಳು_2 ಬೈಟುಗಳು ಪ್ರತಿ output ಸಾಲು;\n\
         32 is implied when ಬೈಟುಗಳು_2 is not specified\n\
"));
      oputs (_("\
      --traditional\n\
         accept arguments ಒಳ ಮೂರನೇ form above\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
\n\
Traditional format specifications may be intermixed; they accumulate:\n\
"), stdout);
      oputs (_("\
  -a   same as -t a,  select named characters, ignoring ಮೇಲಿನ-ಕ್ರಮ ಬಿಟ್\n\
"));
      oputs (_("\
  -b   same as -t o1, select octal ಬೈಟುಗಳು\n\
"));
      oputs (_("\
  -c   same as -t c,  select printable characters or backslash escapes\n\
"));
      oputs (_("\
  -d   same as -t u2, select ಚಿಹ್ನೆರಹಿತ decimal 2-byte units\n\
"));
      oputs (_("\
  -f   same as -t fF, select floats\n\
"));
      oputs (_("\
  -i   same as -t dI, select decimal ints\n\
"));
      oputs (_("\
  -l   same as -t dL, select decimal longs\n\
"));
      oputs (_("\
  -o   same as -t o2, select octal 2-byte units\n\
"));
      oputs (_("\
  -s   same as -t d2, select decimal 2-byte units\n\
"));
      oputs (_("\
  -x   same as -t x2, select hexadecimal 2-byte units\n\
"));
      fputs (_("\
\n\
\n\
TYPE is made ಮೇಲೆ ದ one or ಇನ್ನಷ್ಟು ದ these specifications:\n\
  a          named character, ignoring ಮೇಲಿನ-ಕ್ರಮ ಬಿಟ್\n\
  c          printable character or backslash escape\n\
"), stdout);
      fputs (_("\
  d[ಗಾತ್ರ_3]    ಚಿಹ್ನೆಯುಳ್ಳ decimal, ಗಾತ್ರ_3 ಬೈಟುಗಳು ಪ್ರತಿ integer\n\
  f[ಗಾತ್ರ_3]    floating ಬಿಂದು, ಗಾತ್ರ_3 ಬೈಟುಗಳು ಪ್ರತಿ ದಶಮಾಂಶ\n\
  o[ಗಾತ್ರ_3]    octal, ಗಾತ್ರ_3 ಬೈಟುಗಳು ಪ್ರತಿ integer\n\
  u[ಗಾತ್ರ_3]    ಚಿಹ್ನೆರಹಿತ decimal, ಗಾತ್ರ_3 ಬೈಟುಗಳು ಪ್ರತಿ integer\n\
  x[ಗಾತ್ರ_3]    hexadecimal, ಗಾತ್ರ_3 ಬೈಟುಗಳು ಪ್ರತಿ integer\n\
"), stdout);
      fputs (_("\
\n\
ಗಾತ್ರ_3 is a ಸಂಖ್ಯೆ.  For TYPE ಒಳ [doux], ಗಾತ್ರ_3 may also be C ಗಾಗಿ\n\
ಗಾತ್ರ(ಅಕ್ಷರ), S ಗಾಗಿ ಗಾತ್ರ(ಗಿಡ್ಡ), I ಗಾಗಿ ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ) or L ಗಾಗಿ\n\
ಗಾತ್ರ(ದೀರ್ಘ).  If TYPE is f, ಗಾತ್ರ_3 may also be B ಗಾಗಿ Brain 16 ಬಿಟ್,\n\
H ಗಾಗಿ Half precision ದಶಮಾಂಶ, F ಗಾಗಿ ಗಾತ್ರ(ದಶಮಾಂಶ), D ಗಾಗಿ ಗಾತ್ರ(ದ್ವಿಗುಣ),\n\
or L ಗಾಗಿ ಗಾತ್ರ(ದೀರ್ಘ ದ್ವಿಗುಣ).\n\
"), stdout);
      fputs (_("\
\n\
Adding a z suffix ಗಮ್ಯ any type displays printable characters at the ಕೊನೆ ದ\n\
each output ಸಾಲು.\n\
"), stdout);
      fputs (_("\
\n\
\n\
ಬೈಟುಗಳು_2 is hex with 0x or 0X ಪೂರ್ವಪ್ರತ್ಯಯ, and may have a multiplier suffix:\n\
  b    512\n\
  KB   1000\n\
  K    1024\n\
  MB   1000*1000\n\
  M    1024*1024\n\
and so on ಗಾಗಿ G, T, P, E, Z, Y, R, Q.\n\
ದ್ವಿಮಾನ_2 prefixes can be ಬಳಸಿದ, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Define the print functions.  */

/* Print N spaces, where 0 <= N.
   Do not rely on printf ("%*s", N, "") as N may exceed INT_MAX.  */
ಸ್ಥಿರ ಶೂನ್ಯ
print_n_spaces (intmax_t n)
{
  ಗಾಗಿ (; 0 < n; n--)
    ಅಕ್ಷರ_ಬರೆ (' ');
}

/* If there are FIELDS fields, return the total padding up to the
   start of field I, where I < FIELDS.  PAD is the total padding for
   all fields.  The result equals (PAD * I) / FIELDS, except it does
   not suffer from internal overflow.  */
ಸ್ಥಿರ idx_t
pad_at (idx_t fields, idx_t i, idx_t pad)
{
  /* This implementation assumes that (FIELDS - 1)^2 does not overflow
     intmax_t, an assumption checked by pad_at_overflow.  */
  intmax_t m = pad % fields;
  ಹಿಂತಿರುಗಿ pad / fields * i + m * i / fields;
}
ಸ್ಥಿರ bool
pad_at_ಅತಿಪ್ರವಾಹ (idx_t fields)
{
  intmax_t ಗುಣಲಬ್ಧ;
  ಹಿಂತಿರುಗಿ ckd_mul (&ಗುಣಲಬ್ಧ, fields - 1, fields - 1);
}

#ವ್ಯಾಖ್ಯಾನಿಸು PRINT_FIELDS(N, T, FMT_STRING_DECL, ACTION)                     \
ಸ್ಥಿರ ಶೂನ್ಯ                                                             \
N (idx_t fields, idx_t blank, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *ಖಂಡ,			\
   FMT_STRING_DECL, ಪೂರ್ಣಾಂಕ ಅಗಲ, idx_t pad)				\
{                                                                       \
  T ಸ್ಥಿರಾಂಕ *p = ಖಂಡ;                                                   \
  idx_t pad_ಉಳಿದ = pad;						\
  ಗಾಗಿ (idx_t i = fields; blank < i; i--)				\
    {                                                                   \
      idx_t ಮುಂದಿನ_pad = pad_at (fields, i - 1, pad);			\
      ಪೂರ್ಣಾಂಕ adjusted_ಅಗಲ = pad_ಉಳಿದ - ಮುಂದಿನ_pad + ಅಗಲ;            \
      T x;                                                              \
      ಆದರೆ (ಒಳಹರಿವು_ಅದಲುಬದಲು && ಗಾತ್ರ (T) > 1)                                 \
        {                                                               \
          ಒಕ್ಕೂಟ {                                                       \
            T x;                                                        \
            ಅಕ್ಷರ b[ಗಾತ್ರ (T)];                                         \
          } u;                                                          \
          ಗಾಗಿ (idx_t j = 0; j < ಗಾತ್ರ (T); j++)			\
            u.b[j] = ((ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *) p)[ಗಾತ್ರ (T) - 1 - j];            \
          x = u.x;                                                      \
        }                                                               \
      ಇಲ್ಲದಿದ್ದರೆ                                                              \
        x = *p;                                                         \
      p++;                                                              \
      ACTION;                                                           \
      pad_ಉಳಿದ = ಮುಂದಿನ_pad;                                         \
    }                                                                   \
}

#ವ್ಯಾಖ್ಯಾನಿಸು PRINT_TYPE(N, T)                                                \
  PRINT_FIELDS (N, T, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fmt_string,                           \
                xprintf (fmt_string, adjusted_ಅಗಲ, x))

#ವ್ಯಾಖ್ಯಾನಿಸು PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
  PRINT_FIELDS (N, T, MAYBE_ಬಳಸದ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fmt_string,              \
                ಅಕ್ಷರ buf[BUFSIZE];                                      \
                FTOASTR (buf, ಗಾತ್ರ buf, 0, 0, x);                     \
                xprintf ("%*s", adjusted_ಅಗಲ, buf))

PRINT_TYPE (print_s_char, ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ)
PRINT_TYPE (print_char, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)
PRINT_TYPE (print_s_short, ಗಿಡ್ಡ ಪೂರ್ಣಾಂಕ)
PRINT_TYPE (print_short, ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ ಪೂರ್ಣಾಂಕ)
PRINT_TYPE (print_int, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)
PRINT_TYPE (print_long, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)
PRINT_TYPE (print_long_long, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)
PRINT_TYPE (print_intmax, uintmax_t)

PRINT_FLOATTYPE (print_bfloat, bfloat16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_halffloat, float16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_float, ದಶಮಾಂಶ, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_double, ದ್ವಿಗುಣ, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_long_double, ದೀರ್ಘ ದ್ವಿಗುಣ, ldtoastr, LDBL_BUFSIZE_BOUND)

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು PRINT_TYPE
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು PRINT_FLOATTYPE

ಸ್ಥಿರ ಶೂನ್ಯ
dump_hexl_ಬಹುಲಕ_trailer (idx_t n_ಬೈಟುಗಳು, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಖಂಡ)
{
  fputs ("  >", stdout);
  ಗಾಗಿ (idx_t i = n_ಬೈಟುಗಳು; i > 0; i--)
    {
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c = *ಖಂಡ++;
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c2 = (isprint (c) ? c : '.');
      ಅಕ್ಷರ_ಬರೆ (c2);
    }
  ಅಕ್ಷರ_ಬರೆ ('<');
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_named_ascii (idx_t fields, idx_t blank, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *ಖಂಡ,
                   MAYBE_ಬಳಸದ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಬಳಸದ_fmt_string,
                   ಪೂರ್ಣಾಂಕ ಅಗಲ, idx_t pad)
{
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = ಖಂಡ;
  idx_t pad_ಉಳಿದ = pad;
  ಗಾಗಿ (idx_t i = fields; blank < i; i--)
    {
      ಪೂರ್ಣಾಂಕ masked_c = *p++ & 0x7f;
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s;
      ಅಕ್ಷರ buf[2];

      ಆದರೆ (masked_c == 127)
        s = "del";
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (masked_c <= 040)
        s = charname[masked_c];
      ಇಲ್ಲದಿದ್ದರೆ
        {
          buf[0] = masked_c;
          buf[1] = 0;
          s = buf;
        }

      idx_t ಮುಂದಿನ_pad = pad_at (fields, i - 1, pad);
      ಪೂರ್ಣಾಂಕ adjusted_ಅಗಲ = pad_ಉಳಿದ - ಮುಂದಿನ_pad + ಅಗಲ;
      xprintf ("%*s", adjusted_ಅಗಲ, s);
      pad_ಉಳಿದ = ಮುಂದಿನ_pad;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_ascii (idx_t fields, idx_t blank, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *ಖಂಡ,
             MAYBE_ಬಳಸದ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಬಳಸದ_fmt_string, ಪೂರ್ಣಾಂಕ ಅಗಲ,
             idx_t pad)
{
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = ಖಂಡ;
  idx_t pad_ಉಳಿದ = pad;
  ಗಾಗಿ (idx_t i = fields; blank < i; i--)
    {
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c = *p++;
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s;
      ಅಕ್ಷರ buf[4];

      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ '\0':
          s = "\\0";
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '\a':
          s = "\\a";
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '\b':
          s = "\\b";
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '\f':
          s = "\\f";
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '\n':
          s = "\\n";
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '\r':
          s = "\\r";
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '\t':
          s = "\\t";
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '\v':
          s = "\\v";
          ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
          sprintf (buf, (isprint (c) ? "%c" : "%03o"), c);
          s = buf;
        }

      idx_t ಮುಂದಿನ_pad = pad_at (fields, i - 1, pad);
      ಪೂರ್ಣಾಂಕ adjusted_ಅಗಲ = pad_ಉಳಿದ - ಮುಂದಿನ_pad + ಅಗಲ;
      xprintf ("%*s", adjusted_ಅಗಲ, s);
      pad_ಉಳಿದ = ಮುಂದಿನ_pad;
    }
}

/* Convert a null-terminated (possibly zero-length) string S to an
   int value.  If S points to a non-digit set *P to S,
   *VAL to 0, and return true.  Otherwise, accumulate the integer value of
   the string of digits.  If the string of digits represents a value
   larger than INT_MAX, don't modify *VAL or *P and return false.
   Otherwise, advance *P to the first non-digit after S, set *VAL to
   the result of the conversion and return true.  */

ಸ್ಥಿರ bool
simple_strtoi (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ **p, ಪೂರ್ಣಾಂಕ *val)
{
  ಪೂರ್ಣಾಂಕ ಮೊತ್ತ;

  ಗಾಗಿ (ಮೊತ್ತ = 0; c_isdigit (*s); s++)
    ಆದರೆ (ckd_mul (&ಮೊತ್ತ, ಮೊತ್ತ, 10) || ckd_ಕೂಡಿಸು (&ಮೊತ್ತ, ಮೊತ್ತ, *s - '0'))
      ಹಿಂತಿರುಗಿ false;
  *p = s;
  *val = ಮೊತ್ತ;
  ಹಿಂತಿರುಗಿ true;
}

/* Return a printf format for SPEC, given that remaining arguments are
   formats for int, long, long long, and intmax_t, respectively.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
ispec_ಗಮ್ಯ_format (ಎಣಿಕೆ ಗಾತ್ರ_spec ಗಾತ್ರ_spec,
                 ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *int_format,
                 ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *long_format,
                 ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *long_long_format,
                 ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *intmax_format)
{
  ಹಿಂತಿರುಗಿ (LONG < INTMAX && ಗಾತ್ರ_spec == INTMAX ? intmax_format
          : LONG < LONG_LONG && ಗಾತ್ರ_spec == LONG_LONG ? long_long_format
          : INT < LONG && ಗಾತ್ರ_spec == LONG ? long_format
          : int_format);
}

/* If S points to a single valid modern od format string, put
   a description of that format in *TSPEC, make *NEXT point at the
   character following the just-decoded format (if *NEXT is non-null),
   and return true.  If S is not valid, don't modify *NEXT or *TSPEC,
   give a diagnostic, and return false.  For example, if S were
   "d4afL" *NEXT would be set to "afL" and *TSPEC would be
     {
       fmt = SIGNED_DECIMAL;
       size = INT or LONG; (whichever integral_type_size[4] resolves to)
       print_function = print_int; (assuming size == INT)
       field_width = 11;
       fmt_string = "%*d";
      }
   pad_width is determined later, but is at least as large as the
   number of fields printed per row.
   S_ORIG is solely for reporting errors.  It should be the full format
   string argument.
   */

ಸ್ಥಿರ bool ATTRIBUTE_NONNULL ()
decode_one_format (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s_orig, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ **ಮುಂದಿನ,
                   ರಚನೆ tspec *tspec)
{
  ಎಣಿಕೆ ಗಾತ್ರ_spec ಗಾತ್ರ_spec;
  ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2;
  ಎಣಿಕೆ output_format fmt;
  print_function_type print_function;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p;
  ಅಕ್ಷರ c;
  ಪೂರ್ಣಾಂಕ ಕ್ಷೇತ್ರ_ಅಗಲ;

  ಆಯ್ಕೆ (*s)
    {
    ಸ್ಥಿತಿ 'd':
    ಸ್ಥಿತಿ 'o':
    ಸ್ಥಿತಿ 'u':
    ಸ್ಥಿತಿ 'x':
      c = *s;
      ++s;
      ಆಯ್ಕೆ (*s)
        {
        ಸ್ಥಿತಿ 'C':
          ++s;
          ಗಾತ್ರ_2 = ಗಾತ್ರ (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'S':
          ++s;
          ಗಾತ್ರ_2 = ಗಾತ್ರ (ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ ಪೂರ್ಣಾಂಕ);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'I':
          ++s;
          ಗಾತ್ರ_2 = ಗಾತ್ರ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'L':
          ++s;
          ಗಾತ್ರ_2 = ಗಾತ್ರ (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ);
          ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
          ಆದರೆ (! simple_strtoi (s, &p, &ಗಾತ್ರ_2))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              ಹಿಂತಿರುಗಿ false;
            }
          ಆದರೆ (p == s)
            ಗಾತ್ರ_2 = ಗಾತ್ರ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ);
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಆದರೆ (countof (integral_type_ಗಾತ್ರ) <= ಗಾತ್ರ_2
                  || integral_type_ಗಾತ್ರ[ಗಾತ್ರ_2] == NO_ಗಾತ್ರ)
                {
                  error (0, 0, _("invalid type string %s;\nthis system"
                                 " doesn't provide a %d-byte integral type"),
                         quote (s_orig), ಗಾತ್ರ_2);
                  ಹಿಂತಿರುಗಿ false;
                }
              s = p;
            }
          ನಿಲ್ಲಿಸು;
        }

      ಗಾತ್ರ_spec = integral_type_ಗಾತ್ರ[ಗಾತ್ರ_2];

      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'd':
          fmt = SIGNED_DECIMAL;
          ಕ್ಷೇತ್ರ_ಅಗಲ = INT_ಬಿಟ್ಟುಗಳು_STRLEN_BOUND (CHAR_BIT * ಗಾತ್ರ_2 - 1) + 1;
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_ಗಮ್ಯ_format (ಗಾತ್ರ_spec, "d", "ld", "lld", "jd"));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'o':
          fmt = OCTAL;
          ಕ್ಷೇತ್ರ_ಅಗಲ = (CHAR_BIT * ಗಾತ್ರ_2 + 2) / 3;
          sprintf (tspec->fmt_string, "%%*.%d%s", ಕ್ಷೇತ್ರ_ಅಗಲ,
                   ispec_ಗಮ್ಯ_format (ಗಾತ್ರ_spec, "o", "lo", "llo", "jo"));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'u':
          fmt = UNSIGNED_DECIMAL;
          ಕ್ಷೇತ್ರ_ಅಗಲ = INT_ಬಿಟ್ಟುಗಳು_STRLEN_BOUND (CHAR_BIT * ಗಾತ್ರ_2);
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_ಗಮ್ಯ_format (ಗಾತ್ರ_spec, "u", "lu", "llu", "ju"));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'x':
          fmt = HEXADECIMAL;
          ಕ್ಷೇತ್ರ_ಅಗಲ = (CHAR_BIT * ಗಾತ್ರ_2 + 3) / 4;
          sprintf (tspec->fmt_string, "%%*.%d%s", ಕ್ಷೇತ್ರ_ಅಗಲ,
                   ispec_ಗಮ್ಯ_format (ಗಾತ್ರ_spec, "x", "lx", "llx", "jx"));
          ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
          affirm (false);
        }

      /* Prefer INT, prefer LONG to longer types,
         and prefer INTMAX to LONG_LONG.  */
      print_function
        = (ಗಾತ್ರ_spec == INT
           ? print_int
           : SHORT < INT && ಗಾತ್ರ_spec == SHORT
           ? (fmt == SIGNED_DECIMAL ? print_s_short : print_short)
           : CHAR < SHORT && ಗಾತ್ರ_spec == CHAR
           ? (fmt == SIGNED_DECIMAL ? print_s_char : print_char)
           : INT < LONG && ಗಾತ್ರ_spec == LONG
           ? print_long
           : LONG < INTMAX && ಗಾತ್ರ_spec == INTMAX
           ? print_intmax
           : LONG < LONG_LONG && LONG_LONG < INTMAX && ಗಾತ್ರ_spec == LONG_LONG
           ? print_long_long
           : (affirm (false), (print_function_type) ಖಾಲಿ));
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'f':
      fmt = FLOATING_ಬಿಂದು;
      ++s;
      ಆಯ್ಕೆ (*s)
        {
        ಸ್ಥಿತಿ 'B':
          ++s;
          fmt = BFLOATING_ಬಿಂದು;
          ಗಾತ್ರ_2 = ಗಾತ್ರ (bfloat16);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'H':
          ++s;
          fmt = HFLOATING_ಬಿಂದು;
          ಗಾತ್ರ_2 = ಗಾತ್ರ (float16);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'F':
          ++s;
          ಗಾತ್ರ_2 = ಗಾತ್ರ (ದಶಮಾಂಶ);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'D':
          ++s;
          ಗಾತ್ರ_2 = ಗಾತ್ರ (ದ್ವಿಗುಣ);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'L':
          ++s;
          ಗಾತ್ರ_2 = ಗಾತ್ರ (ದೀರ್ಘ ದ್ವಿಗುಣ);
          ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
          ಆದರೆ (! simple_strtoi (s, &p, &ಗಾತ್ರ_2))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              ಹಿಂತಿರುಗಿ false;
            }
          ಆದರೆ (p == s)
            ಗಾತ್ರ_2 = ಗಾತ್ರ (ದ್ವಿಗುಣ);
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಆದರೆ (countof (fp_type_ಗಾತ್ರ) <= ಗಾತ್ರ_2
                  || fp_type_ಗಾತ್ರ[ಗಾತ್ರ_2] == NO_ಗಾತ್ರ
                  || (! FLOAT16_SUPPORTED && BF16_SUPPORTED
                      && ಗಾತ್ರ_2 == ಗಾತ್ರ (bfloat16)))
                {
                  error (0, 0,
                         _("invalid type string %s;\n"
                           "this system doesn't provide a %d-byte"
                           " floating point type"),
                         quote (s_orig), ಗಾತ್ರ_2);
                  ಹಿಂತಿರುಗಿ false;
                }
              s = p;
            }
          ನಿಲ್ಲಿಸು;
        }
      ಗಾತ್ರ_spec = fp_type_ಗಾತ್ರ[ಗಾತ್ರ_2];

      ಆದರೆ ((! FLOAT16_SUPPORTED && fmt == HFLOATING_ಬಿಂದು)
          || (! BF16_SUPPORTED && fmt == BFLOATING_ಬಿಂದು))
      {
        error (0, 0,
               _("this system doesn't provide a %s floating point type"),
               quote (s_orig));
        ಹಿಂತಿರುಗಿ false;
      }

      {
        ರಚನೆ lconv ಸ್ಥಿರಾಂಕ *locale = localeconv ();
        idx_t decimal_ಬಿಂದು_len =
          (locale->decimal_ಬಿಂದು[0] ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (locale->decimal_ಬಿಂದು) : 1);

        ಆದರೆ (ಗಾತ್ರ_spec == FLOAT_DOUBLE)
          {
            print_function = print_double;
            ಕ್ಷೇತ್ರ_ಅಗಲ = DBL_STRLEN_BOUND_L (decimal_ಬಿಂದು_len);
          }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (FLOAT_SINGLE < FLOAT_DOUBLE && ಗಾತ್ರ_spec == FLOAT_SINGLE)
          {
            print_function = print_float;
            ಕ್ಷೇತ್ರ_ಅಗಲ = FLT_STRLEN_BOUND_L (decimal_ಬಿಂದು_len);
          }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (FLOAT_HALF < FLOAT_SINGLE && ಗಾತ್ರ_spec == FLOAT_HALF)
          {
            print_function = fmt == BFLOATING_ಬಿಂದು
                             ? print_bfloat : print_halffloat;
            ಕ್ಷೇತ್ರ_ಅಗಲ = FLT_STRLEN_BOUND_L (decimal_ಬಿಂದು_len);
          }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (FLOAT_DOUBLE < FLOAT_LONG_DOUBLE
                 && ಗಾತ್ರ_spec == FLOAT_LONG_DOUBLE)
          {
            print_function = print_long_double;
            ಕ್ಷೇತ್ರ_ಅಗಲ = LDBL_STRLEN_BOUND_L (decimal_ಬಿಂದು_len);
          }
        ಇಲ್ಲದಿದ್ದರೆ
          affirm (false);

        ನಿಲ್ಲಿಸು;
      }

    ಸ್ಥಿತಿ 'a':
      ++s;
      fmt = NAMED_CHARACTER;
      ಗಾತ್ರ_spec = CHAR;
      print_function = print_named_ascii;
      ಕ್ಷೇತ್ರ_ಅಗಲ = 3;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'c':
      ++s;
      fmt = CHARACTER;
      ಗಾತ್ರ_spec = CHAR;
      print_function = print_ascii;
      ಕ್ಷೇತ್ರ_ಅಗಲ = 3;
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      error (0, 0, _("invalid character '%c' in type string %s"),
             *s, quote (s_orig));
      ಹಿಂತಿರುಗಿ false;
    }

  tspec->ಗಾತ್ರ_2 = ಗಾತ್ರ_spec;
  tspec->fmt = fmt;
  tspec->print_function = print_function;

  tspec->ಕ್ಷೇತ್ರ_ಅಗಲ = ಕ್ಷೇತ್ರ_ಅಗಲ;
  tspec->hexl_ಬಹುಲಕ_trailer = (*s == 'z');
  ಆದರೆ (tspec->hexl_ಬಹುಲಕ_trailer)
    s++;

  *ಮುಂದಿನ = s;
  ಹಿಂತಿರುಗಿ true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM and the global string INPUT_FILENAME to the
   first one that can be successfully opened. Modify FILE_LIST to
   reference the next filename in the list.  A file name of "-" is
   interpreted as standard input.  If any file open fails, give an error
   message and return false.  */

ಸ್ಥಿರ bool
open_ಮುಂದಿನ_file (ಶೂನ್ಯ)
{
  bool ok = true;

  ಮಾಡು
    {
      ಒಳಹರಿವು_filename = *file_list;
      ಆದರೆ (ಒಳಹರಿವು_filename == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ ok;
      ++file_list;

      ಆದರೆ (streq (ಒಳಹರಿವು_filename, "-"))
        {
          ಒಳಹರಿವು_filename = _("standard input");
          ಒಳ_ಹರಿವು = stdin;
          have_read_stdin = true;
          xset_ದ್ವಿಮಾನ_ಬಹುಲಕ (STDIN_FILENO, O_ದ್ವಿಮಾನ);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಒಳ_ಹರಿವು = fopen (ಒಳಹರಿವು_filename, (O_ದ್ವಿಮಾನ ? "rb" : "r"));
          ಆದರೆ (ಒಳ_ಹರಿವು == ಖಾಲಿ)
            {
              error (0, errno, "%s", quotef (ಒಳಹರಿವು_filename));
              ok = false;
            }
        }
    }
  ಆಗ (ಒಳ_ಹರಿವು == ಖಾಲಿ);

  ಆದರೆ (0 <= ಕೊನೆ_ಸ್ಥಾನಾಂತರ && !flag_dump_ಅಕ್ಷರಸರಣಿಗಳು)
    setvbuf (ಒಳ_ಹರಿವು, ಖಾಲಿ, _IONBF, 0);

  ಹಿಂತಿರುಗಿ ok;
}

/* Test whether there have been errors on in_stream, and close it if
   it is not standard input.  Return false if there has been an error
   on in_stream or stdout; return true otherwise.  This function will
   report more than one error only if both a read and a write error
   have occurred.  IN_ERRNO, if nonzero, is the error number
   corresponding to the most recent action for IN_STREAM.  */

ಸ್ಥಿರ bool
check_and_close (ಪೂರ್ಣಾಂಕ ಒಳ_errno)
{
  bool ok = true;

  ಆದರೆ (ಒಳ_ಹರಿವು != ಖಾಲಿ)
    {
      ಆದರೆ (!ferror (ಒಳ_ಹರಿವು))
        ಒಳ_errno = 0;
      ಆದರೆ (streq (file_list[-1], "-"))
        clearerr (ಒಳ_ಹರಿವು);
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fclose (ಒಳ_ಹರಿವು) != 0 && !ಒಳ_errno)
        ಒಳ_errno = errno;
      ಆದರೆ (ಒಳ_errno)
        {
          error (0, ಒಳ_errno, "%s", quotef (ಒಳಹರಿವು_filename));
          ok = false;
        }

      ಒಳ_ಹರಿವು = ಖಾಲಿ;
    }

  ಆದರೆ (ferror (stdout))
    {
      error (0, 0, _("write error"));
      ok = false;
    }

  ಹಿಂತಿರುಗಿ ok;
}

/* Decode the modern od format string S.  Append the decoded
   representation to the global array SPEC, reallocating SPEC if
   necessary.  Return true if S is valid.  */

ಸ್ಥಿರ bool ATTRIBUTE_NONNULL ()
decode_format_string (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s_orig = s;

  ಆಗ (*s != '\0')
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮುಂದಿನ;

      ಆದರೆ (n_specs_allocated <= n_specs)
        spec = xpalloc (spec, &n_specs_allocated, 1, -1, ಗಾತ್ರ *spec);

      ಆದರೆ (! decode_one_format (s_orig, s, &ಮುಂದಿನ, &spec[n_specs]))
        ಹಿಂತಿರುಗಿ false;

      affirm (s != ಮುಂದಿನ);
      s = ಮುಂದಿನ;
      ++n_specs;
    }

  ಹಿಂತಿರುಗಿ true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM to position N_SKIP in the concatenation of
   those files.  If any file operation fails or if there are fewer than
   N_SKIP bytes in the combined input, give an error message and return
   false.  When possible, use seek rather than read operations to
   advance IN_STREAM.  */

ಸ್ಥಿರ bool
skip (intmax_t n_skip)
{
  bool ok = true;
  ಪೂರ್ಣಾಂಕ ಒಳ_errno = 0;

  ಆದರೆ (n_skip == 0)
    ಹಿಂತಿರುಗಿ true;

  ಆಗ (ಒಳ_ಹರಿವು != ಖಾಲಿ)	/* EOF.  */
    {
      ರಚನೆ stat file_stats;

      /* First try seeking.  For large offsets, this extra work is
         worthwhile.  If the offset is below some threshold it may be
         more efficient to move the pointer by reading.  There are two
         issues when trying to seek:
           - the file must be seekable.
           - before seeking to the specified position, make sure
             that the new position is in the current file.
             Try to do that by getting file's size using fstat.
             But that will work only for regular files.  */

      ಆದರೆ (fstat (fileno (ಒಳ_ಹರಿವು), &file_stats) == 0)
        {
          /* The st_size field is valid for regular files.
             If the number of bytes left to skip is larger than
             the size of the current file, we can decrement n_skip
             and go on to the next file.  Skip this optimization also
             when st_size is no greater than the block size, because
             some kernels report nonsense small file sizes for
             proc-like file systems.  */
          ಆದರೆ (S_ISREG (file_stats.st_ಬಹುಲಕ)
              && STP_BLKSIZE (&file_stats) < file_stats.st_ಗಾತ್ರ)
            {
              ಆದರೆ (file_stats.st_ಗಾತ್ರ < n_skip)
                n_skip -= file_stats.st_ಗಾತ್ರ;
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  ಆದರೆ (fseeko (ಒಳ_ಹರಿವು, n_skip, SEEK_CUR) != 0)
                    {
                      ಒಳ_errno = errno;
                      ok = false;
                    }
                  n_skip = 0;
                }
            }

          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (! (S_ISREG (file_stats.st_ಬಹುಲಕ)
                      || S_TYPEISSHM (&file_stats)
                      || S_TYPEISTMO (&file_stats))
                   && fseeko (ಒಳ_ಹರಿವು, n_skip, SEEK_CUR) == 0)
            n_skip = 0;

          /* If it's neither a seekable file with unusable size, nor a
             regular file so large it can't be in a proc-like file system,
             position the file pointer by reading.  */

          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಅಕ್ಷರ buf[BUFSIZ];
              idx_t n_ಬೈಟುಗಳು_read, n_ಬೈಟುಗಳು_ಗಮ್ಯ_read = BUFSIZ;

              ಆಗ (0 < n_skip)
                {
                  ಆದರೆ (n_skip < n_ಬೈಟುಗಳು_ಗಮ್ಯ_read)
                    n_ಬೈಟುಗಳು_ಗಮ್ಯ_read = n_skip;
                  n_ಬೈಟುಗಳು_read = fread (buf, 1, n_ಬೈಟುಗಳು_ಗಮ್ಯ_read, ಒಳ_ಹರಿವು);
                  n_skip -= n_ಬೈಟುಗಳು_read;
                  ಆದರೆ (n_ಬೈಟುಗಳು_read != n_ಬೈಟುಗಳು_ಗಮ್ಯ_read)
                    {
                      ಆದರೆ (ferror (ಒಳ_ಹರಿವು))
                        {
                          ಒಳ_errno = errno;
                          ok = false;
                          n_skip = 0;
                          ನಿಲ್ಲಿಸು;
                        }
                      ಆದರೆ (feof (ಒಳ_ಹರಿವು))
                        ನಿಲ್ಲಿಸು;
                    }
                }
            }

          ಆದರೆ (n_skip == 0)
            ನಿಲ್ಲಿಸು;
        }

      ಇಲ್ಲದಿದ್ದರೆ   /* cannot fstat() file */
        {
          error (0, errno, "%s", quotef (ಒಳಹರಿವು_filename));
          ok = false;
        }

      ok &= check_and_close (ಒಳ_errno);

      ok &= open_ಮುಂದಿನ_file ();
    }

  ಆದರೆ (n_skip != 0)
    error (EXIT_FAILURE, 0, _("cannot skip past end of combined input"));

  ಹಿಂತಿರುಗಿ ok;
}

ಸ್ಥಿರ ಶೂನ್ಯ
format_ವಿಳಾಸ_none (MAYBE_ಬಳಸದ intmax_t ವಿಳಾಸ,
                     MAYBE_ಬಳಸದ ಅಕ್ಷರ c)
{
}

ಸ್ಥಿರ ಶೂನ್ಯ
format_ವಿಳಾಸ_std (intmax_t ವಿಳಾಸ, ಅಕ್ಷರ c)
{
  ಅಕ್ಷರ buf[MAX_ವಿಳಾಸ_ಉದ್ದಳತೆ + 2];
  ಅಕ್ಷರ *p = buf + ಗಾತ್ರ buf;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *pbound;

  *--p = '\0';
  *--p = c;
  pbound = p - ವಿಳಾಸ_pad_len;

  /* Use a special case of the code for each base.  This is measurably
     faster than generic code.  */
  ಆಯ್ಕೆ (ವಿಳಾಸ_ತಳ)
    {
    ಸ್ಥಿತಿ 8:
      ಮಾಡು
        *--p = '0' + (ವಿಳಾಸ & 7);
      ಆಗ ((ವಿಳಾಸ >>= 3) != 0);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 10:
      ಮಾಡು
        *--p = '0' + (ವಿಳಾಸ % 10);
      ಆಗ ((ವಿಳಾಸ /= 10) != 0);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 16:
      ಮಾಡು
        *--p = "0123456789abcdef"[ವಿಳಾಸ & 15];
      ಆಗ ((ವಿಳಾಸ >>= 4) != 0);
      ನಿಲ್ಲಿಸು;
    }

  ಆಗ (pbound < p)
    *--p = '0';

  fputs (p, stdout);
}

ಸ್ಥಿರ ಶೂನ್ಯ
format_ವಿಳಾಸ_paren (intmax_t ವಿಳಾಸ, ಅಕ್ಷರ c)
{
  ಅಕ್ಷರ_ಬರೆ ('(');
  format_ವಿಳಾಸ_std (ವಿಳಾಸ, ')');
  ಆದರೆ (c)
    ಅಕ್ಷರ_ಬರೆ (c);
}

ಸ್ಥಿರ ಶೂನ್ಯ
format_ವಿಳಾಸ_label (intmax_t ವಿಳಾಸ, ಅಕ್ಷರ c)
{
  format_ವಿಳಾಸ_std (ವಿಳಾಸ, ' ');

  intmax_t addr;
  ಆದರೆ (ckd_ಕೂಡಿಸು (&addr, ವಿಳಾಸ, pseudo_ಸ್ಥಾನಾಂತರ))
    error (EXIT_FAILURE, 0, _("pseudo address too large for input"));

  format_ವಿಳಾಸ_paren (addr, c);
}

/* Write N_BYTES bytes from CURR_BLOCK to standard output once for each
   of the N_SPEC format specs.  CURRENT_OFFSET is the byte address of
   CURR_BLOCK in the concatenation of input files, and it is printed
   (optionally) only before the output line associated with the first
   format spec.  When duplicate blocks are being abbreviated, the output
   for a sequence of identical input blocks is the output for the first
   block followed by an asterisk alone on a line.  It is valid to compare
   the blocks PREV_BLOCK and CURR_BLOCK only when N_BYTES == BYTES_PER_BLOCK.
   That condition may be false only for the last input block.  */

ಸ್ಥಿರ ಶೂನ್ಯ
write_ಖಂಡ (intmax_t current_ಸ್ಥಾನಾಂತರ, idx_t n_ಬೈಟುಗಳು,
             ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *prev_ಖಂಡ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *curr_ಖಂಡ)
{
  ಸ್ಥಿರ bool ಮೊದಲ = true;
  ಸ್ಥಿರ bool prev_pair_equal = false;

  ಆದರೆ (abbreviate_duplicate_ಖಂಡಗಳು
      && !ಮೊದಲ && n_ಬೈಟುಗಳು == ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ
      && memeq (prev_ಖಂಡ, curr_ಖಂಡ, ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ))
    {
      ಆದರೆ (prev_pair_equal)
        {
          /* The two preceding blocks were equal, and the current
             block is the same as the last one, so print nothing.  */
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಬರೆ ("*\n");
          prev_pair_equal = true;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      prev_pair_equal = false;
      ಗಾಗಿ (idx_t i = 0; i < n_specs; i++)
        {
          ಪೂರ್ಣಾಂಕ datum_ಅಗಲ = ಅಗಲ_ಬೈಟುಗಳು[spec[i].ಗಾತ್ರ_2];
          idx_t fields_ಪ್ರತಿ_ಖಂಡ = ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ / datum_ಅಗಲ;
          idx_t blank_fields = (ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ - n_ಬೈಟುಗಳು) / datum_ಅಗಲ;
          ಆದರೆ (i == 0)
            format_ವಿಳಾಸ (current_ಸ್ಥಾನಾಂತರ, '\0');
          ಇಲ್ಲದಿದ್ದರೆ
            print_n_spaces (ವಿಳಾಸ_pad_len);
          (*spec[i].print_function) (fields_ಪ್ರತಿ_ಖಂಡ, blank_fields,
                                     curr_ಖಂಡ, spec[i].fmt_string,
                                     spec[i].ಕ್ಷೇತ್ರ_ಅಗಲ, spec[i].pad_ಅಗಲ);
          ಆದರೆ (spec[i].hexl_ಬಹುಲಕ_trailer)
            {
              /* Space-pad out to full line width, then dump the trailer.  */
              ಪೂರ್ಣಾಂಕ ಕ್ಷೇತ್ರ_ಅಗಲ = spec[i].ಕ್ಷೇತ್ರ_ಅಗಲ;
              ಗಾಗಿ (idx_t f = 0; f < blank_fields; f++)
                print_n_spaces (ಕ್ಷೇತ್ರ_ಅಗಲ);
              idx_t pad_ಅಗಲ = pad_at (fields_ಪ್ರತಿ_ಖಂಡ, blank_fields,
                                        spec[i].pad_ಅಗಲ);
              print_n_spaces (pad_ಅಗಲ);
              dump_hexl_ಬಹುಲಕ_trailer (n_ಬೈಟುಗಳು, curr_ಖಂಡ);
            }
          ಅಕ್ಷರ_ಬರೆ ('\n');
        }
    }
  ಮೊದಲ = false;
}

/* Read a single byte into *C from the concatenation of the input files
   named in the global array FILE_LIST.  On the first call to this
   function, the global variable IN_STREAM is expected to be an open
   stream associated with the input file INPUT_FILENAME.  If IN_STREAM
   is at end-of-file, close it and update the global variables IN_STREAM
   and INPUT_FILENAME so they correspond to the next file in the list.
   Then try to read a byte from the newly opened file.  Repeat if
   necessary until EOF is reached for the last file in FILE_LIST, then
   set *C to EOF and return.  Subsequent calls do likewise.  Return
   true if successful.  */

ಸ್ಥಿರ bool
read_char (ಪೂರ್ಣಾಂಕ *c)
{
  bool ok = true;

  *c = EOF;

  ಆಗ (ಒಳ_ಹರಿವು && (*c = getc (ಒಳ_ಹರಿವು)) < 0)
    {
      ok &= check_and_close (errno);
      ok &= open_ಮುಂದಿನ_file ();
    }

  ಹಿಂತಿರುಗಿ ok;
}

/* Read N bytes into BLOCK from the concatenation of the input files
   named in the global array FILE_LIST.  On the first call to this
   function, the global variable IN_STREAM is expected to be an open
   stream associated with the input file INPUT_FILENAME.  If all N
   bytes cannot be read from IN_STREAM, close IN_STREAM and update
   the global variables IN_STREAM and INPUT_FILENAME.  Then try to
   read the remaining bytes from the newly opened file.  Repeat if
   necessary until EOF is reached for the last file in FILE_LIST.
   On subsequent calls, don't modify BLOCK and return true.  Set
   *N_BYTES_IN_BUFFER to the number of bytes read.  If an error occurs,
   it will be detected through ferror when the stream is about to be
   closed.  If there is an error, give a message but continue reading
   as usual and return false.  Otherwise return true.  */

ಸ್ಥಿರ bool
read_ಖಂಡ (idx_t n, ಅಕ್ಷರ *ಖಂಡ, idx_t *n_ಬೈಟುಗಳು_ಒಳ_ಬಫರ್)
{
  bool ok = true;

  affirm (0 < n && n <= ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ);

  *n_ಬೈಟುಗಳು_ಒಳ_ಬಫರ್ = 0;

  ಆಗ (ಒಳ_ಹರಿವು != ಖಾಲಿ)	/* EOF.  */
    {
      idx_t n_needed = n - *n_ಬೈಟುಗಳು_ಒಳ_ಬಫರ್;
      idx_t n_read = fread (ಖಂಡ + *n_ಬೈಟುಗಳು_ಒಳ_ಬಫರ್,
                            1, n_needed, ಒಳ_ಹರಿವು);

      *n_ಬೈಟುಗಳು_ಒಳ_ಬಫರ್ += n_read;

      ಆದರೆ (n_read == n_needed)
        ನಿಲ್ಲಿಸು;

      ok &= check_and_close (errno);

      ok &= open_ಮುಂದಿನ_file ();
    }

  ಹಿಂತಿರುಗಿ ok;
}

/* Return the least common multiple of the sizes associated
   with the format specs.  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_lcm (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಂಕ l_c_m = 1;

  ಗಾಗಿ (idx_t i = 0; i < n_specs; i++)
    l_c_m = lcm (l_c_m, ಅಗಲ_ಬೈಟುಗಳು[spec[i].ಗಾತ್ರ_2]);
  ಹಿಂತಿರುಗಿ l_c_m;
}

/* Act like xstrtoimax (NPTR, NULL, BASE, VAL, VALID_SUFFIXES),
   except reject negative values, and *VAL may be set if
   LONGINT_INVALID is returned.  */
ಸ್ಥಿರ strtol_error
xstr2nonneg (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ nptr, ಪೂರ್ಣಾಂಕ ತಳ_2, intmax_t *val,
             ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ valid_suffixes)
{
  strtol_error s_err = xstrtoimax (nptr, ಖಾಲಿ, ತಳ_2, val, valid_suffixes);
  ಹಿಂತಿರುಗಿ s_err != LONGINT_INVALID && *val < 0 ? LONGINT_INVALID : s_err;
}

/* If STR is a valid traditional offset specification with an optional
   leading '+', and can be represented in intmax_t, return true and
   set *OFFSET to the offset it denotes.  If it is valid but cannot be
   represented, diagnose the problem and exit.  Otherwise return false
   and possibly set *OFFSET.  */

ಸ್ಥಿರ bool
parse_old_ಸ್ಥಾನಾಂತರ (ಅಕ್ಷರ *str, intmax_t *ಸ್ಥಾನಾಂತರ)
{
  /* Skip over any leading '+'.  */
  ಅಕ್ಷರ *s = str + (str[0] == '+');

  ಆದರೆ (! c_isdigit (s[0]))
    ಹಿಂತಿರುಗಿ false;

  /* Determine the radix for S.  If there is a '.', followed first by
     optional 'b' or (undocumented) 'B' and then by end of string,
     it's decimal, otherwise, if the string begins with '0X' or '0x',
     it's hexadecimal, else octal.  */
  ಅಕ್ಷರ *dot = strchr (s, '.');
  ಆದರೆ (dot && dot[(dot[1] == 'b' || dot[1] == 'B') + 1])
    dot = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ radix = dot ? 10 : s[0] == '0' && (s[1] == 'x' || s[1] == 'X') ? 16 : 8;

  ಆದರೆ (dot)
    {
      /* Temporarily remove the '.' from the decimal string.  */
      dot[0] = dot[1];
      dot[1] = '\0';
    }

  ಎಣಿಕೆ strtol_error s_err = xstr2nonneg (s, radix, ಸ್ಥಾನಾಂತರ, "bB");

  ಆದರೆ (dot)
    {
      /* Restore the decimal string's original value.  */
      dot[1] = dot[0];
      dot[0] = '.';
    }

  ಆದರೆ (s_err == LONGINT_ಅತಿಪ್ರವಾಹ)
    error (EXIT_FAILURE, ERANGE, "%s", quotef (str));
  ಹಿಂತಿರುಗಿ s_err == LONGINT_OK;
}

/* Read a chunk of size BYTES_PER_BLOCK from the input files, write the
   formatted block to standard output, and repeat until the specified
   maximum number of bytes has been read or until all input has been
   processed.  If the last block read is smaller than BYTES_PER_BLOCK
   and its size is not a multiple of the size associated with a format
   spec, extend the input block with zero bytes until its length is a
   multiple of all format spec sizes.  Write the final block.  Finally,
   write on a line by itself the offset of the byte after the last byte
   read.  Accumulate return values from calls to read_block and
   check_and_close, and if any was false, return false.
   Otherwise, return true.  */

ಸ್ಥಿರ bool
dump (ಶೂನ್ಯ)
{
  ಅಕ್ಷರ *ಖಂಡ[2];
  bool idx = false;
  bool ok = true;
  idx_t n_ಬೈಟುಗಳು_read;

  ಖಂಡ[0] = xinmalloc (1 + abbreviate_duplicate_ಖಂಡಗಳು, ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ);
  ಖಂಡ[1] = ಖಂಡ[0] + abbreviate_duplicate_ಖಂಡಗಳು * ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ;

  intmax_t current_ಸ್ಥಾನಾಂತರ = n_ಬೈಟುಗಳು_ಗಮ್ಯ_skip;

  ಮಾಡು
    {
      intmax_t needed_bound
        = ಕೊನೆ_ಸ್ಥಾನಾಂತರ < 0 ? INTMAX_MAX : ಕೊನೆ_ಸ್ಥಾನಾಂತರ - current_ಸ್ಥಾನಾಂತರ;
      ಆದರೆ (needed_bound <= 0)
        {
          n_ಬೈಟುಗಳು_read = 0;
          ನಿಲ್ಲಿಸು;
        }
      idx_t n_needed = MIN (ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ, needed_bound);
      ok &= read_ಖಂಡ (n_needed, ಖಂಡ[idx], &n_ಬೈಟುಗಳು_read);
      ಆದರೆ (n_ಬೈಟುಗಳು_read < ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ)
        ನಿಲ್ಲಿಸು;
      affirm (n_ಬೈಟುಗಳು_read == ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ);
      write_ಖಂಡ (current_ಸ್ಥಾನಾಂತರ, n_ಬೈಟುಗಳು_read,
                   ಖಂಡ[!idx], ಖಂಡ[idx]);
      ಆದರೆ (ferror (stdout))
        ok = false;
      current_ಸ್ಥಾನಾಂತರ += n_ಬೈಟುಗಳು_read;
      idx = !idx;
    }
  ಆಗ (ok);

  ಆದರೆ (n_ಬೈಟುಗಳು_read > 0)
    {
      ಪೂರ್ಣಾಂಕ l_c_m = get_lcm ();

      /* Ensure zero-byte padding up to the smallest multiple of l_c_m that
         is at least as large as n_bytes_read.  */
      idx_t ಬೈಟುಗಳು_ಗಮ್ಯ_write = (n_ಬೈಟುಗಳು_read + l_c_m - 1
                              - (n_ಬೈಟುಗಳು_read + l_c_m - 1) % l_c_m);

      memset (ಖಂಡ[idx] + n_ಬೈಟುಗಳು_read, 0, ಬೈಟುಗಳು_ಗಮ್ಯ_write - n_ಬೈಟುಗಳು_read);
      write_ಖಂಡ (current_ಸ್ಥಾನಾಂತರ, n_ಬೈಟುಗಳು_read, ಖಂಡ[!idx], ಖಂಡ[idx]);
      current_ಸ್ಥಾನಾಂತರ += n_ಬೈಟುಗಳು_read;
    }

  ಆದರೆ (ok || current_ಸ್ಥಾನಾಂತರ)
    format_ವಿಳಾಸ (current_ಸ್ಥಾನಾಂತರ, '\n');

  ಆದರೆ (0 <= ಕೊನೆ_ಸ್ಥಾನಾಂತರ && ಕೊನೆ_ಸ್ಥಾನಾಂತರ <= current_ಸ್ಥಾನಾಂತರ)
    ok &= check_and_close (0);

  ಬಿಡುಗಡೆ (ಖಂಡ[0]);

  ಹಿಂತಿರುಗಿ ok;
}

/* STRINGS mode.  Find each "string constant" in the input.
   A string constant is a run of at least STRING_MIN
   printable characters terminated by a NUL or END_OFFSET.
   Based on a function written by Richard Stallman for a
   traditional version of od.  Return true if successful.  */

ಸ್ಥಿರ bool
dump_ಅಕ್ಷರಸರಣಿಗಳು (ಶೂನ್ಯ)
{
  idx_t bufsize = MAX (100, string_min + 1);
  ಅಕ್ಷರ *buf = ximalloc (bufsize);
  intmax_t ವಿಳಾಸ = n_ಬೈಟುಗಳು_ಗಮ್ಯ_skip;
  bool ok = true;

  ಆಗ (true)
    {
      idx_t i = 0;
      ಪೂರ್ಣಾಂಕ c = 1;  /* Init to 1 so can distinguish if NUL read.  */

      ಆದರೆ (0 <= ಕೊನೆ_ಸ್ಥಾನಾಂತರ
          && (ಕೊನೆ_ಸ್ಥಾನಾಂತರ < string_min || ಕೊನೆ_ಸ್ಥಾನಾಂತರ - string_min < ವಿಳಾಸ))
        ನಿಲ್ಲಿಸು;

      /* Store consecutive printable characters to BUF.  */
      ಆಗ (! (0 <= ಕೊನೆ_ಸ್ಥಾನಾಂತರ && ಕೊನೆ_ಸ್ಥಾನಾಂತರ <= ವಿಳಾಸ))
        {
          ಆದರೆ (i == bufsize - 1)
            buf = xpalloc (buf, &bufsize, 1, -1, ಗಾತ್ರ *buf);
          ok &= read_char (&c);
          ಆದರೆ (c < 0)
            {
              ಬಿಡುಗಡೆ (buf);
              ಹಿಂತಿರುಗಿ ok;
            }
          ವಿಳಾಸ++;
          buf[i++] = c;
          ಆದರೆ (c == '\0')
            ನಿಲ್ಲಿಸು;		/* Print this string.  */
          ಆದರೆ (! isprint (c))
            {
              c = -1;		/* Give up on this string.  */
              ನಿಲ್ಲಿಸು;
            }
        }

      ಆದರೆ (c < 0 || i - !c < string_min)
        ಮುಂದುವರಿ;

      buf[i] = 0;

      /* If we get here, the string is all printable, so print it.  */

      format_ವಿಳಾಸ (ವಿಳಾಸ - i, ' ');

      ಗಾಗಿ (i = 0; (c = buf[i]); i++)
        {
          ಆಯ್ಕೆ (c)
            {
            ಸ್ಥಿತಿ '\a':
              fputs ("\\a", stdout);
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '\b':
              fputs ("\\b", stdout);
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '\f':
              fputs ("\\f", stdout);
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '\n':
              fputs ("\\n", stdout);
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '\r':
              fputs ("\\r", stdout);
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '\t':
              fputs ("\\t", stdout);
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '\v':
              fputs ("\\v", stdout);
              ನಿಲ್ಲಿಸು;

            ಪೂರ್ವನಿಯೋಜಿತ:
              putc (c, stdout);
            }
        }
      ಅಕ್ಷರ_ಬರೆ ('\n');
    }

  /* We reach this point only if we search through
     (max_bytes_to_format - string_min) bytes before reaching EOF.  */

  ಬಿಡುಗಡೆ (buf);

  ok &= check_and_close (0);
  ಹಿಂತಿರುಗಿ ok;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ n_files;
  ಪೂರ್ಣಾಂಕ l_c_m;
  idx_t desired_ಅಗಲ = 0;
  bool modern = false;
  bool ok = true;
  idx_t ಅಗಲ_ಪ್ರತಿ_ಖಂಡ = 0;
  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ multipliers[] = "bEGKkMmPQRTYZ0";

  /* The maximum number of bytes that will be formatted.
     If negative, there is no limit.  */
  intmax_t max_ಬೈಟುಗಳು_ಗಮ್ಯ_format = -1;

  /* The old-style 'pseudo starting address' to be printed in parentheses
     after any true address.  */
  intmax_t pseudo_ಆರಂಭ IF_LINT ( = 0);

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ (true)
    {
      intmax_t tmp;
      ಎಣಿಕೆ strtol_error s_err;
      ಪೂರ್ಣಾಂಕ oi = -1;
      ಪೂರ್ಣಾಂಕ c = getopt_long (argc, argv, short_options, long_options, &oi);
      ಆದರೆ (c == -1)
        ನಿಲ್ಲಿಸು;

      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'A':
          modern = true;
          ಆಯ್ಕೆ (optarg[0])
            {
            ಸ್ಥಿತಿ 'd':
              format_ವಿಳಾಸ = format_ವಿಳಾಸ_std;
              ವಿಳಾಸ_ತಳ = 10;
              ವಿಳಾಸ_pad_len = 7;
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'o':
              format_ವಿಳಾಸ = format_ವಿಳಾಸ_std;
              ವಿಳಾಸ_ತಳ = 8;
              ವಿಳಾಸ_pad_len = 7;
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'x':
              format_ವಿಳಾಸ = format_ವಿಳಾಸ_std;
              ವಿಳಾಸ_ತಳ = 16;
              ವಿಳಾಸ_pad_len = 6;
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'n':
              format_ವಿಳಾಸ = format_ವಿಳಾಸ_none;
              ವಿಳಾಸ_pad_len = 0;
              ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ:
              error (EXIT_FAILURE, 0,
                     _("invalid output address radix '%c';"
                       " it must be one character from [doxn]"),
                     optarg[0]);
              ನಿಲ್ಲಿಸು;
            }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'j':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &n_ಬೈಟುಗಳು_ಗಮ್ಯ_skip, multipliers);
          ಆದರೆ (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'N':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &max_ಬೈಟುಗಳು_ಗಮ್ಯ_format, multipliers);
          ಆದರೆ (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'S':
          modern = true;
          ಆದರೆ (optarg == ಖಾಲಿ)
            string_min = 3;
          ಇಲ್ಲದಿದ್ದರೆ
            {
              s_err = xstr2nonneg (optarg, 0, &tmp, multipliers);
              /* The minimum string length + 1 must fit in idx_t,
                 since we may allocate a buffer of this size + 1.  */
              idx_t i;
              ಆದರೆ (s_err == LONGINT_OK && ckd_ಕೂಡಿಸು (&i, tmp, 1))
                s_err = LONGINT_ಅತಿಪ್ರವಾಹ;
              ಆದರೆ (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
              string_min = tmp;
            }
          flag_dump_ಅಕ್ಷರಸರಣಿಗಳು = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 't':
          modern = true;
          ok &= decode_format_string (optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'v':
          modern = true;
          abbreviate_duplicate_ಖಂಡಗಳು = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ TRADITIONAL_OPTION:
          traditional = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ENDIAN_OPTION:
          ಆಯ್ಕೆ (XARGMATCH ("--endian", optarg, endian_ವಾದಗಳು, endian_types))
            {
            ಸ್ಥಿತಿ endian_big:
              ಒಳಹರಿವು_ಅದಲುಬದಲು = BYTE_ORDER != BIG_ENDIAN;
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ endian_little:
              ಒಳಹರಿವು_ಅದಲುಬದಲು = BYTE_ORDER != LITTLE_ENDIAN;
              ನಿಲ್ಲಿಸು;
            }
          ನಿಲ್ಲಿಸು;

          /* The next several cases map the traditional format
             specification options to the corresponding modern format
             specs.  GNU od accepts any combination of old- and
             new-style options.  Format specification options accumulate.
             The obsolescent and undocumented formats are compatible
             with FreeBSD 4.10 od.  */

#ವ್ಯಾಖ್ಯಾನಿಸು CASE_OLD_ARG(old_char,new_string)		\
        ಸ್ಥಿತಿ old_char:					\
          ok &= decode_format_string (new_string);	\
          ನಿಲ್ಲಿಸು

          CASE_OLD_ARG ('a', "a");
          CASE_OLD_ARG ('b', "o1");
          CASE_OLD_ARG ('c', "c");
          CASE_OLD_ARG ('D', "u4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('d', "u2");
        ಸ್ಥಿತಿ 'F': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('e', "fD"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('f', "fF");
        ಸ್ಥಿತಿ 'X': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('H', "x4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('i', "dI");
        ಸ್ಥಿತಿ 'I': ಸ್ಥಿತಿ 'L': /* obsolescent and undocumented aliases */
          CASE_OLD_ARG ('l', "dL");
          CASE_OLD_ARG ('O', "o4"); /* obsolescent and undocumented */
        ಸ್ಥಿತಿ 'B': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('o', "o2");
          CASE_OLD_ARG ('s', "d2");
        ಸ್ಥಿತಿ 'h': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('x', "x2");

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CASE_OLD_ARG

        ಸ್ಥಿತಿ 'w':
          modern = true;
          ಆದರೆ (optarg == ಖಾಲಿ)
            {
              desired_ಅಗಲ = 32;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              intmax_t w_tmp;
              s_err = xstr2nonneg (optarg, 10, &w_tmp, "");
              ಆದರೆ (s_err == LONGINT_OK)
                {
                  ಆದರೆ (ckd_ಕೂಡಿಸು (&desired_ಅಗಲ, w_tmp, 0))
                    s_err = LONGINT_ಅತಿಪ್ರವಾಹ;
                  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (desired_ಅಗಲ == 0)
                    s_err = LONGINT_INVALID;
                }
              ಆದರೆ (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
            }
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
          ನಿಲ್ಲಿಸು;
        }
    }

  ಆದರೆ (!ok)
    ಹಿಂತಿರುಗಿ EXIT_FAILURE;

  ಆದರೆ (flag_dump_ಅಕ್ಷರಸರಣಿಗಳು && n_specs > 0)
    error (EXIT_FAILURE, 0,
           _("no type may be specified when dumping strings"));

  n_files = argc - optind;

  /* If the --traditional option is used, there may be from
     0 to 3 remaining command line arguments;  handle each case
     separately.
        od [file] [[+]offset[.][b] [[+]label[.][b]]]
     The offset and label have the same syntax.

     If --traditional is not given, and if no modern options are
     given, and if the offset begins with + or (if there are two
     operands) a digit, accept only this form, as per POSIX:
        od [file] [[+]offset[.][b]]
  */

  ಆದರೆ (!modern || traditional)
    {
      intmax_t o1;
      intmax_t o2;

      ಆಯ್ಕೆ (n_files)
        {
        ಸ್ಥಿತಿ 1:
          ಆದರೆ ((traditional || argv[optind][0] == '+')
              && parse_old_ಸ್ಥಾನಾಂತರ (argv[optind], &o1))
            {
              n_ಬೈಟುಗಳು_ಗಮ್ಯ_skip = o1;
              --n_files;
              ++argv;
            }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 2:
          ಆದರೆ ((traditional || argv[optind + 1][0] == '+'
               || c_isdigit (argv[optind + 1][0]))
              && parse_old_ಸ್ಥಾನಾಂತರ (argv[optind + 1], &o2))
            {
              ಆದರೆ (traditional && parse_old_ಸ್ಥಾನಾಂತರ (argv[optind], &o1))
                {
                  n_ಬೈಟುಗಳು_ಗಮ್ಯ_skip = o1;
                  flag_pseudo_ಆರಂಭ = true;
                  pseudo_ಆರಂಭ = o2;
                  argv += 2;
                  n_files -= 2;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  n_ಬೈಟುಗಳು_ಗಮ್ಯ_skip = o2;
                  --n_files;
                  argv[optind + 1] = argv[optind];
                  ++argv;
                }
            }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 3:
          ಆದರೆ (traditional
              && parse_old_ಸ್ಥಾನಾಂತರ (argv[optind + 1], &o1)
              && parse_old_ಸ್ಥಾನಾಂತರ (argv[optind + 2], &o2))
            {
              n_ಬೈಟುಗಳು_ಗಮ್ಯ_skip = o1;
              flag_pseudo_ಆರಂಭ = true;
              pseudo_ಆರಂಭ = o2;
              argv[optind + 2] = argv[optind];
              argv += 2;
              n_files -= 2;
            }
          ನಿಲ್ಲಿಸು;
        }

      ಆದರೆ (traditional && 1 < n_files)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
          error (0, 0, "%s",
                 _("compatibility mode supports at most one file"));
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (flag_pseudo_ಆರಂಭ)
    {
      ಆದರೆ (format_ವಿಳಾಸ == format_ವಿಳಾಸ_none)
        {
          ವಿಳಾಸ_ತಳ = 8;
          ವಿಳಾಸ_pad_len = 7;
          format_ವಿಳಾಸ = format_ವಿಳಾಸ_paren;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        format_ವಿಳಾಸ = format_ವಿಳಾಸ_label;
    }

  ಆದರೆ (0 <= max_ಬೈಟುಗಳು_ಗಮ್ಯ_format
      && ckd_ಕೂಡಿಸು (&ಕೊನೆ_ಸ್ಥಾನಾಂತರ, n_ಬೈಟುಗಳು_ಗಮ್ಯ_skip, max_ಬೈಟುಗಳು_ಗಮ್ಯ_format))
    error (EXIT_FAILURE, 0, _("skip-bytes + read-bytes is too large"));

  ಆದರೆ (n_specs == 0)
    decode_format_string ("oS");

  ಆದರೆ (n_files > 0)
    {
      /* Set the global pointer FILE_LIST so that it
         references the first file-argument on the command-line.  */

      file_list = (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ *) &argv[optind];
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* No files were listed on the command line.
         Set the global pointer FILE_LIST so that it
         references the null-terminated list of one name: "-".  */

      file_list = default_file_list;
    }

  /* open the first input file */
  ok = open_ಮುಂದಿನ_file ();
  ಆದರೆ (ಒಳ_ಹರಿವು == ಖಾಲಿ)
    ಹೋಗು cleanup;

  /* skip over any unwanted header bytes */
  ok &= skip (n_ಬೈಟುಗಳು_ಗಮ್ಯ_skip);
  ಆದರೆ (ಒಳ_ಹರಿವು == ಖಾಲಿ)
    ಹೋಗು cleanup;

  pseudo_ಸ್ಥಾನಾಂತರ = (flag_pseudo_ಆರಂಭ ? pseudo_ಆರಂಭ - n_ಬೈಟುಗಳು_ಗಮ್ಯ_skip : 0);

  /* Compute output block length.  */
  l_c_m = get_lcm ();

  ಆದರೆ (desired_ಅಗಲ != 0)
    {
      ಆದರೆ (desired_ಅಗಲ % l_c_m == 0)
        ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ = desired_ಅಗಲ;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          error (0, 0, _("warning: invalid width %td; using %d instead"),
                 desired_ಅಗಲ, l_c_m);
          ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ = l_c_m;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (l_c_m < DEFAULT_ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ)
        ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ = (DEFAULT_ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ
                           - DEFAULT_ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ % l_c_m);
      ಇಲ್ಲದಿದ್ದರೆ
        ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ = l_c_m;
    }

  /* Compute padding necessary to align output block.  */
  ಗಾಗಿ (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_ಪ್ರತಿ_ಖಂಡ = ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ / ಅಗಲ_ಬೈಟುಗಳು[spec[i].ಗಾತ್ರ_2];
      ಆದರೆ (pad_at_ಅತಿಪ್ರವಾಹ (fields_ಪ್ರತಿ_ಖಂಡ))
        error (EXIT_FAILURE, 0, _("%td is too large"), desired_ಅಗಲ);
      idx_t ಖಂಡ_ಅಗಲ = (spec[i].ಕ್ಷೇತ್ರ_ಅಗಲ + 1) * fields_ಪ್ರತಿ_ಖಂಡ;
      ಆದರೆ (ಅಗಲ_ಪ್ರತಿ_ಖಂಡ < ಖಂಡ_ಅಗಲ)
        ಅಗಲ_ಪ್ರತಿ_ಖಂಡ = ಖಂಡ_ಅಗಲ;
    }
  ಗಾಗಿ (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_ಪ್ರತಿ_ಖಂಡ = ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ / ಅಗಲ_ಬೈಟುಗಳು[spec[i].ಗಾತ್ರ_2];
      idx_t ಖಂಡ_ಅಗಲ = spec[i].ಕ್ಷೇತ್ರ_ಅಗಲ * fields_ಪ್ರತಿ_ಖಂಡ;
      spec[i].pad_ಅಗಲ = ಅಗಲ_ಪ್ರತಿ_ಖಂಡ - ಖಂಡ_ಅಗಲ;
    }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
  ಬರೆ ("lcm=%d, width_per_block=%td\n", l_c_m, ಅಗಲ_ಪ್ರತಿ_ಖಂಡ);
  ಗಾಗಿ (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_ಪ್ರತಿ_ಖಂಡ = ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ / ಅಗಲ_ಬೈಟುಗಳು[spec[i].ಗಾತ್ರ_2];
      affirm (ಬೈಟುಗಳು_ಪ್ರತಿ_ಖಂಡ % ಅಗಲ_ಬೈಟುಗಳು[spec[i].ಗಾತ್ರ_2] == 0);
      affirm (1 <= spec[i].pad_ಅಗಲ / fields_ಪ್ರತಿ_ಖಂಡ);
      ಬರೆ ("%td: fmt=\"%s\" in_width=%d out_width=%d pad=%td\n",
              i, spec[i].fmt_string, ಅಗಲ_ಬೈಟುಗಳು[spec[i].ಗಾತ್ರ_2],
              spec[i].ಕ್ಷೇತ್ರ_ಅಗಲ, spec[i].pad_ಅಗಲ);
    }
#ಆದರೆ_ಅಂತ್ಯ

  ok &= (flag_dump_ಅಕ್ಷರಸರಣಿಗಳು ? dump_ಅಕ್ಷರಸರಣಿಗಳು () : dump ());

cleanup:

  ಆದರೆ (have_read_stdin && fclose (stdin) < 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
