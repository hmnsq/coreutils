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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <endian.h>
#அந்தர்பாவ <float.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ "system.h"
#அந்தர்பாவ "argmatch.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "c-ctype.h"
#அந்தர்பாவ "ftoastr.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "stat-size.h"
#அந்தர்பாவ "xbinary-io.h"
#அந்தர்பாவ "xprintf.h"
#அந்தர்பாவ "xstrtol.h"
#அந்தர்பாவ "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "od"

#define AUTHORS proper_name ("Jim Meyering")

/* The default number of input bytes per output line.  */
#define DEFAULT_BYTES_PER_BLOCK 16

#எனில் FLOAT16_SUPPORTED
  /* Available since clang 6 (2018), and gcc 7 (2017).  */
  வகைவரை _Float16 float16;
#இல்லையெனில்
# define FLOAT16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  வகைவரை தசம float16;
#endif

#எனில் BF16_SUPPORTED
  /* Available since clang 11 (2020), and gcc 13 (2023). */
  வகைவரை __bf16 bfloat16;
#இல்லையெனில்
# define BF16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  வகைவரை தசம bfloat16;
#endif

எண்ணிக்கை size_spec
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

எண்ணிக்கை output_format
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

எண்ணிக்கை { MAX_INTEGRAL_TYPE_WIDTH = UINTMAX_WIDTH };

/* The maximum number of bytes needed for a format string, including
   the trailing nul.  Each format string expects a variable amount of
   padding (guaranteed to be at least 1 plus the field width), then an
   element that will be formatted in the field.  */
எண்ணிக்கை
  {
    FMT_BYTES_ALLOCATED =
      அளவு "%*.%dlld" - அளவு "%d" + INT_STRLEN_BOUND (முழு) + 1
  };

/* Ensure that our choice for FMT_BYTES_ALLOCATED is reasonable.  */
static_assert (MAX_INTEGRAL_TYPE_WIDTH <= 3 * 99);

/* The type of a print function.  FIELDS is the number of fields per
   line, BLANK is the number of fields to leave blank.  WIDTH is width
   of one field, excluding leading space, and PAD is total pad to
   divide among FIELDS.  PAD is at least as large as FIELDS.  */
வகைவரை வெறுமை (*print_function_type)
  (idx_t fields, idx_t blank, வெறுமை மாறிலி *data,
   எழுத்து மாறிலி *fmt, முழு width, idx_t pad);

/* Each output format specification (from '-t spec' or from
   old-style options) is represented by one of these structures.  */
கட்டமைப்பு tspec
  {
    எண்ணிக்கை output_format fmt;
    எண்ணிக்கை size_spec size; /* Type of input object.  */
    print_function_type print_function;
    எழுத்து fmt_string[FMT_BYTES_ALLOCATED]; /* Of the style "%*d".  */
    bool hexl_mode_trailer;
    முழு field_width; /* Minimum width of a field, excluding leading space.  */
    idx_t pad_width; /* Total padding to be divided among fields.  */
  };

/* Convert enum size_spec to the size of the named type.  */
நிலையான மாறிலி முழு width_bytes[] =
{
  -1,
  அளவு (unsigned எழுத்து),
#எனில் UCHAR_MAX < USHRT_MAX
  அளவு (unsigned short முழு),
#endif
#எனில் USHRT_MAX < UINT_MAX
  அளவு (unsigned முழு),
#endif
#எனில் UINT_MAX < ULONG_MAX
  அளவு (unsigned long முழு),
#endif
#எனில் ULONG_MAX < ULLONG_MAX
  அளவு (unsigned long long முழு),
#endif
#எனில் ULLONG_MAX < UINTMAX_MAX
  அளவு (uintmax_t),
#endif
#எனில் BF16_SUPPORTED
  அளவு (bfloat16),
#அல்லதெனில் FLOAT16_SUPPORTED
  அளவு (float16),
#endif
#எனில் FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
  அளவு (தசம),
#endif
  அளவு (இரட்டை),
#எனில் DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
  அளவு (long இரட்டை),
#endif
};

/* Ensure that for each member of 'enum size_spec' there is an
   initializer in the width_bytes array.  */
static_assert (countof (width_bytes) == N_SIZE_SPECS);

/* Names for some non-printing characters.  */
நிலையான எழுத்து மாறிலி charname[33][4] =
{
  "nul", "soh", "stx", "etx", "eot", "enq", "ack", "bel",
  "bs", "ht", "nl", "vt", "ff", "cr", "so", "si",
  "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb",
  "can", "em", "sub", "esc", "fs", "gs", "rs", "us",
  "sp"
};

/* Address base (8, 10 or 16).  */
நிலையான முழு address_base = 8;

/* The number of octal digits required to represent the largest
   address value.  */
எண்ணிக்கை { MAX_ADDRESS_LENGTH = ((INTMAX_WIDTH - 1) / 3
                             + ((INTMAX_WIDTH - 1) % 3 != 0)) };

/* Width of a normal address.  */
நிலையான முழு address_pad_len = 7;

/* Minimum length when detecting --strings.  */
நிலையான idx_t string_min;

/* True when in --strings mode.  */
நிலையான bool flag_dump_strings;

/* True if we should recognize the older non-option arguments
   that specified at most one file and optional arguments specifying
   offset and pseudo-start address.  */
நிலையான bool traditional;

/* True if an old-style 'pseudo-address' was specified.  */
நிலையான bool flag_pseudo_start;

/* The difference between the old-style pseudo starting address and
   the number of bytes to skip.  */
நிலையான intmax_t pseudo_offset;

/* Function that accepts an address and an optional following char,
   and prints the address and char to stdout.  */
நிலையான வெறுமை format_address_std (intmax_t, எழுத்து);
நிலையான வெறுமை (*format_address) (intmax_t, எழுத்து) = format_address_std;

/* The number of input bytes to skip before formatting and writing.  */
நிலையான intmax_t n_bytes_to_skip;

/* The offset of the first byte after the last byte to be formatted.
   If negative, there is no limit.  */
நிலையான intmax_t end_offset = -1;

/* When true and two or more consecutive blocks are equal, format
   only the first block and output an asterisk alone on the following
   line to indicate that identical blocks have been elided.  */
நிலையான bool abbreviate_duplicate_blocks = true;

/* An array of specs describing how to format each input block.  */
நிலையான கட்டமைப்பு tspec *spec;

/* The number of format specs.  */
நிலையான idx_t n_specs;

/* The allocated length of SPEC.  */
நிலையான idx_t n_specs_allocated;

/* The number of input bytes formatted per output line.  It must be
   a multiple of the least common multiple of the sizes associated with
   the specified output types.  It should be as large as possible, but
   no larger than 16 -- unless specified with the -w option.  */
நிலையான idx_t bytes_per_block;

/* Human-readable representation of *file_list (for error messages).
   It differs from file_list[-1] only when file_list[-1] is "-".  */
நிலையான எழுத்து மாறிலி *உள்ளீடு_filename;

