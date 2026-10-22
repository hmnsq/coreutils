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

#ضمن <config.h>

#ضمن <endian.h>
#ضمن <float.h>
#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>
#ضمن "system.h"
#ضمن "argmatch.h"
#ضمن "assure.h"
#ضمن "c-ctype.h"
#ضمن "ftoastr.h"
#ضمن "quote.h"
#ضمن "stat-size.h"
#ضمن "xbinary-io.h"
#ضمن "xprintf.h"
#ضمن "xstrtol.h"
#ضمن "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "od"

#تعریف_کرو AUTHORS proper_name ("Jim Meyering")

/* The default number of input bytes per output line.  */
#تعریف_کرو DEFAULT_بائٹاں_فی_بلاک 16

#إذا FLOAT16_SUPPORTED
  /* Available since clang 6 (2018), and gcc 7 (2017).  */
  تعريفنوع _Float16 float16;
#وإلا
# define FLOAT16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  تعريفنوع عائم float16;
#جے_انت

#إذا BF16_SUPPORTED
  /* Available since clang 11 (2020), and gcc 13 (2023). */
  تعريفنوع __bf16 bfloat16;
#وإلا
# define BF16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  تعريفنوع عائم bfloat16;
#جے_انت

تعداد ناپ_spec
  {
    NO_ناپ = 0,
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
    N_ناپ_SPECS
  };

تعداد output_format
  {
    SIGNED_DECIMAL,
    UNSIGNED_DECIMAL,
    OCTAL,
    HEXADECIMAL,
    FLOATING_نقطہ,
    HFLOATING_نقطہ,
    BFLOATING_نقطہ,
    NAMED_CHARACTER,
    CHARACTER
  };

تعداد { MAX_INTEGRAL_TYPE_چوڑائی = UINTMAX_چوڑائی };

/* The maximum number of bytes needed for a format string, including
   the trailing nul.  Each format string expects a variable amount of
   padding (guaranteed to be at least 1 plus the field width), then an
   element that will be formatted in the field.  */
تعداد
  {
    FMT_بائٹاں_ALLOCATED =
      حجم "%*.%dlld" - حجم "%d" + INT_STRLEN_BOUND (صحيح) + 1
  };

/* Ensure that our choice for FMT_BYTES_ALLOCATED is reasonable.  */
static_assert (MAX_INTEGRAL_TYPE_چوڑائی <= 3 * 99);

/* The type of a print function.  FIELDS is the number of fields per
   line, BLANK is the number of fields to leave blank.  WIDTH is width
   of one field, excluding leading space, and PAD is total pad to
   divide among FIELDS.  PAD is at least as large as FIELDS.  */
تعريفنوع فراغ (*print_function_type)
  (idx_t fields, idx_t blank, فراغ ثابت *انکڑے,
   حرف ثابت *fmt, صحيح چوڑائی, idx_t pad);

/* Each output format specification (from '-t spec' or from
   old-style options) is represented by one of these structures.  */
بنية tspec
  {
    تعداد output_format fmt;
    تعداد ناپ_spec ناپ; /* Type of input object.  */
    print_function_type print_function;
    حرف fmt_string[FMT_بائٹاں_ALLOCATED]; /* Of the style "%*d".  */
    bool hexl_منوال_trailer;
    صحيح میدان_چوڑائی; /* Minimum width of a field, excluding leading space.  */
    idx_t pad_چوڑائی; /* Total padding to be divided among fields.  */
  };

/* Convert enum size_spec to the size of the named type.  */
ساكن ثابت صحيح چوڑائی_بائٹاں[] =
{
  -1,
  حجم (بنانشانی حرف),
#إذا UCHAR_MAX < USHRT_MAX
  حجم (بنانشانی چھوٹا صحيح),
#جے_انت
#إذا USHRT_MAX < UINT_MAX
  حجم (بنانشانی صحيح),
#جے_انت
#إذا UINT_MAX < ULONG_MAX
  حجم (بنانشانی لما صحيح),
#جے_انت
#إذا ULONG_MAX < ULLONG_MAX
  حجم (بنانشانی لما لما صحيح),
#جے_انت
#إذا ULLONG_MAX < UINTMAX_MAX
  حجم (uintmax_t),
#جے_انت
#إذا BF16_SUPPORTED
  حجم (bfloat16),
#وإلاإذا FLOAT16_SUPPORTED
  حجم (float16),
#جے_انت
#إذا FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
  حجم (عائم),
#جے_انت
  حجم (مزدوج),
#إذا DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
  حجم (لما مزدوج),
#جے_انت
};

/* Ensure that for each member of 'enum size_spec' there is an
   initializer in the width_bytes array.  */
static_assert (countof (چوڑائی_بائٹاں) == N_ناپ_SPECS);

/* Names for some non-printing characters.  */
ساكن حرف ثابت charname[33][4] =
{
  "nul", "soh", "stx", "etx", "eot", "enq", "ack", "bel",
  "bs", "ht", "nl", "vt", "ff", "cr", "so", "si",
  "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb",
  "can", "em", "sub", "esc", "fs", "gs", "rs", "us",
  "sp"
};

/* Address base (8, 10 or 16).  */
ساكن صحيح پتہ_ادھار = 8;

/* The number of octal digits required to represent the largest
   address value.  */
تعداد { MAX_پتہ_لمائی = ((INTMAX_چوڑائی - 1) / 3
                             + ((INTMAX_چوڑائی - 1) % 3 != 0)) };

/* Width of a normal address.  */
ساكن صحيح پتہ_pad_len = 7;

/* Minimum length when detecting --strings.  */
ساكن idx_t string_min;

/* True when in --strings mode.  */
ساكن bool flag_dump_لڑیاں;

/* True if we should recognize the older non-option arguments
   that specified at most one file and optional arguments specifying
   offset and pseudo-start address.  */
ساكن bool traditional;

/* True if an old-style 'pseudo-address' was specified.  */
ساكن bool flag_pseudo_شروع;

/* The difference between the old-style pseudo starting address and
   the number of bytes to skip.  */
ساكن intmax_t pseudo_وتھ;

/* Function that accepts an address and an optional following char,
   and prints the address and char to stdout.  */
ساكن فراغ format_پتہ_std (intmax_t, حرف);
ساكن فراغ (*format_پتہ) (intmax_t, حرف) = format_پتہ_std;

/* The number of input bytes to skip before formatting and writing.  */
ساكن intmax_t n_بائٹاں_منزل_skip;

/* The offset of the first byte after the last byte to be formatted.
   If negative, there is no limit.  */
ساكن intmax_t سرا_وتھ = -1;

/* When true and two or more consecutive blocks are equal, format
   only the first block and output an asterisk alone on the following
   line to indicate that identical blocks have been elided.  */
ساكن bool abbreviate_duplicate_بلاکاں = true;

/* An array of specs describing how to format each input block.  */
ساكن بنية tspec *spec;

/* The number of format specs.  */
ساكن idx_t n_specs;

/* The allocated length of SPEC.  */
ساكن idx_t n_specs_allocated;

/* The number of input bytes formatted per output line.  It must be
   a multiple of the least common multiple of the sizes associated with
   the specified output types.  It should be as large as possible, but
   no larger than 16 -- unless specified with the -w option.  */
ساكن idx_t بائٹاں_فی_بلاک;

/* Human-readable representation of *file_list (for error messages).
   It differs from file_list[-1] only when file_list[-1] is "-".  */
ساكن حرف ثابت *داخلہ_filename;

/* A null-terminated list of the file-arguments from the command line.  */
ساكن حرف ثابت *ثابت *file_list;

/* Initializer for file_list if no file-arguments
   were specified on the command line.  */
ساكن حرف ثابت *ثابت default_file_list[] = {"-", عدم};

/* The input stream associated with the current file.  */
ساكن FILE *وچ_دھارا;

/* If true, at least one of the files we read was standard input.  */
ساكن bool have_read_stdin;

