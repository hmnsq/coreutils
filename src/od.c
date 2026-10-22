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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <endian.h>
#ଅନ୍ତର୍ଭାଵ <float.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "argmatch.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "c-ctype.h"
#ଅନ୍ତର୍ଭାଵ "ftoastr.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "stat-size.h"
#ଅନ୍ତର୍ଭାଵ "xbinary-io.h"
#ଅନ୍ତର୍ଭାଵ "xprintf.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "od"

#define AUTHORS proper_name ("Jim Meyering")

/* The default number of input bytes per output line.  */
#define DEFAULT_BYTES_PER_BLOCK 16

#ଯଦି FLOAT16_SUPPORTED
  /* Available since clang 6 (2018), and gcc 7 (2017).  */
  ପ୍ରକାରବ୍ୟାଖ୍ୟା _Float16 float16;
#ଅନ୍ୟଥା
# define FLOAT16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ଦଶମିକ float16;
#endif

#ଯଦି BF16_SUPPORTED
  /* Available since clang 11 (2020), and gcc 13 (2023). */
  ପ୍ରକାରବ୍ୟାଖ୍ୟା __bf16 bfloat16;
#ଅନ୍ୟଥା
# define BF16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ଦଶମିକ bfloat16;
#endif

ଗଣନା size_spec
  {
    NO_SIZE = 0,
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
    N_SIZE_SPECS
  };

ଗଣନା output_format
  {
    SIGNED_DECIMAL,
    UNSIGNED_DECIMAL,
    OCTAL,
    HEXADECIMAL,
    FLOATING_POINT,
    HFLOATING_POINT,
    BFLOATING_POINT,
    NAMED_CHARACTER,
    CHARACTER
  };

ଗଣନା { MAX_INTEGRAL_TYPE_WIDTH = UINTMAX_WIDTH };

/* The maximum number of bytes needed for a format string, including
   the trailing nul.  Each format string expects a variable amount of
   padding (guaranteed to be at least 1 plus the field width), then an
   element that will be formatted in the field.  */
ଗଣନା
  {
    FMT_BYTES_ALLOCATED =
      ଆକାର "%*.%dlld" - ଆକାର "%d" + INT_STRLEN_BOUND (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) + 1
  };

/* Ensure that our choice for FMT_BYTES_ALLOCATED is reasonable.  */
static_assert (MAX_INTEGRAL_TYPE_WIDTH <= 3 * 99);

/* The type of a print function.  FIELDS is the number of fields per
   line, BLANK is the number of fields to leave blank.  WIDTH is width
   of one field, excluding leading space, and PAD is total pad to
   divide among FIELDS.  PAD is at least as large as FIELDS.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*print_function_type)
  (idx_t fields, idx_t blank, ଶୂନ୍ୟ ଅଚଳ *data,
   ଅକ୍ଷର ଅଚଳ *fmt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, idx_t pad);

/* Each output format specification (from '-t spec' or from
   old-style options) is represented by one of these structures.  */
ସଂରଚନା tspec
  {
    ଗଣନା output_format fmt;
    ଗଣନା size_spec size; /* Type of input object.  */
    print_function_type print_function;
    ଅକ୍ଷର fmt_string[FMT_BYTES_ALLOCATED]; /* Of the style "%*d".  */
    bool hexl_mode_trailer;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା field_width; /* Minimum width of a field, excluding leading space.  */
    idx_t pad_width; /* Total padding to be divided among fields.  */
  };

/* Convert enum size_spec to the size of the named type.  */
ସ୍ଥିର ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା width_bytes[] =
{
  -1,
  ଆକାର (unsigned ଅକ୍ଷର),
#ଯଦି UCHAR_MAX < USHRT_MAX
  ଆକାର (unsigned short ପୂର୍ଣ୍ଣସଂଖ୍ୟା),
#endif
#ଯଦି USHRT_MAX < UINT_MAX
  ଆକାର (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା),
#endif
#ଯଦି UINT_MAX < ULONG_MAX
  ଆକାର (unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା),
#endif
#ଯଦି ULONG_MAX < ULLONG_MAX
  ଆକାର (unsigned long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା),
#endif
#ଯଦି ULLONG_MAX < UINTMAX_MAX
  ଆକାର (uintmax_t),
#endif
#ଯଦି BF16_SUPPORTED
  ଆକାର (bfloat16),
#ନଚେତଯଦି FLOAT16_SUPPORTED
  ଆକାର (float16),
#endif
#ଯଦି FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
  ଆକାର (ଦଶମିକ),
#endif
  ଆକାର (ଦ୍ୱିଗୁଣ),
#ଯଦି DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
  ଆକାର (long ଦ୍ୱିଗୁଣ),
#endif
};

/* Ensure that for each member of 'enum size_spec' there is an
   initializer in the width_bytes array.  */
static_assert (countof (width_bytes) == N_SIZE_SPECS);

/* Names for some non-printing characters.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ charname[33][4] =
{
  "nul", "soh", "stx", "etx", "eot", "enq", "ack", "bel",
  "bs", "ht", "nl", "vt", "ff", "cr", "so", "si",
  "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb",
  "can", "em", "sub", "esc", "fs", "gs", "rs", "us",
  "sp"
};

/* Address base (8, 10 or 16).  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା address_base = 8;

/* The number of octal digits required to represent the largest
   address value.  */
ଗଣନା { MAX_ADDRESS_LENGTH = ((INTMAX_WIDTH - 1) / 3
                             + ((INTMAX_WIDTH - 1) % 3 != 0)) };

/* Width of a normal address.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା address_pad_len = 7;

/* Minimum length when detecting --strings.  */
ସ୍ଥିର idx_t string_min;

/* True when in --strings mode.  */
ସ୍ଥିର bool flag_dump_strings;

/* True if we should recognize the older non-option arguments
   that specified at most one file and optional arguments specifying
   offset and pseudo-start address.  */
ସ୍ଥିର bool traditional;

/* True if an old-style 'pseudo-address' was specified.  */
ସ୍ଥିର bool flag_pseudo_start;

/* The difference between the old-style pseudo starting address and
   the number of bytes to skip.  */
ସ୍ଥିର intmax_t pseudo_offset;

/* Function that accepts an address and an optional following char,
   and prints the address and char to stdout.  */
ସ୍ଥିର ଶୂନ୍ୟ format_address_std (intmax_t, ଅକ୍ଷର);
ସ୍ଥିର ଶୂନ୍ୟ (*format_address) (intmax_t, ଅକ୍ଷର) = format_address_std;

/* The number of input bytes to skip before formatting and writing.  */
ସ୍ଥିର intmax_t n_bytes_to_skip;

/* The offset of the first byte after the last byte to be formatted.
   If negative, there is no limit.  */
ସ୍ଥିର intmax_t end_offset = -1;

/* When true and two or more consecutive blocks are equal, format
   only the first block and output an asterisk alone on the following
   line to indicate that identical blocks have been elided.  */
ସ୍ଥିର bool abbreviate_duplicate_blocks = true;

/* An array of specs describing how to format each input block.  */
ସ୍ଥିର ସଂରଚନା tspec *spec;

/* The number of format specs.  */
ସ୍ଥିର idx_t n_specs;

/* The allocated length of SPEC.  */
ସ୍ଥିର idx_t n_specs_allocated;

/* The number of input bytes formatted per output line.  It must be
   a multiple of the least common multiple of the sizes associated with
   the specified output types.  It should be as large as possible, but
   no larger than 16 -- unless specified with the -w option.  */
ସ୍ଥିର idx_t bytes_per_block;

/* Human-readable representation of *file_list (for error messages).
   It differs from file_list[-1] only when file_list[-1] is "-".  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ନିବେଶ_filename;

/* A null-terminated list of the file-arguments from the command line.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ *file_list;

/* Initializer for file_list if no file-arguments
   were specified on the command line.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ default_file_list[] = {"-", ଖାଲି};

/* The input stream associated with the current file.  */
ସ୍ଥିର FILE *in_stream;