/* A null-terminated list of the file-arguments from the command line.  */
நிலையான எழுத்து மாறிலி *மாறிலி *file_list;

/* Initializer for file_list if no file-arguments
   were specified on the command line.  */
நிலையான எழுத்து மாறிலி *மாறிலி default_file_list[] = {"-", வெற்று};

/* The input stream associated with the current file.  */
நிலையான FILE *in_stream;

/* If true, at least one of the files we read was standard input.  */
நிலையான bool have_read_stdin;

/* Map the size in bytes to a type identifier.
   When two types have the same machine layout:
     - Prefer unsigned int to other types, as its format is shorter.
     - Prefer unsigned long to higher-ranked types, as it is older.
     - Prefer uintmax_t to unsigned long long int; this wins if %lld
       does not work but %jd does (e.g., MS-Windows).  */
நிலையான எண்ணிக்கை size_spec மாறிலி integral_type_size[] =
  {
#எனில் UCHAR_MAX < USHRT_MAX
    [அளவு (unsigned எழுத்து)] = CHAR,
#endif
#எனில் USHRT_MAX < UINT_MAX
    [அளவு (unsigned short முழு)] = SHORT,
#endif
    [அளவு (unsigned முழு)] = INT,
#எனில் UINT_MAX < ULONG_MAX
    [அளவு (unsigned long முழு)] = LONG,
#endif
#எனில் ULONG_MAX < ULLONG_MAX && ULLONG_MAX < UINTMAX_MAX
    [அளவு (unsigned long long முழு)] = LONG_LONG,
#endif
#எனில் ULONG_MAX < UINTMAX_MAX
    [அளவு (uintmax_t)] = INTMAX,
#endif
  };

/* Map the size in bytes to a floating type identifier.
   When two types have the same machine layout:
     - Prefer double to the other types, as its format is shorter.  */
நிலையான எண்ணிக்கை size_spec மாறிலி fp_type_size[] =
  {
#எனில் FLOAT16_SUPPORTED
    [அளவு (float16)] = FLOAT_HALF,
#அல்லதெனில் BF16_SUPPORTED
    [அளவு (bfloat16)] = FLOAT_HALF,
#endif
#எனில் FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
    [அளவு (தசம)] = FLOAT_SINGLE,
#endif
    [அளவு (இரட்டை)] = FLOAT_DOUBLE,
#எனில் DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
    [அளவு (long இரட்டை)] = FLOAT_LONG_DOUBLE,
#endif
  };

/* Use native endianness by default.  */
நிலையான bool உள்ளீடு_swap;

நிலையான எழுத்து மாறிலி short_options[] = "A:aBbcDdeFfHhIij:LlN:OoS:st:vw::Xx";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  TRADITIONAL_OPTION = CHAR_MAX + 1,
  ENDIAN_OPTION,
};

எண்ணிக்கை endian_type
{
  endian_little,
  endian_big
};

நிலையான எழுத்து மாறிலி *மாறிலி endian_args[] =
{
  "little", "big", வெற்று
};

நிலையான எண்ணிக்கை endian_type மாறிலி endian_types[] =
{
  endian_little, endian_big
};