/* Map the size in bytes to a type identifier.
   When two types have the same machine layout:
     - Prefer unsigned int to other types, as its format is shorter.
     - Prefer unsigned long to higher-ranked types, as it is older.
     - Prefer uintmax_t to unsigned long long int; this wins if %lld
       does not work but %jd does (e.g., MS-Windows).  */
ساكن تعداد ناپ_spec ثابت integral_type_ناپ[] =
  {
#إذا UCHAR_MAX < USHRT_MAX
    [حجم (بنانشانی حرف)] = CHAR,
#جے_انت
#إذا USHRT_MAX < UINT_MAX
    [حجم (بنانشانی چھوٹا صحيح)] = SHORT,
#جے_انت
    [حجم (بنانشانی صحيح)] = INT,
#إذا UINT_MAX < ULONG_MAX
    [حجم (بنانشانی لما صحيح)] = LONG,
#جے_انت
#إذا ULONG_MAX < ULLONG_MAX && ULLONG_MAX < UINTMAX_MAX
    [حجم (بنانشانی لما لما صحيح)] = LONG_LONG,
#جے_انت
#إذا ULONG_MAX < UINTMAX_MAX
    [حجم (uintmax_t)] = INTMAX,
#جے_انت
  };

/* Map the size in bytes to a floating type identifier.
   When two types have the same machine layout:
     - Prefer double to the other types, as its format is shorter.  */
ساكن تعداد ناپ_spec ثابت fp_type_ناپ[] =
  {
#إذا FLOAT16_SUPPORTED
    [حجم (float16)] = FLOAT_HALF,
#وإلاإذا BF16_SUPPORTED
    [حجم (bfloat16)] = FLOAT_HALF,
#جے_انت
#إذا FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
    [حجم (عائم)] = FLOAT_SINGLE,
#جے_انت
    [حجم (مزدوج)] = FLOAT_DOUBLE,
#إذا DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
    [حجم (لما مزدوج)] = FLOAT_LONG_DOUBLE,
#جے_انت
  };

/* Use native endianness by default.  */
ساكن bool داخلہ_وٹاندرا;

ساكن حرف ثابت short_options[] = "A:aBbcDdeFfHhIij:LlN:OoS:st:vw::Xx";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  TRADITIONAL_OPTION = CHAR_MAX + 1,
  ENDIAN_OPTION,
};

تعداد endian_type
{
  endian_little,
  endian_big
};

ساكن حرف ثابت *ثابت endian_دلیلاں[] =
{
  "little", "big", عدم
};

ساكن تعداد endian_type ثابت endian_types[] =
{
  endian_little, endian_big
};