/* If true, at least one of the files we read was standard input.  */
ସ୍ଥିର bool have_read_stdin;

/* Map the size in bytes to a type identifier.
   When two types have the same machine layout:
     - Prefer unsigned int to other types, as its format is shorter.
     - Prefer unsigned long to higher-ranked types, as it is older.
     - Prefer uintmax_t to unsigned long long int; this wins if %lld
       does not work but %jd does (e.g., MS-Windows).  */
ସ୍ଥିର ଗଣନା size_spec ଅଚଳ integral_type_size[] =
  {
#ଯଦି UCHAR_MAX < USHRT_MAX
    [ଆକାର (unsigned ଅକ୍ଷର)] = CHAR,
#endif
#ଯଦି USHRT_MAX < UINT_MAX
    [ଆକାର (unsigned short ପୂର୍ଣ୍ଣସଂଖ୍ୟା)] = SHORT,
#endif
    [ଆକାର (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)] = INT,
#ଯଦି UINT_MAX < ULONG_MAX
    [ଆକାର (unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)] = LONG,
#endif
#ଯଦି ULONG_MAX < ULLONG_MAX && ULLONG_MAX < UINTMAX_MAX
    [ଆକାର (unsigned long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)] = LONG_LONG,
#endif
#ଯଦି ULONG_MAX < UINTMAX_MAX
    [ଆକାର (uintmax_t)] = INTMAX,
#endif
  };

/* Map the size in bytes to a floating type identifier.
   When two types have the same machine layout:
     - Prefer double to the other types, as its format is shorter.  */
ସ୍ଥିର ଗଣନା size_spec ଅଚଳ fp_type_size[] =
  {
#ଯଦି FLOAT16_SUPPORTED
    [ଆକାର (float16)] = FLOAT_HALF,
#ନଚେତଯଦି BF16_SUPPORTED
    [ଆକାର (bfloat16)] = FLOAT_HALF,
#endif
#ଯଦି FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
    [ଆକାର (ଦଶମିକ)] = FLOAT_SINGLE,
#endif
    [ଆକାର (ଦ୍ୱିଗୁଣ)] = FLOAT_DOUBLE,
#ଯଦି DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
    [ଆକାର (long ଦ୍ୱିଗୁଣ)] = FLOAT_LONG_DOUBLE,
#endif
  };

/* Use native endianness by default.  */
ସ୍ଥିର bool ନିବେଶ_swap;

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ short_options[] = "A:aBbcDdeFfHhIij:LlN:OoS:st:vw::Xx";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  TRADITIONAL_OPTION = CHAR_MAX + 1,
  ENDIAN_OPTION,
};

ଗଣନା endian_type
{
  endian_little,
  endian_big
};

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ endian_args[] =
{
  "little", "big", ଖାଲି
};

