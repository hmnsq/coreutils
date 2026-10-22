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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <endian.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <float.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ftoastr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-size.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xbinary-io.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xprintf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "od"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("Jim Meyering")

/* The default number of input bytes per output line.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ 16

#ਜੇ FLOAT16_SUPPORTED
  /* Available since clang 6 (2018), and gcc 7 (2017).  */
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ _Float16 float16;
#ਨਹੀਂਤਾਂ
# define FLOAT16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਦਸ਼ਮਲਵ float16;
#ਜੇ_ਅੰਤ

#ਜੇ BF16_SUPPORTED
  /* Available since clang 11 (2020), and gcc 13 (2023). */
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ __bf16 bfloat16;
#ਨਹੀਂਤਾਂ
# define BF16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਦਸ਼ਮਲਵ bfloat16;
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ ਮਾਪ_spec
  {
    NO_ਮਾਪ = 0,
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
    N_ਮਾਪ_SPECS
  };

ਗਿਣਤੀ output_format
  {
    SIGNED_DECIMAL,
    UNSIGNED_DECIMAL,
    OCTAL,
    HEXADECIMAL,
    FLOATING_ਬਿੰਦੂ,
    HFLOATING_ਬਿੰਦੂ,
    BFLOATING_ਬਿੰਦੂ,
    NAMED_CHARACTER,
    CHARACTER
  };

ਗਿਣਤੀ { MAX_INTEGRAL_TYPE_ਚੌੜਾਈ = UINTMAX_ਚੌੜਾਈ };

/* The maximum number of bytes needed for a format string, including
   the trailing nul.  Each format string expects a variable amount of
   padding (guaranteed to be at least 1 plus the field width), then an
   element that will be formatted in the field.  */
ਗਿਣਤੀ
  {
    FMT_ਬਾਈਟਾਂ_ALLOCATED =
      ਆਕਾਰ "%*.%dlld" - ਆਕਾਰ "%d" + INT_STRLEN_BOUND (ਪੂਰਨ) + 1
  };

/* Ensure that our choice for FMT_BYTES_ALLOCATED is reasonable.  */
static_assert (MAX_INTEGRAL_TYPE_ਚੌੜਾਈ <= 3 * 99);

/* The type of a print function.  FIELDS is the number of fields per
   line, BLANK is the number of fields to leave blank.  WIDTH is width
   of one field, excluding leading space, and PAD is total pad to
   divide among FIELDS.  PAD is at least as large as FIELDS.  */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*print_function_type)
  (idx_t fields, idx_t blank, ਖਾਲੀ ਅਚਲ *ਅੰਕੜੇ,
   ਅੱਖਰ ਅਚਲ *fmt, ਪੂਰਨ ਚੌੜਾਈ, idx_t pad);

/* Each output format specification (from '-t spec' or from
   old-style options) is represented by one of these structures.  */
ਬਣਤਰ tspec
  {
    ਗਿਣਤੀ output_format fmt;
    ਗਿਣਤੀ ਮਾਪ_spec ਮਾਪ; /* Type of input object.  */
    print_function_type print_function;
    ਅੱਖਰ fmt_string[FMT_ਬਾਈਟਾਂ_ALLOCATED]; /* Of the style "%*d".  */
    bool hexl_ਬਹੁਲਕ_trailer;
    ਪੂਰਨ ਖੇਤਰ_ਚੌੜਾਈ; /* Minimum width of a field, excluding leading space.  */
    idx_t pad_ਚੌੜਾਈ; /* Total padding to be divided among fields.  */
  };

/* Convert enum size_spec to the size of the named type.  */
ਸਥਿਰ ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ_ਬਾਈਟਾਂ[] =
{
  -1,
  ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ),
#ਜੇ UCHAR_MAX < USHRT_MAX
  ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ ਪੂਰਨ),
#ਜੇ_ਅੰਤ
#ਜੇ USHRT_MAX < UINT_MAX
  ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ),
#ਜੇ_ਅੰਤ
#ਜੇ UINT_MAX < ULONG_MAX
  ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ),
#ਜੇ_ਅੰਤ
#ਜੇ ULONG_MAX < ULLONG_MAX
  ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ),
#ਜੇ_ਅੰਤ
#ਜੇ ULLONG_MAX < UINTMAX_MAX
  ਆਕਾਰ (uintmax_t),
#ਜੇ_ਅੰਤ
#ਜੇ BF16_SUPPORTED
  ਆਕਾਰ (bfloat16),
#ਜਾਂਜੇ FLOAT16_SUPPORTED
  ਆਕਾਰ (float16),
#ਜੇ_ਅੰਤ
#ਜੇ FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
  ਆਕਾਰ (ਦਸ਼ਮਲਵ),
#ਜੇ_ਅੰਤ
  ਆਕਾਰ (ਦੁੱਗਣਾ),
#ਜੇ DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
  ਆਕਾਰ (ਲੰਮਾ ਦੁੱਗਣਾ),
#ਜੇ_ਅੰਤ
};

/* Ensure that for each member of 'enum size_spec' there is an
   initializer in the width_bytes array.  */
static_assert (countof (ਚੌੜਾਈ_ਬਾਈਟਾਂ) == N_ਮਾਪ_SPECS);

/* Names for some non-printing characters.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ charname[33][4] =
{
  "nul", "soh", "stx", "etx", "eot", "enq", "ack", "bel",
  "bs", "ht", "nl", "vt", "ff", "cr", "so", "si",
  "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb",
  "can", "em", "sub", "esc", "fs", "gs", "rs", "us",
  "sp"
};

/* Address base (8, 10 or 16).  */
ਸਥਿਰ ਪੂਰਨ ਪਤਾ_ਅਧਾਰ = 8;

/* The number of octal digits required to represent the largest
   address value.  */
ਗਿਣਤੀ { MAX_ਪਤਾ_ਲੰਮਾਈ = ((INTMAX_ਚੌੜਾਈ - 1) / 3
                             + ((INTMAX_ਚੌੜਾਈ - 1) % 3 != 0)) };

/* Width of a normal address.  */
ਸਥਿਰ ਪੂਰਨ ਪਤਾ_pad_len = 7;

/* Minimum length when detecting --strings.  */
ਸਥਿਰ idx_t string_min;

/* True when in --strings mode.  */
ਸਥਿਰ bool flag_dump_ਲੜੀਆਂ;

/* True if we should recognize the older non-option arguments
   that specified at most one file and optional arguments specifying
   offset and pseudo-start address.  */
ਸਥਿਰ bool traditional;

/* True if an old-style 'pseudo-address' was specified.  */
ਸਥਿਰ bool flag_pseudo_ਸ਼ੁਰੂ;

/* The difference between the old-style pseudo starting address and
   the number of bytes to skip.  */
ਸਥਿਰ intmax_t pseudo_ਵਿੱਥ;

/* Function that accepts an address and an optional following char,
   and prints the address and char to stdout.  */
ਸਥਿਰ ਖਾਲੀ format_ਪਤਾ_std (intmax_t, ਅੱਖਰ);
ਸਥਿਰ ਖਾਲੀ (*format_ਪਤਾ) (intmax_t, ਅੱਖਰ) = format_ਪਤਾ_std;

/* The number of input bytes to skip before formatting and writing.  */
ਸਥਿਰ intmax_t n_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_skip;

/* The offset of the first byte after the last byte to be formatted.
   If negative, there is no limit.  */
ਸਥਿਰ intmax_t ਸਿਰਾ_ਵਿੱਥ = -1;

/* When true and two or more consecutive blocks are equal, format
   only the first block and output an asterisk alone on the following
   line to indicate that identical blocks have been elided.  */
ਸਥਿਰ bool abbreviate_duplicate_ਖੰਡਾਂ = true;

/* An array of specs describing how to format each input block.  */
ਸਥਿਰ ਬਣਤਰ tspec *spec;

/* The number of format specs.  */
ਸਥਿਰ idx_t n_specs;

/* The allocated length of SPEC.  */
ਸਥਿਰ idx_t n_specs_allocated;

/* The number of input bytes formatted per output line.  It must be
   a multiple of the least common multiple of the sizes associated with
   the specified output types.  It should be as large as possible, but
   no larger than 16 -- unless specified with the -w option.  */
ਸਥਿਰ idx_t ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ;

/* Human-readable representation of *file_list (for error messages).
   It differs from file_list[-1] only when file_list[-1] is "-".  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਨਿਵੇਸ਼_filename;

/* A null-terminated list of the file-arguments from the command line.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ *file_list;

/* Initializer for file_list if no file-arguments
   were specified on the command line.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ default_file_list[] = {"-", ਨਿਰਾ};

/* The input stream associated with the current file.  */
ਸਥਿਰ FILE *ਵਿੱਚ_ਧਾਰਾ;

/* If true, at least one of the files we read was standard input.  */
ਸਥਿਰ bool have_read_stdin;

/* Map the size in bytes to a type identifier.
   When two types have the same machine layout:
     - Prefer unsigned int to other types, as its format is shorter.
     - Prefer unsigned long to higher-ranked types, as it is older.
     - Prefer uintmax_t to unsigned long long int; this wins if %lld
       does not work but %jd does (e.g., MS-Windows).  */
ਸਥਿਰ ਗਿਣਤੀ ਮਾਪ_spec ਅਚਲ integral_type_ਮਾਪ[] =
  {
#ਜੇ UCHAR_MAX < USHRT_MAX
    [ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)] = CHAR,
#ਜੇ_ਅੰਤ
#ਜੇ USHRT_MAX < UINT_MAX
    [ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ ਪੂਰਨ)] = SHORT,
#ਜੇ_ਅੰਤ
    [ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)] = INT,
#ਜੇ UINT_MAX < ULONG_MAX
    [ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ)] = LONG,
#ਜੇ_ਅੰਤ
#ਜੇ ULONG_MAX < ULLONG_MAX && ULLONG_MAX < UINTMAX_MAX
    [ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)] = LONG_LONG,
#ਜੇ_ਅੰਤ
#ਜੇ ULONG_MAX < UINTMAX_MAX
    [ਆਕਾਰ (uintmax_t)] = INTMAX,
#ਜੇ_ਅੰਤ
  };