ساكن بنية option ثابت long_options[] =
{
  {"skip-bytes", required_دلیل, عدم, 'j'},
  {"address-radix", required_دلیل, عدم, 'A'},
  {"read-bytes", required_دلیل, عدم, 'N'},
  {"format", required_دلیل, عدم, 't'},
  {"output-duplicates", no_دلیل, عدم, 'v'},
  {"strings", optional_دلیل, عدم, 'S'},
  {"traditional", no_دلیل, عدم, TRADITIONAL_OPTION},
  {"width", optional_دلیل, عدم, 'w'},
  {"endian", required_دلیل, عدم, ENDIAN_OPTION },

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
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [-abcdfilosx]... [FILE] [[+]وتھ_2[.][b]]\n\
  or:  %s --traditional [OPTION]... [FILE] [[+]وتھ_2[.][b] [+][LABEL][.][b]]\n\
"),
              پروگرام_name, پروگرام_name, پروگرام_name);
      fputs (_("\n\
Write an unambiguous representation, octal بائٹاں by افتراضي,\n\
دا FILE منزل standard output.  With ہور than one FILE دلیل,\n\
concatenate them وچ the listed ترتیب منزل form the داخلہ.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
If پہلا and دوجا call formats both apply, the دوجا format is assumed\n\
إذا the آخری عملیہ_2 begins with + or (إذا there are 2 operands) a digit.\n\
An وتھ_2 عملیہ_2 means -j وتھ_2.  LABEL is the pseudo-پتہ\n\
at پہلا byte printed, incremented when dump is progressing.\n\
For وتھ_2 and LABEL, a 0x or 0X اگیتر indicates hexadecimal;\n\
suffixes may be . لكل octal and b لكل multiply by 512.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --پتہ-radix=RADIX\n\
         output format لكل file offsets;\n\
         RADIX is one دا [doxn], لكل Decimal, Octal, Hex or None\n\
"));
      oputs (_("\
      --endian={big|little}\n\
         وٹاندرا داخلہ بائٹاں according the specified ترتیب\n\
"));
      oputs (_("\
  -j, --skip-بائٹاں=بائٹاں_2\n\
         skip بائٹاں_2 داخلہ بائٹاں پہلا\n\
"));
      oputs (_("\
  -N, --اقرأ-بائٹاں=بائٹاں_2\n\
         حد_بندی dump منزل بائٹاں_2 داخلہ بائٹاں\n\
"));
      oputs (_("\
  -S بائٹاں_2, --لڑیاں[=بائٹاں_2]\n\
         show only NUL terminated لڑیاں\n\
         دا at گھٹ_توں_گھٹ بائٹاں_2 (افتراضي 3) printable characters\n\
"));
      oputs (_("\
  -t, --format=TYPE\n\
         select output format or formats\n\
"));
      oputs (_("\
  -v, --output-duplicates\n\
         افعل not use * منزل mark سطر suppression\n\
"));
      oputs (_("\
  -w[بائٹاں_2], --چوڑائی[=بائٹاں_2]\n\
         output بائٹاں_2 بائٹاں فی output سطر;\n\
         32 is implied when بائٹاں_2 is not specified\n\
"));
      oputs (_("\
      --traditional\n\
         accept arguments وچ تیجا form above\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
\n\
Traditional format specifications may be intermixed; they accumulate:\n\
"), stdout);
      oputs (_("\
  -a   same as -t a,  select named characters, ignoring بلند-ترتیب ثنائی_ہندسہ\n\
"));
      oputs (_("\
  -b   same as -t o1, select octal بائٹاں\n\
"));
      oputs (_("\
  -c   same as -t c,  select printable characters or backslash escapes\n\
"));
      oputs (_("\
  -d   same as -t u2, select بنانشانی decimal 2-byte units\n\
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
TYPE is made اتے دا one or ہور دا these specifications:\n\
  a          named character, ignoring بلند-ترتیب ثنائی_ہندسہ\n\
  c          printable character or backslash escape\n\
"), stdout);
      fputs (_("\
  d[ناپ_2]    نشانیوالا decimal, ناپ_2 بائٹاں فی integer\n\
  f[ناپ_2]    floating نقطہ, ناپ_2 بائٹاں فی عائم\n\
  o[ناپ_2]    octal, ناپ_2 بائٹاں فی integer\n\
  u[ناپ_2]    بنانشانی decimal, ناپ_2 بائٹاں فی integer\n\
  x[ناپ_2]    hexadecimal, ناپ_2 بائٹاں فی integer\n\
"), stdout);
      fputs (_("\
\n\
ناپ_2 is a نمبر.  For TYPE وچ [doux], ناپ_2 may also be C لكل\n\
حجم(حرف), S لكل حجم(چھوٹا), I لكل حجم(صحيح) or L لكل\n\
حجم(لما).  If TYPE is f, ناپ_2 may also be B لكل Brain 16 ثنائی_ہندسہ,\n\
H لكل Half precision عائم, F لكل حجم(عائم), D لكل حجم(مزدوج),\n\
or L لكل حجم(لما مزدوج).\n\
"), stdout);
      fputs (_("\
\n\
Adding a z suffix منزل any type displays printable characters at the سرا دا\n\
each output سطر.\n\
"), stdout);
      fputs (_("\
\n\
\n\
بائٹاں_2 is hex with 0x or 0X اگیتر, and may have a multiplier suffix:\n\
  b    512\n\
  KB   1000\n\
  K    1024\n\
  MB   1000*1000\n\
  M    1024*1024\n\
and so on لكل G, T, P, E, Z, Y, R, Q.\n\
ثنائی_2 prefixes can be ورتیا, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

/* Define the print functions.  */

/* Print N spaces, where 0 <= N.
   Do not rely on printf ("%*s", N, "") as N may exceed INT_MAX.  */
ساكن فراغ
print_n_spaces (intmax_t n)
{
  لكل (; 0 < n; n--)
    حرف_لکھو (' ');
}

/* If there are FIELDS fields, return the total padding up to the
   start of field I, where I < FIELDS.  PAD is the total padding for
   all fields.  The result equals (PAD * I) / FIELDS, except it does
   not suffer from internal overflow.  */
ساكن idx_t
pad_at (idx_t fields, idx_t i, idx_t pad)
{
  /* This implementation assumes that (FIELDS - 1)^2 does not overflow
     intmax_t, an assumption checked by pad_at_overflow.  */
  intmax_t m = pad % fields;
  أرجع pad / fields * i + m * i / fields;
}
ساكن bool
pad_at_حد_توں_ودھ (idx_t fields)
{
  intmax_t ضرب_حاصل;
  أرجع ckd_mul (&ضرب_حاصل, fields - 1, fields - 1);
}

#تعریف_کرو PRINT_FIELDS(N, T, FMT_STRING_DECL, ACTION)                     \
ساكن فراغ                                                             \
N (idx_t fields, idx_t blank, فراغ ثابت *بلاک,			\
   FMT_STRING_DECL, صحيح چوڑائی, idx_t pad)				\
{                                                                       \
  T ثابت *p = بلاک;                                                   \
  idx_t pad_بچیا = pad;						\
  لكل (idx_t i = fields; blank < i; i--)				\
    {                                                                   \
      idx_t اگلا_pad = pad_at (fields, i - 1, pad);			\
      صحيح adjusted_چوڑائی = pad_بچیا - اگلا_pad + چوڑائی;            \
      T x;                                                              \
      إذا (داخلہ_وٹاندرا && حجم (T) > 1)                                 \
        {                                                               \
          اتحاد {                                                       \
            T x;                                                        \
            حرف b[حجم (T)];                                         \
          } u;                                                          \
          لكل (idx_t j = 0; j < حجم (T); j++)			\
            u.b[j] = ((حرف ثابت *) p)[حجم (T) - 1 - j];            \
          x = u.x;                                                      \
        }                                                               \
      وإلا                                                              \
        x = *p;                                                         \
      p++;                                                              \
      ACTION;                                                           \
      pad_بچیا = اگلا_pad;                                         \
    }                                                                   \
}

#تعریف_کرو PRINT_TYPE(N, T)                                                \
  PRINT_FIELDS (N, T, حرف ثابت *fmt_string,                           \
                xprintf (fmt_string, adjusted_چوڑائی, x))

#تعریف_کرو PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
  PRINT_FIELDS (N, T, MAYBE_بن_ورتیا حرف ثابت *fmt_string,              \
                حرف buf[BUFSIZE];                                      \
                FTOASTR (buf, حجم buf, 0, 0, x);                     \
                xprintf ("%*s", adjusted_چوڑائی, buf))

PRINT_TYPE (print_s_char, نشانیوالا حرف)
PRINT_TYPE (print_char, بنانشانی حرف)
PRINT_TYPE (print_s_short, چھوٹا صحيح)
PRINT_TYPE (print_short, بنانشانی چھوٹا صحيح)
PRINT_TYPE (print_int, بنانشانی صحيح)
PRINT_TYPE (print_long, بنانشانی لما صحيح)
PRINT_TYPE (print_long_long, بنانشانی لما لما صحيح)
PRINT_TYPE (print_intmax, uintmax_t)

PRINT_FLOATTYPE (print_bfloat, bfloat16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_halffloat, float16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_float, عائم, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_double, مزدوج, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_long_double, لما مزدوج, ldtoastr, LDBL_BUFSIZE_BOUND)

#تعریف_رد PRINT_TYPE
#تعریف_رد PRINT_FLOATTYPE

ساكن فراغ
dump_hexl_منوال_trailer (idx_t n_بائٹاں, حرف ثابت *بلاک)
{
  fputs ("  >", stdout);
  لكل (idx_t i = n_بائٹاں; i > 0; i--)
    {
      بنانشانی حرف c = *بلاک++;
      بنانشانی حرف c2 = (isprint (c) ? c : '.');
      حرف_لکھو (c2);
    }
  حرف_لکھو ('<');
}

ساكن فراغ
print_named_ascii (idx_t fields, idx_t blank, فراغ ثابت *بلاک,
                   MAYBE_بن_ورتیا حرف ثابت *بن_ورتیا_fmt_string,
                   صحيح چوڑائی, idx_t pad)
{
  بنانشانی حرف ثابت *p = بلاک;
  idx_t pad_بچیا = pad;
  لكل (idx_t i = fields; blank < i; i--)
    {
      صحيح masked_c = *p++ & 0x7f;
      حرف ثابت *s;
      حرف buf[2];

      إذا (masked_c == 127)
        s = "del";
      وإلا إذا (masked_c <= 040)
        s = charname[masked_c];
      وإلا
        {
          buf[0] = masked_c;
          buf[1] = 0;
          s = buf;
        }

      idx_t اگلا_pad = pad_at (fields, i - 1, pad);
      صحيح adjusted_چوڑائی = pad_بچیا - اگلا_pad + چوڑائی;
      xprintf ("%*s", adjusted_چوڑائی, s);
      pad_بچیا = اگلا_pad;
    }
}

ساكن فراغ
print_ascii (idx_t fields, idx_t blank, فراغ ثابت *بلاک,
             MAYBE_بن_ورتیا حرف ثابت *بن_ورتیا_fmt_string, صحيح چوڑائی,
             idx_t pad)
{
  بنانشانی حرف ثابت *p = بلاک;
  idx_t pad_بچیا = pad;
  لكل (idx_t i = fields; blank < i; i--)
    {
      بنانشانی حرف c = *p++;
      حرف ثابت *s;
      حرف buf[4];

      بدل (c)
        {
        حالة '\0':
          s = "\\0";
          اكسر;

        حالة '\a':
          s = "\\a";
          اكسر;

        حالة '\b':
          s = "\\b";
          اكسر;

        حالة '\f':
          s = "\\f";
          اكسر;

        حالة '\n':
          s = "\\n";
          اكسر;

        حالة '\r':
          s = "\\r";
          اكسر;

        حالة '\t':
          s = "\\t";
          اكسر;

        حالة '\v':
          s = "\\v";
          اكسر;

        افتراضي:
          sprintf (buf, (isprint (c) ? "%c" : "%03o"), c);
          s = buf;
        }

      idx_t اگلا_pad = pad_at (fields, i - 1, pad);
      صحيح adjusted_چوڑائی = pad_بچیا - اگلا_pad + چوڑائی;
      xprintf ("%*s", adjusted_چوڑائی, s);
      pad_بچیا = اگلا_pad;
    }
}

/* Convert a null-terminated (possibly zero-length) string S to an
   int value.  If S points to a non-digit set *P to S,
   *VAL to 0, and return true.  Otherwise, accumulate the integer value of
   the string of digits.  If the string of digits represents a value
   larger than INT_MAX, don't modify *VAL or *P and return false.
   Otherwise, advance *P to the first non-digit after S, set *VAL to
   the result of the conversion and return true.  */

ساكن bool
simple_strtoi (حرف ثابت *s, حرف ثابت **p, صحيح *val)
{
  صحيح جوڑ;

  لكل (جوڑ = 0; c_isdigit (*s); s++)
    إذا (ckd_mul (&جوڑ, جوڑ, 10) || ckd_جوڑو (&جوڑ, جوڑ, *s - '0'))
      أرجع false;
  *p = s;
  *val = جوڑ;
  أرجع true;
}

/* Return a printf format for SPEC, given that remaining arguments are
   formats for int, long, long long, and intmax_t, respectively.  */
ساكن حرف ثابت *
ispec_منزل_format (تعداد ناپ_spec ناپ_spec,
                 حرف ثابت *int_format,
                 حرف ثابت *long_format,
                 حرف ثابت *long_long_format,
                 حرف ثابت *intmax_format)
{
  أرجع (LONG < INTMAX && ناپ_spec == INTMAX ? intmax_format
          : LONG < LONG_LONG && ناپ_spec == LONG_LONG ? long_long_format
          : INT < LONG && ناپ_spec == LONG ? long_format
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

ساكن bool ATTRIBUTE_NONNULL ()
decode_one_format (حرف ثابت *s_orig, حرف ثابت *s, حرف ثابت **اگلا,
                   بنية tspec *tspec)
{
  تعداد ناپ_spec ناپ_spec;
  صحيح ناپ;
  تعداد output_format fmt;
  print_function_type print_function;
  حرف ثابت *p;
  حرف c;
  صحيح میدان_چوڑائی;

  بدل (*s)
    {
    حالة 'd':
    حالة 'o':
    حالة 'u':
    حالة 'x':
      c = *s;
      ++s;
      بدل (*s)
        {
        حالة 'C':
          ++s;
          ناپ = حجم (بنانشانی حرف);
          اكسر;

        حالة 'S':
          ++s;
          ناپ = حجم (بنانشانی چھوٹا صحيح);
          اكسر;

        حالة 'I':
          ++s;
          ناپ = حجم (بنانشانی صحيح);
          اكسر;

        حالة 'L':
          ++s;
          ناپ = حجم (بنانشانی لما صحيح);
          اكسر;

        افتراضي:
          إذا (! simple_strtoi (s, &p, &ناپ))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              أرجع false;
            }
          إذا (p == s)
            ناپ = حجم (بنانشانی صحيح);
          وإلا
            {
              إذا (countof (integral_type_ناپ) <= ناپ
                  || integral_type_ناپ[ناپ] == NO_ناپ)
                {
                  error (0, 0, _("invalid type string %s;\nthis system"
                                 " doesn't provide a %d-byte integral type"),
                         quote (s_orig), ناپ);
                  أرجع false;
                }
              s = p;
            }
          اكسر;
        }

      ناپ_spec = integral_type_ناپ[ناپ];

      بدل (c)
        {
        حالة 'd':
          fmt = SIGNED_DECIMAL;
          میدان_چوڑائی = INT_بٹ_STRLEN_BOUND (CHAR_BIT * ناپ - 1) + 1;
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_منزل_format (ناپ_spec, "d", "ld", "lld", "jd"));
          اكسر;

        حالة 'o':
          fmt = OCTAL;
          میدان_چوڑائی = (CHAR_BIT * ناپ + 2) / 3;
          sprintf (tspec->fmt_string, "%%*.%d%s", میدان_چوڑائی,
                   ispec_منزل_format (ناپ_spec, "o", "lo", "llo", "jo"));
          اكسر;

        حالة 'u':
          fmt = UNSIGNED_DECIMAL;
          میدان_چوڑائی = INT_بٹ_STRLEN_BOUND (CHAR_BIT * ناپ);
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_منزل_format (ناپ_spec, "u", "lu", "llu", "ju"));
          اكسر;

        حالة 'x':
          fmt = HEXADECIMAL;
          میدان_چوڑائی = (CHAR_BIT * ناپ + 3) / 4;
          sprintf (tspec->fmt_string, "%%*.%d%s", میدان_چوڑائی,
                   ispec_منزل_format (ناپ_spec, "x", "lx", "llx", "jx"));
          اكسر;

        افتراضي:
          affirm (false);
        }

      /* Prefer INT, prefer LONG to longer types,
         and prefer INTMAX to LONG_LONG.  */
      print_function
        = (ناپ_spec == INT
           ? print_int
           : SHORT < INT && ناپ_spec == SHORT
           ? (fmt == SIGNED_DECIMAL ? print_s_short : print_short)
           : CHAR < SHORT && ناپ_spec == CHAR
           ? (fmt == SIGNED_DECIMAL ? print_s_char : print_char)
           : INT < LONG && ناپ_spec == LONG
           ? print_long
           : LONG < INTMAX && ناپ_spec == INTMAX
           ? print_intmax
           : LONG < LONG_LONG && LONG_LONG < INTMAX && ناپ_spec == LONG_LONG
           ? print_long_long
           : (affirm (false), (print_function_type) عدم));
      اكسر;

    حالة 'f':
      fmt = FLOATING_نقطہ;
      ++s;
      بدل (*s)
        {
        حالة 'B':
          ++s;
          fmt = BFLOATING_نقطہ;
          ناپ = حجم (bfloat16);
          اكسر;

        حالة 'H':
          ++s;
          fmt = HFLOATING_نقطہ;
          ناپ = حجم (float16);
          اكسر;

        حالة 'F':
          ++s;
          ناپ = حجم (عائم);
          اكسر;

        حالة 'D':
          ++s;
          ناپ = حجم (مزدوج);
          اكسر;

        حالة 'L':
          ++s;
          ناپ = حجم (لما مزدوج);
          اكسر;

        افتراضي:
          إذا (! simple_strtoi (s, &p, &ناپ))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              أرجع false;
            }
          إذا (p == s)
            ناپ = حجم (مزدوج);
          وإلا
            {
              إذا (countof (fp_type_ناپ) <= ناپ
                  || fp_type_ناپ[ناپ] == NO_ناپ
                  || (! FLOAT16_SUPPORTED && BF16_SUPPORTED
                      && ناپ == حجم (bfloat16)))
                {
                  error (0, 0,
                         _("invalid type string %s;\n"
                           "this system doesn't provide a %d-byte"
                           " floating point type"),
                         quote (s_orig), ناپ);
                  أرجع false;
                }
              s = p;
            }
          اكسر;
        }
      ناپ_spec = fp_type_ناپ[ناپ];

      إذا ((! FLOAT16_SUPPORTED && fmt == HFLOATING_نقطہ)
          || (! BF16_SUPPORTED && fmt == BFLOATING_نقطہ))
      {
        error (0, 0,
               _("this system doesn't provide a %s floating point type"),
               quote (s_orig));
        أرجع false;
      }

      {
        بنية lconv ثابت *locale = localeconv ();
        idx_t decimal_نقطہ_len =
          (locale->decimal_نقطہ[0] ? لڑی_لمبائی (locale->decimal_نقطہ) : 1);

        إذا (ناپ_spec == FLOAT_DOUBLE)
          {
            print_function = print_double;
            میدان_چوڑائی = DBL_STRLEN_BOUND_L (decimal_نقطہ_len);
          }
        وإلا إذا (FLOAT_SINGLE < FLOAT_DOUBLE && ناپ_spec == FLOAT_SINGLE)
          {
            print_function = print_float;
            میدان_چوڑائی = FLT_STRLEN_BOUND_L (decimal_نقطہ_len);
          }
        وإلا إذا (FLOAT_HALF < FLOAT_SINGLE && ناپ_spec == FLOAT_HALF)
          {
            print_function = fmt == BFLOATING_نقطہ
                             ? print_bfloat : print_halffloat;
            میدان_چوڑائی = FLT_STRLEN_BOUND_L (decimal_نقطہ_len);
          }
        وإلا إذا (FLOAT_DOUBLE < FLOAT_LONG_DOUBLE
                 && ناپ_spec == FLOAT_LONG_DOUBLE)
          {
            print_function = print_long_double;
            میدان_چوڑائی = LDBL_STRLEN_BOUND_L (decimal_نقطہ_len);
          }
        وإلا
          affirm (false);

        اكسر;
      }

    حالة 'a':
      ++s;
      fmt = NAMED_CHARACTER;
      ناپ_spec = CHAR;
      print_function = print_named_ascii;
      میدان_چوڑائی = 3;
      اكسر;

    حالة 'c':
      ++s;
      fmt = CHARACTER;
      ناپ_spec = CHAR;
      print_function = print_ascii;
      میدان_چوڑائی = 3;
      اكسر;

    افتراضي:
      error (0, 0, _("invalid character '%c' in type string %s"),
             *s, quote (s_orig));
      أرجع false;
    }

  tspec->ناپ = ناپ_spec;
  tspec->fmt = fmt;
  tspec->print_function = print_function;

  tspec->میدان_چوڑائی = میدان_چوڑائی;
  tspec->hexl_منوال_trailer = (*s == 'z');
  إذا (tspec->hexl_منوال_trailer)
    s++;

  *اگلا = s;
  أرجع true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM and the global string INPUT_FILENAME to the
   first one that can be successfully opened. Modify FILE_LIST to
   reference the next filename in the list.  A file name of "-" is
   interpreted as standard input.  If any file open fails, give an error
   message and return false.  */

ساكن bool
open_اگلا_file (فراغ)
{
  bool ok = true;

  افعل
    {
      داخلہ_filename = *file_list;
      إذا (داخلہ_filename == عدم)
        أرجع ok;
      ++file_list;

      إذا (streq (داخلہ_filename, "-"))
        {
          داخلہ_filename = _("standard input");
          وچ_دھارا = stdin;
          have_read_stdin = true;
          xset_ثنائی_منوال (STDIN_FILENO, O_ثنائی);
        }
      وإلا
        {
          وچ_دھارا = fopen (داخلہ_filename, (O_ثنائی ? "rb" : "r"));
          إذا (وچ_دھارا == عدم)
            {
              error (0, errno, "%s", quotef (داخلہ_filename));
              ok = false;
            }
        }
    }
  طالما (وچ_دھارا == عدم);

  إذا (0 <= سرا_وتھ && !flag_dump_لڑیاں)
    setvbuf (وچ_دھارا, عدم, _IONBF, 0);

  أرجع ok;
}

/* Test whether there have been errors on in_stream, and close it if
   it is not standard input.  Return false if there has been an error
   on in_stream or stdout; return true otherwise.  This function will
   report more than one error only if both a read and a write error
   have occurred.  IN_ERRNO, if nonzero, is the error number
   corresponding to the most recent action for IN_STREAM.  */

ساكن bool
check_and_close (صحيح وچ_errno)
{
  bool ok = true;

  إذا (وچ_دھارا != عدم)
    {
      إذا (!ferror (وچ_دھارا))
        وچ_errno = 0;
      إذا (streq (file_list[-1], "-"))
        clearerr (وچ_دھارا);
      وإلا إذا (fclose (وچ_دھارا) != 0 && !وچ_errno)
        وچ_errno = errno;
      إذا (وچ_errno)
        {
          error (0, وچ_errno, "%s", quotef (داخلہ_filename));
          ok = false;
        }

      وچ_دھارا = عدم;
    }

  إذا (ferror (stdout))
    {
      error (0, 0, _("write error"));
      ok = false;
    }

  أرجع ok;
}

/* Decode the modern od format string S.  Append the decoded
   representation to the global array SPEC, reallocating SPEC if
   necessary.  Return true if S is valid.  */

ساكن bool ATTRIBUTE_NONNULL ()
decode_format_string (حرف ثابت *s)
{
  حرف ثابت *s_orig = s;

  طالما (*s != '\0')
    {
      حرف ثابت *اگلا;

      إذا (n_specs_allocated <= n_specs)
        spec = xpalloc (spec, &n_specs_allocated, 1, -1, حجم *spec);

      إذا (! decode_one_format (s_orig, s, &اگلا, &spec[n_specs]))
        أرجع false;

      affirm (s != اگلا);
      s = اگلا;
      ++n_specs;
    }

  أرجع true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM to position N_SKIP in the concatenation of
   those files.  If any file operation fails or if there are fewer than
   N_SKIP bytes in the combined input, give an error message and return
   false.  When possible, use seek rather than read operations to
   advance IN_STREAM.  */

ساكن bool
skip (intmax_t n_skip)
{
  bool ok = true;
  صحيح وچ_errno = 0;

  إذا (n_skip == 0)
    أرجع true;

  طالما (وچ_دھارا != عدم)	/* EOF.  */
    {
      بنية stat file_stats;

      /* First try seeking.  For large offsets, this extra work is
         worthwhile.  If the offset is below some threshold it may be
         more efficient to move the pointer by reading.  There are two
         issues when trying to seek:
           - the file must be seekable.
           - before seeking to the specified position, make sure
             that the new position is in the current file.
             Try to do that by getting file's size using fstat.
             But that will work only for regular files.  */

      إذا (fstat (fileno (وچ_دھارا), &file_stats) == 0)
        {
          /* The st_size field is valid for regular files.
             If the number of bytes left to skip is larger than
             the size of the current file, we can decrement n_skip
             and go on to the next file.  Skip this optimization also
             when st_size is no greater than the block size, because
             some kernels report nonsense small file sizes for
             proc-like file systems.  */
          إذا (S_ISREG (file_stats.st_منوال)
              && STP_BLKSIZE (&file_stats) < file_stats.st_ناپ)
            {
              إذا (file_stats.st_ناپ < n_skip)
                n_skip -= file_stats.st_ناپ;
              وإلا
                {
                  إذا (fseeko (وچ_دھارا, n_skip, SEEK_CUR) != 0)
                    {
                      وچ_errno = errno;
                      ok = false;
                    }
                  n_skip = 0;
                }
            }

          وإلا إذا (! (S_ISREG (file_stats.st_منوال)
                      || S_TYPEISSHM (&file_stats)
                      || S_TYPEISTMO (&file_stats))
                   && fseeko (وچ_دھارا, n_skip, SEEK_CUR) == 0)
            n_skip = 0;

          /* If it's neither a seekable file with unusable size, nor a
             regular file so large it can't be in a proc-like file system,
             position the file pointer by reading.  */

          وإلا
            {
              حرف buf[BUFSIZ];
              idx_t n_بائٹاں_read, n_بائٹاں_منزل_read = BUFSIZ;

              طالما (0 < n_skip)
                {
                  إذا (n_skip < n_بائٹاں_منزل_read)
                    n_بائٹاں_منزل_read = n_skip;
                  n_بائٹاں_read = fread (buf, 1, n_بائٹاں_منزل_read, وچ_دھارا);
                  n_skip -= n_بائٹاں_read;
                  إذا (n_بائٹاں_read != n_بائٹاں_منزل_read)
                    {
                      إذا (ferror (وچ_دھارا))
                        {
                          وچ_errno = errno;
                          ok = false;
                          n_skip = 0;
                          اكسر;
                        }
                      إذا (feof (وچ_دھارا))
                        اكسر;
                    }
                }
            }

          إذا (n_skip == 0)
            اكسر;
        }

      وإلا   /* cannot fstat() file */
        {
          error (0, errno, "%s", quotef (داخلہ_filename));
          ok = false;
        }

      ok &= check_and_close (وچ_errno);

      ok &= open_اگلا_file ();
    }

  إذا (n_skip != 0)
    error (EXIT_FAILURE, 0, _("cannot skip past end of combined input"));

  أرجع ok;
}

ساكن فراغ
format_پتہ_none (MAYBE_بن_ورتیا intmax_t پتہ,
                     MAYBE_بن_ورتیا حرف c)
{
}

ساكن فراغ
format_پتہ_std (intmax_t پتہ, حرف c)
{
  حرف buf[MAX_پتہ_لمائی + 2];
  حرف *p = buf + حجم buf;
  حرف ثابت *pbound;

  *--p = '\0';
  *--p = c;
  pbound = p - پتہ_pad_len;

  /* Use a special case of the code for each base.  This is measurably
     faster than generic code.  */
  بدل (پتہ_ادھار)
    {
    حالة 8:
      افعل
        *--p = '0' + (پتہ & 7);
      طالما ((پتہ >>= 3) != 0);
      اكسر;

    حالة 10:
      افعل
        *--p = '0' + (پتہ % 10);
      طالما ((پتہ /= 10) != 0);
      اكسر;

    حالة 16:
      افعل
        *--p = "0123456789abcdef"[پتہ & 15];
      طالما ((پتہ >>= 4) != 0);
      اكسر;
    }

  طالما (pbound < p)
    *--p = '0';

  fputs (p, stdout);
}

ساكن فراغ
format_پتہ_paren (intmax_t پتہ, حرف c)
{
  حرف_لکھو ('(');
  format_پتہ_std (پتہ, ')');
  إذا (c)
    حرف_لکھو (c);
}

ساكن فراغ
format_پتہ_label (intmax_t پتہ, حرف c)
{
  format_پتہ_std (پتہ, ' ');

  intmax_t addr;
  إذا (ckd_جوڑو (&addr, پتہ, pseudo_وتھ))
    error (EXIT_FAILURE, 0, _("pseudo address too large for input"));

  format_پتہ_paren (addr, c);
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

ساكن فراغ
write_بلاک (intmax_t current_وتھ, idx_t n_بائٹاں,
             حرف ثابت *prev_بلاک, حرف ثابت *curr_بلاک)
{
  ساكن bool پہلا = true;
  ساكن bool prev_pair_equal = false;

  إذا (abbreviate_duplicate_بلاکاں
      && !پہلا && n_بائٹاں == بائٹاں_فی_بلاک
      && memeq (prev_بلاک, curr_بلاک, بائٹاں_فی_بلاک))
    {
      إذا (prev_pair_equal)
        {
          /* The two preceding blocks were equal, and the current
             block is the same as the last one, so print nothing.  */
        }
      وإلا
        {
          لکھو ("*\n");
          prev_pair_equal = true;
        }
    }
  وإلا
    {
      prev_pair_equal = false;
      لكل (idx_t i = 0; i < n_specs; i++)
        {
          صحيح datum_چوڑائی = چوڑائی_بائٹاں[spec[i].ناپ];
          idx_t fields_فی_بلاک = بائٹاں_فی_بلاک / datum_چوڑائی;
          idx_t blank_fields = (بائٹاں_فی_بلاک - n_بائٹاں) / datum_چوڑائی;
          إذا (i == 0)
            format_پتہ (current_وتھ, '\0');
          وإلا
            print_n_spaces (پتہ_pad_len);
          (*spec[i].print_function) (fields_فی_بلاک, blank_fields,
                                     curr_بلاک, spec[i].fmt_string,
                                     spec[i].میدان_چوڑائی, spec[i].pad_چوڑائی);
          إذا (spec[i].hexl_منوال_trailer)
            {
              /* Space-pad out to full line width, then dump the trailer.  */
              صحيح میدان_چوڑائی = spec[i].میدان_چوڑائی;
              لكل (idx_t f = 0; f < blank_fields; f++)
                print_n_spaces (میدان_چوڑائی);
              idx_t pad_چوڑائی = pad_at (fields_فی_بلاک, blank_fields,
                                        spec[i].pad_چوڑائی);
              print_n_spaces (pad_چوڑائی);
              dump_hexl_منوال_trailer (n_بائٹاں, curr_بلاک);
            }
          حرف_لکھو ('\n');
        }
    }
  پہلا = false;
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

ساكن bool
read_char (صحيح *c)
{
  bool ok = true;

  *c = EOF;

  طالما (وچ_دھارا && (*c = getc (وچ_دھارا)) < 0)
    {
      ok &= check_and_close (errno);
      ok &= open_اگلا_file ();
    }

  أرجع ok;
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

ساكن bool
read_بلاک (idx_t n, حرف *بلاک, idx_t *n_بائٹاں_وچ_بفر)
{
  bool ok = true;

  affirm (0 < n && n <= بائٹاں_فی_بلاک);

  *n_بائٹاں_وچ_بفر = 0;

  طالما (وچ_دھارا != عدم)	/* EOF.  */
    {
      idx_t n_needed = n - *n_بائٹاں_وچ_بفر;
      idx_t n_read = fread (بلاک + *n_بائٹاں_وچ_بفر,
                            1, n_needed, وچ_دھارا);

      *n_بائٹاں_وچ_بفر += n_read;

      إذا (n_read == n_needed)
        اكسر;

      ok &= check_and_close (errno);

      ok &= open_اگلا_file ();
    }

  أرجع ok;
}

/* Return the least common multiple of the sizes associated
   with the format specs.  */

ATTRIBUTE_خالص
ساكن صحيح
get_lcm (فراغ)
{
  صحيح l_c_m = 1;

  لكل (idx_t i = 0; i < n_specs; i++)
    l_c_m = lcm (l_c_m, چوڑائی_بائٹاں[spec[i].ناپ]);
  أرجع l_c_m;
}

/* Act like xstrtoimax (NPTR, NULL, BASE, VAL, VALID_SUFFIXES),
   except reject negative values, and *VAL may be set if
   LONGINT_INVALID is returned.  */
ساكن strtol_error
xstr2nonneg (حرف ثابت *محدود nptr, صحيح ادھار_2, intmax_t *val,
             حرف ثابت *محدود valid_suffixes)
{
  strtol_error s_err = xstrtoimax (nptr, عدم, ادھار_2, val, valid_suffixes);
  أرجع s_err != LONGINT_INVALID && *val < 0 ? LONGINT_INVALID : s_err;
}

/* If STR is a valid traditional offset specification with an optional
   leading '+', and can be represented in intmax_t, return true and
   set *OFFSET to the offset it denotes.  If it is valid but cannot be
   represented, diagnose the problem and exit.  Otherwise return false
   and possibly set *OFFSET.  */

ساكن bool
parse_old_وتھ (حرف *str, intmax_t *وتھ)
{
  /* Skip over any leading '+'.  */
  حرف *s = str + (str[0] == '+');

  إذا (! c_isdigit (s[0]))
    أرجع false;

  /* Determine the radix for S.  If there is a '.', followed first by
     optional 'b' or (undocumented) 'B' and then by end of string,
     it's decimal, otherwise, if the string begins with '0X' or '0x',
     it's hexadecimal, else octal.  */
  حرف *dot = strchr (s, '.');
  إذا (dot && dot[(dot[1] == 'b' || dot[1] == 'B') + 1])
    dot = عدم;
  صحيح radix = dot ? 10 : s[0] == '0' && (s[1] == 'x' || s[1] == 'X') ? 16 : 8;

  إذا (dot)
    {
      /* Temporarily remove the '.' from the decimal string.  */
      dot[0] = dot[1];
      dot[1] = '\0';
    }

  تعداد strtol_error s_err = xstr2nonneg (s, radix, وتھ, "bB");

  إذا (dot)
    {
      /* Restore the decimal string's original value.  */
      dot[1] = dot[0];
      dot[0] = '.';
    }

  إذا (s_err == LONGINT_حد_توں_ودھ)
    error (EXIT_FAILURE, ERANGE, "%s", quotef (str));
  أرجع s_err == LONGINT_OK;
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

ساكن bool
dump (فراغ)
{
  حرف *بلاک[2];
  bool idx = false;
  bool ok = true;
  idx_t n_بائٹاں_read;

  بلاک[0] = xinmalloc (1 + abbreviate_duplicate_بلاکاں, بائٹاں_فی_بلاک);
  بلاک[1] = بلاک[0] + abbreviate_duplicate_بلاکاں * بائٹاں_فی_بلاک;

  intmax_t current_وتھ = n_بائٹاں_منزل_skip;

  افعل
    {
      intmax_t needed_bound
        = سرا_وتھ < 0 ? INTMAX_MAX : سرا_وتھ - current_وتھ;
      إذا (needed_bound <= 0)
        {
          n_بائٹاں_read = 0;
          اكسر;
        }
      idx_t n_needed = MIN (بائٹاں_فی_بلاک, needed_bound);
      ok &= read_بلاک (n_needed, بلاک[idx], &n_بائٹاں_read);
      إذا (n_بائٹاں_read < بائٹاں_فی_بلاک)
        اكسر;
      affirm (n_بائٹاں_read == بائٹاں_فی_بلاک);
      write_بلاک (current_وتھ, n_بائٹاں_read,
                   بلاک[!idx], بلاک[idx]);
      إذا (ferror (stdout))
        ok = false;
      current_وتھ += n_بائٹاں_read;
      idx = !idx;
    }
  طالما (ok);

  إذا (n_بائٹاں_read > 0)
    {
      صحيح l_c_m = get_lcm ();

      /* Ensure zero-byte padding up to the smallest multiple of l_c_m that
         is at least as large as n_bytes_read.  */
      idx_t بائٹاں_منزل_write = (n_بائٹاں_read + l_c_m - 1
                              - (n_بائٹاں_read + l_c_m - 1) % l_c_m);

      memset (بلاک[idx] + n_بائٹاں_read, 0, بائٹاں_منزل_write - n_بائٹاں_read);
      write_بلاک (current_وتھ, n_بائٹاں_read, بلاک[!idx], بلاک[idx]);
      current_وتھ += n_بائٹاں_read;
    }

  إذا (ok || current_وتھ)
    format_پتہ (current_وتھ, '\n');

  إذا (0 <= سرا_وتھ && سرا_وتھ <= current_وتھ)
    ok &= check_and_close (0);

  حرر (بلاک[0]);

  أرجع ok;
}

/* STRINGS mode.  Find each "string constant" in the input.
   A string constant is a run of at least STRING_MIN
   printable characters terminated by a NUL or END_OFFSET.
   Based on a function written by Richard Stallman for a
   traditional version of od.  Return true if successful.  */

ساكن bool
dump_لڑیاں (فراغ)
{
  idx_t bufsize = MAX (100, string_min + 1);
  حرف *buf = ximalloc (bufsize);
  intmax_t پتہ = n_بائٹاں_منزل_skip;
  bool ok = true;

  طالما (true)
    {
      idx_t i = 0;
      صحيح c = 1;  /* Init to 1 so can distinguish if NUL read.  */

      إذا (0 <= سرا_وتھ
          && (سرا_وتھ < string_min || سرا_وتھ - string_min < پتہ))
        اكسر;

      /* Store consecutive printable characters to BUF.  */
      طالما (! (0 <= سرا_وتھ && سرا_وتھ <= پتہ))
        {
          إذا (i == bufsize - 1)
            buf = xpalloc (buf, &bufsize, 1, -1, حجم *buf);
          ok &= read_char (&c);
          إذا (c < 0)
            {
              حرر (buf);
              أرجع ok;
            }
          پتہ++;
          buf[i++] = c;
          إذا (c == '\0')
            اكسر;		/* Print this string.  */
          إذا (! isprint (c))
            {
              c = -1;		/* Give up on this string.  */
              اكسر;
            }
        }

      إذا (c < 0 || i - !c < string_min)
        استمر;

      buf[i] = 0;

      /* If we get here, the string is all printable, so print it.  */

      format_پتہ (پتہ - i, ' ');

      لكل (i = 0; (c = buf[i]); i++)
        {
          بدل (c)
            {
            حالة '\a':
              fputs ("\\a", stdout);
              اكسر;

            حالة '\b':
              fputs ("\\b", stdout);
              اكسر;

            حالة '\f':
              fputs ("\\f", stdout);
              اكسر;

            حالة '\n':
              fputs ("\\n", stdout);
              اكسر;

            حالة '\r':
              fputs ("\\r", stdout);
              اكسر;

            حالة '\t':
              fputs ("\\t", stdout);
              اكسر;

            حالة '\v':
              fputs ("\\v", stdout);
              اكسر;

            افتراضي:
              putc (c, stdout);
            }
        }
      حرف_لکھو ('\n');
    }

  /* We reach this point only if we search through
     (max_bytes_to_format - string_min) bytes before reaching EOF.  */

  حرر (buf);

  ok &= check_and_close (0);
  أرجع ok;
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  صحيح n_files;
  صحيح l_c_m;
  idx_t desired_چوڑائی = 0;
  bool modern = false;
  bool ok = true;
  idx_t چوڑائی_فی_بلاک = 0;
  ساكن حرف ثابت multipliers[] = "bEGKkMmPQRTYZ0";

  /* The maximum number of bytes that will be formatted.
     If negative, there is no limit.  */
  intmax_t max_بائٹاں_منزل_format = -1;

  /* The old-style 'pseudo starting address' to be printed in parentheses
     after any true address.  */
  intmax_t pseudo_شروع IF_LINT ( = 0);

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما (true)
    {
      intmax_t tmp;
      تعداد strtol_error s_err;
      صحيح oi = -1;
      صحيح c = getopt_long (argc, argv, short_options, long_options, &oi);
      إذا (c == -1)
        اكسر;

      بدل (c)
        {
        حالة 'A':
          modern = true;
          بدل (optarg[0])
            {
            حالة 'd':
              format_پتہ = format_پتہ_std;
              پتہ_ادھار = 10;
              پتہ_pad_len = 7;
              اكسر;
            حالة 'o':
              format_پتہ = format_پتہ_std;
              پتہ_ادھار = 8;
              پتہ_pad_len = 7;
              اكسر;
            حالة 'x':
              format_پتہ = format_پتہ_std;
              پتہ_ادھار = 16;
              پتہ_pad_len = 6;
              اكسر;
            حالة 'n':
              format_پتہ = format_پتہ_none;
              پتہ_pad_len = 0;
              اكسر;
            افتراضي:
              error (EXIT_FAILURE, 0,
                     _("invalid output address radix '%c';"
                       " it must be one character from [doxn]"),
                     optarg[0]);
              اكسر;
            }
          اكسر;

        حالة 'j':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &n_بائٹاں_منزل_skip, multipliers);
          إذا (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          اكسر;

        حالة 'N':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &max_بائٹاں_منزل_format, multipliers);
          إذا (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          اكسر;

        حالة 'S':
          modern = true;
          إذا (optarg == عدم)
            string_min = 3;
          وإلا
            {
              s_err = xstr2nonneg (optarg, 0, &tmp, multipliers);
              /* The minimum string length + 1 must fit in idx_t,
                 since we may allocate a buffer of this size + 1.  */
              idx_t i;
              إذا (s_err == LONGINT_OK && ckd_جوڑو (&i, tmp, 1))
                s_err = LONGINT_حد_توں_ودھ;
              إذا (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
              string_min = tmp;
            }
          flag_dump_لڑیاں = true;
          اكسر;

        حالة 't':
          modern = true;
          ok &= decode_format_string (optarg);
          اكسر;

        حالة 'v':
          modern = true;
          abbreviate_duplicate_بلاکاں = false;
          اكسر;

        حالة TRADITIONAL_OPTION:
          traditional = true;
          اكسر;

        حالة ENDIAN_OPTION:
          بدل (XARGMATCH ("--endian", optarg, endian_دلیلاں, endian_types))
            {
            حالة endian_big:
              داخلہ_وٹاندرا = BYTE_ORDER != BIG_ENDIAN;
              اكسر;
            حالة endian_little:
              داخلہ_وٹاندرا = BYTE_ORDER != LITTLE_ENDIAN;
              اكسر;
            }
          اكسر;

          /* The next several cases map the traditional format
             specification options to the corresponding modern format
             specs.  GNU od accepts any combination of old- and
             new-style options.  Format specification options accumulate.
             The obsolescent and undocumented formats are compatible
             with FreeBSD 4.10 od.  */

#تعریف_کرو CASE_OLD_ARG(old_char,new_string)		\
        حالة old_char:					\
          ok &= decode_format_string (new_string);	\
          اكسر

          CASE_OLD_ARG ('a', "a");
          CASE_OLD_ARG ('b', "o1");
          CASE_OLD_ARG ('c', "c");
          CASE_OLD_ARG ('D', "u4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('d', "u2");
        حالة 'F': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('e', "fD"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('f', "fF");
        حالة 'X': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('H', "x4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('i', "dI");
        حالة 'I': حالة 'L': /* obsolescent and undocumented aliases */
          CASE_OLD_ARG ('l', "dL");
          CASE_OLD_ARG ('O', "o4"); /* obsolescent and undocumented */
        حالة 'B': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('o', "o2");
          CASE_OLD_ARG ('s', "d2");
        حالة 'h': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('x', "x2");

#تعریف_رد CASE_OLD_ARG

        حالة 'w':
          modern = true;
          إذا (optarg == عدم)
            {
              desired_چوڑائی = 32;
            }
          وإلا
            {
              intmax_t w_tmp;
              s_err = xstr2nonneg (optarg, 10, &w_tmp, "");
              إذا (s_err == LONGINT_OK)
                {
                  إذا (ckd_جوڑو (&desired_چوڑائی, w_tmp, 0))
                    s_err = LONGINT_حد_توں_ودھ;
                  وإلا إذا (desired_چوڑائی == 0)
                    s_err = LONGINT_INVALID;
                }
              إذا (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
            }
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
          اكسر;
        }
    }

  إذا (!ok)
    أرجع EXIT_FAILURE;

  إذا (flag_dump_لڑیاں && n_specs > 0)
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

  إذا (!modern || traditional)
    {
      intmax_t o1;
      intmax_t o2;

      بدل (n_files)
        {
        حالة 1:
          إذا ((traditional || argv[optind][0] == '+')
              && parse_old_وتھ (argv[optind], &o1))
            {
              n_بائٹاں_منزل_skip = o1;
              --n_files;
              ++argv;
            }
          اكسر;

        حالة 2:
          إذا ((traditional || argv[optind + 1][0] == '+'
               || c_isdigit (argv[optind + 1][0]))
              && parse_old_وتھ (argv[optind + 1], &o2))
            {
              إذا (traditional && parse_old_وتھ (argv[optind], &o1))
                {
                  n_بائٹاں_منزل_skip = o1;
                  flag_pseudo_شروع = true;
                  pseudo_شروع = o2;
                  argv += 2;
                  n_files -= 2;
                }
              وإلا
                {
                  n_بائٹاں_منزل_skip = o2;
                  --n_files;
                  argv[optind + 1] = argv[optind];
                  ++argv;
                }
            }
          اكسر;

        حالة 3:
          إذا (traditional
              && parse_old_وتھ (argv[optind + 1], &o1)
              && parse_old_وتھ (argv[optind + 2], &o2))
            {
              n_بائٹاں_منزل_skip = o1;
              flag_pseudo_شروع = true;
              pseudo_شروع = o2;
              argv[optind + 2] = argv[optind];
              argv += 2;
              n_files -= 2;
            }
          اكسر;
        }

      إذا (traditional && 1 < n_files)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
          error (0, 0, "%s",
                 _("compatibility mode supports at most one file"));
          usage (EXIT_FAILURE);
        }
    }

  إذا (flag_pseudo_شروع)
    {
      إذا (format_پتہ == format_پتہ_none)
        {
          پتہ_ادھار = 8;
          پتہ_pad_len = 7;
          format_پتہ = format_پتہ_paren;
        }
      وإلا
        format_پتہ = format_پتہ_label;
    }

  إذا (0 <= max_بائٹاں_منزل_format
      && ckd_جوڑو (&سرا_وتھ, n_بائٹاں_منزل_skip, max_بائٹاں_منزل_format))
    error (EXIT_FAILURE, 0, _("skip-bytes + read-bytes is too large"));

  إذا (n_specs == 0)
    decode_format_string ("oS");

  إذا (n_files > 0)
    {
      /* Set the global pointer FILE_LIST so that it
         references the first file-argument on the command-line.  */

      file_list = (حرف ثابت *ثابت *) &argv[optind];
    }
  وإلا
    {
      /* No files were listed on the command line.
         Set the global pointer FILE_LIST so that it
         references the null-terminated list of one name: "-".  */

      file_list = default_file_list;
    }

  /* open the first input file */
  ok = open_اگلا_file ();
  إذا (وچ_دھارا == عدم)
    اذهب cleanup;

  /* skip over any unwanted header bytes */
  ok &= skip (n_بائٹاں_منزل_skip);
  إذا (وچ_دھارا == عدم)
    اذهب cleanup;

  pseudo_وتھ = (flag_pseudo_شروع ? pseudo_شروع - n_بائٹاں_منزل_skip : 0);

  /* Compute output block length.  */
  l_c_m = get_lcm ();

  إذا (desired_چوڑائی != 0)
    {
      إذا (desired_چوڑائی % l_c_m == 0)
        بائٹاں_فی_بلاک = desired_چوڑائی;
      وإلا
        {
          error (0, 0, _("warning: invalid width %td; using %d instead"),
                 desired_چوڑائی, l_c_m);
          بائٹاں_فی_بلاک = l_c_m;
        }
    }
  وإلا
    {
      إذا (l_c_m < DEFAULT_بائٹاں_فی_بلاک)
        بائٹاں_فی_بلاک = (DEFAULT_بائٹاں_فی_بلاک
                           - DEFAULT_بائٹاں_فی_بلاک % l_c_m);
      وإلا
        بائٹاں_فی_بلاک = l_c_m;
    }

  /* Compute padding necessary to align output block.  */
  لكل (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_فی_بلاک = بائٹاں_فی_بلاک / چوڑائی_بائٹاں[spec[i].ناپ];
      إذا (pad_at_حد_توں_ودھ (fields_فی_بلاک))
        error (EXIT_FAILURE, 0, _("%td is too large"), desired_چوڑائی);
      idx_t بلاک_چوڑائی = (spec[i].میدان_چوڑائی + 1) * fields_فی_بلاک;
      إذا (چوڑائی_فی_بلاک < بلاک_چوڑائی)
        چوڑائی_فی_بلاک = بلاک_چوڑائی;
    }
  لكل (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_فی_بلاک = بائٹاں_فی_بلاک / چوڑائی_بائٹاں[spec[i].ناپ];
      idx_t بلاک_چوڑائی = spec[i].میدان_چوڑائی * fields_فی_بلاک;
      spec[i].pad_چوڑائی = چوڑائی_فی_بلاک - بلاک_چوڑائی;
    }

#جے_تعریف_شدہ DEBUG
  لکھو ("lcm=%d, width_per_block=%td\n", l_c_m, چوڑائی_فی_بلاک);
  لكل (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_فی_بلاک = بائٹاں_فی_بلاک / چوڑائی_بائٹاں[spec[i].ناپ];
      affirm (بائٹاں_فی_بلاک % چوڑائی_بائٹاں[spec[i].ناپ] == 0);
      affirm (1 <= spec[i].pad_چوڑائی / fields_فی_بلاک);
      لکھو ("%td: fmt=\"%s\" in_width=%d out_width=%d pad=%td\n",
              i, spec[i].fmt_string, چوڑائی_بائٹاں[spec[i].ناپ],
              spec[i].میدان_چوڑائی, spec[i].pad_چوڑائی);
    }
#جے_انت

  ok &= (flag_dump_لڑیاں ? dump_لڑیاں () : dump ());

cleanup:

  إذا (have_read_stdin && fclose (stdin) < 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