ସ୍ଥିର ଗଣନା endian_type ଅଚଳ endian_types[] =
{
  endian_little, endian_big
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {"skip-bytes", required_argument, ଖାଲି, 'j'},
  {"address-radix", required_argument, ଖାଲି, 'A'},
  {"read-bytes", required_argument, ଖାଲି, 'N'},
  {"format", required_argument, ଖାଲି, 't'},
  {"output-duplicates", no_argument, ଖାଲି, 'v'},
  {"strings", optional_argument, ଖାଲି, 'S'},
  {"traditional", no_argument, ଖାଲି, TRADITIONAL_OPTION},
  {"width", optional_argument, ଖାଲି, 'w'},
  {"endian", required_argument, ଖାଲି, ENDIAN_OPTION },

  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [-abcdfilosx]... [FILE] [[+]OFFSET[.][b]]\n\
  or:  %s --traditional [OPTION]... [FILE] [[+]OFFSET[.][b] [+][LABEL][.][b]]\n\
"),
              program_name, program_name, program_name);
      fputs (_("\n\
Write an unambiguous representation, octal bytes by ପୂର୍ବନିର୍ଦ୍ଧାରିତ,\n\
of FILE to standard output.  With more than one FILE argument,\n\
concatenate them in the listed order to form the ନିବେଶ.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
If first and second call formats both apply, the second format is assumed\n\
ଯଦି the last operand begins with + or (ଯଦି there are 2 operands) a digit.\n\
An OFFSET operand means -j OFFSET.  LABEL is the pseudo-address\n\
at first byte printed, incremented when dump is progressing.\n\
For OFFSET and LABEL, a 0x or 0X prefix indicates hexadecimal;\n\
suffixes may be . ପାଇଁ octal and b ପାଇଁ multiply by 512.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --address-radix=RADIX\n\
         output format ପାଇଁ file offsets;\n\
         RADIX is one of [doxn], ପାଇଁ Decimal, Octal, Hex or None\n\
"));
      oputs (_("\
      --endian={big|little}\n\
         swap ନିବେଶ bytes according the specified order\n\
"));
      oputs (_("\
  -j, --skip-bytes=BYTES\n\
         skip BYTES ନିବେଶ bytes first\n\
"));
      oputs (_("\
  -N, --ପଠ-bytes=BYTES\n\
         limit dump to BYTES ନିବେଶ bytes\n\
"));
      oputs (_("\
  -S BYTES, --strings[=BYTES]\n\
         show only NUL terminated strings\n\
         of at least BYTES (ପୂର୍ବନିର୍ଦ୍ଧାରିତ 3) printable characters\n\
"));
      oputs (_("\
  -t, --format=TYPE\n\
         select output format or formats\n\
"));
      oputs (_("\
  -v, --output-duplicates\n\
         କର not use * to mark ଧାଡ଼ି suppression\n\
"));
      oputs (_("\
  -w[BYTES], --width[=BYTES]\n\
         output BYTES bytes per output ଧାଡ଼ି;\n\
         32 is implied when BYTES is not specified\n\
"));
      oputs (_("\
      --traditional\n\
         accept arguments in third form above\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
\n\
Traditional format specifications may be intermixed; they accumulate:\n\
"), stdout);
      oputs (_("\
  -a   same as -t a,  select named characters, ignoring ଉପର-order bit\n\
"));
      oputs (_("\
  -b   same as -t o1, select octal bytes\n\
"));
      oputs (_("\
  -c   same as -t c,  select printable characters or backslash escapes\n\
"));
      oputs (_("\
  -d   same as -t u2, select unsigned decimal 2-byte units\n\
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
TYPE is made up of one or more of these specifications:\n\
  a          named character, ignoring ଉପର-order bit\n\
  c          printable character or backslash escape\n\
"), stdout);
      fputs (_("\
  d[SIZE]    signed decimal, SIZE bytes per integer\n\
  f[SIZE]    floating point, SIZE bytes per ଦଶମିକ\n\
  o[SIZE]    octal, SIZE bytes per integer\n\
  u[SIZE]    unsigned decimal, SIZE bytes per integer\n\
  x[SIZE]    hexadecimal, SIZE bytes per integer\n\
"), stdout);
      fputs (_("\
\n\
SIZE is a ସଂଖ୍ୟା.  For TYPE in [doux], SIZE may also be C ପାଇଁ\n\
ଆକାର(ଅକ୍ଷର), S ପାଇଁ ଆକାର(short), I ପାଇଁ ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା) or L ପାଇଁ\n\
ଆକାର(long).  If TYPE is f, SIZE may also be B ପାଇଁ Brain 16 bit,\n\
H ପାଇଁ Half precision ଦଶମିକ, F ପାଇଁ ଆକାର(ଦଶମିକ), D ପାଇଁ ଆକାର(ଦ୍ୱିଗୁଣ),\n\
or L ପାଇଁ ଆକାର(long ଦ୍ୱିଗୁଣ).\n\
"), stdout);
      fputs (_("\
\n\
Adding a z suffix to any type displays printable characters at the end of\n\
each output ଧାଡ଼ି.\n\
"), stdout);
      fputs (_("\
\n\
\n\
BYTES is hex with 0x or 0X prefix, and may have a multiplier suffix:\n\
  b    512\n\
  KB   1000\n\
  K    1024\n\
  MB   1000*1000\n\
  M    1024*1024\n\
and so on ପାଇଁ G, T, P, E, Z, Y, R, Q.\n\
ଦ୍ୱିମିକ_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Define the print functions.  */

/* Print N spaces, where 0 <= N.
   Do not rely on printf ("%*s", N, "") as N may exceed INT_MAX.  */
ସ୍ଥିର ଶୂନ୍ୟ
print_n_spaces (intmax_t n)
{
  ପାଇଁ (; 0 < n; n--)
    putchar (' ');
}

/* If there are FIELDS fields, return the total padding up to the
   start of field I, where I < FIELDS.  PAD is the total padding for
   all fields.  The result equals (PAD * I) / FIELDS, except it does
   not suffer from internal overflow.  */
ସ୍ଥିର idx_t
pad_at (idx_t fields, idx_t i, idx_t pad)
{
  /* This implementation assumes that (FIELDS - 1)^2 does not overflow
     intmax_t, an assumption checked by pad_at_overflow.  */
  intmax_t m = pad % fields;
  ଫେରନ୍ତୁ pad / fields * i + m * i / fields;
}
ସ୍ଥିର bool
pad_at_overflow (idx_t fields)
{
  intmax_t product;
  ଫେରନ୍ତୁ ckd_mul (&product, fields - 1, fields - 1);
}

#define PRINT_FIELDS(N, T, FMT_STRING_DECL, ACTION)                     \
ସ୍ଥିର ଶୂନ୍ୟ                                                             \
N (idx_t fields, idx_t blank, ଶୂନ୍ୟ ଅଚଳ *block,			\
   FMT_STRING_DECL, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, idx_t pad)				\
{                                                                       \
  T ଅଚଳ *p = block;                                                   \
  idx_t pad_remaining = pad;						\
  ପାଇଁ (idx_t i = fields; blank < i; i--)				\
    {                                                                   \
      idx_t next_pad = pad_at (fields, i - 1, pad);			\
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା adjusted_width = pad_remaining - next_pad + width;            \
      T x;                                                              \
      ଯଦି (ନିବେଶ_swap && ଆକାର (T) > 1)                                 \
        {                                                               \
          ସଂଘ {                                                       \
            T x;                                                        \
            ଅକ୍ଷର b[ଆକାର (T)];                                         \
          } u;                                                          \
          ପାଇଁ (idx_t j = 0; j < ଆକାର (T); j++)			\
            u.b[j] = ((ଅକ୍ଷର ଅଚଳ *) p)[ଆକାର (T) - 1 - j];            \
          x = u.x;                                                      \
        }                                                               \
      ଅନ୍ୟଥା                                                              \
        x = *p;                                                         \
      p++;                                                              \
      ACTION;                                                           \
      pad_remaining = next_pad;                                         \
    }                                                                   \
}

#define PRINT_TYPE(N, T)                                                \
  PRINT_FIELDS (N, T, ଅକ୍ଷର ଅଚଳ *fmt_string,                           \
                xprintf (fmt_string, adjusted_width, x))

#define PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
  PRINT_FIELDS (N, T, MAYBE_UNUSED ଅକ୍ଷର ଅଚଳ *fmt_string,              \
                ଅକ୍ଷର buf[BUFSIZE];                                      \
                FTOASTR (buf, ଆକାର buf, 0, 0, x);                     \
                xprintf ("%*s", adjusted_width, buf))

PRINT_TYPE (print_s_char, signed ଅକ୍ଷର)
PRINT_TYPE (print_char, unsigned ଅକ୍ଷର)
PRINT_TYPE (print_s_short, short ପୂର୍ଣ୍ଣସଂଖ୍ୟା)
PRINT_TYPE (print_short, unsigned short ପୂର୍ଣ୍ଣସଂଖ୍ୟା)
PRINT_TYPE (print_int, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)
PRINT_TYPE (print_long, unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)
PRINT_TYPE (print_long_long, unsigned long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)
PRINT_TYPE (print_intmax, uintmax_t)

PRINT_FLOATTYPE (print_bfloat, bfloat16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_halffloat, float16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_float, ଦଶମିକ, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_double, ଦ୍ୱିଗୁଣ, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_long_double, long ଦ୍ୱିଗୁଣ, ldtoastr, LDBL_BUFSIZE_BOUND)

#undef PRINT_TYPE
#undef PRINT_FLOATTYPE

ସ୍ଥିର ଶୂନ୍ୟ
dump_hexl_mode_trailer (idx_t n_bytes, ଅକ୍ଷର ଅଚଳ *block)
{
  fputs ("  >", stdout);
  ପାଇଁ (idx_t i = n_bytes; i > 0; i--)
    {
      unsigned ଅକ୍ଷର c = *block++;
      unsigned ଅକ୍ଷର c2 = (isprint (c) ? c : '.');
      putchar (c2);
    }
  putchar ('<');
}

ସ୍ଥିର ଶୂନ୍ୟ
print_named_ascii (idx_t fields, idx_t blank, ଶୂନ୍ୟ ଅଚଳ *block,
                   MAYBE_UNUSED ଅକ୍ଷର ଅଚଳ *unused_fmt_string,
                   ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, idx_t pad)
{
  unsigned ଅକ୍ଷର ଅଚଳ *p = block;
  idx_t pad_remaining = pad;
  ପାଇଁ (idx_t i = fields; blank < i; i--)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା masked_c = *p++ & 0x7f;
      ଅକ୍ଷର ଅଚଳ *s;
      ଅକ୍ଷର buf[2];

      ଯଦି (masked_c == 127)
        s = "del";
      ଅନ୍ୟଥା ଯଦି (masked_c <= 040)
        s = charname[masked_c];
      ଅନ୍ୟଥା
        {
          buf[0] = masked_c;
          buf[1] = 0;
          s = buf;
        }

      idx_t next_pad = pad_at (fields, i - 1, pad);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା adjusted_width = pad_remaining - next_pad + width;
      xprintf ("%*s", adjusted_width, s);
      pad_remaining = next_pad;
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
print_ascii (idx_t fields, idx_t blank, ଶୂନ୍ୟ ଅଚଳ *block,
             MAYBE_UNUSED ଅକ୍ଷର ଅଚଳ *unused_fmt_string, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width,
             idx_t pad)
{
  unsigned ଅକ୍ଷର ଅଚଳ *p = block;
  idx_t pad_remaining = pad;
  ପାଇଁ (idx_t i = fields; blank < i; i--)
    {
      unsigned ଅକ୍ଷର c = *p++;
      ଅକ୍ଷର ଅଚଳ *s;
      ଅକ୍ଷର buf[4];

      ବାଛ (c)
        {
        ସ୍ଥିତି '\0':
          s = "\\0";
          ଭାଙ୍ଗ;

        ସ୍ଥିତି '\a':
          s = "\\a";
          ଭାଙ୍ଗ;

        ସ୍ଥିତି '\b':
          s = "\\b";
          ଭାଙ୍ଗ;

        ସ୍ଥିତି '\f':
          s = "\\f";
          ଭାଙ୍ଗ;

        ସ୍ଥିତି '\n':
          s = "\\n";
          ଭାଙ୍ଗ;

        ସ୍ଥିତି '\r':
          s = "\\r";
          ଭାଙ୍ଗ;

        ସ୍ଥିତି '\t':
          s = "\\t";
          ଭାଙ୍ଗ;

        ସ୍ଥିତି '\v':
          s = "\\v";
          ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          sprintf (buf, (isprint (c) ? "%c" : "%03o"), c);
          s = buf;
        }

      idx_t next_pad = pad_at (fields, i - 1, pad);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା adjusted_width = pad_remaining - next_pad + width;
      xprintf ("%*s", adjusted_width, s);
      pad_remaining = next_pad;
    }
}

/* Convert a null-terminated (possibly zero-length) string S to an
   int value.  If S points to a non-digit set *P to S,
   *VAL to 0, and return true.  Otherwise, accumulate the integer value of
   the string of digits.  If the string of digits represents a value
   larger than INT_MAX, don't modify *VAL or *P and return false.
   Otherwise, advance *P to the first non-digit after S, set *VAL to
   the result of the conversion and return true.  */

ସ୍ଥିର bool
simple_strtoi (ଅକ୍ଷର ଅଚଳ *s, ଅକ୍ଷର ଅଚଳ **p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *val)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sum;

  ପାଇଁ (sum = 0; c_isdigit (*s); s++)
    ଯଦି (ckd_mul (&sum, sum, 10) || ckd_add (&sum, sum, *s - '0'))
      ଫେରନ୍ତୁ false;
  *p = s;
  *val = sum;
  ଫେରନ୍ତୁ true;
}

/* Return a printf format for SPEC, given that remaining arguments are
   formats for int, long, long long, and intmax_t, respectively.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *
ispec_to_format (ଗଣନା size_spec size_spec,
                 ଅକ୍ଷର ଅଚଳ *int_format,
                 ଅକ୍ଷର ଅଚଳ *long_format,
                 ଅକ୍ଷର ଅଚଳ *long_long_format,
                 ଅକ୍ଷର ଅଚଳ *intmax_format)
{
  ଫେରନ୍ତୁ (LONG < INTMAX && size_spec == INTMAX ? intmax_format
          : LONG < LONG_LONG && size_spec == LONG_LONG ? long_long_format
          : INT < LONG && size_spec == LONG ? long_format
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

ସ୍ଥିର bool ATTRIBUTE_NONNULL ()
decode_one_format (ଅକ୍ଷର ଅଚଳ *s_orig, ଅକ୍ଷର ଅଚଳ *s, ଅକ୍ଷର ଅଚଳ **next,
                   ସଂରଚନା tspec *tspec)
{
  ଗଣନା size_spec size_spec;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା size;
  ଗଣନା output_format fmt;
  print_function_type print_function;
  ଅକ୍ଷର ଅଚଳ *p;
  ଅକ୍ଷର c;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା field_width;

  ବାଛ (*s)
    {
    ସ୍ଥିତି 'd':
    ସ୍ଥିତି 'o':
    ସ୍ଥିତି 'u':
    ସ୍ଥିତି 'x':
      c = *s;
      ++s;
      ବାଛ (*s)
        {
        ସ୍ଥିତି 'C':
          ++s;
          size = ଆକାର (unsigned ଅକ୍ଷର);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'S':
          ++s;
          size = ଆକାର (unsigned short ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'I':
          ++s;
          size = ଆକାର (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'L':
          ++s;
          size = ଆକାର (unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
          ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          ଯଦି (! simple_strtoi (s, &p, &size))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              ଫେରନ୍ତୁ false;
            }
          ଯଦି (p == s)
            size = ଆକାର (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
          ଅନ୍ୟଥା
            {
              ଯଦି (countof (integral_type_size) <= size
                  || integral_type_size[size] == NO_SIZE)
                {
                  error (0, 0, _("invalid type string %s;\nthis system"
                                 " doesn't provide a %d-byte integral type"),
                         quote (s_orig), size);
                  ଫେରନ୍ତୁ false;
                }
              s = p;
            }
          ଭାଙ୍ଗ;
        }

      size_spec = integral_type_size[size];

      ବାଛ (c)
        {
        ସ୍ଥିତି 'd':
          fmt = SIGNED_DECIMAL;
          field_width = INT_BITS_STRLEN_BOUND (CHAR_BIT * size - 1) + 1;
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_to_format (size_spec, "d", "ld", "lld", "jd"));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'o':
          fmt = OCTAL;
          field_width = (CHAR_BIT * size + 2) / 3;
          sprintf (tspec->fmt_string, "%%*.%d%s", field_width,
                   ispec_to_format (size_spec, "o", "lo", "llo", "jo"));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'u':
          fmt = UNSIGNED_DECIMAL;
          field_width = INT_BITS_STRLEN_BOUND (CHAR_BIT * size);
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_to_format (size_spec, "u", "lu", "llu", "ju"));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'x':
          fmt = HEXADECIMAL;
          field_width = (CHAR_BIT * size + 3) / 4;
          sprintf (tspec->fmt_string, "%%*.%d%s", field_width,
                   ispec_to_format (size_spec, "x", "lx", "llx", "jx"));
          ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          affirm (false);
        }

      /* Prefer INT, prefer LONG to longer types,
         and prefer INTMAX to LONG_LONG.  */
      print_function
        = (size_spec == INT
           ? print_int
           : SHORT < INT && size_spec == SHORT
           ? (fmt == SIGNED_DECIMAL ? print_s_short : print_short)
           : CHAR < SHORT && size_spec == CHAR
           ? (fmt == SIGNED_DECIMAL ? print_s_char : print_char)
           : INT < LONG && size_spec == LONG
           ? print_long
           : LONG < INTMAX && size_spec == INTMAX
           ? print_intmax
           : LONG < LONG_LONG && LONG_LONG < INTMAX && size_spec == LONG_LONG
           ? print_long_long
           : (affirm (false), (print_function_type) ଖାଲି));
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 'f':
      fmt = FLOATING_POINT;
      ++s;
      ବାଛ (*s)
        {
        ସ୍ଥିତି 'B':
          ++s;
          fmt = BFLOATING_POINT;
          size = ଆକାର (bfloat16);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'H':
          ++s;
          fmt = HFLOATING_POINT;
          size = ଆକାର (float16);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'F':
          ++s;
          size = ଆକାର (ଦଶମିକ);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'D':
          ++s;
          size = ଆକାର (ଦ୍ୱିଗୁଣ);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'L':
          ++s;
          size = ଆକାର (long ଦ୍ୱିଗୁଣ);
          ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          ଯଦି (! simple_strtoi (s, &p, &size))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              ଫେରନ୍ତୁ false;
            }
          ଯଦି (p == s)
            size = ଆକାର (ଦ୍ୱିଗୁଣ);
          ଅନ୍ୟଥା
            {
              ଯଦି (countof (fp_type_size) <= size
                  || fp_type_size[size] == NO_SIZE
                  || (! FLOAT16_SUPPORTED && BF16_SUPPORTED
                      && size == ଆକାର (bfloat16)))
                {
                  error (0, 0,
                         _("invalid type string %s;\n"
                           "this system doesn't provide a %d-byte"
                           " floating point type"),
                         quote (s_orig), size);
                  ଫେରନ୍ତୁ false;
                }
              s = p;
            }
          ଭାଙ୍ଗ;
        }
      size_spec = fp_type_size[size];

      ଯଦି ((! FLOAT16_SUPPORTED && fmt == HFLOATING_POINT)
          || (! BF16_SUPPORTED && fmt == BFLOATING_POINT))
      {
        error (0, 0,
               _("this system doesn't provide a %s floating point type"),
               quote (s_orig));
        ଫେରନ୍ତୁ false;
      }

      {
        ସଂରଚନା lconv ଅଚଳ *locale = localeconv ();
        idx_t decimal_point_len =
          (locale->decimal_point[0] ? strlen (locale->decimal_point) : 1);

        ଯଦି (size_spec == FLOAT_DOUBLE)
          {
            print_function = print_double;
            field_width = DBL_STRLEN_BOUND_L (decimal_point_len);
          }
        ଅନ୍ୟଥା ଯଦି (FLOAT_SINGLE < FLOAT_DOUBLE && size_spec == FLOAT_SINGLE)
          {
            print_function = print_float;
            field_width = FLT_STRLEN_BOUND_L (decimal_point_len);
          }
        ଅନ୍ୟଥା ଯଦି (FLOAT_HALF < FLOAT_SINGLE && size_spec == FLOAT_HALF)
          {
            print_function = fmt == BFLOATING_POINT
                             ? print_bfloat : print_halffloat;
            field_width = FLT_STRLEN_BOUND_L (decimal_point_len);
          }
        ଅନ୍ୟଥା ଯଦି (FLOAT_DOUBLE < FLOAT_LONG_DOUBLE
                 && size_spec == FLOAT_LONG_DOUBLE)
          {
            print_function = print_long_double;
            field_width = LDBL_STRLEN_BOUND_L (decimal_point_len);
          }
        ଅନ୍ୟଥା
          affirm (false);

        ଭାଙ୍ଗ;
      }

    ସ୍ଥିତି 'a':
      ++s;
      fmt = NAMED_CHARACTER;
      size_spec = CHAR;
      print_function = print_named_ascii;
      field_width = 3;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 'c':
      ++s;
      fmt = CHARACTER;
      size_spec = CHAR;
      print_function = print_ascii;
      field_width = 3;
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      error (0, 0, _("invalid character '%c' in type string %s"),
             *s, quote (s_orig));
      ଫେରନ୍ତୁ false;
    }

  tspec->size = size_spec;
  tspec->fmt = fmt;
  tspec->print_function = print_function;

  tspec->field_width = field_width;
  tspec->hexl_mode_trailer = (*s == 'z');
  ଯଦି (tspec->hexl_mode_trailer)
    s++;

  *next = s;
  ଫେରନ୍ତୁ true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM and the global string INPUT_FILENAME to the
   first one that can be successfully opened. Modify FILE_LIST to
   reference the next filename in the list.  A file name of "-" is
   interpreted as standard input.  If any file open fails, give an error
   message and return false.  */

ସ୍ଥିର bool
open_next_file (ଶୂନ୍ୟ)
{
  bool ok = true;

  କର
    {
      ନିବେଶ_filename = *file_list;
      ଯଦି (ନିବେଶ_filename == ଖାଲି)
        ଫେରନ୍ତୁ ok;
      ++file_list;

      ଯଦି (streq (ନିବେଶ_filename, "-"))
        {
          ନିବେଶ_filename = _("standard input");
          in_stream = stdin;
          have_read_stdin = true;
          xset_ଦ୍ୱିମିକ_mode (STDIN_FILENO, O_ଦ୍ୱିମିକ);
        }
      ଅନ୍ୟଥା
        {
          in_stream = fopen (ନିବେଶ_filename, (O_ଦ୍ୱିମିକ ? "rb" : "r"));
          ଯଦି (in_stream == ଖାଲି)
            {
              error (0, errno, "%s", quotef (ନିବେଶ_filename));
              ok = false;
            }
        }
    }
  ଯେତେବେଳେ (in_stream == ଖାଲି);

  ଯଦି (0 <= end_offset && !flag_dump_strings)
    setvbuf (in_stream, ଖାଲି, _IONBF, 0);

  ଫେରନ୍ତୁ ok;
}

/* Test whether there have been errors on in_stream, and close it if
   it is not standard input.  Return false if there has been an error
   on in_stream or stdout; return true otherwise.  This function will
   report more than one error only if both a read and a write error
   have occurred.  IN_ERRNO, if nonzero, is the error number
   corresponding to the most recent action for IN_STREAM.  */

ସ୍ଥିର bool
check_and_close (ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_errno)
{
  bool ok = true;

  ଯଦି (in_stream != ଖାଲି)
    {
      ଯଦି (!ferror (in_stream))
        in_errno = 0;
      ଯଦି (streq (file_list[-1], "-"))
        clearerr (in_stream);
      ଅନ୍ୟଥା ଯଦି (fclose (in_stream) != 0 && !in_errno)
        in_errno = errno;
      ଯଦି (in_errno)
        {
          error (0, in_errno, "%s", quotef (ନିବେଶ_filename));
          ok = false;
        }

      in_stream = ଖାଲି;
    }

  ଯଦି (ferror (stdout))
    {
      error (0, 0, _("write error"));
      ok = false;
    }

  ଫେରନ୍ତୁ ok;
}

/* Decode the modern od format string S.  Append the decoded
   representation to the global array SPEC, reallocating SPEC if
   necessary.  Return true if S is valid.  */

ସ୍ଥିର bool ATTRIBUTE_NONNULL ()
decode_format_string (ଅକ୍ଷର ଅଚଳ *s)
{
  ଅକ୍ଷର ଅଚଳ *s_orig = s;

  ଯେତେବେଳେ (*s != '\0')
    {
      ଅକ୍ଷର ଅଚଳ *next;

      ଯଦି (n_specs_allocated <= n_specs)
        spec = xpalloc (spec, &n_specs_allocated, 1, -1, ଆକାର *spec);

      ଯଦି (! decode_one_format (s_orig, s, &next, &spec[n_specs]))
        ଫେରନ୍ତୁ false;

      affirm (s != next);
      s = next;
      ++n_specs;
    }

  ଫେରନ୍ତୁ true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM to position N_SKIP in the concatenation of
   those files.  If any file operation fails or if there are fewer than
   N_SKIP bytes in the combined input, give an error message and return
   false.  When possible, use seek rather than read operations to
   advance IN_STREAM.  */

ସ୍ଥିର bool
skip (intmax_t n_skip)
{
  bool ok = true;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_errno = 0;

  ଯଦି (n_skip == 0)
    ଫେରନ୍ତୁ true;

  ଯେତେବେଳେ (in_stream != ଖାଲି)	/* EOF.  */
    {
      ସଂରଚନା stat file_stats;

      /* First try seeking.  For large offsets, this extra work is
         worthwhile.  If the offset is below some threshold it may be
         more efficient to move the pointer by reading.  There are two
         issues when trying to seek:
           - the file must be seekable.
           - before seeking to the specified position, make sure
             that the new position is in the current file.
             Try to do that by getting file's size using fstat.
             But that will work only for regular files.  */

      ଯଦି (fstat (fileno (in_stream), &file_stats) == 0)
        {
          /* The st_size field is valid for regular files.
             If the number of bytes left to skip is larger than
             the size of the current file, we can decrement n_skip
             and go on to the next file.  Skip this optimization also
             when st_size is no greater than the block size, because
             some kernels report nonsense small file sizes for
             proc-like file systems.  */
          ଯଦି (S_ISREG (file_stats.st_mode)
              && STP_BLKSIZE (&file_stats) < file_stats.st_size)
            {
              ଯଦି (file_stats.st_size < n_skip)
                n_skip -= file_stats.st_size;
              ଅନ୍ୟଥା
                {
                  ଯଦି (fseeko (in_stream, n_skip, SEEK_CUR) != 0)
                    {
                      in_errno = errno;
                      ok = false;
                    }
                  n_skip = 0;
                }
            }

          ଅନ୍ୟଥା ଯଦି (! (S_ISREG (file_stats.st_mode)
                      || S_TYPEISSHM (&file_stats)
                      || S_TYPEISTMO (&file_stats))
                   && fseeko (in_stream, n_skip, SEEK_CUR) == 0)
            n_skip = 0;

          /* If it's neither a seekable file with unusable size, nor a
             regular file so large it can't be in a proc-like file system,
             position the file pointer by reading.  */

          ଅନ୍ୟଥା
            {
              ଅକ୍ଷର buf[BUFSIZ];
              idx_t n_bytes_read, n_bytes_to_read = BUFSIZ;

              ଯେତେବେଳେ (0 < n_skip)
                {
                  ଯଦି (n_skip < n_bytes_to_read)
                    n_bytes_to_read = n_skip;
                  n_bytes_read = fread (buf, 1, n_bytes_to_read, in_stream);
                  n_skip -= n_bytes_read;
                  ଯଦି (n_bytes_read != n_bytes_to_read)
                    {
                      ଯଦି (ferror (in_stream))
                        {
                          in_errno = errno;
                          ok = false;
                          n_skip = 0;
                          ଭାଙ୍ଗ;
                        }
                      ଯଦି (feof (in_stream))
                        ଭାଙ୍ଗ;
                    }
                }
            }

          ଯଦି (n_skip == 0)
            ଭାଙ୍ଗ;
        }

      ଅନ୍ୟଥା   /* cannot fstat() file */
        {
          error (0, errno, "%s", quotef (ନିବେଶ_filename));
          ok = false;
        }

      ok &= check_and_close (in_errno);

      ok &= open_next_file ();
    }

  ଯଦି (n_skip != 0)
    error (EXIT_FAILURE, 0, _("cannot skip past end of combined input"));

  ଫେରନ୍ତୁ ok;
}

ସ୍ଥିର ଶୂନ୍ୟ
format_address_none (MAYBE_UNUSED intmax_t address,
                     MAYBE_UNUSED ଅକ୍ଷର c)
{
}

ସ୍ଥିର ଶୂନ୍ୟ
format_address_std (intmax_t address, ଅକ୍ଷର c)
{
  ଅକ୍ଷର buf[MAX_ADDRESS_LENGTH + 2];
  ଅକ୍ଷର *p = buf + ଆକାର buf;
  ଅକ୍ଷର ଅଚଳ *pbound;

  *--p = '\0';
  *--p = c;
  pbound = p - address_pad_len;

  /* Use a special case of the code for each base.  This is measurably
     faster than generic code.  */
  ବାଛ (address_base)
    {
    ସ୍ଥିତି 8:
      କର
        *--p = '0' + (address & 7);
      ଯେତେବେଳେ ((address >>= 3) != 0);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 10:
      କର
        *--p = '0' + (address % 10);
      ଯେତେବେଳେ ((address /= 10) != 0);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 16:
      କର
        *--p = "0123456789abcdef"[address & 15];
      ଯେତେବେଳେ ((address >>= 4) != 0);
      ଭାଙ୍ଗ;
    }

  ଯେତେବେଳେ (pbound < p)
    *--p = '0';

  fputs (p, stdout);
}

ସ୍ଥିର ଶୂନ୍ୟ
format_address_paren (intmax_t address, ଅକ୍ଷର c)
{
  putchar ('(');
  format_address_std (address, ')');
  ଯଦି (c)
    putchar (c);
}

ସ୍ଥିର ଶୂନ୍ୟ
format_address_label (intmax_t address, ଅକ୍ଷର c)
{
  format_address_std (address, ' ');

  intmax_t addr;
  ଯଦି (ckd_add (&addr, address, pseudo_offset))
    error (EXIT_FAILURE, 0, _("pseudo address too large for input"));

  format_address_paren (addr, c);
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

ସ୍ଥିର ଶୂନ୍ୟ
write_block (intmax_t current_offset, idx_t n_bytes,
             ଅକ୍ଷର ଅଚଳ *prev_block, ଅକ୍ଷର ଅଚଳ *curr_block)
{
  ସ୍ଥିର bool first = true;
  ସ୍ଥିର bool prev_pair_equal = false;

  ଯଦି (abbreviate_duplicate_blocks
      && !first && n_bytes == bytes_per_block
      && memeq (prev_block, curr_block, bytes_per_block))
    {
      ଯଦି (prev_pair_equal)
        {
          /* The two preceding blocks were equal, and the current
             block is the same as the last one, so print nothing.  */
        }
      ଅନ୍ୟଥା
        {
          printf ("*\n");
          prev_pair_equal = true;
        }
    }
  ଅନ୍ୟଥା
    {
      prev_pair_equal = false;
      ପାଇଁ (idx_t i = 0; i < n_specs; i++)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା datum_width = width_bytes[spec[i].size];
          idx_t fields_per_block = bytes_per_block / datum_width;
          idx_t blank_fields = (bytes_per_block - n_bytes) / datum_width;
          ଯଦି (i == 0)
            format_address (current_offset, '\0');
          ଅନ୍ୟଥା
            print_n_spaces (address_pad_len);
          (*spec[i].print_function) (fields_per_block, blank_fields,
                                     curr_block, spec[i].fmt_string,
                                     spec[i].field_width, spec[i].pad_width);
          ଯଦି (spec[i].hexl_mode_trailer)
            {
              /* Space-pad out to full line width, then dump the trailer.  */
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା field_width = spec[i].field_width;
              ପାଇଁ (idx_t f = 0; f < blank_fields; f++)
                print_n_spaces (field_width);
              idx_t pad_width = pad_at (fields_per_block, blank_fields,
                                        spec[i].pad_width);
              print_n_spaces (pad_width);
              dump_hexl_mode_trailer (n_bytes, curr_block);
            }
          putchar ('\n');
        }
    }
  first = false;
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

ସ୍ଥିର bool
read_char (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *c)
{
  bool ok = true;

  *c = EOF;

  ଯେତେବେଳେ (in_stream && (*c = getc (in_stream)) < 0)
    {
      ok &= check_and_close (errno);
      ok &= open_next_file ();
    }

  ଫେରନ୍ତୁ ok;
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

ସ୍ଥିର bool
read_block (idx_t n, ଅକ୍ଷର *block, idx_t *n_bytes_in_buffer)
{
  bool ok = true;

  affirm (0 < n && n <= bytes_per_block);

  *n_bytes_in_buffer = 0;

  ଯେତେବେଳେ (in_stream != ଖାଲି)	/* EOF.  */
    {
      idx_t n_needed = n - *n_bytes_in_buffer;
      idx_t n_read = fread (block + *n_bytes_in_buffer,
                            1, n_needed, in_stream);

      *n_bytes_in_buffer += n_read;

      ଯଦି (n_read == n_needed)
        ଭାଙ୍ଗ;

      ok &= check_and_close (errno);

      ok &= open_next_file ();
    }

  ଫେରନ୍ତୁ ok;
}

/* Return the least common multiple of the sizes associated
   with the format specs.  */

ATTRIBUTE_PURE
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_lcm (ଶୂନ୍ୟ)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା l_c_m = 1;

  ପାଇଁ (idx_t i = 0; i < n_specs; i++)
    l_c_m = lcm (l_c_m, width_bytes[spec[i].size]);
  ଫେରନ୍ତୁ l_c_m;
}

/* Act like xstrtoimax (NPTR, NULL, BASE, VAL, VALID_SUFFIXES),
   except reject negative values, and *VAL may be set if
   LONGINT_INVALID is returned.  */
ସ୍ଥିର strtol_error
xstr2nonneg (ଅକ୍ଷର ଅଚଳ *restrict nptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା base, intmax_t *val,
             ଅକ୍ଷର ଅଚଳ *restrict valid_suffixes)
{
  strtol_error s_err = xstrtoimax (nptr, ଖାଲି, base, val, valid_suffixes);
  ଫେରନ୍ତୁ s_err != LONGINT_INVALID && *val < 0 ? LONGINT_INVALID : s_err;
}

/* If STR is a valid traditional offset specification with an optional
   leading '+', and can be represented in intmax_t, return true and
   set *OFFSET to the offset it denotes.  If it is valid but cannot be
   represented, diagnose the problem and exit.  Otherwise return false
   and possibly set *OFFSET.  */

ସ୍ଥିର bool
parse_old_offset (ଅକ୍ଷର *str, intmax_t *offset)
{
  /* Skip over any leading '+'.  */
  ଅକ୍ଷର *s = str + (str[0] == '+');

  ଯଦି (! c_isdigit (s[0]))
    ଫେରନ୍ତୁ false;

  /* Determine the radix for S.  If there is a '.', followed first by
     optional 'b' or (undocumented) 'B' and then by end of string,
     it's decimal, otherwise, if the string begins with '0X' or '0x',
     it's hexadecimal, else octal.  */
  ଅକ୍ଷର *dot = strchr (s, '.');
  ଯଦି (dot && dot[(dot[1] == 'b' || dot[1] == 'B') + 1])
    dot = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା radix = dot ? 10 : s[0] == '0' && (s[1] == 'x' || s[1] == 'X') ? 16 : 8;

  ଯଦି (dot)
    {
      /* Temporarily remove the '.' from the decimal string.  */
      dot[0] = dot[1];
      dot[1] = '\0';
    }

  ଗଣନା strtol_error s_err = xstr2nonneg (s, radix, offset, "bB");

  ଯଦି (dot)
    {
      /* Restore the decimal string's original value.  */
      dot[1] = dot[0];
      dot[0] = '.';
    }

  ଯଦି (s_err == LONGINT_OVERFLOW)
    error (EXIT_FAILURE, ERANGE, "%s", quotef (str));
  ଫେରନ୍ତୁ s_err == LONGINT_OK;
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

ସ୍ଥିର bool
dump (ଶୂନ୍ୟ)
{
  ଅକ୍ଷର *block[2];
  bool idx = false;
  bool ok = true;
  idx_t n_bytes_read;

  block[0] = xinmalloc (1 + abbreviate_duplicate_blocks, bytes_per_block);
  block[1] = block[0] + abbreviate_duplicate_blocks * bytes_per_block;

  intmax_t current_offset = n_bytes_to_skip;

  କର
    {
      intmax_t needed_bound
        = end_offset < 0 ? INTMAX_MAX : end_offset - current_offset;
      ଯଦି (needed_bound <= 0)
        {
          n_bytes_read = 0;
          ଭାଙ୍ଗ;
        }
      idx_t n_needed = MIN (bytes_per_block, needed_bound);
      ok &= read_block (n_needed, block[idx], &n_bytes_read);
      ଯଦି (n_bytes_read < bytes_per_block)
        ଭାଙ୍ଗ;
      affirm (n_bytes_read == bytes_per_block);
      write_block (current_offset, n_bytes_read,
                   block[!idx], block[idx]);
      ଯଦି (ferror (stdout))
        ok = false;
      current_offset += n_bytes_read;
      idx = !idx;
    }
  ଯେତେବେଳେ (ok);

  ଯଦି (n_bytes_read > 0)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା l_c_m = get_lcm ();

      /* Ensure zero-byte padding up to the smallest multiple of l_c_m that
         is at least as large as n_bytes_read.  */
      idx_t bytes_to_write = (n_bytes_read + l_c_m - 1
                              - (n_bytes_read + l_c_m - 1) % l_c_m);

      memset (block[idx] + n_bytes_read, 0, bytes_to_write - n_bytes_read);
      write_block (current_offset, n_bytes_read, block[!idx], block[idx]);
      current_offset += n_bytes_read;
    }

  ଯଦି (ok || current_offset)
    format_address (current_offset, '\n');

  ଯଦି (0 <= end_offset && end_offset <= current_offset)
    ok &= check_and_close (0);

  ମୁକ୍ତ (block[0]);

  ଫେରନ୍ତୁ ok;
}

/* STRINGS mode.  Find each "string constant" in the input.
   A string constant is a run of at least STRING_MIN
   printable characters terminated by a NUL or END_OFFSET.
   Based on a function written by Richard Stallman for a
   traditional version of od.  Return true if successful.  */

ସ୍ଥିର bool
dump_strings (ଶୂନ୍ୟ)
{
  idx_t bufsize = MAX (100, string_min + 1);
  ଅକ୍ଷର *buf = ximalloc (bufsize);
  intmax_t address = n_bytes_to_skip;
  bool ok = true;

  ଯେତେବେଳେ (true)
    {
      idx_t i = 0;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = 1;  /* Init to 1 so can distinguish if NUL read.  */

      ଯଦି (0 <= end_offset
          && (end_offset < string_min || end_offset - string_min < address))
        ଭାଙ୍ଗ;

      /* Store consecutive printable characters to BUF.  */
      ଯେତେବେଳେ (! (0 <= end_offset && end_offset <= address))
        {
          ଯଦି (i == bufsize - 1)
            buf = xpalloc (buf, &bufsize, 1, -1, ଆକାର *buf);
          ok &= read_char (&c);
          ଯଦି (c < 0)
            {
              ମୁକ୍ତ (buf);
              ଫେରନ୍ତୁ ok;
            }
          address++;
          buf[i++] = c;
          ଯଦି (c == '\0')
            ଭାଙ୍ଗ;		/* Print this string.  */
          ଯଦି (! isprint (c))
            {
              c = -1;		/* Give up on this string.  */
              ଭାଙ୍ଗ;
            }
        }

      ଯଦି (c < 0 || i - !c < string_min)
        ଜାରି;

      buf[i] = 0;

      /* If we get here, the string is all printable, so print it.  */

      format_address (address - i, ' ');

      ପାଇଁ (i = 0; (c = buf[i]); i++)
        {
          ବାଛ (c)
            {
            ସ୍ଥିତି '\a':
              fputs ("\\a", stdout);
              ଭାଙ୍ଗ;

            ସ୍ଥିତି '\b':
              fputs ("\\b", stdout);
              ଭାଙ୍ଗ;

            ସ୍ଥିତି '\f':
              fputs ("\\f", stdout);
              ଭାଙ୍ଗ;

            ସ୍ଥିତି '\n':
              fputs ("\\n", stdout);
              ଭାଙ୍ଗ;

            ସ୍ଥିତି '\r':
              fputs ("\\r", stdout);
              ଭାଙ୍ଗ;

            ସ୍ଥିତି '\t':
              fputs ("\\t", stdout);
              ଭାଙ୍ଗ;

            ସ୍ଥିତି '\v':
              fputs ("\\v", stdout);
              ଭାଙ୍ଗ;

            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
              putc (c, stdout);
            }
        }
      putchar ('\n');
    }

  /* We reach this point only if we search through
     (max_bytes_to_format - string_min) bytes before reaching EOF.  */

  ମୁକ୍ତ (buf);

  ok &= check_and_close (0);
  ଫେରନ୍ତୁ ok;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_files;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା l_c_m;
  idx_t desired_width = 0;
  bool modern = false;
  bool ok = true;
  idx_t width_per_block = 0;
  ସ୍ଥିର ଅକ୍ଷର ଅଚଳ multipliers[] = "bEGKkMmPQRTYZ0";

  /* The maximum number of bytes that will be formatted.
     If negative, there is no limit.  */
  intmax_t max_bytes_to_format = -1;

  /* The old-style 'pseudo starting address' to be printed in parentheses
     after any true address.  */
  intmax_t pseudo_start IF_LINT ( = 0);

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ (true)
    {
      intmax_t tmp;
      ଗଣନା strtol_error s_err;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା oi = -1;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = getopt_long (argc, argv, short_options, long_options, &oi);
      ଯଦି (c == -1)
        ଭାଙ୍ଗ;

      ବାଛ (c)
        {
        ସ୍ଥିତି 'A':
          modern = true;
          ବାଛ (optarg[0])
            {
            ସ୍ଥିତି 'd':
              format_address = format_address_std;
              address_base = 10;
              address_pad_len = 7;
              ଭାଙ୍ଗ;
            ସ୍ଥିତି 'o':
              format_address = format_address_std;
              address_base = 8;
              address_pad_len = 7;
              ଭାଙ୍ଗ;
            ସ୍ଥିତି 'x':
              format_address = format_address_std;
              address_base = 16;
              address_pad_len = 6;
              ଭାଙ୍ଗ;
            ସ୍ଥିତି 'n':
              format_address = format_address_none;
              address_pad_len = 0;
              ଭାଙ୍ଗ;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
              error (EXIT_FAILURE, 0,
                     _("invalid output address radix '%c';"
                       " it must be one character from [doxn]"),
                     optarg[0]);
              ଭାଙ୍ଗ;
            }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'j':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &n_bytes_to_skip, multipliers);
          ଯଦି (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'N':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &max_bytes_to_format, multipliers);
          ଯଦି (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'S':
          modern = true;
          ଯଦି (optarg == ଖାଲି)
            string_min = 3;
          ଅନ୍ୟଥା
            {
              s_err = xstr2nonneg (optarg, 0, &tmp, multipliers);
              /* The minimum string length + 1 must fit in idx_t,
                 since we may allocate a buffer of this size + 1.  */
              idx_t i;
              ଯଦି (s_err == LONGINT_OK && ckd_add (&i, tmp, 1))
                s_err = LONGINT_OVERFLOW;
              ଯଦି (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
              string_min = tmp;
            }
          flag_dump_strings = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 't':
          modern = true;
          ok &= decode_format_string (optarg);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'v':
          modern = true;
          abbreviate_duplicate_blocks = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି TRADITIONAL_OPTION:
          traditional = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି ENDIAN_OPTION:
          ବାଛ (XARGMATCH ("--endian", optarg, endian_args, endian_types))
            {
            ସ୍ଥିତି endian_big:
              ନିବେଶ_swap = BYTE_ORDER != BIG_ENDIAN;
              ଭାଙ୍ଗ;
            ସ୍ଥିତି endian_little:
              ନିବେଶ_swap = BYTE_ORDER != LITTLE_ENDIAN;
              ଭାଙ୍ଗ;
            }
          ଭାଙ୍ଗ;

          /* The next several cases map the traditional format
             specification options to the corresponding modern format
             specs.  GNU od accepts any combination of old- and
             new-style options.  Format specification options accumulate.
             The obsolescent and undocumented formats are compatible
             with FreeBSD 4.10 od.  */

#define CASE_OLD_ARG(old_char,new_string)		\
        ସ୍ଥିତି old_char:					\
          ok &= decode_format_string (new_string);	\
          ଭାଙ୍ଗ

          CASE_OLD_ARG ('a', "a");
          CASE_OLD_ARG ('b', "o1");
          CASE_OLD_ARG ('c', "c");
          CASE_OLD_ARG ('D', "u4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('d', "u2");
        ସ୍ଥିତି 'F': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('e', "fD"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('f', "fF");
        ସ୍ଥିତି 'X': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('H', "x4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('i', "dI");
        ସ୍ଥିତି 'I': ସ୍ଥିତି 'L': /* obsolescent and undocumented aliases */
          CASE_OLD_ARG ('l', "dL");
          CASE_OLD_ARG ('O', "o4"); /* obsolescent and undocumented */
        ସ୍ଥିତି 'B': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('o', "o2");
          CASE_OLD_ARG ('s', "d2");
        ସ୍ଥିତି 'h': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('x', "x2");

#undef CASE_OLD_ARG

        ସ୍ଥିତି 'w':
          modern = true;
          ଯଦି (optarg == ଖାଲି)
            {
              desired_width = 32;
            }
          ଅନ୍ୟଥା
            {
              intmax_t w_tmp;
              s_err = xstr2nonneg (optarg, 10, &w_tmp, "");
              ଯଦି (s_err == LONGINT_OK)
                {
                  ଯଦି (ckd_add (&desired_width, w_tmp, 0))
                    s_err = LONGINT_OVERFLOW;
                  ଅନ୍ୟଥା ଯଦି (desired_width == 0)
                    s_err = LONGINT_INVALID;
                }
              ଯଦି (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
            }
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
          ଭାଙ୍ଗ;
        }
    }

  ଯଦି (!ok)
    ଫେରନ୍ତୁ EXIT_FAILURE;

  ଯଦି (flag_dump_strings && n_specs > 0)
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

  ଯଦି (!modern || traditional)
    {
      intmax_t o1;
      intmax_t o2;

      ବାଛ (n_files)
        {
        ସ୍ଥିତି 1:
          ଯଦି ((traditional || argv[optind][0] == '+')
              && parse_old_offset (argv[optind], &o1))
            {
              n_bytes_to_skip = o1;
              --n_files;
              ++argv;
            }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 2:
          ଯଦି ((traditional || argv[optind + 1][0] == '+'
               || c_isdigit (argv[optind + 1][0]))
              && parse_old_offset (argv[optind + 1], &o2))
            {
              ଯଦି (traditional && parse_old_offset (argv[optind], &o1))
                {
                  n_bytes_to_skip = o1;
                  flag_pseudo_start = true;
                  pseudo_start = o2;
                  argv += 2;
                  n_files -= 2;
                }
              ଅନ୍ୟଥା
                {
                  n_bytes_to_skip = o2;
                  --n_files;
                  argv[optind + 1] = argv[optind];
                  ++argv;
                }
            }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 3:
          ଯଦି (traditional
              && parse_old_offset (argv[optind + 1], &o1)
              && parse_old_offset (argv[optind + 2], &o2))
            {
              n_bytes_to_skip = o1;
              flag_pseudo_start = true;
              pseudo_start = o2;
              argv[optind + 2] = argv[optind];
              argv += 2;
              n_files -= 2;
            }
          ଭାଙ୍ଗ;
        }

      ଯଦି (traditional && 1 < n_files)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
          error (0, 0, "%s",
                 _("compatibility mode supports at most one file"));
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (flag_pseudo_start)
    {
      ଯଦି (format_address == format_address_none)
        {
          address_base = 8;
          address_pad_len = 7;
          format_address = format_address_paren;
        }
      ଅନ୍ୟଥା
        format_address = format_address_label;
    }

  ଯଦି (0 <= max_bytes_to_format
      && ckd_add (&end_offset, n_bytes_to_skip, max_bytes_to_format))
    error (EXIT_FAILURE, 0, _("skip-bytes + read-bytes is too large"));

  ଯଦି (n_specs == 0)
    decode_format_string ("oS");

  ଯଦି (n_files > 0)
    {
      /* Set the global pointer FILE_LIST so that it
         references the first file-argument on the command-line.  */

      file_list = (ଅକ୍ଷର ଅଚଳ *ଅଚଳ *) &argv[optind];
    }
  ଅନ୍ୟଥା
    {
      /* No files were listed on the command line.
         Set the global pointer FILE_LIST so that it
         references the null-terminated list of one name: "-".  */

      file_list = default_file_list;
    }

  /* open the first input file */
  ok = open_next_file ();
  ଯଦି (in_stream == ଖାଲି)
    ଯାଅ cleanup;

  /* skip over any unwanted header bytes */
  ok &= skip (n_bytes_to_skip);
  ଯଦି (in_stream == ଖାଲି)
    ଯାଅ cleanup;

  pseudo_offset = (flag_pseudo_start ? pseudo_start - n_bytes_to_skip : 0);

  /* Compute output block length.  */
  l_c_m = get_lcm ();

  ଯଦି (desired_width != 0)
    {
      ଯଦି (desired_width % l_c_m == 0)
        bytes_per_block = desired_width;
      ଅନ୍ୟଥା
        {
          error (0, 0, _("warning: invalid width %td; using %d instead"),
                 desired_width, l_c_m);
          bytes_per_block = l_c_m;
        }
    }
  ଅନ୍ୟଥା
    {
      ଯଦି (l_c_m < DEFAULT_BYTES_PER_BLOCK)
        bytes_per_block = (DEFAULT_BYTES_PER_BLOCK
                           - DEFAULT_BYTES_PER_BLOCK % l_c_m);
      ଅନ୍ୟଥା
        bytes_per_block = l_c_m;
    }

  /* Compute padding necessary to align output block.  */
  ପାଇଁ (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_per_block = bytes_per_block / width_bytes[spec[i].size];
      ଯଦି (pad_at_overflow (fields_per_block))
        error (EXIT_FAILURE, 0, _("%td is too large"), desired_width);
      idx_t block_width = (spec[i].field_width + 1) * fields_per_block;
      ଯଦି (width_per_block < block_width)
        width_per_block = block_width;
    }
  ପାଇଁ (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_per_block = bytes_per_block / width_bytes[spec[i].size];
      idx_t block_width = spec[i].field_width * fields_per_block;
      spec[i].pad_width = width_per_block - block_width;
    }

#ifdef DEBUG
  printf ("lcm=%d, width_per_block=%td\n", l_c_m, width_per_block);
  ପାଇଁ (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_per_block = bytes_per_block / width_bytes[spec[i].size];
      affirm (bytes_per_block % width_bytes[spec[i].size] == 0);
      affirm (1 <= spec[i].pad_width / fields_per_block);
      printf ("%td: fmt=\"%s\" in_width=%d out_width=%d pad=%td\n",
              i, spec[i].fmt_string, width_bytes[spec[i].size],
              spec[i].field_width, spec[i].pad_width);
    }
#endif

  ok &= (flag_dump_strings ? dump_strings () : dump ());

cleanup:

  ଯଦି (have_read_stdin && fclose (stdin) < 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  ଫେରନ୍ତୁ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