/* Map the size in bytes to a floating type identifier.
   When two types have the same machine layout:
     - Prefer double to the other types, as its format is shorter.  */
ਸਥਿਰ ਗਿਣਤੀ ਮਾਪ_spec ਅਚਲ fp_type_ਮਾਪ[] =
  {
#ਜੇ FLOAT16_SUPPORTED
    [ਆਕਾਰ (float16)] = FLOAT_HALF,
#ਜਾਂਜੇ BF16_SUPPORTED
    [ਆਕਾਰ (bfloat16)] = FLOAT_HALF,
#ਜੇ_ਅੰਤ
#ਜੇ FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
    [ਆਕਾਰ (ਦਸ਼ਮਲਵ)] = FLOAT_SINGLE,
#ਜੇ_ਅੰਤ
    [ਆਕਾਰ (ਦੁੱਗਣਾ)] = FLOAT_DOUBLE,
#ਜੇ DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
    [ਆਕਾਰ (ਲੰਮਾ ਦੁੱਗਣਾ)] = FLOAT_LONG_DOUBLE,
#ਜੇ_ਅੰਤ
  };

/* Use native endianness by default.  */
ਸਥਿਰ bool ਨਿਵੇਸ਼_ਅਦਲਾ_ਬਦਲੀ;

ਸਥਿਰ ਅੱਖਰ ਅਚਲ short_options[] = "A:aBbcDdeFfHhIij:LlN:OoS:st:vw::Xx";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  TRADITIONAL_OPTION = CHAR_MAX + 1,
  ENDIAN_OPTION,
};