நிலையான கட்டமைப்பு option மாறிலி long_options[] =
{
  {"skip-bytes", required_argument, வெற்று, 'j'},
  {"address-radix", required_argument, வெற்று, 'A'},
  {"read-bytes", required_argument, வெற்று, 'N'},
  {"format", required_argument, வெற்று, 't'},
  {"output-duplicates", no_argument, வெற்று, 'v'},
  {"strings", optional_argument, வெற்று, 'S'},
  {"traditional", no_argument, வெற்று, TRADITIONAL_OPTION},
  {"width", optional_argument, வெற்று, 'w'},
  {"endian", required_argument, வெற்று, ENDIAN_OPTION },

  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [-abcdfilosx]... [FILE] [[+]OFFSET[.][b]]\n\
  or:  %s --traditional [OPTION]... [FILE] [[+]OFFSET[.][b] [+][LABEL][.][b]]\n\
"),
              program_name, program_name, program_name);
      fputs (_("\n\
Write an unambiguous representation, octal bytes by இயல்பு,\n\
of FILE to standard output.  With more than one FILE argument,\n\
concatenate them in the listed order to form the உள்ளீடு.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
If first and second call formats both apply, the second format is assumed\n\
எனில் the last operand begins with + or (எனில் there are 2 operands) a digit.\n\
An OFFSET operand means -j OFFSET.  LABEL is the pseudo-address\n\
at first byte printed, incremented when dump is progressing.\n\
For OFFSET and LABEL, a 0x or 0X prefix indicates hexadecimal;\n\
suffixes may be . க்கு octal and b க்கு multiply by 512.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --address-radix=RADIX\n\
         output format க்கு file offsets;\n\
         RADIX is one of [doxn], க்கு Decimal, Octal, Hex or None\n\
"));
      oputs (_("\
      --endian={big|little}\n\
         swap உள்ளீடு bytes according the specified order\n\
"));
      oputs (_("\
  -j, --skip-bytes=BYTES\n\
         skip BYTES உள்ளீடு bytes first\n\
"));
      oputs (_("\
  -N, --பட-bytes=BYTES\n\
         limit dump to BYTES உள்ளீடு bytes\n\
"));
      oputs (_("\
  -S BYTES, --strings[=BYTES]\n\
         show only NUL terminated strings\n\
         of at least BYTES (இயல்பு 3) printable characters\n\
"));
      oputs (_("\
  -t, --format=TYPE\n\
         select output format or formats\n\
"));
      oputs (_("\
  -v, --output-duplicates\n\
         செய் not use * to mark வரி suppression\n\
"));
      oputs (_("\
  -w[BYTES], --width[=BYTES]\n\
         output BYTES bytes per output வரி;\n\
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
  -a   same as -t a,  select named characters, ignoring உயர்வு-order bit\n\
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
  a          named character, ignoring உயர்வு-order bit\n\
  c          printable character or backslash escape\n\
"), stdout);
      fputs (_("\
  d[SIZE]    signed decimal, SIZE bytes per integer\n\
  f[SIZE]    floating point, SIZE bytes per தசம\n\
  o[SIZE]    octal, SIZE bytes per integer\n\
  u[SIZE]    unsigned decimal, SIZE bytes per integer\n\
  x[SIZE]    hexadecimal, SIZE bytes per integer\n\
"), stdout);
      fputs (_("\
\n\
SIZE is a எண்.  For TYPE in [doux], SIZE may also be C க்கு\n\
அளவு(எழுத்து), S க்கு அளவு(short), I க்கு அளவு(முழு) or L க்கு\n\
அளவு(long).  If TYPE is f, SIZE may also be B க்கு Brain 16 bit,\n\
H க்கு Half precision தசம, F க்கு அளவு(தசம), D க்கு அளவு(இரட்டை),\n\
or L க்கு அளவு(long இரட்டை).\n\
"), stdout);
      fputs (_("\
\n\
Adding a z suffix to any type displays printable characters at the end of\n\
each output வரி.\n\
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
and so on க்கு G, T, P, E, Z, Y, R, Q.\n\
இருமம்_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Define the print functions.  */

/* Print N spaces, where 0 <= N.
   Do not rely on printf ("%*s", N, "") as N may exceed INT_MAX.  */
நிலையான வெறுமை
print_n_spaces (intmax_t n)
{
  க்கு (; 0 < n; n--)
    putchar (' ');
}

/* If there are FIELDS fields, return the total padding up to the
   start of field I, where I < FIELDS.  PAD is the total padding for
   all fields.  The result equals (PAD * I) / FIELDS, except it does
   not suffer from internal overflow.  */
நிலையான idx_t
pad_at (idx_t fields, idx_t i, idx_t pad)
{
  /* This implementation assumes that (FIELDS - 1)^2 does not overflow
     intmax_t, an assumption checked by pad_at_overflow.  */
  intmax_t m = pad % fields;
  திரும்பு pad / fields * i + m * i / fields;
}
நிலையான bool
pad_at_overflow (idx_t fields)
{
  intmax_t product;
  திரும்பு ckd_mul (&product, fields - 1, fields - 1);
}

#define PRINT_FIELDS(N, T, FMT_STRING_DECL, ACTION)                     \
நிலையான வெறுமை                                                             \
N (idx_t fields, idx_t blank, வெறுமை மாறிலி *block,			\
   FMT_STRING_DECL, முழு width, idx_t pad)				\
{                                                                       \
  T மாறிலி *p = block;                                                   \
  idx_t pad_remaining = pad;						\
  க்கு (idx_t i = fields; blank < i; i--)				\
    {                                                                   \
      idx_t next_pad = pad_at (fields, i - 1, pad);			\
      முழு adjusted_width = pad_remaining - next_pad + width;            \
      T x;                                                              \
      எனில் (உள்ளீடு_swap && அளவு (T) > 1)                                 \
        {                                                               \
          ஒன்றிப்பு {                                                       \
            T x;                                                        \
            எழுத்து b[அளவு (T)];                                         \
          } u;                                                          \
          க்கு (idx_t j = 0; j < அளவு (T); j++)			\
            u.b[j] = ((எழுத்து மாறிலி *) p)[அளவு (T) - 1 - j];            \
          x = u.x;                                                      \
        }                                                               \
      இல்லையெனில்                                                              \
        x = *p;                                                         \
      p++;                                                              \
      ACTION;                                                           \
      pad_remaining = next_pad;                                         \
    }                                                                   \
}

#define PRINT_TYPE(N, T)                                                \
  PRINT_FIELDS (N, T, எழுத்து மாறிலி *fmt_string,                           \
                xprintf (fmt_string, adjusted_width, x))

#define PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
  PRINT_FIELDS (N, T, MAYBE_UNUSED எழுத்து மாறிலி *fmt_string,              \
                எழுத்து buf[BUFSIZE];                                      \
                FTOASTR (buf, அளவு buf, 0, 0, x);                     \
                xprintf ("%*s", adjusted_width, buf))

PRINT_TYPE (print_s_char, signed எழுத்து)
PRINT_TYPE (print_char, unsigned எழுத்து)
PRINT_TYPE (print_s_short, short முழு)
PRINT_TYPE (print_short, unsigned short முழு)
PRINT_TYPE (print_int, unsigned முழு)
PRINT_TYPE (print_long, unsigned long முழு)
PRINT_TYPE (print_long_long, unsigned long long முழு)
PRINT_TYPE (print_intmax, uintmax_t)

PRINT_FLOATTYPE (print_bfloat, bfloat16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_halffloat, float16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_float, தசம, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_double, இரட்டை, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_long_double, long இரட்டை, ldtoastr, LDBL_BUFSIZE_BOUND)

#undef PRINT_TYPE
#undef PRINT_FLOATTYPE

நிலையான வெறுமை
dump_hexl_mode_trailer (idx_t n_bytes, எழுத்து மாறிலி *block)
{
  fputs ("  >", stdout);
  க்கு (idx_t i = n_bytes; i > 0; i--)
    {
      unsigned எழுத்து c = *block++;
      unsigned எழுத்து c2 = (isprint (c) ? c : '.');
      putchar (c2);
    }
  putchar ('<');
}

நிலையான வெறுமை
print_named_ascii (idx_t fields, idx_t blank, வெறுமை மாறிலி *block,
                   MAYBE_UNUSED எழுத்து மாறிலி *unused_fmt_string,
                   முழு width, idx_t pad)
{
  unsigned எழுத்து மாறிலி *p = block;
  idx_t pad_remaining = pad;
  க்கு (idx_t i = fields; blank < i; i--)
    {
      முழு masked_c = *p++ & 0x7f;
      எழுத்து மாறிலி *s;
      எழுத்து buf[2];

      எனில் (masked_c == 127)
        s = "del";
      இல்லையெனில் எனில் (masked_c <= 040)
        s = charname[masked_c];
      இல்லையெனில்
        {
          buf[0] = masked_c;
          buf[1] = 0;
          s = buf;
        }

      idx_t next_pad = pad_at (fields, i - 1, pad);
      முழு adjusted_width = pad_remaining - next_pad + width;
      xprintf ("%*s", adjusted_width, s);
      pad_remaining = next_pad;
    }
}

நிலையான வெறுமை
print_ascii (idx_t fields, idx_t blank, வெறுமை மாறிலி *block,
             MAYBE_UNUSED எழுத்து மாறிலி *unused_fmt_string, முழு width,
             idx_t pad)
{
  unsigned எழுத்து மாறிலி *p = block;
  idx_t pad_remaining = pad;
  க்கு (idx_t i = fields; blank < i; i--)
    {
      unsigned எழுத்து c = *p++;
      எழுத்து மாறிலி *s;
      எழுத்து buf[4];

      தேர்வு (c)
        {
        நிலை '\0':
          s = "\\0";
          நிறுத்து;

        நிலை '\a':
          s = "\\a";
          நிறுத்து;

        நிலை '\b':
          s = "\\b";
          நிறுத்து;

        நிலை '\f':
          s = "\\f";
          நிறுத்து;

        நிலை '\n':
          s = "\\n";
          நிறுத்து;

        நிலை '\r':
          s = "\\r";
          நிறுத்து;

        நிலை '\t':
          s = "\\t";
          நிறுத்து;

        நிலை '\v':
          s = "\\v";
          நிறுத்து;

        இயல்பு:
          sprintf (buf, (isprint (c) ? "%c" : "%03o"), c);
          s = buf;
        }

      idx_t next_pad = pad_at (fields, i - 1, pad);
      முழு adjusted_width = pad_remaining - next_pad + width;
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

நிலையான bool
simple_strtoi (எழுத்து மாறிலி *s, எழுத்து மாறிலி **p, முழு *val)
{
  முழு sum;

  க்கு (sum = 0; c_isdigit (*s); s++)
    எனில் (ckd_mul (&sum, sum, 10) || ckd_add (&sum, sum, *s - '0'))
      திரும்பு false;
  *p = s;
  *val = sum;
  திரும்பு true;
}

/* Return a printf format for SPEC, given that remaining arguments are
   formats for int, long, long long, and intmax_t, respectively.  */
நிலையான எழுத்து மாறிலி *
ispec_to_format (எண்ணிக்கை size_spec size_spec,
                 எழுத்து மாறிலி *int_format,
                 எழுத்து மாறிலி *long_format,
                 எழுத்து மாறிலி *long_long_format,
                 எழுத்து மாறிலி *intmax_format)
{
  திரும்பு (LONG < INTMAX && size_spec == INTMAX ? intmax_format
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

நிலையான bool ATTRIBUTE_NONNULL ()
decode_one_format (எழுத்து மாறிலி *s_orig, எழுத்து மாறிலி *s, எழுத்து மாறிலி **next,
                   கட்டமைப்பு tspec *tspec)
{
  எண்ணிக்கை size_spec size_spec;
  முழு size;
  எண்ணிக்கை output_format fmt;
  print_function_type print_function;
  எழுத்து மாறிலி *p;
  எழுத்து c;
  முழு field_width;

  தேர்வு (*s)
    {
    நிலை 'd':
    நிலை 'o':
    நிலை 'u':
    நிலை 'x':
      c = *s;
      ++s;
      தேர்வு (*s)
        {
        நிலை 'C':
          ++s;
          size = அளவு (unsigned எழுத்து);
          நிறுத்து;

        நிலை 'S':
          ++s;
          size = அளவு (unsigned short முழு);
          நிறுத்து;

        நிலை 'I':
          ++s;
          size = அளவு (unsigned முழு);
          நிறுத்து;

        நிலை 'L':
          ++s;
          size = அளவு (unsigned long முழு);
          நிறுத்து;

        இயல்பு:
          எனில் (! simple_strtoi (s, &p, &size))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              திரும்பு false;
            }
          எனில் (p == s)
            size = அளவு (unsigned முழு);
          இல்லையெனில்
            {
              எனில் (countof (integral_type_size) <= size
                  || integral_type_size[size] == NO_SIZE)
                {
                  error (0, 0, _("invalid type string %s;\nthis system"
                                 " doesn't provide a %d-byte integral type"),
                         quote (s_orig), size);
                  திரும்பு false;
                }
              s = p;
            }
          நிறுத்து;
        }

      size_spec = integral_type_size[size];

      தேர்வு (c)
        {
        நிலை 'd':
          fmt = SIGNED_DECIMAL;
          field_width = INT_BITS_STRLEN_BOUND (CHAR_BIT * size - 1) + 1;
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_to_format (size_spec, "d", "ld", "lld", "jd"));
          நிறுத்து;

        நிலை 'o':
          fmt = OCTAL;
          field_width = (CHAR_BIT * size + 2) / 3;
          sprintf (tspec->fmt_string, "%%*.%d%s", field_width,
                   ispec_to_format (size_spec, "o", "lo", "llo", "jo"));
          நிறுத்து;

        நிலை 'u':
          fmt = UNSIGNED_DECIMAL;
          field_width = INT_BITS_STRLEN_BOUND (CHAR_BIT * size);
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_to_format (size_spec, "u", "lu", "llu", "ju"));
          நிறுத்து;

        நிலை 'x':
          fmt = HEXADECIMAL;
          field_width = (CHAR_BIT * size + 3) / 4;
          sprintf (tspec->fmt_string, "%%*.%d%s", field_width,
                   ispec_to_format (size_spec, "x", "lx", "llx", "jx"));
          நிறுத்து;

        இயல்பு:
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
           : (affirm (false), (print_function_type) வெற்று));
      நிறுத்து;

    நிலை 'f':
      fmt = FLOATING_POINT;
      ++s;
      தேர்வு (*s)
        {
        நிலை 'B':
          ++s;
          fmt = BFLOATING_POINT;
          size = அளவு (bfloat16);
          நிறுத்து;

        நிலை 'H':
          ++s;
          fmt = HFLOATING_POINT;
          size = அளவு (float16);
          நிறுத்து;

        நிலை 'F':
          ++s;
          size = அளவு (தசம);
          நிறுத்து;

        நிலை 'D':
          ++s;
          size = அளவு (இரட்டை);
          நிறுத்து;

        நிலை 'L':
          ++s;
          size = அளவு (long இரட்டை);
          நிறுத்து;

        இயல்பு:
          எனில் (! simple_strtoi (s, &p, &size))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              திரும்பு false;
            }
          எனில் (p == s)
            size = அளவு (இரட்டை);
          இல்லையெனில்
            {
              எனில் (countof (fp_type_size) <= size
                  || fp_type_size[size] == NO_SIZE
                  || (! FLOAT16_SUPPORTED && BF16_SUPPORTED
                      && size == அளவு (bfloat16)))
                {
                  error (0, 0,
                         _("invalid type string %s;\n"
                           "this system doesn't provide a %d-byte"
                           " floating point type"),
                         quote (s_orig), size);
                  திரும்பு false;
                }
              s = p;
            }
          நிறுத்து;
        }
      size_spec = fp_type_size[size];

      எனில் ((! FLOAT16_SUPPORTED && fmt == HFLOATING_POINT)
          || (! BF16_SUPPORTED && fmt == BFLOATING_POINT))
      {
        error (0, 0,
               _("this system doesn't provide a %s floating point type"),
               quote (s_orig));
        திரும்பு false;
      }

      {
        கட்டமைப்பு lconv மாறிலி *locale = localeconv ();
        idx_t decimal_point_len =
          (locale->decimal_point[0] ? strlen (locale->decimal_point) : 1);

        எனில் (size_spec == FLOAT_DOUBLE)
          {
            print_function = print_double;
            field_width = DBL_STRLEN_BOUND_L (decimal_point_len);
          }
        இல்லையெனில் எனில் (FLOAT_SINGLE < FLOAT_DOUBLE && size_spec == FLOAT_SINGLE)
          {
            print_function = print_float;
            field_width = FLT_STRLEN_BOUND_L (decimal_point_len);
          }
        இல்லையெனில் எனில் (FLOAT_HALF < FLOAT_SINGLE && size_spec == FLOAT_HALF)
          {
            print_function = fmt == BFLOATING_POINT
                             ? print_bfloat : print_halffloat;
            field_width = FLT_STRLEN_BOUND_L (decimal_point_len);
          }
        இல்லையெனில் எனில் (FLOAT_DOUBLE < FLOAT_LONG_DOUBLE
                 && size_spec == FLOAT_LONG_DOUBLE)
          {
            print_function = print_long_double;
            field_width = LDBL_STRLEN_BOUND_L (decimal_point_len);
          }
        இல்லையெனில்
          affirm (false);

        நிறுத்து;
      }

    நிலை 'a':
      ++s;
      fmt = NAMED_CHARACTER;
      size_spec = CHAR;
      print_function = print_named_ascii;
      field_width = 3;
      நிறுத்து;

    நிலை 'c':
      ++s;
      fmt = CHARACTER;
      size_spec = CHAR;
      print_function = print_ascii;
      field_width = 3;
      நிறுத்து;

    இயல்பு:
      error (0, 0, _("invalid character '%c' in type string %s"),
             *s, quote (s_orig));
      திரும்பு false;
    }

  tspec->size = size_spec;
  tspec->fmt = fmt;
  tspec->print_function = print_function;

  tspec->field_width = field_width;
  tspec->hexl_mode_trailer = (*s == 'z');
  எனில் (tspec->hexl_mode_trailer)
    s++;

  *next = s;
  திரும்பு true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM and the global string INPUT_FILENAME to the
   first one that can be successfully opened. Modify FILE_LIST to
   reference the next filename in the list.  A file name of "-" is
   interpreted as standard input.  If any file open fails, give an error
   message and return false.  */

நிலையான bool
open_next_file (வெறுமை)
{
  bool ok = true;

  செய்
    {
      உள்ளீடு_filename = *file_list;
      எனில் (உள்ளீடு_filename == வெற்று)
        திரும்பு ok;
      ++file_list;

      எனில் (streq (உள்ளீடு_filename, "-"))
        {
          உள்ளீடு_filename = _("standard input");
          in_stream = stdin;
          have_read_stdin = true;
          xset_இருமம்_mode (STDIN_FILENO, O_இருமம்);
        }
      இல்லையெனில்
        {
          in_stream = fopen (உள்ளீடு_filename, (O_இருமம் ? "rb" : "r"));
          எனில் (in_stream == வெற்று)
            {
              error (0, errno, "%s", quotef (உள்ளீடு_filename));
              ok = false;
            }
        }
    }
  போது (in_stream == வெற்று);

  எனில் (0 <= end_offset && !flag_dump_strings)
    setvbuf (in_stream, வெற்று, _IONBF, 0);

  திரும்பு ok;
}

/* Test whether there have been errors on in_stream, and close it if
   it is not standard input.  Return false if there has been an error
   on in_stream or stdout; return true otherwise.  This function will
   report more than one error only if both a read and a write error
   have occurred.  IN_ERRNO, if nonzero, is the error number
   corresponding to the most recent action for IN_STREAM.  */

நிலையான bool
check_and_close (முழு in_errno)
{
  bool ok = true;

  எனில் (in_stream != வெற்று)
    {
      எனில் (!ferror (in_stream))
        in_errno = 0;
      எனில் (streq (file_list[-1], "-"))
        clearerr (in_stream);
      இல்லையெனில் எனில் (fclose (in_stream) != 0 && !in_errno)
        in_errno = errno;
      எனில் (in_errno)
        {
          error (0, in_errno, "%s", quotef (உள்ளீடு_filename));
          ok = false;
        }

      in_stream = வெற்று;
    }

  எனில் (ferror (stdout))
    {
      error (0, 0, _("write error"));
      ok = false;
    }

  திரும்பு ok;
}

/* Decode the modern od format string S.  Append the decoded
   representation to the global array SPEC, reallocating SPEC if
   necessary.  Return true if S is valid.  */

நிலையான bool ATTRIBUTE_NONNULL ()
decode_format_string (எழுத்து மாறிலி *s)
{
  எழுத்து மாறிலி *s_orig = s;

  போது (*s != '\0')
    {
      எழுத்து மாறிலி *next;

      எனில் (n_specs_allocated <= n_specs)
        spec = xpalloc (spec, &n_specs_allocated, 1, -1, அளவு *spec);

      எனில் (! decode_one_format (s_orig, s, &next, &spec[n_specs]))
        திரும்பு false;

      affirm (s != next);
      s = next;
      ++n_specs;
    }

  திரும்பு true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM to position N_SKIP in the concatenation of
   those files.  If any file operation fails or if there are fewer than
   N_SKIP bytes in the combined input, give an error message and return
   false.  When possible, use seek rather than read operations to
   advance IN_STREAM.  */

நிலையான bool
skip (intmax_t n_skip)
{
  bool ok = true;
  முழு in_errno = 0;

  எனில் (n_skip == 0)
    திரும்பு true;

  போது (in_stream != வெற்று)	/* EOF.  */
    {
      கட்டமைப்பு stat file_stats;

      /* First try seeking.  For large offsets, this extra work is
         worthwhile.  If the offset is below some threshold it may be
         more efficient to move the pointer by reading.  There are two
         issues when trying to seek:
           - the file must be seekable.
           - before seeking to the specified position, make sure
             that the new position is in the current file.
             Try to do that by getting file's size using fstat.
             But that will work only for regular files.  */

      எனில் (fstat (fileno (in_stream), &file_stats) == 0)
        {
          /* The st_size field is valid for regular files.
             If the number of bytes left to skip is larger than
             the size of the current file, we can decrement n_skip
             and go on to the next file.  Skip this optimization also
             when st_size is no greater than the block size, because
             some kernels report nonsense small file sizes for
             proc-like file systems.  */
          எனில் (S_ISREG (file_stats.st_mode)
              && STP_BLKSIZE (&file_stats) < file_stats.st_size)
            {
              எனில் (file_stats.st_size < n_skip)
                n_skip -= file_stats.st_size;
              இல்லையெனில்
                {
                  எனில் (fseeko (in_stream, n_skip, SEEK_CUR) != 0)
                    {
                      in_errno = errno;
                      ok = false;
                    }
                  n_skip = 0;
                }
            }

          இல்லையெனில் எனில் (! (S_ISREG (file_stats.st_mode)
                      || S_TYPEISSHM (&file_stats)
                      || S_TYPEISTMO (&file_stats))
                   && fseeko (in_stream, n_skip, SEEK_CUR) == 0)
            n_skip = 0;

          /* If it's neither a seekable file with unusable size, nor a
             regular file so large it can't be in a proc-like file system,
             position the file pointer by reading.  */

          இல்லையெனில்
            {
              எழுத்து buf[BUFSIZ];
              idx_t n_bytes_read, n_bytes_to_read = BUFSIZ;

              போது (0 < n_skip)
                {
                  எனில் (n_skip < n_bytes_to_read)
                    n_bytes_to_read = n_skip;
                  n_bytes_read = fread (buf, 1, n_bytes_to_read, in_stream);
                  n_skip -= n_bytes_read;
                  எனில் (n_bytes_read != n_bytes_to_read)
                    {
                      எனில் (ferror (in_stream))
                        {
                          in_errno = errno;
                          ok = false;
                          n_skip = 0;
                          நிறுத்து;
                        }
                      எனில் (feof (in_stream))
                        நிறுத்து;
                    }
                }
            }

          எனில் (n_skip == 0)
            நிறுத்து;
        }

      இல்லையெனில்   /* cannot fstat() file */
        {
          error (0, errno, "%s", quotef (உள்ளீடு_filename));
          ok = false;
        }

      ok &= check_and_close (in_errno);

      ok &= open_next_file ();
    }

  எனில் (n_skip != 0)
    error (EXIT_FAILURE, 0, _("cannot skip past end of combined input"));

  திரும்பு ok;
}

நிலையான வெறுமை
format_address_none (MAYBE_UNUSED intmax_t address,
                     MAYBE_UNUSED எழுத்து c)
{
}

நிலையான வெறுமை
format_address_std (intmax_t address, எழுத்து c)
{
  எழுத்து buf[MAX_ADDRESS_LENGTH + 2];
  எழுத்து *p = buf + அளவு buf;
  எழுத்து மாறிலி *pbound;

  *--p = '\0';
  *--p = c;
  pbound = p - address_pad_len;

  /* Use a special case of the code for each base.  This is measurably
     faster than generic code.  */
  தேர்வு (address_base)
    {
    நிலை 8:
      செய்
        *--p = '0' + (address & 7);
      போது ((address >>= 3) != 0);
      நிறுத்து;

    நிலை 10:
      செய்
        *--p = '0' + (address % 10);
      போது ((address /= 10) != 0);
      நிறுத்து;

    நிலை 16:
      செய்
        *--p = "0123456789abcdef"[address & 15];
      போது ((address >>= 4) != 0);
      நிறுத்து;
    }

  போது (pbound < p)
    *--p = '0';

  fputs (p, stdout);
}

நிலையான வெறுமை
format_address_paren (intmax_t address, எழுத்து c)
{
  putchar ('(');
  format_address_std (address, ')');
  எனில் (c)
    putchar (c);
}

நிலையான வெறுமை
format_address_label (intmax_t address, எழுத்து c)
{
  format_address_std (address, ' ');

  intmax_t addr;
  எனில் (ckd_add (&addr, address, pseudo_offset))
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

நிலையான வெறுமை
write_block (intmax_t current_offset, idx_t n_bytes,
             எழுத்து மாறிலி *prev_block, எழுத்து மாறிலி *curr_block)
{
  நிலையான bool first = true;
  நிலையான bool prev_pair_equal = false;

  எனில் (abbreviate_duplicate_blocks
      && !first && n_bytes == bytes_per_block
      && memeq (prev_block, curr_block, bytes_per_block))
    {
      எனில் (prev_pair_equal)
        {
          /* The two preceding blocks were equal, and the current
             block is the same as the last one, so print nothing.  */
        }
      இல்லையெனில்
        {
          printf ("*\n");
          prev_pair_equal = true;
        }
    }
  இல்லையெனில்
    {
      prev_pair_equal = false;
      க்கு (idx_t i = 0; i < n_specs; i++)
        {
          முழு datum_width = width_bytes[spec[i].size];
          idx_t fields_per_block = bytes_per_block / datum_width;
          idx_t blank_fields = (bytes_per_block - n_bytes) / datum_width;
          எனில் (i == 0)
            format_address (current_offset, '\0');
          இல்லையெனில்
            print_n_spaces (address_pad_len);
          (*spec[i].print_function) (fields_per_block, blank_fields,
                                     curr_block, spec[i].fmt_string,
                                     spec[i].field_width, spec[i].pad_width);
          எனில் (spec[i].hexl_mode_trailer)
            {
              /* Space-pad out to full line width, then dump the trailer.  */
              முழு field_width = spec[i].field_width;
              க்கு (idx_t f = 0; f < blank_fields; f++)
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

நிலையான bool
read_char (முழு *c)
{
  bool ok = true;

  *c = EOF;

  போது (in_stream && (*c = getc (in_stream)) < 0)
    {
      ok &= check_and_close (errno);
      ok &= open_next_file ();
    }

  திரும்பு ok;
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

நிலையான bool
read_block (idx_t n, எழுத்து *block, idx_t *n_bytes_in_buffer)
{
  bool ok = true;

  affirm (0 < n && n <= bytes_per_block);

  *n_bytes_in_buffer = 0;

  போது (in_stream != வெற்று)	/* EOF.  */
    {
      idx_t n_needed = n - *n_bytes_in_buffer;
      idx_t n_read = fread (block + *n_bytes_in_buffer,
                            1, n_needed, in_stream);

      *n_bytes_in_buffer += n_read;

      எனில் (n_read == n_needed)
        நிறுத்து;

      ok &= check_and_close (errno);

      ok &= open_next_file ();
    }

  திரும்பு ok;
}

/* Return the least common multiple of the sizes associated
   with the format specs.  */

ATTRIBUTE_PURE
நிலையான முழு
get_lcm (வெறுமை)
{
  முழு l_c_m = 1;

  க்கு (idx_t i = 0; i < n_specs; i++)
    l_c_m = lcm (l_c_m, width_bytes[spec[i].size]);
  திரும்பு l_c_m;
}

/* Act like xstrtoimax (NPTR, NULL, BASE, VAL, VALID_SUFFIXES),
   except reject negative values, and *VAL may be set if
   LONGINT_INVALID is returned.  */
நிலையான strtol_error
xstr2nonneg (எழுத்து மாறிலி *restrict nptr, முழு base, intmax_t *val,
             எழுத்து மாறிலி *restrict valid_suffixes)
{
  strtol_error s_err = xstrtoimax (nptr, வெற்று, base, val, valid_suffixes);
  திரும்பு s_err != LONGINT_INVALID && *val < 0 ? LONGINT_INVALID : s_err;
}

/* If STR is a valid traditional offset specification with an optional
   leading '+', and can be represented in intmax_t, return true and
   set *OFFSET to the offset it denotes.  If it is valid but cannot be
   represented, diagnose the problem and exit.  Otherwise return false
   and possibly set *OFFSET.  */

நிலையான bool
parse_old_offset (எழுத்து *str, intmax_t *offset)
{
  /* Skip over any leading '+'.  */
  எழுத்து *s = str + (str[0] == '+');

  எனில் (! c_isdigit (s[0]))
    திரும்பு false;

  /* Determine the radix for S.  If there is a '.', followed first by
     optional 'b' or (undocumented) 'B' and then by end of string,
     it's decimal, otherwise, if the string begins with '0X' or '0x',
     it's hexadecimal, else octal.  */
  எழுத்து *dot = strchr (s, '.');
  எனில் (dot && dot[(dot[1] == 'b' || dot[1] == 'B') + 1])
    dot = வெற்று;
  முழு radix = dot ? 10 : s[0] == '0' && (s[1] == 'x' || s[1] == 'X') ? 16 : 8;

  எனில் (dot)
    {
      /* Temporarily remove the '.' from the decimal string.  */
      dot[0] = dot[1];
      dot[1] = '\0';
    }

  எண்ணிக்கை strtol_error s_err = xstr2nonneg (s, radix, offset, "bB");

  எனில் (dot)
    {
      /* Restore the decimal string's original value.  */
      dot[1] = dot[0];
      dot[0] = '.';
    }

  எனில் (s_err == LONGINT_OVERFLOW)
    error (EXIT_FAILURE, ERANGE, "%s", quotef (str));
  திரும்பு s_err == LONGINT_OK;
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

நிலையான bool
dump (வெறுமை)
{
  எழுத்து *block[2];
  bool idx = false;
  bool ok = true;
  idx_t n_bytes_read;

  block[0] = xinmalloc (1 + abbreviate_duplicate_blocks, bytes_per_block);
  block[1] = block[0] + abbreviate_duplicate_blocks * bytes_per_block;

  intmax_t current_offset = n_bytes_to_skip;

  செய்
    {
      intmax_t needed_bound
        = end_offset < 0 ? INTMAX_MAX : end_offset - current_offset;
      எனில் (needed_bound <= 0)
        {
          n_bytes_read = 0;
          நிறுத்து;
        }
      idx_t n_needed = MIN (bytes_per_block, needed_bound);
      ok &= read_block (n_needed, block[idx], &n_bytes_read);
      எனில் (n_bytes_read < bytes_per_block)
        நிறுத்து;
      affirm (n_bytes_read == bytes_per_block);
      write_block (current_offset, n_bytes_read,
                   block[!idx], block[idx]);
      எனில் (ferror (stdout))
        ok = false;
      current_offset += n_bytes_read;
      idx = !idx;
    }
  போது (ok);

  எனில் (n_bytes_read > 0)
    {
      முழு l_c_m = get_lcm ();

      /* Ensure zero-byte padding up to the smallest multiple of l_c_m that
         is at least as large as n_bytes_read.  */
      idx_t bytes_to_write = (n_bytes_read + l_c_m - 1
                              - (n_bytes_read + l_c_m - 1) % l_c_m);

      memset (block[idx] + n_bytes_read, 0, bytes_to_write - n_bytes_read);
      write_block (current_offset, n_bytes_read, block[!idx], block[idx]);
      current_offset += n_bytes_read;
    }

  எனில் (ok || current_offset)
    format_address (current_offset, '\n');

  எனில் (0 <= end_offset && end_offset <= current_offset)
    ok &= check_and_close (0);

  விடுவி (block[0]);

  திரும்பு ok;
}

/* STRINGS mode.  Find each "string constant" in the input.
   A string constant is a run of at least STRING_MIN
   printable characters terminated by a NUL or END_OFFSET.
   Based on a function written by Richard Stallman for a
   traditional version of od.  Return true if successful.  */

நிலையான bool
dump_strings (வெறுமை)
{
  idx_t bufsize = MAX (100, string_min + 1);
  எழுத்து *buf = ximalloc (bufsize);
  intmax_t address = n_bytes_to_skip;
  bool ok = true;

  போது (true)
    {
      idx_t i = 0;
      முழு c = 1;  /* Init to 1 so can distinguish if NUL read.  */

      எனில் (0 <= end_offset
          && (end_offset < string_min || end_offset - string_min < address))
        நிறுத்து;

      /* Store consecutive printable characters to BUF.  */
      போது (! (0 <= end_offset && end_offset <= address))
        {
          எனில் (i == bufsize - 1)
            buf = xpalloc (buf, &bufsize, 1, -1, அளவு *buf);
          ok &= read_char (&c);
          எனில் (c < 0)
            {
              விடுவி (buf);
              திரும்பு ok;
            }
          address++;
          buf[i++] = c;
          எனில் (c == '\0')
            நிறுத்து;		/* Print this string.  */
          எனில் (! isprint (c))
            {
              c = -1;		/* Give up on this string.  */
              நிறுத்து;
            }
        }

      எனில் (c < 0 || i - !c < string_min)
        தொடர்;

      buf[i] = 0;

      /* If we get here, the string is all printable, so print it.  */

      format_address (address - i, ' ');

      க்கு (i = 0; (c = buf[i]); i++)
        {
          தேர்வு (c)
            {
            நிலை '\a':
              fputs ("\\a", stdout);
              நிறுத்து;

            நிலை '\b':
              fputs ("\\b", stdout);
              நிறுத்து;

            நிலை '\f':
              fputs ("\\f", stdout);
              நிறுத்து;

            நிலை '\n':
              fputs ("\\n", stdout);
              நிறுத்து;

            நிலை '\r':
              fputs ("\\r", stdout);
              நிறுத்து;

            நிலை '\t':
              fputs ("\\t", stdout);
              நிறுத்து;

            நிலை '\v':
              fputs ("\\v", stdout);
              நிறுத்து;

            இயல்பு:
              putc (c, stdout);
            }
        }
      putchar ('\n');
    }

  /* We reach this point only if we search through
     (max_bytes_to_format - string_min) bytes before reaching EOF.  */

  விடுவி (buf);

  ok &= check_and_close (0);
  திரும்பு ok;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  முழு n_files;
  முழு l_c_m;
  idx_t desired_width = 0;
  bool modern = false;
  bool ok = true;
  idx_t width_per_block = 0;
  நிலையான எழுத்து மாறிலி multipliers[] = "bEGKkMmPQRTYZ0";

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

  போது (true)
    {
      intmax_t tmp;
      எண்ணிக்கை strtol_error s_err;
      முழு oi = -1;
      முழு c = getopt_long (argc, argv, short_options, long_options, &oi);
      எனில் (c == -1)
        நிறுத்து;

      தேர்வு (c)
        {
        நிலை 'A':
          modern = true;
          தேர்வு (optarg[0])
            {
            நிலை 'd':
              format_address = format_address_std;
              address_base = 10;
              address_pad_len = 7;
              நிறுத்து;
            நிலை 'o':
              format_address = format_address_std;
              address_base = 8;
              address_pad_len = 7;
              நிறுத்து;
            நிலை 'x':
              format_address = format_address_std;
              address_base = 16;
              address_pad_len = 6;
              நிறுத்து;
            நிலை 'n':
              format_address = format_address_none;
              address_pad_len = 0;
              நிறுத்து;
            இயல்பு:
              error (EXIT_FAILURE, 0,
                     _("invalid output address radix '%c';"
                       " it must be one character from [doxn]"),
                     optarg[0]);
              நிறுத்து;
            }
          நிறுத்து;

        நிலை 'j':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &n_bytes_to_skip, multipliers);
          எனில் (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          நிறுத்து;

        நிலை 'N':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &max_bytes_to_format, multipliers);
          எனில் (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          நிறுத்து;

        நிலை 'S':
          modern = true;
          எனில் (optarg == வெற்று)
            string_min = 3;
          இல்லையெனில்
            {
              s_err = xstr2nonneg (optarg, 0, &tmp, multipliers);
              /* The minimum string length + 1 must fit in idx_t,
                 since we may allocate a buffer of this size + 1.  */
              idx_t i;
              எனில் (s_err == LONGINT_OK && ckd_add (&i, tmp, 1))
                s_err = LONGINT_OVERFLOW;
              எனில் (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
              string_min = tmp;
            }
          flag_dump_strings = true;
          நிறுத்து;

        நிலை 't':
          modern = true;
          ok &= decode_format_string (optarg);
          நிறுத்து;

        நிலை 'v':
          modern = true;
          abbreviate_duplicate_blocks = false;
          நிறுத்து;

        நிலை TRADITIONAL_OPTION:
          traditional = true;
          நிறுத்து;

        நிலை ENDIAN_OPTION:
          தேர்வு (XARGMATCH ("--endian", optarg, endian_args, endian_types))
            {
            நிலை endian_big:
              உள்ளீடு_swap = BYTE_ORDER != BIG_ENDIAN;
              நிறுத்து;
            நிலை endian_little:
              உள்ளீடு_swap = BYTE_ORDER != LITTLE_ENDIAN;
              நிறுத்து;
            }
          நிறுத்து;

          /* The next several cases map the traditional format
             specification options to the corresponding modern format
             specs.  GNU od accepts any combination of old- and
             new-style options.  Format specification options accumulate.
             The obsolescent and undocumented formats are compatible
             with FreeBSD 4.10 od.  */

#define CASE_OLD_ARG(old_char,new_string)		\
        நிலை old_char:					\
          ok &= decode_format_string (new_string);	\
          நிறுத்து

          CASE_OLD_ARG ('a', "a");
          CASE_OLD_ARG ('b', "o1");
          CASE_OLD_ARG ('c', "c");
          CASE_OLD_ARG ('D', "u4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('d', "u2");
        நிலை 'F': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('e', "fD"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('f', "fF");
        நிலை 'X': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('H', "x4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('i', "dI");
        நிலை 'I': நிலை 'L': /* obsolescent and undocumented aliases */
          CASE_OLD_ARG ('l', "dL");
          CASE_OLD_ARG ('O', "o4"); /* obsolescent and undocumented */
        நிலை 'B': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('o', "o2");
          CASE_OLD_ARG ('s', "d2");
        நிலை 'h': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('x', "x2");

#undef CASE_OLD_ARG

        நிலை 'w':
          modern = true;
          எனில் (optarg == வெற்று)
            {
              desired_width = 32;
            }
          இல்லையெனில்
            {
              intmax_t w_tmp;
              s_err = xstr2nonneg (optarg, 10, &w_tmp, "");
              எனில் (s_err == LONGINT_OK)
                {
                  எனில் (ckd_add (&desired_width, w_tmp, 0))
                    s_err = LONGINT_OVERFLOW;
                  இல்லையெனில் எனில் (desired_width == 0)
                    s_err = LONGINT_INVALID;
                }
              எனில் (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
            }
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
          நிறுத்து;
        }
    }

  எனில் (!ok)
    திரும்பு EXIT_FAILURE;

  எனில் (flag_dump_strings && n_specs > 0)
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

  எனில் (!modern || traditional)
    {
      intmax_t o1;
      intmax_t o2;

      தேர்வு (n_files)
        {
        நிலை 1:
          எனில் ((traditional || argv[optind][0] == '+')
              && parse_old_offset (argv[optind], &o1))
            {
              n_bytes_to_skip = o1;
              --n_files;
              ++argv;
            }
          நிறுத்து;

        நிலை 2:
          எனில் ((traditional || argv[optind + 1][0] == '+'
               || c_isdigit (argv[optind + 1][0]))
              && parse_old_offset (argv[optind + 1], &o2))
            {
              எனில் (traditional && parse_old_offset (argv[optind], &o1))
                {
                  n_bytes_to_skip = o1;
                  flag_pseudo_start = true;
                  pseudo_start = o2;
                  argv += 2;
                  n_files -= 2;
                }
              இல்லையெனில்
                {
                  n_bytes_to_skip = o2;
                  --n_files;
                  argv[optind + 1] = argv[optind];
                  ++argv;
                }
            }
          நிறுத்து;

        நிலை 3:
          எனில் (traditional
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
          நிறுத்து;
        }

      எனில் (traditional && 1 < n_files)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
          error (0, 0, "%s",
                 _("compatibility mode supports at most one file"));
          usage (EXIT_FAILURE);
        }
    }

  எனில் (flag_pseudo_start)
    {
      எனில் (format_address == format_address_none)
        {
          address_base = 8;
          address_pad_len = 7;
          format_address = format_address_paren;
        }
      இல்லையெனில்
        format_address = format_address_label;
    }

  எனில் (0 <= max_bytes_to_format
      && ckd_add (&end_offset, n_bytes_to_skip, max_bytes_to_format))
    error (EXIT_FAILURE, 0, _("skip-bytes + read-bytes is too large"));

  எனில் (n_specs == 0)
    decode_format_string ("oS");

  எனில் (n_files > 0)
    {
      /* Set the global pointer FILE_LIST so that it
         references the first file-argument on the command-line.  */

      file_list = (எழுத்து மாறிலி *மாறிலி *) &argv[optind];
    }
  இல்லையெனில்
    {
      /* No files were listed on the command line.
         Set the global pointer FILE_LIST so that it
         references the null-terminated list of one name: "-".  */

      file_list = default_file_list;
    }

  /* open the first input file */
  ok = open_next_file ();
  எனில் (in_stream == வெற்று)
    செல் cleanup;

  /* skip over any unwanted header bytes */
  ok &= skip (n_bytes_to_skip);
  எனில் (in_stream == வெற்று)
    செல் cleanup;

  pseudo_offset = (flag_pseudo_start ? pseudo_start - n_bytes_to_skip : 0);

  /* Compute output block length.  */
  l_c_m = get_lcm ();

  எனில் (desired_width != 0)
    {
      எனில் (desired_width % l_c_m == 0)
        bytes_per_block = desired_width;
      இல்லையெனில்
        {
          error (0, 0, _("warning: invalid width %td; using %d instead"),
                 desired_width, l_c_m);
          bytes_per_block = l_c_m;
        }
    }
  இல்லையெனில்
    {
      எனில் (l_c_m < DEFAULT_BYTES_PER_BLOCK)
        bytes_per_block = (DEFAULT_BYTES_PER_BLOCK
                           - DEFAULT_BYTES_PER_BLOCK % l_c_m);
      இல்லையெனில்
        bytes_per_block = l_c_m;
    }

  /* Compute padding necessary to align output block.  */
  க்கு (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_per_block = bytes_per_block / width_bytes[spec[i].size];
      எனில் (pad_at_overflow (fields_per_block))
        error (EXIT_FAILURE, 0, _("%td is too large"), desired_width);
      idx_t block_width = (spec[i].field_width + 1) * fields_per_block;
      எனில் (width_per_block < block_width)
        width_per_block = block_width;
    }
  க்கு (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_per_block = bytes_per_block / width_bytes[spec[i].size];
      idx_t block_width = spec[i].field_width * fields_per_block;
      spec[i].pad_width = width_per_block - block_width;
    }

#ifdef DEBUG
  printf ("lcm=%d, width_per_block=%td\n", l_c_m, width_per_block);
  க்கு (idx_t i = 0; i < n_specs; i++)
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

  எனில் (have_read_stdin && fclose (stdin) < 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