ਗਿਣਤੀ endian_type
{
  endian_little,
  endian_big
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ endian_ਦਲੀਲਾਂ[] =
{
  "little", "big", ਨਿਰਾ
};

ਸਥਿਰ ਗਿਣਤੀ endian_type ਅਚਲ endian_types[] =
{
  endian_little, endian_big
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"skip-bytes", required_ਦਲੀਲ, ਨਿਰਾ, 'j'},
  {"address-radix", required_ਦਲੀਲ, ਨਿਰਾ, 'A'},
  {"read-bytes", required_ਦਲੀਲ, ਨਿਰਾ, 'N'},
  {"format", required_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {"output-duplicates", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {"strings", optional_ਦਲੀਲ, ਨਿਰਾ, 'S'},
  {"traditional", no_ਦਲੀਲ, ਨਿਰਾ, TRADITIONAL_OPTION},
  {"width", optional_ਦਲੀਲ, ਨਿਰਾ, 'w'},
  {"endian", required_ਦਲੀਲ, ਨਿਰਾ, ENDIAN_OPTION },

  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [-abcdfilosx]... [FILE] [[+]ਵਿੱਥ_2[.][b]]\n\
  or:  %s --traditional [OPTION]... [FILE] [[+]ਵਿੱਥ_2[.][b] [+][LABEL][.][b]]\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\n\
Write an unambiguous representation, octal ਬਾਈਟਾਂ by ਮੂਲ,\n\
ਦਾ FILE ਮੰਜ਼ਿਲ standard output.  With ਹੋਰ than one FILE ਦਲੀਲ,\n\
concatenate them ਵਿੱਚ the listed ਕ੍ਰਮ ਮੰਜ਼ਿਲ form the ਨਿਵੇਸ਼.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
If ਪਹਿਲਾ and ਦੂਜਾ call formats both apply, the ਦੂਜਾ format is assumed\n\
ਜੇ the ਆਖਰੀ ਸੰਕਾਰਜ_2 begins with + or (ਜੇ there are 2 operands) a digit.\n\
An ਵਿੱਥ_2 ਸੰਕਾਰਜ_2 means -j ਵਿੱਥ_2.  LABEL is the pseudo-ਪਤਾ\n\
at ਪਹਿਲਾ byte printed, incremented when dump is progressing.\n\
For ਵਿੱਥ_2 and LABEL, a 0x or 0X ਅਗੇਤਰ indicates hexadecimal;\n\
suffixes may be . ਲਈ octal and b ਲਈ multiply by 512.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --ਪਤਾ-radix=RADIX\n\
         output format ਲਈ file offsets;\n\
         RADIX is one ਦਾ [doxn], ਲਈ Decimal, Octal, Hex or None\n\
"));
      oputs (_("\
      --endian={big|little}\n\
         ਅਦਲਾ_ਬਦਲੀ ਨਿਵੇਸ਼ ਬਾਈਟਾਂ according the specified ਕ੍ਰਮ\n\
"));
      oputs (_("\
  -j, --skip-ਬਾਈਟਾਂ=ਬਾਈਟਾਂ_2\n\
         skip ਬਾਈਟਾਂ_2 ਨਿਵੇਸ਼ ਬਾਈਟਾਂ ਪਹਿਲਾ\n\
"));
      oputs (_("\
  -N, --ਪਠ-ਬਾਈਟਾਂ=ਬਾਈਟਾਂ_2\n\
         ਹੱਦਬੰਦੀ dump ਮੰਜ਼ਿਲ ਬਾਈਟਾਂ_2 ਨਿਵੇਸ਼ ਬਾਈਟਾਂ\n\
"));
      oputs (_("\
  -S ਬਾਈਟਾਂ_2, --ਲੜੀਆਂ[=ਬਾਈਟਾਂ_2]\n\
         show only NUL terminated ਲੜੀਆਂ\n\
         ਦਾ at ਘੱਟ_ਤੋਂ_ਘੱਟ ਬਾਈਟਾਂ_2 (ਮੂਲ 3) printable characters\n\
"));
      oputs (_("\
  -t, --format=TYPE\n\
         select output format or formats\n\
"));
      oputs (_("\
  -v, --output-duplicates\n\
         ਕਰੋ not use * ਮੰਜ਼ਿਲ mark ਸਤਰ suppression\n\
"));
      oputs (_("\
  -w[ਬਾਈਟਾਂ_2], --ਚੌੜਾਈ[=ਬਾਈਟਾਂ_2]\n\
         output ਬਾਈਟਾਂ_2 ਬਾਈਟਾਂ ਪ੍ਰਤੀ output ਸਤਰ;\n\
         32 is implied when ਬਾਈਟਾਂ_2 is not specified\n\
"));
      oputs (_("\
      --traditional\n\
         accept arguments ਵਿੱਚ ਤੀਜਾ form above\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
\n\
Traditional format specifications may be intermixed; they accumulate:\n\
"), stdout);
      oputs (_("\
  -a   same as -t a,  select named characters, ignoring ਉੱਚਾ-ਕ੍ਰਮ ਦੋਆਧਾਰੀ_ਅੰਕ\n\
"));
      oputs (_("\
  -b   same as -t o1, select octal ਬਾਈਟਾਂ\n\
"));
      oputs (_("\
  -c   same as -t c,  select printable characters or backslash escapes\n\
"));
      oputs (_("\
  -d   same as -t u2, select ਬਿਨਾਨਿਸ਼ਾਨੀ decimal 2-byte units\n\
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
TYPE is made ਉੱਪਰ ਦਾ one or ਹੋਰ ਦਾ these specifications:\n\
  a          named character, ignoring ਉੱਚਾ-ਕ੍ਰਮ ਦੋਆਧਾਰੀ_ਅੰਕ\n\
  c          printable character or backslash escape\n\
"), stdout);
      fputs (_("\
  d[ਮਾਪ_2]    ਨਿਸ਼ਾਨੀਵਾਲਾ decimal, ਮਾਪ_2 ਬਾਈਟਾਂ ਪ੍ਰਤੀ integer\n\
  f[ਮਾਪ_2]    floating ਬਿੰਦੂ, ਮਾਪ_2 ਬਾਈਟਾਂ ਪ੍ਰਤੀ ਦਸ਼ਮਲਵ\n\
  o[ਮਾਪ_2]    octal, ਮਾਪ_2 ਬਾਈਟਾਂ ਪ੍ਰਤੀ integer\n\
  u[ਮਾਪ_2]    ਬਿਨਾਨਿਸ਼ਾਨੀ decimal, ਮਾਪ_2 ਬਾਈਟਾਂ ਪ੍ਰਤੀ integer\n\
  x[ਮਾਪ_2]    hexadecimal, ਮਾਪ_2 ਬਾਈਟਾਂ ਪ੍ਰਤੀ integer\n\
"), stdout);
      fputs (_("\
\n\
ਮਾਪ_2 is a ਸੰਖਿਆ.  For TYPE ਵਿੱਚ [doux], ਮਾਪ_2 may also be C ਲਈ\n\
ਆਕਾਰ(ਅੱਖਰ), S ਲਈ ਆਕਾਰ(ਛੋਟਾ), I ਲਈ ਆਕਾਰ(ਪੂਰਨ) or L ਲਈ\n\
ਆਕਾਰ(ਲੰਮਾ).  If TYPE is f, ਮਾਪ_2 may also be B ਲਈ Brain 16 ਦੋਆਧਾਰੀ_ਅੰਕ,\n\
H ਲਈ Half precision ਦਸ਼ਮਲਵ, F ਲਈ ਆਕਾਰ(ਦਸ਼ਮਲਵ), D ਲਈ ਆਕਾਰ(ਦੁੱਗਣਾ),\n\
or L ਲਈ ਆਕਾਰ(ਲੰਮਾ ਦੁੱਗਣਾ).\n\
"), stdout);
      fputs (_("\
\n\
Adding a z suffix ਮੰਜ਼ਿਲ any type displays printable characters at the ਸਿਰਾ ਦਾ\n\
each output ਸਤਰ.\n\
"), stdout);
      fputs (_("\
\n\
\n\
ਬਾਈਟਾਂ_2 is hex with 0x or 0X ਅਗੇਤਰ, and may have a multiplier suffix:\n\
  b    512\n\
  KB   1000\n\
  K    1024\n\
  MB   1000*1000\n\
  M    1024*1024\n\
and so on ਲਈ G, T, P, E, Z, Y, R, Q.\n\
ਦੁਆਧਾਰੀ_2 prefixes can be ਵਰਤਿਆ, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Define the print functions.  */

/* Print N spaces, where 0 <= N.
   Do not rely on printf ("%*s", N, "") as N may exceed INT_MAX.  */
ਸਥਿਰ ਖਾਲੀ
print_n_spaces (intmax_t n)
{
  ਲਈ (; 0 < n; n--)
    ਅੱਖਰ_ਲਿਖੋ (' ');
}

/* If there are FIELDS fields, return the total padding up to the
   start of field I, where I < FIELDS.  PAD is the total padding for
   all fields.  The result equals (PAD * I) / FIELDS, except it does
   not suffer from internal overflow.  */
ਸਥਿਰ idx_t
pad_at (idx_t fields, idx_t i, idx_t pad)
{
  /* This implementation assumes that (FIELDS - 1)^2 does not overflow
     intmax_t, an assumption checked by pad_at_overflow.  */
  intmax_t m = pad % fields;
  ਵਾਪਸ pad / fields * i + m * i / fields;
}
ਸਥਿਰ bool
pad_at_ਉਛਾਲ (idx_t fields)
{
  intmax_t ਗੁਣਨਫਲ;
  ਵਾਪਸ ckd_mul (&ਗੁਣਨਫਲ, fields - 1, fields - 1);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRINT_FIELDS(N, T, FMT_STRING_DECL, ACTION)                     \
ਸਥਿਰ ਖਾਲੀ                                                             \
N (idx_t fields, idx_t blank, ਖਾਲੀ ਅਚਲ *ਖੰਡ,			\
   FMT_STRING_DECL, ਪੂਰਨ ਚੌੜਾਈ, idx_t pad)				\
{                                                                       \
  T ਅਚਲ *p = ਖੰਡ;                                                   \
  idx_t pad_ਬਚਿਆ = pad;						\
  ਲਈ (idx_t i = fields; blank < i; i--)				\
    {                                                                   \
      idx_t ਅਗਲਾ_pad = pad_at (fields, i - 1, pad);			\
      ਪੂਰਨ adjusted_ਚੌੜਾਈ = pad_ਬਚਿਆ - ਅਗਲਾ_pad + ਚੌੜਾਈ;            \
      T x;                                                              \
      ਜੇ (ਨਿਵੇਸ਼_ਅਦਲਾ_ਬਦਲੀ && ਆਕਾਰ (T) > 1)                                 \
        {                                                               \
          ਸੰਘ {                                                       \
            T x;                                                        \
            ਅੱਖਰ b[ਆਕਾਰ (T)];                                         \
          } u;                                                          \
          ਲਈ (idx_t j = 0; j < ਆਕਾਰ (T); j++)			\
            u.b[j] = ((ਅੱਖਰ ਅਚਲ *) p)[ਆਕਾਰ (T) - 1 - j];            \
          x = u.x;                                                      \
        }                                                               \
      ਨਹੀਂਤਾਂ                                                              \
        x = *p;                                                         \
      p++;                                                              \
      ACTION;                                                           \
      pad_ਬਚਿਆ = ਅਗਲਾ_pad;                                         \
    }                                                                   \
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRINT_TYPE(N, T)                                                \
  PRINT_FIELDS (N, T, ਅੱਖਰ ਅਚਲ *fmt_string,                           \
                xprintf (fmt_string, adjusted_ਚੌੜਾਈ, x))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
  PRINT_FIELDS (N, T, MAYBE_ਅਣਵਰਤਿਆ ਅੱਖਰ ਅਚਲ *fmt_string,              \
                ਅੱਖਰ buf[BUFSIZE];                                      \
                FTOASTR (buf, ਆਕਾਰ buf, 0, 0, x);                     \
                xprintf ("%*s", adjusted_ਚੌੜਾਈ, buf))

PRINT_TYPE (print_s_char, ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ)
PRINT_TYPE (print_char, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)
PRINT_TYPE (print_s_short, ਛੋਟਾ ਪੂਰਨ)
PRINT_TYPE (print_short, ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ ਪੂਰਨ)
PRINT_TYPE (print_int, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)
PRINT_TYPE (print_long, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ)
PRINT_TYPE (print_long_long, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)
PRINT_TYPE (print_intmax, uintmax_t)

PRINT_FLOATTYPE (print_bfloat, bfloat16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_halffloat, float16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_float, ਦਸ਼ਮਲਵ, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_double, ਦੁੱਗਣਾ, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_long_double, ਲੰਮਾ ਦੁੱਗਣਾ, ldtoastr, LDBL_BUFSIZE_BOUND)

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PRINT_TYPE
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PRINT_FLOATTYPE

ਸਥਿਰ ਖਾਲੀ
dump_hexl_ਬਹੁਲਕ_trailer (idx_t n_ਬਾਈਟਾਂ, ਅੱਖਰ ਅਚਲ *ਖੰਡ)
{
  fputs ("  >", stdout);
  ਲਈ (idx_t i = n_ਬਾਈਟਾਂ; i > 0; i--)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = *ਖੰਡ++;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c2 = (isprint (c) ? c : '.');
      ਅੱਖਰ_ਲਿਖੋ (c2);
    }
  ਅੱਖਰ_ਲਿਖੋ ('<');
}

ਸਥਿਰ ਖਾਲੀ
print_named_ascii (idx_t fields, idx_t blank, ਖਾਲੀ ਅਚਲ *ਖੰਡ,
                   MAYBE_ਅਣਵਰਤਿਆ ਅੱਖਰ ਅਚਲ *ਅਣਵਰਤਿਆ_fmt_string,
                   ਪੂਰਨ ਚੌੜਾਈ, idx_t pad)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਅਚਲ *p = ਖੰਡ;
  idx_t pad_ਬਚਿਆ = pad;
  ਲਈ (idx_t i = fields; blank < i; i--)
    {
      ਪੂਰਨ masked_c = *p++ & 0x7f;
      ਅੱਖਰ ਅਚਲ *s;
      ਅੱਖਰ buf[2];

      ਜੇ (masked_c == 127)
        s = "del";
      ਨਹੀਂਤਾਂ ਜੇ (masked_c <= 040)
        s = charname[masked_c];
      ਨਹੀਂਤਾਂ
        {
          buf[0] = masked_c;
          buf[1] = 0;
          s = buf;
        }

      idx_t ਅਗਲਾ_pad = pad_at (fields, i - 1, pad);
      ਪੂਰਨ adjusted_ਚੌੜਾਈ = pad_ਬਚਿਆ - ਅਗਲਾ_pad + ਚੌੜਾਈ;
      xprintf ("%*s", adjusted_ਚੌੜਾਈ, s);
      pad_ਬਚਿਆ = ਅਗਲਾ_pad;
    }
}

ਸਥਿਰ ਖਾਲੀ
print_ascii (idx_t fields, idx_t blank, ਖਾਲੀ ਅਚਲ *ਖੰਡ,
             MAYBE_ਅਣਵਰਤਿਆ ਅੱਖਰ ਅਚਲ *ਅਣਵਰਤਿਆ_fmt_string, ਪੂਰਨ ਚੌੜਾਈ,
             idx_t pad)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਅਚਲ *p = ਖੰਡ;
  idx_t pad_ਬਚਿਆ = pad;
  ਲਈ (idx_t i = fields; blank < i; i--)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = *p++;
      ਅੱਖਰ ਅਚਲ *s;
      ਅੱਖਰ buf[4];

      ਚੋਣ (c)
        {
        ਹਾਲਤ '\0':
          s = "\\0";
          ਰੋਕੋ;

        ਹਾਲਤ '\a':
          s = "\\a";
          ਰੋਕੋ;

        ਹਾਲਤ '\b':
          s = "\\b";
          ਰੋਕੋ;

        ਹਾਲਤ '\f':
          s = "\\f";
          ਰੋਕੋ;

        ਹਾਲਤ '\n':
          s = "\\n";
          ਰੋਕੋ;

        ਹਾਲਤ '\r':
          s = "\\r";
          ਰੋਕੋ;

        ਹਾਲਤ '\t':
          s = "\\t";
          ਰੋਕੋ;

        ਹਾਲਤ '\v':
          s = "\\v";
          ਰੋਕੋ;

        ਮੂਲ:
          sprintf (buf, (isprint (c) ? "%c" : "%03o"), c);
          s = buf;
        }

      idx_t ਅਗਲਾ_pad = pad_at (fields, i - 1, pad);
      ਪੂਰਨ adjusted_ਚੌੜਾਈ = pad_ਬਚਿਆ - ਅਗਲਾ_pad + ਚੌੜਾਈ;
      xprintf ("%*s", adjusted_ਚੌੜਾਈ, s);
      pad_ਬਚਿਆ = ਅਗਲਾ_pad;
    }
}

/* Convert a null-terminated (possibly zero-length) string S to an
   int value.  If S points to a non-digit set *P to S,
   *VAL to 0, and return true.  Otherwise, accumulate the integer value of
   the string of digits.  If the string of digits represents a value
   larger than INT_MAX, don't modify *VAL or *P and return false.
   Otherwise, advance *P to the first non-digit after S, set *VAL to
   the result of the conversion and return true.  */

ਸਥਿਰ bool
simple_strtoi (ਅੱਖਰ ਅਚਲ *s, ਅੱਖਰ ਅਚਲ **p, ਪੂਰਨ *val)
{
  ਪੂਰਨ ਜੋੜ;

  ਲਈ (ਜੋੜ = 0; c_isdigit (*s); s++)
    ਜੇ (ckd_mul (&ਜੋੜ, ਜੋੜ, 10) || ckd_ਜੋੜੋ (&ਜੋੜ, ਜੋੜ, *s - '0'))
      ਵਾਪਸ false;
  *p = s;
  *val = ਜੋੜ;
  ਵਾਪਸ true;
}

/* Return a printf format for SPEC, given that remaining arguments are
   formats for int, long, long long, and intmax_t, respectively.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *
ispec_ਮੰਜ਼ਿਲ_format (ਗਿਣਤੀ ਮਾਪ_spec ਮਾਪ_spec,
                 ਅੱਖਰ ਅਚਲ *int_format,
                 ਅੱਖਰ ਅਚਲ *long_format,
                 ਅੱਖਰ ਅਚਲ *long_long_format,
                 ਅੱਖਰ ਅਚਲ *intmax_format)
{
  ਵਾਪਸ (LONG < INTMAX && ਮਾਪ_spec == INTMAX ? intmax_format
          : LONG < LONG_LONG && ਮਾਪ_spec == LONG_LONG ? long_long_format
          : INT < LONG && ਮਾਪ_spec == LONG ? long_format
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

ਸਥਿਰ bool ATTRIBUTE_NONNULL ()
decode_one_format (ਅੱਖਰ ਅਚਲ *s_orig, ਅੱਖਰ ਅਚਲ *s, ਅੱਖਰ ਅਚਲ **ਅਗਲਾ,
                   ਬਣਤਰ tspec *tspec)
{
  ਗਿਣਤੀ ਮਾਪ_spec ਮਾਪ_spec;
  ਪੂਰਨ ਮਾਪ;
  ਗਿਣਤੀ output_format fmt;
  print_function_type print_function;
  ਅੱਖਰ ਅਚਲ *p;
  ਅੱਖਰ c;
  ਪੂਰਨ ਖੇਤਰ_ਚੌੜਾਈ;

  ਚੋਣ (*s)
    {
    ਹਾਲਤ 'd':
    ਹਾਲਤ 'o':
    ਹਾਲਤ 'u':
    ਹਾਲਤ 'x':
      c = *s;
      ++s;
      ਚੋਣ (*s)
        {
        ਹਾਲਤ 'C':
          ++s;
          ਮਾਪ = ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ);
          ਰੋਕੋ;

        ਹਾਲਤ 'S':
          ++s;
          ਮਾਪ = ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ ਪੂਰਨ);
          ਰੋਕੋ;

        ਹਾਲਤ 'I':
          ++s;
          ਮਾਪ = ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ);
          ਰੋਕੋ;

        ਹਾਲਤ 'L':
          ++s;
          ਮਾਪ = ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ);
          ਰੋਕੋ;

        ਮੂਲ:
          ਜੇ (! simple_strtoi (s, &p, &ਮਾਪ))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              ਵਾਪਸ false;
            }
          ਜੇ (p == s)
            ਮਾਪ = ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ);
          ਨਹੀਂਤਾਂ
            {
              ਜੇ (countof (integral_type_ਮਾਪ) <= ਮਾਪ
                  || integral_type_ਮਾਪ[ਮਾਪ] == NO_ਮਾਪ)
                {
                  error (0, 0, _("invalid type string %s;\nthis system"
                                 " doesn't provide a %d-byte integral type"),
                         quote (s_orig), ਮਾਪ);
                  ਵਾਪਸ false;
                }
              s = p;
            }
          ਰੋਕੋ;
        }

      ਮਾਪ_spec = integral_type_ਮਾਪ[ਮਾਪ];

      ਚੋਣ (c)
        {
        ਹਾਲਤ 'd':
          fmt = SIGNED_DECIMAL;
          ਖੇਤਰ_ਚੌੜਾਈ = INT_ਬਿੱਟ_STRLEN_BOUND (CHAR_BIT * ਮਾਪ - 1) + 1;
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_ਮੰਜ਼ਿਲ_format (ਮਾਪ_spec, "d", "ld", "lld", "jd"));
          ਰੋਕੋ;

        ਹਾਲਤ 'o':
          fmt = OCTAL;
          ਖੇਤਰ_ਚੌੜਾਈ = (CHAR_BIT * ਮਾਪ + 2) / 3;
          sprintf (tspec->fmt_string, "%%*.%d%s", ਖੇਤਰ_ਚੌੜਾਈ,
                   ispec_ਮੰਜ਼ਿਲ_format (ਮਾਪ_spec, "o", "lo", "llo", "jo"));
          ਰੋਕੋ;

        ਹਾਲਤ 'u':
          fmt = UNSIGNED_DECIMAL;
          ਖੇਤਰ_ਚੌੜਾਈ = INT_ਬਿੱਟ_STRLEN_BOUND (CHAR_BIT * ਮਾਪ);
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_ਮੰਜ਼ਿਲ_format (ਮਾਪ_spec, "u", "lu", "llu", "ju"));
          ਰੋਕੋ;

        ਹਾਲਤ 'x':
          fmt = HEXADECIMAL;
          ਖੇਤਰ_ਚੌੜਾਈ = (CHAR_BIT * ਮਾਪ + 3) / 4;
          sprintf (tspec->fmt_string, "%%*.%d%s", ਖੇਤਰ_ਚੌੜਾਈ,
                   ispec_ਮੰਜ਼ਿਲ_format (ਮਾਪ_spec, "x", "lx", "llx", "jx"));
          ਰੋਕੋ;

        ਮੂਲ:
          affirm (false);
        }

      /* Prefer INT, prefer LONG to longer types,
         and prefer INTMAX to LONG_LONG.  */
      print_function
        = (ਮਾਪ_spec == INT
           ? print_int
           : SHORT < INT && ਮਾਪ_spec == SHORT
           ? (fmt == SIGNED_DECIMAL ? print_s_short : print_short)
           : CHAR < SHORT && ਮਾਪ_spec == CHAR
           ? (fmt == SIGNED_DECIMAL ? print_s_char : print_char)
           : INT < LONG && ਮਾਪ_spec == LONG
           ? print_long
           : LONG < INTMAX && ਮਾਪ_spec == INTMAX
           ? print_intmax
           : LONG < LONG_LONG && LONG_LONG < INTMAX && ਮਾਪ_spec == LONG_LONG
           ? print_long_long
           : (affirm (false), (print_function_type) ਨਿਰਾ));
      ਰੋਕੋ;

    ਹਾਲਤ 'f':
      fmt = FLOATING_ਬਿੰਦੂ;
      ++s;
      ਚੋਣ (*s)
        {
        ਹਾਲਤ 'B':
          ++s;
          fmt = BFLOATING_ਬਿੰਦੂ;
          ਮਾਪ = ਆਕਾਰ (bfloat16);
          ਰੋਕੋ;

        ਹਾਲਤ 'H':
          ++s;
          fmt = HFLOATING_ਬਿੰਦੂ;
          ਮਾਪ = ਆਕਾਰ (float16);
          ਰੋਕੋ;

        ਹਾਲਤ 'F':
          ++s;
          ਮਾਪ = ਆਕਾਰ (ਦਸ਼ਮਲਵ);
          ਰੋਕੋ;

        ਹਾਲਤ 'D':
          ++s;
          ਮਾਪ = ਆਕਾਰ (ਦੁੱਗਣਾ);
          ਰੋਕੋ;

        ਹਾਲਤ 'L':
          ++s;
          ਮਾਪ = ਆਕਾਰ (ਲੰਮਾ ਦੁੱਗਣਾ);
          ਰੋਕੋ;

        ਮੂਲ:
          ਜੇ (! simple_strtoi (s, &p, &ਮਾਪ))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              ਵਾਪਸ false;
            }
          ਜੇ (p == s)
            ਮਾਪ = ਆਕਾਰ (ਦੁੱਗਣਾ);
          ਨਹੀਂਤਾਂ
            {
              ਜੇ (countof (fp_type_ਮਾਪ) <= ਮਾਪ
                  || fp_type_ਮਾਪ[ਮਾਪ] == NO_ਮਾਪ
                  || (! FLOAT16_SUPPORTED && BF16_SUPPORTED
                      && ਮਾਪ == ਆਕਾਰ (bfloat16)))
                {
                  error (0, 0,
                         _("invalid type string %s;\n"
                           "this system doesn't provide a %d-byte"
                           " floating point type"),
                         quote (s_orig), ਮਾਪ);
                  ਵਾਪਸ false;
                }
              s = p;
            }
          ਰੋਕੋ;
        }
      ਮਾਪ_spec = fp_type_ਮਾਪ[ਮਾਪ];

      ਜੇ ((! FLOAT16_SUPPORTED && fmt == HFLOATING_ਬਿੰਦੂ)
          || (! BF16_SUPPORTED && fmt == BFLOATING_ਬਿੰਦੂ))
      {
        error (0, 0,
               _("this system doesn't provide a %s floating point type"),
               quote (s_orig));
        ਵਾਪਸ false;
      }

      {
        ਬਣਤਰ lconv ਅਚਲ *locale = localeconv ();
        idx_t decimal_ਬਿੰਦੂ_len =
          (locale->decimal_ਬਿੰਦੂ[0] ? ਲੜੀ_ਲੰਬਾਈ (locale->decimal_ਬਿੰਦੂ) : 1);

        ਜੇ (ਮਾਪ_spec == FLOAT_DOUBLE)
          {
            print_function = print_double;
            ਖੇਤਰ_ਚੌੜਾਈ = DBL_STRLEN_BOUND_L (decimal_ਬਿੰਦੂ_len);
          }
        ਨਹੀਂਤਾਂ ਜੇ (FLOAT_SINGLE < FLOAT_DOUBLE && ਮਾਪ_spec == FLOAT_SINGLE)
          {
            print_function = print_float;
            ਖੇਤਰ_ਚੌੜਾਈ = FLT_STRLEN_BOUND_L (decimal_ਬਿੰਦੂ_len);
          }
        ਨਹੀਂਤਾਂ ਜੇ (FLOAT_HALF < FLOAT_SINGLE && ਮਾਪ_spec == FLOAT_HALF)
          {
            print_function = fmt == BFLOATING_ਬਿੰਦੂ
                             ? print_bfloat : print_halffloat;
            ਖੇਤਰ_ਚੌੜਾਈ = FLT_STRLEN_BOUND_L (decimal_ਬਿੰਦੂ_len);
          }
        ਨਹੀਂਤਾਂ ਜੇ (FLOAT_DOUBLE < FLOAT_LONG_DOUBLE
                 && ਮਾਪ_spec == FLOAT_LONG_DOUBLE)
          {
            print_function = print_long_double;
            ਖੇਤਰ_ਚੌੜਾਈ = LDBL_STRLEN_BOUND_L (decimal_ਬਿੰਦੂ_len);
          }
        ਨਹੀਂਤਾਂ
          affirm (false);

        ਰੋਕੋ;
      }

    ਹਾਲਤ 'a':
      ++s;
      fmt = NAMED_CHARACTER;
      ਮਾਪ_spec = CHAR;
      print_function = print_named_ascii;
      ਖੇਤਰ_ਚੌੜਾਈ = 3;
      ਰੋਕੋ;

    ਹਾਲਤ 'c':
      ++s;
      fmt = CHARACTER;
      ਮਾਪ_spec = CHAR;
      print_function = print_ascii;
      ਖੇਤਰ_ਚੌੜਾਈ = 3;
      ਰੋਕੋ;

    ਮੂਲ:
      error (0, 0, _("invalid character '%c' in type string %s"),
             *s, quote (s_orig));
      ਵਾਪਸ false;
    }

  tspec->ਮਾਪ = ਮਾਪ_spec;
  tspec->fmt = fmt;
  tspec->print_function = print_function;

  tspec->ਖੇਤਰ_ਚੌੜਾਈ = ਖੇਤਰ_ਚੌੜਾਈ;
  tspec->hexl_ਬਹੁਲਕ_trailer = (*s == 'z');
  ਜੇ (tspec->hexl_ਬਹੁਲਕ_trailer)
    s++;

  *ਅਗਲਾ = s;
  ਵਾਪਸ true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM and the global string INPUT_FILENAME to the
   first one that can be successfully opened. Modify FILE_LIST to
   reference the next filename in the list.  A file name of "-" is
   interpreted as standard input.  If any file open fails, give an error
   message and return false.  */

ਸਥਿਰ bool
open_ਅਗਲਾ_file (ਖਾਲੀ)
{
  bool ok = true;

  ਕਰੋ
    {
      ਨਿਵੇਸ਼_filename = *file_list;
      ਜੇ (ਨਿਵੇਸ਼_filename == ਨਿਰਾ)
        ਵਾਪਸ ok;
      ++file_list;

      ਜੇ (streq (ਨਿਵੇਸ਼_filename, "-"))
        {
          ਨਿਵੇਸ਼_filename = _("standard input");
          ਵਿੱਚ_ਧਾਰਾ = stdin;
          have_read_stdin = true;
          xset_ਦੁਆਧਾਰੀ_ਬਹੁਲਕ (STDIN_FILENO, O_ਦੁਆਧਾਰੀ);
        }
      ਨਹੀਂਤਾਂ
        {
          ਵਿੱਚ_ਧਾਰਾ = fopen (ਨਿਵੇਸ਼_filename, (O_ਦੁਆਧਾਰੀ ? "rb" : "r"));
          ਜੇ (ਵਿੱਚ_ਧਾਰਾ == ਨਿਰਾ)
            {
              error (0, errno, "%s", quotef (ਨਿਵੇਸ਼_filename));
              ok = false;
            }
        }
    }
  ਜਦੋਂ (ਵਿੱਚ_ਧਾਰਾ == ਨਿਰਾ);

  ਜੇ (0 <= ਸਿਰਾ_ਵਿੱਥ && !flag_dump_ਲੜੀਆਂ)
    setvbuf (ਵਿੱਚ_ਧਾਰਾ, ਨਿਰਾ, _IONBF, 0);

  ਵਾਪਸ ok;
}

/* Test whether there have been errors on in_stream, and close it if
   it is not standard input.  Return false if there has been an error
   on in_stream or stdout; return true otherwise.  This function will
   report more than one error only if both a read and a write error
   have occurred.  IN_ERRNO, if nonzero, is the error number
   corresponding to the most recent action for IN_STREAM.  */

ਸਥਿਰ bool
check_and_close (ਪੂਰਨ ਵਿੱਚ_errno)
{
  bool ok = true;

  ਜੇ (ਵਿੱਚ_ਧਾਰਾ != ਨਿਰਾ)
    {
      ਜੇ (!ferror (ਵਿੱਚ_ਧਾਰਾ))
        ਵਿੱਚ_errno = 0;
      ਜੇ (streq (file_list[-1], "-"))
        clearerr (ਵਿੱਚ_ਧਾਰਾ);
      ਨਹੀਂਤਾਂ ਜੇ (fclose (ਵਿੱਚ_ਧਾਰਾ) != 0 && !ਵਿੱਚ_errno)
        ਵਿੱਚ_errno = errno;
      ਜੇ (ਵਿੱਚ_errno)
        {
          error (0, ਵਿੱਚ_errno, "%s", quotef (ਨਿਵੇਸ਼_filename));
          ok = false;
        }

      ਵਿੱਚ_ਧਾਰਾ = ਨਿਰਾ;
    }

  ਜੇ (ferror (stdout))
    {
      error (0, 0, _("write error"));
      ok = false;
    }

  ਵਾਪਸ ok;
}

/* Decode the modern od format string S.  Append the decoded
   representation to the global array SPEC, reallocating SPEC if
   necessary.  Return true if S is valid.  */

ਸਥਿਰ bool ATTRIBUTE_NONNULL ()
decode_format_string (ਅੱਖਰ ਅਚਲ *s)
{
  ਅੱਖਰ ਅਚਲ *s_orig = s;

  ਜਦੋਂ (*s != '\0')
    {
      ਅੱਖਰ ਅਚਲ *ਅਗਲਾ;

      ਜੇ (n_specs_allocated <= n_specs)
        spec = xpalloc (spec, &n_specs_allocated, 1, -1, ਆਕਾਰ *spec);

      ਜੇ (! decode_one_format (s_orig, s, &ਅਗਲਾ, &spec[n_specs]))
        ਵਾਪਸ false;

      affirm (s != ਅਗਲਾ);
      s = ਅਗਲਾ;
      ++n_specs;
    }

  ਵਾਪਸ true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM to position N_SKIP in the concatenation of
   those files.  If any file operation fails or if there are fewer than
   N_SKIP bytes in the combined input, give an error message and return
   false.  When possible, use seek rather than read operations to
   advance IN_STREAM.  */

ਸਥਿਰ bool
skip (intmax_t n_skip)
{
  bool ok = true;
  ਪੂਰਨ ਵਿੱਚ_errno = 0;

  ਜੇ (n_skip == 0)
    ਵਾਪਸ true;

  ਜਦੋਂ (ਵਿੱਚ_ਧਾਰਾ != ਨਿਰਾ)	/* EOF.  */
    {
      ਬਣਤਰ stat file_stats;

      /* First try seeking.  For large offsets, this extra work is
         worthwhile.  If the offset is below some threshold it may be
         more efficient to move the pointer by reading.  There are two
         issues when trying to seek:
           - the file must be seekable.
           - before seeking to the specified position, make sure
             that the new position is in the current file.
             Try to do that by getting file's size using fstat.
             But that will work only for regular files.  */

      ਜੇ (fstat (fileno (ਵਿੱਚ_ਧਾਰਾ), &file_stats) == 0)
        {
          /* The st_size field is valid for regular files.
             If the number of bytes left to skip is larger than
             the size of the current file, we can decrement n_skip
             and go on to the next file.  Skip this optimization also
             when st_size is no greater than the block size, because
             some kernels report nonsense small file sizes for
             proc-like file systems.  */
          ਜੇ (S_ISREG (file_stats.st_ਬਹੁਲਕ)
              && STP_BLKSIZE (&file_stats) < file_stats.st_ਮਾਪ)
            {
              ਜੇ (file_stats.st_ਮਾਪ < n_skip)
                n_skip -= file_stats.st_ਮਾਪ;
              ਨਹੀਂਤਾਂ
                {
                  ਜੇ (fseeko (ਵਿੱਚ_ਧਾਰਾ, n_skip, SEEK_CUR) != 0)
                    {
                      ਵਿੱਚ_errno = errno;
                      ok = false;
                    }
                  n_skip = 0;
                }
            }

          ਨਹੀਂਤਾਂ ਜੇ (! (S_ISREG (file_stats.st_ਬਹੁਲਕ)
                      || S_TYPEISSHM (&file_stats)
                      || S_TYPEISTMO (&file_stats))
                   && fseeko (ਵਿੱਚ_ਧਾਰਾ, n_skip, SEEK_CUR) == 0)
            n_skip = 0;

          /* If it's neither a seekable file with unusable size, nor a
             regular file so large it can't be in a proc-like file system,
             position the file pointer by reading.  */

          ਨਹੀਂਤਾਂ
            {
              ਅੱਖਰ buf[BUFSIZ];
              idx_t n_ਬਾਈਟਾਂ_read, n_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_read = BUFSIZ;

              ਜਦੋਂ (0 < n_skip)
                {
                  ਜੇ (n_skip < n_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_read)
                    n_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_read = n_skip;
                  n_ਬਾਈਟਾਂ_read = fread (buf, 1, n_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_read, ਵਿੱਚ_ਧਾਰਾ);
                  n_skip -= n_ਬਾਈਟਾਂ_read;
                  ਜੇ (n_ਬਾਈਟਾਂ_read != n_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_read)
                    {
                      ਜੇ (ferror (ਵਿੱਚ_ਧਾਰਾ))
                        {
                          ਵਿੱਚ_errno = errno;
                          ok = false;
                          n_skip = 0;
                          ਰੋਕੋ;
                        }
                      ਜੇ (feof (ਵਿੱਚ_ਧਾਰਾ))
                        ਰੋਕੋ;
                    }
                }
            }

          ਜੇ (n_skip == 0)
            ਰੋਕੋ;
        }

      ਨਹੀਂਤਾਂ   /* cannot fstat() file */
        {
          error (0, errno, "%s", quotef (ਨਿਵੇਸ਼_filename));
          ok = false;
        }

      ok &= check_and_close (ਵਿੱਚ_errno);

      ok &= open_ਅਗਲਾ_file ();
    }

  ਜੇ (n_skip != 0)
    error (EXIT_FAILURE, 0, _("cannot skip past end of combined input"));

  ਵਾਪਸ ok;
}

ਸਥਿਰ ਖਾਲੀ
format_ਪਤਾ_none (MAYBE_ਅਣਵਰਤਿਆ intmax_t ਪਤਾ,
                     MAYBE_ਅਣਵਰਤਿਆ ਅੱਖਰ c)
{
}

ਸਥਿਰ ਖਾਲੀ
format_ਪਤਾ_std (intmax_t ਪਤਾ, ਅੱਖਰ c)
{
  ਅੱਖਰ buf[MAX_ਪਤਾ_ਲੰਮਾਈ + 2];
  ਅੱਖਰ *p = buf + ਆਕਾਰ buf;
  ਅੱਖਰ ਅਚਲ *pbound;

  *--p = '\0';
  *--p = c;
  pbound = p - ਪਤਾ_pad_len;

  /* Use a special case of the code for each base.  This is measurably
     faster than generic code.  */
  ਚੋਣ (ਪਤਾ_ਅਧਾਰ)
    {
    ਹਾਲਤ 8:
      ਕਰੋ
        *--p = '0' + (ਪਤਾ & 7);
      ਜਦੋਂ ((ਪਤਾ >>= 3) != 0);
      ਰੋਕੋ;

    ਹਾਲਤ 10:
      ਕਰੋ
        *--p = '0' + (ਪਤਾ % 10);
      ਜਦੋਂ ((ਪਤਾ /= 10) != 0);
      ਰੋਕੋ;

    ਹਾਲਤ 16:
      ਕਰੋ
        *--p = "0123456789abcdef"[ਪਤਾ & 15];
      ਜਦੋਂ ((ਪਤਾ >>= 4) != 0);
      ਰੋਕੋ;
    }

  ਜਦੋਂ (pbound < p)
    *--p = '0';

  fputs (p, stdout);
}

ਸਥਿਰ ਖਾਲੀ
format_ਪਤਾ_paren (intmax_t ਪਤਾ, ਅੱਖਰ c)
{
  ਅੱਖਰ_ਲਿਖੋ ('(');
  format_ਪਤਾ_std (ਪਤਾ, ')');
  ਜੇ (c)
    ਅੱਖਰ_ਲਿਖੋ (c);
}

ਸਥਿਰ ਖਾਲੀ
format_ਪਤਾ_label (intmax_t ਪਤਾ, ਅੱਖਰ c)
{
  format_ਪਤਾ_std (ਪਤਾ, ' ');

  intmax_t addr;
  ਜੇ (ckd_ਜੋੜੋ (&addr, ਪਤਾ, pseudo_ਵਿੱਥ))
    error (EXIT_FAILURE, 0, _("pseudo address too large for input"));

  format_ਪਤਾ_paren (addr, c);
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

ਸਥਿਰ ਖਾਲੀ
write_ਖੰਡ (intmax_t current_ਵਿੱਥ, idx_t n_ਬਾਈਟਾਂ,
             ਅੱਖਰ ਅਚਲ *prev_ਖੰਡ, ਅੱਖਰ ਅਚਲ *curr_ਖੰਡ)
{
  ਸਥਿਰ bool ਪਹਿਲਾ = true;
  ਸਥਿਰ bool prev_pair_equal = false;

  ਜੇ (abbreviate_duplicate_ਖੰਡਾਂ
      && !ਪਹਿਲਾ && n_ਬਾਈਟਾਂ == ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ
      && memeq (prev_ਖੰਡ, curr_ਖੰਡ, ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ))
    {
      ਜੇ (prev_pair_equal)
        {
          /* The two preceding blocks were equal, and the current
             block is the same as the last one, so print nothing.  */
        }
      ਨਹੀਂਤਾਂ
        {
          ਲਿਖੋ ("*\n");
          prev_pair_equal = true;
        }
    }
  ਨਹੀਂਤਾਂ
    {
      prev_pair_equal = false;
      ਲਈ (idx_t i = 0; i < n_specs; i++)
        {
          ਪੂਰਨ datum_ਚੌੜਾਈ = ਚੌੜਾਈ_ਬਾਈਟਾਂ[spec[i].ਮਾਪ];
          idx_t fields_ਪ੍ਰਤੀ_ਖੰਡ = ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ / datum_ਚੌੜਾਈ;
          idx_t blank_fields = (ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ - n_ਬਾਈਟਾਂ) / datum_ਚੌੜਾਈ;
          ਜੇ (i == 0)
            format_ਪਤਾ (current_ਵਿੱਥ, '\0');
          ਨਹੀਂਤਾਂ
            print_n_spaces (ਪਤਾ_pad_len);
          (*spec[i].print_function) (fields_ਪ੍ਰਤੀ_ਖੰਡ, blank_fields,
                                     curr_ਖੰਡ, spec[i].fmt_string,
                                     spec[i].ਖੇਤਰ_ਚੌੜਾਈ, spec[i].pad_ਚੌੜਾਈ);
          ਜੇ (spec[i].hexl_ਬਹੁਲਕ_trailer)
            {
              /* Space-pad out to full line width, then dump the trailer.  */
              ਪੂਰਨ ਖੇਤਰ_ਚੌੜਾਈ = spec[i].ਖੇਤਰ_ਚੌੜਾਈ;
              ਲਈ (idx_t f = 0; f < blank_fields; f++)
                print_n_spaces (ਖੇਤਰ_ਚੌੜਾਈ);
              idx_t pad_ਚੌੜਾਈ = pad_at (fields_ਪ੍ਰਤੀ_ਖੰਡ, blank_fields,
                                        spec[i].pad_ਚੌੜਾਈ);
              print_n_spaces (pad_ਚੌੜਾਈ);
              dump_hexl_ਬਹੁਲਕ_trailer (n_ਬਾਈਟਾਂ, curr_ਖੰਡ);
            }
          ਅੱਖਰ_ਲਿਖੋ ('\n');
        }
    }
  ਪਹਿਲਾ = false;
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

ਸਥਿਰ bool
read_char (ਪੂਰਨ *c)
{
  bool ok = true;

  *c = EOF;

  ਜਦੋਂ (ਵਿੱਚ_ਧਾਰਾ && (*c = getc (ਵਿੱਚ_ਧਾਰਾ)) < 0)
    {
      ok &= check_and_close (errno);
      ok &= open_ਅਗਲਾ_file ();
    }

  ਵਾਪਸ ok;
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

ਸਥਿਰ bool
read_ਖੰਡ (idx_t n, ਅੱਖਰ *ਖੰਡ, idx_t *n_ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ)
{
  bool ok = true;

  affirm (0 < n && n <= ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ);

  *n_ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ = 0;

  ਜਦੋਂ (ਵਿੱਚ_ਧਾਰਾ != ਨਿਰਾ)	/* EOF.  */
    {
      idx_t n_needed = n - *n_ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ;
      idx_t n_read = fread (ਖੰਡ + *n_ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ,
                            1, n_needed, ਵਿੱਚ_ਧਾਰਾ);

      *n_ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ += n_read;

      ਜੇ (n_read == n_needed)
        ਰੋਕੋ;

      ok &= check_and_close (errno);

      ok &= open_ਅਗਲਾ_file ();
    }

  ਵਾਪਸ ok;
}

/* Return the least common multiple of the sizes associated
   with the format specs.  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਪੂਰਨ
get_lcm (ਖਾਲੀ)
{
  ਪੂਰਨ l_c_m = 1;

  ਲਈ (idx_t i = 0; i < n_specs; i++)
    l_c_m = lcm (l_c_m, ਚੌੜਾਈ_ਬਾਈਟਾਂ[spec[i].ਮਾਪ]);
  ਵਾਪਸ l_c_m;
}

/* Act like xstrtoimax (NPTR, NULL, BASE, VAL, VALID_SUFFIXES),
   except reject negative values, and *VAL may be set if
   LONGINT_INVALID is returned.  */
ਸਥਿਰ strtol_error
xstr2nonneg (ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ nptr, ਪੂਰਨ ਅਧਾਰ_2, intmax_t *val,
             ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ valid_suffixes)
{
  strtol_error s_err = xstrtoimax (nptr, ਨਿਰਾ, ਅਧਾਰ_2, val, valid_suffixes);
  ਵਾਪਸ s_err != LONGINT_INVALID && *val < 0 ? LONGINT_INVALID : s_err;
}

/* If STR is a valid traditional offset specification with an optional
   leading '+', and can be represented in intmax_t, return true and
   set *OFFSET to the offset it denotes.  If it is valid but cannot be
   represented, diagnose the problem and exit.  Otherwise return false
   and possibly set *OFFSET.  */

ਸਥਿਰ bool
parse_old_ਵਿੱਥ (ਅੱਖਰ *str, intmax_t *ਵਿੱਥ)
{
  /* Skip over any leading '+'.  */
  ਅੱਖਰ *s = str + (str[0] == '+');

  ਜੇ (! c_isdigit (s[0]))
    ਵਾਪਸ false;

  /* Determine the radix for S.  If there is a '.', followed first by
     optional 'b' or (undocumented) 'B' and then by end of string,
     it's decimal, otherwise, if the string begins with '0X' or '0x',
     it's hexadecimal, else octal.  */
  ਅੱਖਰ *dot = strchr (s, '.');
  ਜੇ (dot && dot[(dot[1] == 'b' || dot[1] == 'B') + 1])
    dot = ਨਿਰਾ;
  ਪੂਰਨ radix = dot ? 10 : s[0] == '0' && (s[1] == 'x' || s[1] == 'X') ? 16 : 8;

  ਜੇ (dot)
    {
      /* Temporarily remove the '.' from the decimal string.  */
      dot[0] = dot[1];
      dot[1] = '\0';
    }

  ਗਿਣਤੀ strtol_error s_err = xstr2nonneg (s, radix, ਵਿੱਥ, "bB");

  ਜੇ (dot)
    {
      /* Restore the decimal string's original value.  */
      dot[1] = dot[0];
      dot[0] = '.';
    }

  ਜੇ (s_err == LONGINT_ਉਛਾਲ)
    error (EXIT_FAILURE, ERANGE, "%s", quotef (str));
  ਵਾਪਸ s_err == LONGINT_OK;
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

ਸਥਿਰ bool
dump (ਖਾਲੀ)
{
  ਅੱਖਰ *ਖੰਡ[2];
  bool idx = false;
  bool ok = true;
  idx_t n_ਬਾਈਟਾਂ_read;

  ਖੰਡ[0] = xinmalloc (1 + abbreviate_duplicate_ਖੰਡਾਂ, ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ);
  ਖੰਡ[1] = ਖੰਡ[0] + abbreviate_duplicate_ਖੰਡਾਂ * ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ;

  intmax_t current_ਵਿੱਥ = n_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_skip;

  ਕਰੋ
    {
      intmax_t needed_bound
        = ਸਿਰਾ_ਵਿੱਥ < 0 ? INTMAX_MAX : ਸਿਰਾ_ਵਿੱਥ - current_ਵਿੱਥ;
      ਜੇ (needed_bound <= 0)
        {
          n_ਬਾਈਟਾਂ_read = 0;
          ਰੋਕੋ;
        }
      idx_t n_needed = MIN (ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ, needed_bound);
      ok &= read_ਖੰਡ (n_needed, ਖੰਡ[idx], &n_ਬਾਈਟਾਂ_read);
      ਜੇ (n_ਬਾਈਟਾਂ_read < ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ)
        ਰੋਕੋ;
      affirm (n_ਬਾਈਟਾਂ_read == ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ);
      write_ਖੰਡ (current_ਵਿੱਥ, n_ਬਾਈਟਾਂ_read,
                   ਖੰਡ[!idx], ਖੰਡ[idx]);
      ਜੇ (ferror (stdout))
        ok = false;
      current_ਵਿੱਥ += n_ਬਾਈਟਾਂ_read;
      idx = !idx;
    }
  ਜਦੋਂ (ok);

  ਜੇ (n_ਬਾਈਟਾਂ_read > 0)
    {
      ਪੂਰਨ l_c_m = get_lcm ();

      /* Ensure zero-byte padding up to the smallest multiple of l_c_m that
         is at least as large as n_bytes_read.  */
      idx_t ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_write = (n_ਬਾਈਟਾਂ_read + l_c_m - 1
                              - (n_ਬਾਈਟਾਂ_read + l_c_m - 1) % l_c_m);

      memset (ਖੰਡ[idx] + n_ਬਾਈਟਾਂ_read, 0, ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_write - n_ਬਾਈਟਾਂ_read);
      write_ਖੰਡ (current_ਵਿੱਥ, n_ਬਾਈਟਾਂ_read, ਖੰਡ[!idx], ਖੰਡ[idx]);
      current_ਵਿੱਥ += n_ਬਾਈਟਾਂ_read;
    }

  ਜੇ (ok || current_ਵਿੱਥ)
    format_ਪਤਾ (current_ਵਿੱਥ, '\n');

  ਜੇ (0 <= ਸਿਰਾ_ਵਿੱਥ && ਸਿਰਾ_ਵਿੱਥ <= current_ਵਿੱਥ)
    ok &= check_and_close (0);

  ਮੁਕਤ (ਖੰਡ[0]);

  ਵਾਪਸ ok;
}

/* STRINGS mode.  Find each "string constant" in the input.
   A string constant is a run of at least STRING_MIN
   printable characters terminated by a NUL or END_OFFSET.
   Based on a function written by Richard Stallman for a
   traditional version of od.  Return true if successful.  */

ਸਥਿਰ bool
dump_ਲੜੀਆਂ (ਖਾਲੀ)
{
  idx_t bufsize = MAX (100, string_min + 1);
  ਅੱਖਰ *buf = ximalloc (bufsize);
  intmax_t ਪਤਾ = n_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_skip;
  bool ok = true;

  ਜਦੋਂ (true)
    {
      idx_t i = 0;
      ਪੂਰਨ c = 1;  /* Init to 1 so can distinguish if NUL read.  */

      ਜੇ (0 <= ਸਿਰਾ_ਵਿੱਥ
          && (ਸਿਰਾ_ਵਿੱਥ < string_min || ਸਿਰਾ_ਵਿੱਥ - string_min < ਪਤਾ))
        ਰੋਕੋ;

      /* Store consecutive printable characters to BUF.  */
      ਜਦੋਂ (! (0 <= ਸਿਰਾ_ਵਿੱਥ && ਸਿਰਾ_ਵਿੱਥ <= ਪਤਾ))
        {
          ਜੇ (i == bufsize - 1)
            buf = xpalloc (buf, &bufsize, 1, -1, ਆਕਾਰ *buf);
          ok &= read_char (&c);
          ਜੇ (c < 0)
            {
              ਮੁਕਤ (buf);
              ਵਾਪਸ ok;
            }
          ਪਤਾ++;
          buf[i++] = c;
          ਜੇ (c == '\0')
            ਰੋਕੋ;		/* Print this string.  */
          ਜੇ (! isprint (c))
            {
              c = -1;		/* Give up on this string.  */
              ਰੋਕੋ;
            }
        }

      ਜੇ (c < 0 || i - !c < string_min)
        ਜਾਰੀ;

      buf[i] = 0;

      /* If we get here, the string is all printable, so print it.  */

      format_ਪਤਾ (ਪਤਾ - i, ' ');

      ਲਈ (i = 0; (c = buf[i]); i++)
        {
          ਚੋਣ (c)
            {
            ਹਾਲਤ '\a':
              fputs ("\\a", stdout);
              ਰੋਕੋ;

            ਹਾਲਤ '\b':
              fputs ("\\b", stdout);
              ਰੋਕੋ;

            ਹਾਲਤ '\f':
              fputs ("\\f", stdout);
              ਰੋਕੋ;

            ਹਾਲਤ '\n':
              fputs ("\\n", stdout);
              ਰੋਕੋ;

            ਹਾਲਤ '\r':
              fputs ("\\r", stdout);
              ਰੋਕੋ;

            ਹਾਲਤ '\t':
              fputs ("\\t", stdout);
              ਰੋਕੋ;

            ਹਾਲਤ '\v':
              fputs ("\\v", stdout);
              ਰੋਕੋ;

            ਮੂਲ:
              putc (c, stdout);
            }
        }
      ਅੱਖਰ_ਲਿਖੋ ('\n');
    }

  /* We reach this point only if we search through
     (max_bytes_to_format - string_min) bytes before reaching EOF.  */

  ਮੁਕਤ (buf);

  ok &= check_and_close (0);
  ਵਾਪਸ ok;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ n_files;
  ਪੂਰਨ l_c_m;
  idx_t desired_ਚੌੜਾਈ = 0;
  bool modern = false;
  bool ok = true;
  idx_t ਚੌੜਾਈ_ਪ੍ਰਤੀ_ਖੰਡ = 0;
  ਸਥਿਰ ਅੱਖਰ ਅਚਲ multipliers[] = "bEGKkMmPQRTYZ0";

  /* The maximum number of bytes that will be formatted.
     If negative, there is no limit.  */
  intmax_t max_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_format = -1;

  /* The old-style 'pseudo starting address' to be printed in parentheses
     after any true address.  */
  intmax_t pseudo_ਸ਼ੁਰੂ IF_LINT ( = 0);

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ (true)
    {
      intmax_t tmp;
      ਗਿਣਤੀ strtol_error s_err;
      ਪੂਰਨ oi = -1;
      ਪੂਰਨ c = getopt_long (argc, argv, short_options, long_options, &oi);
      ਜੇ (c == -1)
        ਰੋਕੋ;

      ਚੋਣ (c)
        {
        ਹਾਲਤ 'A':
          modern = true;
          ਚੋਣ (optarg[0])
            {
            ਹਾਲਤ 'd':
              format_ਪਤਾ = format_ਪਤਾ_std;
              ਪਤਾ_ਅਧਾਰ = 10;
              ਪਤਾ_pad_len = 7;
              ਰੋਕੋ;
            ਹਾਲਤ 'o':
              format_ਪਤਾ = format_ਪਤਾ_std;
              ਪਤਾ_ਅਧਾਰ = 8;
              ਪਤਾ_pad_len = 7;
              ਰੋਕੋ;
            ਹਾਲਤ 'x':
              format_ਪਤਾ = format_ਪਤਾ_std;
              ਪਤਾ_ਅਧਾਰ = 16;
              ਪਤਾ_pad_len = 6;
              ਰੋਕੋ;
            ਹਾਲਤ 'n':
              format_ਪਤਾ = format_ਪਤਾ_none;
              ਪਤਾ_pad_len = 0;
              ਰੋਕੋ;
            ਮੂਲ:
              error (EXIT_FAILURE, 0,
                     _("invalid output address radix '%c';"
                       " it must be one character from [doxn]"),
                     optarg[0]);
              ਰੋਕੋ;
            }
          ਰੋਕੋ;

        ਹਾਲਤ 'j':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &n_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_skip, multipliers);
          ਜੇ (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          ਰੋਕੋ;

        ਹਾਲਤ 'N':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &max_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_format, multipliers);
          ਜੇ (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          ਰੋਕੋ;

        ਹਾਲਤ 'S':
          modern = true;
          ਜੇ (optarg == ਨਿਰਾ)
            string_min = 3;
          ਨਹੀਂਤਾਂ
            {
              s_err = xstr2nonneg (optarg, 0, &tmp, multipliers);
              /* The minimum string length + 1 must fit in idx_t,
                 since we may allocate a buffer of this size + 1.  */
              idx_t i;
              ਜੇ (s_err == LONGINT_OK && ckd_ਜੋੜੋ (&i, tmp, 1))
                s_err = LONGINT_ਉਛਾਲ;
              ਜੇ (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
              string_min = tmp;
            }
          flag_dump_ਲੜੀਆਂ = true;
          ਰੋਕੋ;

        ਹਾਲਤ 't':
          modern = true;
          ok &= decode_format_string (optarg);
          ਰੋਕੋ;

        ਹਾਲਤ 'v':
          modern = true;
          abbreviate_duplicate_ਖੰਡਾਂ = false;
          ਰੋਕੋ;

        ਹਾਲਤ TRADITIONAL_OPTION:
          traditional = true;
          ਰੋਕੋ;

        ਹਾਲਤ ENDIAN_OPTION:
          ਚੋਣ (XARGMATCH ("--endian", optarg, endian_ਦਲੀਲਾਂ, endian_types))
            {
            ਹਾਲਤ endian_big:
              ਨਿਵੇਸ਼_ਅਦਲਾ_ਬਦਲੀ = BYTE_ORDER != BIG_ENDIAN;
              ਰੋਕੋ;
            ਹਾਲਤ endian_little:
              ਨਿਵੇਸ਼_ਅਦਲਾ_ਬਦਲੀ = BYTE_ORDER != LITTLE_ENDIAN;
              ਰੋਕੋ;
            }
          ਰੋਕੋ;

          /* The next several cases map the traditional format
             specification options to the corresponding modern format
             specs.  GNU od accepts any combination of old- and
             new-style options.  Format specification options accumulate.
             The obsolescent and undocumented formats are compatible
             with FreeBSD 4.10 od.  */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CASE_OLD_ARG(old_char,new_string)		\
        ਹਾਲਤ old_char:					\
          ok &= decode_format_string (new_string);	\
          ਰੋਕੋ

          CASE_OLD_ARG ('a', "a");
          CASE_OLD_ARG ('b', "o1");
          CASE_OLD_ARG ('c', "c");
          CASE_OLD_ARG ('D', "u4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('d', "u2");
        ਹਾਲਤ 'F': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('e', "fD"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('f', "fF");
        ਹਾਲਤ 'X': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('H', "x4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('i', "dI");
        ਹਾਲਤ 'I': ਹਾਲਤ 'L': /* obsolescent and undocumented aliases */
          CASE_OLD_ARG ('l', "dL");
          CASE_OLD_ARG ('O', "o4"); /* obsolescent and undocumented */
        ਹਾਲਤ 'B': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('o', "o2");
          CASE_OLD_ARG ('s', "d2");
        ਹਾਲਤ 'h': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('x', "x2");

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CASE_OLD_ARG

        ਹਾਲਤ 'w':
          modern = true;
          ਜੇ (optarg == ਨਿਰਾ)
            {
              desired_ਚੌੜਾਈ = 32;
            }
          ਨਹੀਂਤਾਂ
            {
              intmax_t w_tmp;
              s_err = xstr2nonneg (optarg, 10, &w_tmp, "");
              ਜੇ (s_err == LONGINT_OK)
                {
                  ਜੇ (ckd_ਜੋੜੋ (&desired_ਚੌੜਾਈ, w_tmp, 0))
                    s_err = LONGINT_ਉਛਾਲ;
                  ਨਹੀਂਤਾਂ ਜੇ (desired_ਚੌੜਾਈ == 0)
                    s_err = LONGINT_INVALID;
                }
              ਜੇ (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
            }
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
          ਰੋਕੋ;
        }
    }

  ਜੇ (!ok)
    ਵਾਪਸ EXIT_FAILURE;

  ਜੇ (flag_dump_ਲੜੀਆਂ && n_specs > 0)
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

  ਜੇ (!modern || traditional)
    {
      intmax_t o1;
      intmax_t o2;

      ਚੋਣ (n_files)
        {
        ਹਾਲਤ 1:
          ਜੇ ((traditional || argv[optind][0] == '+')
              && parse_old_ਵਿੱਥ (argv[optind], &o1))
            {
              n_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_skip = o1;
              --n_files;
              ++argv;
            }
          ਰੋਕੋ;

        ਹਾਲਤ 2:
          ਜੇ ((traditional || argv[optind + 1][0] == '+'
               || c_isdigit (argv[optind + 1][0]))
              && parse_old_ਵਿੱਥ (argv[optind + 1], &o2))
            {
              ਜੇ (traditional && parse_old_ਵਿੱਥ (argv[optind], &o1))
                {
                  n_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_skip = o1;
                  flag_pseudo_ਸ਼ੁਰੂ = true;
                  pseudo_ਸ਼ੁਰੂ = o2;
                  argv += 2;
                  n_files -= 2;
                }
              ਨਹੀਂਤਾਂ
                {
                  n_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_skip = o2;
                  --n_files;
                  argv[optind + 1] = argv[optind];
                  ++argv;
                }
            }
          ਰੋਕੋ;

        ਹਾਲਤ 3:
          ਜੇ (traditional
              && parse_old_ਵਿੱਥ (argv[optind + 1], &o1)
              && parse_old_ਵਿੱਥ (argv[optind + 2], &o2))
            {
              n_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_skip = o1;
              flag_pseudo_ਸ਼ੁਰੂ = true;
              pseudo_ਸ਼ੁਰੂ = o2;
              argv[optind + 2] = argv[optind];
              argv += 2;
              n_files -= 2;
            }
          ਰੋਕੋ;
        }

      ਜੇ (traditional && 1 < n_files)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
          error (0, 0, "%s",
                 _("compatibility mode supports at most one file"));
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (flag_pseudo_ਸ਼ੁਰੂ)
    {
      ਜੇ (format_ਪਤਾ == format_ਪਤਾ_none)
        {
          ਪਤਾ_ਅਧਾਰ = 8;
          ਪਤਾ_pad_len = 7;
          format_ਪਤਾ = format_ਪਤਾ_paren;
        }
      ਨਹੀਂਤਾਂ
        format_ਪਤਾ = format_ਪਤਾ_label;
    }

  ਜੇ (0 <= max_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_format
      && ckd_ਜੋੜੋ (&ਸਿਰਾ_ਵਿੱਥ, n_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_skip, max_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_format))
    error (EXIT_FAILURE, 0, _("skip-bytes + read-bytes is too large"));

  ਜੇ (n_specs == 0)
    decode_format_string ("oS");

  ਜੇ (n_files > 0)
    {
      /* Set the global pointer FILE_LIST so that it
         references the first file-argument on the command-line.  */

      file_list = (ਅੱਖਰ ਅਚਲ *ਅਚਲ *) &argv[optind];
    }
  ਨਹੀਂਤਾਂ
    {
      /* No files were listed on the command line.
         Set the global pointer FILE_LIST so that it
         references the null-terminated list of one name: "-".  */

      file_list = default_file_list;
    }

  /* open the first input file */
  ok = open_ਅਗਲਾ_file ();
  ਜੇ (ਵਿੱਚ_ਧਾਰਾ == ਨਿਰਾ)
    ਜਾਓ cleanup;

  /* skip over any unwanted header bytes */
  ok &= skip (n_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_skip);
  ਜੇ (ਵਿੱਚ_ਧਾਰਾ == ਨਿਰਾ)
    ਜਾਓ cleanup;

  pseudo_ਵਿੱਥ = (flag_pseudo_ਸ਼ੁਰੂ ? pseudo_ਸ਼ੁਰੂ - n_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_skip : 0);

  /* Compute output block length.  */
  l_c_m = get_lcm ();

  ਜੇ (desired_ਚੌੜਾਈ != 0)
    {
      ਜੇ (desired_ਚੌੜਾਈ % l_c_m == 0)
        ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ = desired_ਚੌੜਾਈ;
      ਨਹੀਂਤਾਂ
        {
          error (0, 0, _("warning: invalid width %td; using %d instead"),
                 desired_ਚੌੜਾਈ, l_c_m);
          ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ = l_c_m;
        }
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (l_c_m < DEFAULT_ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ)
        ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ = (DEFAULT_ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ
                           - DEFAULT_ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ % l_c_m);
      ਨਹੀਂਤਾਂ
        ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ = l_c_m;
    }

  /* Compute padding necessary to align output block.  */
  ਲਈ (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_ਪ੍ਰਤੀ_ਖੰਡ = ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ / ਚੌੜਾਈ_ਬਾਈਟਾਂ[spec[i].ਮਾਪ];
      ਜੇ (pad_at_ਉਛਾਲ (fields_ਪ੍ਰਤੀ_ਖੰਡ))
        error (EXIT_FAILURE, 0, _("%td is too large"), desired_ਚੌੜਾਈ);
      idx_t ਖੰਡ_ਚੌੜਾਈ = (spec[i].ਖੇਤਰ_ਚੌੜਾਈ + 1) * fields_ਪ੍ਰਤੀ_ਖੰਡ;
      ਜੇ (ਚੌੜਾਈ_ਪ੍ਰਤੀ_ਖੰਡ < ਖੰਡ_ਚੌੜਾਈ)
        ਚੌੜਾਈ_ਪ੍ਰਤੀ_ਖੰਡ = ਖੰਡ_ਚੌੜਾਈ;
    }
  ਲਈ (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_ਪ੍ਰਤੀ_ਖੰਡ = ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ / ਚੌੜਾਈ_ਬਾਈਟਾਂ[spec[i].ਮਾਪ];
      idx_t ਖੰਡ_ਚੌੜਾਈ = spec[i].ਖੇਤਰ_ਚੌੜਾਈ * fields_ਪ੍ਰਤੀ_ਖੰਡ;
      spec[i].pad_ਚੌੜਾਈ = ਚੌੜਾਈ_ਪ੍ਰਤੀ_ਖੰਡ - ਖੰਡ_ਚੌੜਾਈ;
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
  ਲਿਖੋ ("lcm=%d, width_per_block=%td\n", l_c_m, ਚੌੜਾਈ_ਪ੍ਰਤੀ_ਖੰਡ);
  ਲਈ (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_ਪ੍ਰਤੀ_ਖੰਡ = ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ / ਚੌੜਾਈ_ਬਾਈਟਾਂ[spec[i].ਮਾਪ];
      affirm (ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਖੰਡ % ਚੌੜਾਈ_ਬਾਈਟਾਂ[spec[i].ਮਾਪ] == 0);
      affirm (1 <= spec[i].pad_ਚੌੜਾਈ / fields_ਪ੍ਰਤੀ_ਖੰਡ);
      ਲਿਖੋ ("%td: fmt=\"%s\" in_width=%d out_width=%d pad=%td\n",
              i, spec[i].fmt_string, ਚੌੜਾਈ_ਬਾਈਟਾਂ[spec[i].ਮਾਪ],
              spec[i].ਖੇਤਰ_ਚੌੜਾਈ, spec[i].pad_ਚੌੜਾਈ);
    }
#ਜੇ_ਅੰਤ

  ok &= (flag_dump_ਲੜੀਆਂ ? dump_ਲੜੀਆਂ () : dump ());

cleanup:

  ਜੇ (have_read_stdin && fclose (stdin) < 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
