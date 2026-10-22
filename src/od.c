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
#تعریف_کړه پروګرام_NAME "od"

#تعریف_کړه AUTHORS proper_name ("Jim Meyering")

/* The default number of input bytes per output line.  */
#تعریف_کړه DEFAULT_بایټونه_فی_بلاک 16

#إذا FLOAT16_SUPPORTED
  /* Available since clang 6 (2018), and gcc 7 (2017).  */
  تعريفنوع _Float16 float16;
#وإلا
# define FLOAT16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  تعريفنوع عائم float16;
#د_که_پای

#إذا BF16_SUPPORTED
  /* Available since clang 11 (2020), and gcc 13 (2023). */
  تعريفنوع __bf16 bfloat16;
#وإلا
# define BF16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  تعريفنوع عائم bfloat16;
#د_که_پای

تعداد اندازه_spec
  {
    NO_اندازه = 0,
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
    N_اندازه_SPECS
  };

تعداد output_format
  {
    SIGNED_DECIMAL,
    UNSIGNED_DECIMAL,
    OCTAL,
    HEXADECIMAL,
    FLOATING_ټکی,
    HFLOATING_ټکی,
    BFLOATING_ټکی,
    NAMED_CHARACTER,
    CHARACTER
  };

تعداد { MAX_INTEGRAL_TYPE_پلنوالی = UINTMAX_پلنوالی };

/* The maximum number of bytes needed for a format string, including
   the trailing nul.  Each format string expects a variable amount of
   padding (guaranteed to be at least 1 plus the field width), then an
   element that will be formatted in the field.  */
تعداد
  {
    FMT_بایټونه_ALLOCATED =
      حجم "%*.%dlld" - حجم "%d" + INT_STRLEN_BOUND (صحيح) + 1
  };

/* Ensure that our choice for FMT_BYTES_ALLOCATED is reasonable.  */
static_assert (MAX_INTEGRAL_TYPE_پلنوالی <= 3 * 99);

/* The type of a print function.  FIELDS is the number of fields per
   line, BLANK is the number of fields to leave blank.  WIDTH is width
   of one field, excluding leading space, and PAD is total pad to
   divide among FIELDS.  PAD is at least as large as FIELDS.  */
تعريفنوع فراغ (*print_function_type)
  (idx_t fields, idx_t blank, فراغ ثابت *معلومات,
   حرف ثابت *fmt, صحيح پلنوالی, idx_t pad);

/* Each output format specification (from '-t spec' or from
   old-style options) is represented by one of these structures.  */
بنية tspec
  {
    تعداد output_format fmt;
    تعداد اندازه_spec اندازه; /* Type of input object.  */
    print_function_type print_function;
    حرف fmt_string[FMT_بایټونه_ALLOCATED]; /* Of the style "%*d".  */
    bool hexl_ډېرپېښ_trailer;
    صحيح ډګر_پلنوالی; /* Minimum width of a field, excluding leading space.  */
    idx_t pad_پلنوالی; /* Total padding to be divided among fields.  */
  };

/* Convert enum size_spec to the size of the named type.  */
ساكن ثابت صحيح پلنوالی_بایټونه[] =
{
  -1,
  حجم (بینښه حرف),
#إذا UCHAR_MAX < USHRT_MAX
  حجم (بینښه لنډ صحيح),
#د_که_پای
#إذا USHRT_MAX < UINT_MAX
  حجم (بینښه صحيح),
#د_که_پای
#إذا UINT_MAX < ULONG_MAX
  حجم (بینښه اوږد صحيح),
#د_که_پای
#إذا ULONG_MAX < ULLONG_MAX
  حجم (بینښه اوږد اوږد صحيح),
#د_که_پای
#إذا ULLONG_MAX < UINTMAX_MAX
  حجم (uintmax_t),
#د_که_پای
#إذا BF16_SUPPORTED
  حجم (bfloat16),
#وإلاإذا FLOAT16_SUPPORTED
  حجم (float16),
#د_که_پای
#إذا FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
  حجم (عائم),
#د_که_پای
  حجم (مزدوج),
#إذا DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
  حجم (اوږد مزدوج),
#د_که_پای
};

/* Ensure that for each member of 'enum size_spec' there is an
   initializer in the width_bytes array.  */
static_assert (countof (پلنوالی_بایټونه) == N_اندازه_SPECS);

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
ساكن صحيح پته_بنسټ = 8;

/* The number of octal digits required to represent the largest
   address value.  */
تعداد { MAX_پته_اوږدتیا = ((INTMAX_پلنوالی - 1) / 3
                             + ((INTMAX_پلنوالی - 1) % 3 != 0)) };

/* Width of a normal address.  */
ساكن صحيح پته_pad_len = 7;

/* Minimum length when detecting --strings.  */
ساكن idx_t string_min;

/* True when in --strings mode.  */
ساكن bool flag_dump_مزي_ګانې;

/* True if we should recognize the older non-option arguments
   that specified at most one file and optional arguments specifying
   offset and pseudo-start address.  */
ساكن bool traditional;

/* True if an old-style 'pseudo-address' was specified.  */
ساكن bool flag_pseudo_پیل;

/* The difference between the old-style pseudo starting address and
   the number of bytes to skip.  */
ساكن intmax_t pseudo_دننه_واټن;

/* Function that accepts an address and an optional following char,
   and prints the address and char to stdout.  */
ساكن فراغ format_پته_std (intmax_t, حرف);
ساكن فراغ (*format_پته) (intmax_t, حرف) = format_پته_std;

/* The number of input bytes to skip before formatting and writing.  */
ساكن intmax_t n_بایټونه_منزل_skip;

/* The offset of the first byte after the last byte to be formatted.
   If negative, there is no limit.  */
ساكن intmax_t پای_ټکی_دننه_واټن = -1;

/* When true and two or more consecutive blocks are equal, format
   only the first block and output an asterisk alone on the following
   line to indicate that identical blocks have been elided.  */
ساكن bool abbreviate_duplicate_بلاکونه = true;

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
ساكن idx_t بایټونه_فی_بلاک;

/* Human-readable representation of *file_list (for error messages).
   It differs from file_list[-1] only when file_list[-1] is "-".  */
ساكن حرف ثابت *ننوت_filename;

/* A null-terminated list of the file-arguments from the command line.  */
ساكن حرف ثابت *ثابت *file_list;

/* Initializer for file_list if no file-arguments
   were specified on the command line.  */
ساكن حرف ثابت *ثابت default_file_list[] = {"-", عدم};

/* The input stream associated with the current file.  */
ساكن FILE *دننه_بهیر;

/* If true, at least one of the files we read was standard input.  */
ساكن bool have_read_stdin;

/* Map the size in bytes to a type identifier.
   When two types have the same machine layout:
     - Prefer unsigned int to other types, as its format is shorter.
     - Prefer unsigned long to higher-ranked types, as it is older.
     - Prefer uintmax_t to unsigned long long int; this wins if %lld
       does not work but %jd does (e.g., MS-Windows).  */
ساكن تعداد اندازه_spec ثابت integral_type_اندازه[] =
  {
#إذا UCHAR_MAX < USHRT_MAX
    [حجم (بینښه حرف)] = CHAR,
#د_که_پای
#إذا USHRT_MAX < UINT_MAX
    [حجم (بینښه لنډ صحيح)] = SHORT,
#د_که_پای
    [حجم (بینښه صحيح)] = INT,
#إذا UINT_MAX < ULONG_MAX
    [حجم (بینښه اوږد صحيح)] = LONG,
#د_که_پای
#إذا ULONG_MAX < ULLONG_MAX && ULLONG_MAX < UINTMAX_MAX
    [حجم (بینښه اوږد اوږد صحيح)] = LONG_LONG,
#د_که_پای
#إذا ULONG_MAX < UINTMAX_MAX
    [حجم (uintmax_t)] = INTMAX,
#د_که_پای
  };

/* Map the size in bytes to a floating type identifier.
   When two types have the same machine layout:
     - Prefer double to the other types, as its format is shorter.  */
ساكن تعداد اندازه_spec ثابت fp_type_اندازه[] =
  {
#إذا FLOAT16_SUPPORTED
    [حجم (float16)] = FLOAT_HALF,
#وإلاإذا BF16_SUPPORTED
    [حجم (bfloat16)] = FLOAT_HALF,
#د_که_پای
#إذا FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
    [حجم (عائم)] = FLOAT_SINGLE,
#د_که_پای
    [حجم (مزدوج)] = FLOAT_DOUBLE,
#إذا DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
    [حجم (اوږد مزدوج)] = FLOAT_LONG_DOUBLE,
#د_که_پای
  };

/* Use native endianness by default.  */
ساكن bool ننوت_بدلون;

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

ساكن حرف ثابت *ثابت endian_دلیلونه[] =
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
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [-abcdfilosx]... [FILE] [[+]دننه_واټن_2[.][b]]\n\
  or:  %s --traditional [OPTION]... [FILE] [[+]دننه_واټن_2[.][b] [+][LABEL][.][b]]\n\
"),
              پروګرام_name, پروګرام_name, پروګرام_name);
      fputs (_("\n\
Write an unambiguous representation, octal بایټونه by افتراضي,\n\
د FILE منزل standard output.  With نور than one FILE دلیل,\n\
concatenate them دننه the listed ترتیب منزل form the ننوت.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
If لومړی and دوهم call formats both apply, the دوهم format is assumed\n\
إذا the وروستی عملوند_2 begins with + or (إذا there are 2 operands) a digit.\n\
An دننه_واټن_2 عملوند_2 means -j دننه_واټن_2.  LABEL is the pseudo-پته\n\
at لومړی byte printed, incremented when dump is progressing.\n\
For دننه_واټن_2 and LABEL, a 0x or 0X مختاړی indicates hexadecimal;\n\
suffixes may be . لكل octal and b لكل multiply by 512.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --پته-radix=RADIX\n\
         output format لكل file offsets;\n\
         RADIX is one د [doxn], لكل Decimal, Octal, Hex or None\n\
"));
      oputs (_("\
      --endian={big|little}\n\
         بدلون ننوت بایټونه according the specified ترتیب\n\
"));
      oputs (_("\
  -j, --skip-بایټونه=بایټونه_2\n\
         skip بایټونه_2 ننوت بایټونه لومړی\n\
"));
      oputs (_("\
  -N, --اقرأ-بایټونه=بایټونه_2\n\
         حد dump منزل بایټونه_2 ننوت بایټونه\n\
"));
      oputs (_("\
  -S بایټونه_2, --مزي_ګانې[=بایټونه_2]\n\
         show only NUL terminated مزي_ګانې\n\
         د at لږترلږه بایټونه_2 (افتراضي 3) printable characters\n\
"));
      oputs (_("\
  -t, --format=TYPE\n\
         select output format or formats\n\
"));
      oputs (_("\
  -v, --output-duplicates\n\
         افعل not use * منزل mark کرښه suppression\n\
"));
      oputs (_("\
  -w[بایټونه_2], --پلنوالی[=بایټونه_2]\n\
         output بایټونه_2 بایټونه فی output کرښه;\n\
         32 is implied when بایټونه_2 is not specified\n\
"));
      oputs (_("\
      --traditional\n\
         accept arguments دننه دریم form above\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
\n\
Traditional format specifications may be intermixed; they accumulate:\n\
"), stdout);
      oputs (_("\
  -a   same as -t a,  select named characters, ignoring لوړ-ترتیب بټ\n\
"));
      oputs (_("\
  -b   same as -t o1, select octal بایټونه\n\
"));
      oputs (_("\
  -c   same as -t c,  select printable characters or backslash escapes\n\
"));
      oputs (_("\
  -d   same as -t u2, select بینښه decimal 2-byte units\n\
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
TYPE is made پورته د one or نور د these specifications:\n\
  a          named character, ignoring لوړ-ترتیب بټ\n\
  c          printable character or backslash escape\n\
"), stdout);
      fputs (_("\
  d[اندازه_2]    نښهدار decimal, اندازه_2 بایټونه فی integer\n\
  f[اندازه_2]    floating ټکی, اندازه_2 بایټونه فی عائم\n\
  o[اندازه_2]    octal, اندازه_2 بایټونه فی integer\n\
  u[اندازه_2]    بینښه decimal, اندازه_2 بایټونه فی integer\n\
  x[اندازه_2]    hexadecimal, اندازه_2 بایټونه فی integer\n\
"), stdout);
      fputs (_("\
\n\
اندازه_2 is a شمېره.  For TYPE دننه [doux], اندازه_2 may also be C لكل\n\
حجم(حرف), S لكل حجم(لنډ), I لكل حجم(صحيح) or L لكل\n\
حجم(اوږد).  If TYPE is f, اندازه_2 may also be B لكل Brain 16 بټ,\n\
H لكل Half precision عائم, F لكل حجم(عائم), D لكل حجم(مزدوج),\n\
or L لكل حجم(اوږد مزدوج).\n\
"), stdout);
      fputs (_("\
\n\
Adding a z suffix منزل any type displays printable characters at the پای_ټکی د\n\
each output کرښه.\n\
"), stdout);
      fputs (_("\
\n\
\n\
بایټونه_2 is hex with 0x or 0X مختاړی, and may have a multiplier suffix:\n\
  b    512\n\
  KB   1000\n\
  K    1024\n\
  MB   1000*1000\n\
  M    1024*1024\n\
and so on لكل G, T, P, E, Z, Y, R, Q.\n\
دوګونی_2 prefixes can be کارول_شوی, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
}

/* Define the print functions.  */

/* Print N spaces, where 0 <= N.
   Do not rely on printf ("%*s", N, "") as N may exceed INT_MAX.  */
ساكن فراغ
print_n_spaces (intmax_t n)
{
  لكل (; 0 < n; n--)
    توری_چاپ_کړه (' ');
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
pad_at_ډکېدنه (idx_t fields)
{
  intmax_t حاصل_ضرب;
  أرجع ckd_mul (&حاصل_ضرب, fields - 1, fields - 1);
}

#تعریف_کړه PRINT_FIELDS(N, T, FMT_STRING_DECL, ACTION)                     \
ساكن فراغ                                                             \
N (idx_t fields, idx_t blank, فراغ ثابت *بلاک,			\
   FMT_STRING_DECL, صحيح پلنوالی, idx_t pad)				\
{                                                                       \
  T ثابت *p = بلاک;                                                   \
  idx_t pad_پاتې_شوی = pad;						\
  لكل (idx_t i = fields; blank < i; i--)				\
    {                                                                   \
      idx_t بل_pad = pad_at (fields, i - 1, pad);			\
      صحيح adjusted_پلنوالی = pad_پاتې_شوی - بل_pad + پلنوالی;            \
      T x;                                                              \
      إذا (ننوت_بدلون && حجم (T) > 1)                                 \
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
      pad_پاتې_شوی = بل_pad;                                         \
    }                                                                   \
}

#تعریف_کړه PRINT_TYPE(N, T)                                                \
  PRINT_FIELDS (N, T, حرف ثابت *fmt_string,                           \
                xprintf (fmt_string, adjusted_پلنوالی, x))

#تعریف_کړه PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
  PRINT_FIELDS (N, T, MAYBE_ناکارول_شوی حرف ثابت *fmt_string,              \
                حرف buf[BUFSIZE];                                      \
                FTOASTR (buf, حجم buf, 0, 0, x);                     \
                xprintf ("%*s", adjusted_پلنوالی, buf))

PRINT_TYPE (print_s_char, نښهدار حرف)
PRINT_TYPE (print_char, بینښه حرف)
PRINT_TYPE (print_s_short, لنډ صحيح)
PRINT_TYPE (print_short, بینښه لنډ صحيح)
PRINT_TYPE (print_int, بینښه صحيح)
PRINT_TYPE (print_long, بینښه اوږد صحيح)
PRINT_TYPE (print_long_long, بینښه اوږد اوږد صحيح)
PRINT_TYPE (print_intmax, uintmax_t)

PRINT_FLOATTYPE (print_bfloat, bfloat16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_halffloat, float16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_float, عائم, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_double, مزدوج, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_long_double, اوږد مزدوج, ldtoastr, LDBL_BUFSIZE_BOUND)

#تعریف_لغوه PRINT_TYPE
#تعریف_لغوه PRINT_FLOATTYPE

ساكن فراغ
dump_hexl_ډېرپېښ_trailer (idx_t n_بایټونه, حرف ثابت *بلاک)
{
  fputs ("  >", stdout);
  لكل (idx_t i = n_بایټونه; i > 0; i--)
    {
      بینښه حرف c = *بلاک++;
      بینښه حرف c2 = (isprint (c) ? c : '.');
      توری_چاپ_کړه (c2);
    }
  توری_چاپ_کړه ('<');
}

ساكن فراغ
print_named_ascii (idx_t fields, idx_t blank, فراغ ثابت *بلاک,
                   MAYBE_ناکارول_شوی حرف ثابت *ناکارول_شوی_fmt_string,
                   صحيح پلنوالی, idx_t pad)
{
  بینښه حرف ثابت *p = بلاک;
  idx_t pad_پاتې_شوی = pad;
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

      idx_t بل_pad = pad_at (fields, i - 1, pad);
      صحيح adjusted_پلنوالی = pad_پاتې_شوی - بل_pad + پلنوالی;
      xprintf ("%*s", adjusted_پلنوالی, s);
      pad_پاتې_شوی = بل_pad;
    }
}

ساكن فراغ
print_ascii (idx_t fields, idx_t blank, فراغ ثابت *بلاک,
             MAYBE_ناکارول_شوی حرف ثابت *ناکارول_شوی_fmt_string, صحيح پلنوالی,
             idx_t pad)
{
  بینښه حرف ثابت *p = بلاک;
  idx_t pad_پاتې_شوی = pad;
  لكل (idx_t i = fields; blank < i; i--)
    {
      بینښه حرف c = *p++;
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

      idx_t بل_pad = pad_at (fields, i - 1, pad);
      صحيح adjusted_پلنوالی = pad_پاتې_شوی - بل_pad + پلنوالی;
      xprintf ("%*s", adjusted_پلنوالی, s);
      pad_پاتې_شوی = بل_pad;
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
  صحيح مجموعه;

  لكل (مجموعه = 0; c_isdigit (*s); s++)
    إذا (ckd_mul (&مجموعه, مجموعه, 10) || ckd_جمع_کړه (&مجموعه, مجموعه, *s - '0'))
      أرجع false;
  *p = s;
  *val = مجموعه;
  أرجع true;
}

/* Return a printf format for SPEC, given that remaining arguments are
   formats for int, long, long long, and intmax_t, respectively.  */
ساكن حرف ثابت *
ispec_منزل_format (تعداد اندازه_spec اندازه_spec,
                 حرف ثابت *int_format,
                 حرف ثابت *long_format,
                 حرف ثابت *long_long_format,
                 حرف ثابت *intmax_format)
{
  أرجع (LONG < INTMAX && اندازه_spec == INTMAX ? intmax_format
          : LONG < LONG_LONG && اندازه_spec == LONG_LONG ? long_long_format
          : INT < LONG && اندازه_spec == LONG ? long_format
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
decode_one_format (حرف ثابت *s_orig, حرف ثابت *s, حرف ثابت **بل,
                   بنية tspec *tspec)
{
  تعداد اندازه_spec اندازه_spec;
  صحيح اندازه;
  تعداد output_format fmt;
  print_function_type print_function;
  حرف ثابت *p;
  حرف c;
  صحيح ډګر_پلنوالی;

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
          اندازه = حجم (بینښه حرف);
          اكسر;

        حالة 'S':
          ++s;
          اندازه = حجم (بینښه لنډ صحيح);
          اكسر;

        حالة 'I':
          ++s;
          اندازه = حجم (بینښه صحيح);
          اكسر;

        حالة 'L':
          ++s;
          اندازه = حجم (بینښه اوږد صحيح);
          اكسر;

        افتراضي:
          إذا (! simple_strtoi (s, &p, &اندازه))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              أرجع false;
            }
          إذا (p == s)
            اندازه = حجم (بینښه صحيح);
          وإلا
            {
              إذا (countof (integral_type_اندازه) <= اندازه
                  || integral_type_اندازه[اندازه] == NO_اندازه)
                {
                  error (0, 0, _("invalid type string %s;\nthis system"
                                 " doesn't provide a %d-byte integral type"),
                         quote (s_orig), اندازه);
                  أرجع false;
                }
              s = p;
            }
          اكسر;
        }

      اندازه_spec = integral_type_اندازه[اندازه];

      بدل (c)
        {
        حالة 'd':
          fmt = SIGNED_DECIMAL;
          ډګر_پلنوالی = INT_بټونه_STRLEN_BOUND (CHAR_BIT * اندازه - 1) + 1;
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_منزل_format (اندازه_spec, "d", "ld", "lld", "jd"));
          اكسر;

        حالة 'o':
          fmt = OCTAL;
          ډګر_پلنوالی = (CHAR_BIT * اندازه + 2) / 3;
          sprintf (tspec->fmt_string, "%%*.%d%s", ډګر_پلنوالی,
                   ispec_منزل_format (اندازه_spec, "o", "lo", "llo", "jo"));
          اكسر;

        حالة 'u':
          fmt = UNSIGNED_DECIMAL;
          ډګر_پلنوالی = INT_بټونه_STRLEN_BOUND (CHAR_BIT * اندازه);
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_منزل_format (اندازه_spec, "u", "lu", "llu", "ju"));
          اكسر;

        حالة 'x':
          fmt = HEXADECIMAL;
          ډګر_پلنوالی = (CHAR_BIT * اندازه + 3) / 4;
          sprintf (tspec->fmt_string, "%%*.%d%s", ډګر_پلنوالی,
                   ispec_منزل_format (اندازه_spec, "x", "lx", "llx", "jx"));
          اكسر;

        افتراضي:
          affirm (false);
        }

      /* Prefer INT, prefer LONG to longer types,
         and prefer INTMAX to LONG_LONG.  */
      print_function
        = (اندازه_spec == INT
           ? print_int
           : SHORT < INT && اندازه_spec == SHORT
           ? (fmt == SIGNED_DECIMAL ? print_s_short : print_short)
           : CHAR < SHORT && اندازه_spec == CHAR
           ? (fmt == SIGNED_DECIMAL ? print_s_char : print_char)
           : INT < LONG && اندازه_spec == LONG
           ? print_long
           : LONG < INTMAX && اندازه_spec == INTMAX
           ? print_intmax
           : LONG < LONG_LONG && LONG_LONG < INTMAX && اندازه_spec == LONG_LONG
           ? print_long_long
           : (affirm (false), (print_function_type) عدم));
      اكسر;

    حالة 'f':
      fmt = FLOATING_ټکی;
      ++s;
      بدل (*s)
        {
        حالة 'B':
          ++s;
          fmt = BFLOATING_ټکی;
          اندازه = حجم (bfloat16);
          اكسر;

        حالة 'H':
          ++s;
          fmt = HFLOATING_ټکی;
          اندازه = حجم (float16);
          اكسر;

        حالة 'F':
          ++s;
          اندازه = حجم (عائم);
          اكسر;

        حالة 'D':
          ++s;
          اندازه = حجم (مزدوج);
          اكسر;

        حالة 'L':
          ++s;
          اندازه = حجم (اوږد مزدوج);
          اكسر;

        افتراضي:
          إذا (! simple_strtoi (s, &p, &اندازه))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              أرجع false;
            }
          إذا (p == s)
            اندازه = حجم (مزدوج);
          وإلا
            {
              إذا (countof (fp_type_اندازه) <= اندازه
                  || fp_type_اندازه[اندازه] == NO_اندازه
                  || (! FLOAT16_SUPPORTED && BF16_SUPPORTED
                      && اندازه == حجم (bfloat16)))
                {
                  error (0, 0,
                         _("invalid type string %s;\n"
                           "this system doesn't provide a %d-byte"
                           " floating point type"),
                         quote (s_orig), اندازه);
                  أرجع false;
                }
              s = p;
            }
          اكسر;
        }
      اندازه_spec = fp_type_اندازه[اندازه];

      إذا ((! FLOAT16_SUPPORTED && fmt == HFLOATING_ټکی)
          || (! BF16_SUPPORTED && fmt == BFLOATING_ټکی))
      {
        error (0, 0,
               _("this system doesn't provide a %s floating point type"),
               quote (s_orig));
        أرجع false;
      }

      {
        بنية lconv ثابت *locale = localeconv ();
        idx_t decimal_ټکی_len =
          (locale->decimal_ټکی[0] ? مزي_اوږدوالی (locale->decimal_ټکی) : 1);

        إذا (اندازه_spec == FLOAT_DOUBLE)
          {
            print_function = print_double;
            ډګر_پلنوالی = DBL_STRLEN_BOUND_L (decimal_ټکی_len);
          }
        وإلا إذا (FLOAT_SINGLE < FLOAT_DOUBLE && اندازه_spec == FLOAT_SINGLE)
          {
            print_function = print_float;
            ډګر_پلنوالی = FLT_STRLEN_BOUND_L (decimal_ټکی_len);
          }
        وإلا إذا (FLOAT_HALF < FLOAT_SINGLE && اندازه_spec == FLOAT_HALF)
          {
            print_function = fmt == BFLOATING_ټکی
                             ? print_bfloat : print_halffloat;
            ډګر_پلنوالی = FLT_STRLEN_BOUND_L (decimal_ټکی_len);
          }
        وإلا إذا (FLOAT_DOUBLE < FLOAT_LONG_DOUBLE
                 && اندازه_spec == FLOAT_LONG_DOUBLE)
          {
            print_function = print_long_double;
            ډګر_پلنوالی = LDBL_STRLEN_BOUND_L (decimal_ټکی_len);
          }
        وإلا
          affirm (false);

        اكسر;
      }

    حالة 'a':
      ++s;
      fmt = NAMED_CHARACTER;
      اندازه_spec = CHAR;
      print_function = print_named_ascii;
      ډګر_پلنوالی = 3;
      اكسر;

    حالة 'c':
      ++s;
      fmt = CHARACTER;
      اندازه_spec = CHAR;
      print_function = print_ascii;
      ډګر_پلنوالی = 3;
      اكسر;

    افتراضي:
      error (0, 0, _("invalid character '%c' in type string %s"),
             *s, quote (s_orig));
      أرجع false;
    }

  tspec->اندازه = اندازه_spec;
  tspec->fmt = fmt;
  tspec->print_function = print_function;

  tspec->ډګر_پلنوالی = ډګر_پلنوالی;
  tspec->hexl_ډېرپېښ_trailer = (*s == 'z');
  إذا (tspec->hexl_ډېرپېښ_trailer)
    s++;

  *بل = s;
  أرجع true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM and the global string INPUT_FILENAME to the
   first one that can be successfully opened. Modify FILE_LIST to
   reference the next filename in the list.  A file name of "-" is
   interpreted as standard input.  If any file open fails, give an error
   message and return false.  */

ساكن bool
open_بل_file (فراغ)
{
  bool ok = true;

  افعل
    {
      ننوت_filename = *file_list;
      إذا (ننوت_filename == عدم)
        أرجع ok;
      ++file_list;

      إذا (streq (ننوت_filename, "-"))
        {
          ننوت_filename = _("standard input");
          دننه_بهیر = stdin;
          have_read_stdin = true;
          xset_دوګونی_ډېرپېښ (STDIN_FILENO, O_دوګونی);
        }
      وإلا
        {
          دننه_بهیر = fopen (ننوت_filename, (O_دوګونی ? "rb" : "r"));
          إذا (دننه_بهیر == عدم)
            {
              error (0, errno, "%s", quotef (ننوت_filename));
              ok = false;
            }
        }
    }
  طالما (دننه_بهیر == عدم);

  إذا (0 <= پای_ټکی_دننه_واټن && !flag_dump_مزي_ګانې)
    setvbuf (دننه_بهیر, عدم, _IONBF, 0);

  أرجع ok;
}

/* Test whether there have been errors on in_stream, and close it if
   it is not standard input.  Return false if there has been an error
   on in_stream or stdout; return true otherwise.  This function will
   report more than one error only if both a read and a write error
   have occurred.  IN_ERRNO, if nonzero, is the error number
   corresponding to the most recent action for IN_STREAM.  */

ساكن bool
check_and_close (صحيح دننه_errno)
{
  bool ok = true;

  إذا (دننه_بهیر != عدم)
    {
      إذا (!ferror (دننه_بهیر))
        دننه_errno = 0;
      إذا (streq (file_list[-1], "-"))
        clearerr (دننه_بهیر);
      وإلا إذا (fclose (دننه_بهیر) != 0 && !دننه_errno)
        دننه_errno = errno;
      إذا (دننه_errno)
        {
          error (0, دننه_errno, "%s", quotef (ننوت_filename));
          ok = false;
        }

      دننه_بهیر = عدم;
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
      حرف ثابت *بل;

      إذا (n_specs_allocated <= n_specs)
        spec = xpalloc (spec, &n_specs_allocated, 1, -1, حجم *spec);

      إذا (! decode_one_format (s_orig, s, &بل, &spec[n_specs]))
        أرجع false;

      affirm (s != بل);
      s = بل;
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
  صحيح دننه_errno = 0;

  إذا (n_skip == 0)
    أرجع true;

  طالما (دننه_بهیر != عدم)	/* EOF.  */
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

      إذا (fstat (fileno (دننه_بهیر), &file_stats) == 0)
        {
          /* The st_size field is valid for regular files.
             If the number of bytes left to skip is larger than
             the size of the current file, we can decrement n_skip
             and go on to the next file.  Skip this optimization also
             when st_size is no greater than the block size, because
             some kernels report nonsense small file sizes for
             proc-like file systems.  */
          إذا (S_ISREG (file_stats.st_ډېرپېښ)
              && STP_BLKSIZE (&file_stats) < file_stats.st_اندازه)
            {
              إذا (file_stats.st_اندازه < n_skip)
                n_skip -= file_stats.st_اندازه;
              وإلا
                {
                  إذا (fseeko (دننه_بهیر, n_skip, SEEK_CUR) != 0)
                    {
                      دننه_errno = errno;
                      ok = false;
                    }
                  n_skip = 0;
                }
            }

          وإلا إذا (! (S_ISREG (file_stats.st_ډېرپېښ)
                      || S_TYPEISSHM (&file_stats)
                      || S_TYPEISTMO (&file_stats))
                   && fseeko (دننه_بهیر, n_skip, SEEK_CUR) == 0)
            n_skip = 0;

          /* If it's neither a seekable file with unusable size, nor a
             regular file so large it can't be in a proc-like file system,
             position the file pointer by reading.  */

          وإلا
            {
              حرف buf[BUFSIZ];
              idx_t n_بایټونه_read, n_بایټونه_منزل_read = BUFSIZ;

              طالما (0 < n_skip)
                {
                  إذا (n_skip < n_بایټونه_منزل_read)
                    n_بایټونه_منزل_read = n_skip;
                  n_بایټونه_read = fread (buf, 1, n_بایټونه_منزل_read, دننه_بهیر);
                  n_skip -= n_بایټونه_read;
                  إذا (n_بایټونه_read != n_بایټونه_منزل_read)
                    {
                      إذا (ferror (دننه_بهیر))
                        {
                          دننه_errno = errno;
                          ok = false;
                          n_skip = 0;
                          اكسر;
                        }
                      إذا (feof (دننه_بهیر))
                        اكسر;
                    }
                }
            }

          إذا (n_skip == 0)
            اكسر;
        }

      وإلا   /* cannot fstat() file */
        {
          error (0, errno, "%s", quotef (ننوت_filename));
          ok = false;
        }

      ok &= check_and_close (دننه_errno);

      ok &= open_بل_file ();
    }

  إذا (n_skip != 0)
    error (EXIT_FAILURE, 0, _("cannot skip past end of combined input"));

  أرجع ok;
}

ساكن فراغ
format_پته_none (MAYBE_ناکارول_شوی intmax_t پته,
                     MAYBE_ناکارول_شوی حرف c)
{
}

ساكن فراغ
format_پته_std (intmax_t پته, حرف c)
{
  حرف buf[MAX_پته_اوږدتیا + 2];
  حرف *p = buf + حجم buf;
  حرف ثابت *pbound;

  *--p = '\0';
  *--p = c;
  pbound = p - پته_pad_len;

  /* Use a special case of the code for each base.  This is measurably
     faster than generic code.  */
  بدل (پته_بنسټ)
    {
    حالة 8:
      افعل
        *--p = '0' + (پته & 7);
      طالما ((پته >>= 3) != 0);
      اكسر;

    حالة 10:
      افعل
        *--p = '0' + (پته % 10);
      طالما ((پته /= 10) != 0);
      اكسر;

    حالة 16:
      افعل
        *--p = "0123456789abcdef"[پته & 15];
      طالما ((پته >>= 4) != 0);
      اكسر;
    }

  طالما (pbound < p)
    *--p = '0';

  fputs (p, stdout);
}

ساكن فراغ
format_پته_paren (intmax_t پته, حرف c)
{
  توری_چاپ_کړه ('(');
  format_پته_std (پته, ')');
  إذا (c)
    توری_چاپ_کړه (c);
}

ساكن فراغ
format_پته_label (intmax_t پته, حرف c)
{
  format_پته_std (پته, ' ');

  intmax_t addr;
  إذا (ckd_جمع_کړه (&addr, پته, pseudo_دننه_واټن))
    error (EXIT_FAILURE, 0, _("pseudo address too large for input"));

  format_پته_paren (addr, c);
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
write_بلاک (intmax_t current_دننه_واټن, idx_t n_بایټونه,
             حرف ثابت *prev_بلاک, حرف ثابت *curr_بلاک)
{
  ساكن bool لومړی = true;
  ساكن bool prev_pair_equal = false;

  إذا (abbreviate_duplicate_بلاکونه
      && !لومړی && n_بایټونه == بایټونه_فی_بلاک
      && memeq (prev_بلاک, curr_بلاک, بایټونه_فی_بلاک))
    {
      إذا (prev_pair_equal)
        {
          /* The two preceding blocks were equal, and the current
             block is the same as the last one, so print nothing.  */
        }
      وإلا
        {
          چاپ_کړه ("*\n");
          prev_pair_equal = true;
        }
    }
  وإلا
    {
      prev_pair_equal = false;
      لكل (idx_t i = 0; i < n_specs; i++)
        {
          صحيح datum_پلنوالی = پلنوالی_بایټونه[spec[i].اندازه];
          idx_t fields_فی_بلاک = بایټونه_فی_بلاک / datum_پلنوالی;
          idx_t blank_fields = (بایټونه_فی_بلاک - n_بایټونه) / datum_پلنوالی;
          إذا (i == 0)
            format_پته (current_دننه_واټن, '\0');
          وإلا
            print_n_spaces (پته_pad_len);
          (*spec[i].print_function) (fields_فی_بلاک, blank_fields,
                                     curr_بلاک, spec[i].fmt_string,
                                     spec[i].ډګر_پلنوالی, spec[i].pad_پلنوالی);
          إذا (spec[i].hexl_ډېرپېښ_trailer)
            {
              /* Space-pad out to full line width, then dump the trailer.  */
              صحيح ډګر_پلنوالی = spec[i].ډګر_پلنوالی;
              لكل (idx_t f = 0; f < blank_fields; f++)
                print_n_spaces (ډګر_پلنوالی);
              idx_t pad_پلنوالی = pad_at (fields_فی_بلاک, blank_fields,
                                        spec[i].pad_پلنوالی);
              print_n_spaces (pad_پلنوالی);
              dump_hexl_ډېرپېښ_trailer (n_بایټونه, curr_بلاک);
            }
          توری_چاپ_کړه ('\n');
        }
    }
  لومړی = false;
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

  طالما (دننه_بهیر && (*c = getc (دننه_بهیر)) < 0)
    {
      ok &= check_and_close (errno);
      ok &= open_بل_file ();
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
read_بلاک (idx_t n, حرف *بلاک, idx_t *n_بایټونه_دننه_بفر)
{
  bool ok = true;

  affirm (0 < n && n <= بایټونه_فی_بلاک);

  *n_بایټونه_دننه_بفر = 0;

  طالما (دننه_بهیر != عدم)	/* EOF.  */
    {
      idx_t n_needed = n - *n_بایټونه_دننه_بفر;
      idx_t n_read = fread (بلاک + *n_بایټونه_دننه_بفر,
                            1, n_needed, دننه_بهیر);

      *n_بایټونه_دننه_بفر += n_read;

      إذا (n_read == n_needed)
        اكسر;

      ok &= check_and_close (errno);

      ok &= open_بل_file ();
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
    l_c_m = lcm (l_c_m, پلنوالی_بایټونه[spec[i].اندازه]);
  أرجع l_c_m;
}

/* Act like xstrtoimax (NPTR, NULL, BASE, VAL, VALID_SUFFIXES),
   except reject negative values, and *VAL may be set if
   LONGINT_INVALID is returned.  */
ساكن strtol_error
xstr2nonneg (حرف ثابت *محدود nptr, صحيح بنسټ_2, intmax_t *val,
             حرف ثابت *محدود valid_suffixes)
{
  strtol_error s_err = xstrtoimax (nptr, عدم, بنسټ_2, val, valid_suffixes);
  أرجع s_err != LONGINT_INVALID && *val < 0 ? LONGINT_INVALID : s_err;
}

/* If STR is a valid traditional offset specification with an optional
   leading '+', and can be represented in intmax_t, return true and
   set *OFFSET to the offset it denotes.  If it is valid but cannot be
   represented, diagnose the problem and exit.  Otherwise return false
   and possibly set *OFFSET.  */

ساكن bool
parse_old_دننه_واټن (حرف *str, intmax_t *دننه_واټن)
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

  تعداد strtol_error s_err = xstr2nonneg (s, radix, دننه_واټن, "bB");

  إذا (dot)
    {
      /* Restore the decimal string's original value.  */
      dot[1] = dot[0];
      dot[0] = '.';
    }

  إذا (s_err == LONGINT_ډکېدنه)
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
  idx_t n_بایټونه_read;

  بلاک[0] = xinmalloc (1 + abbreviate_duplicate_بلاکونه, بایټونه_فی_بلاک);
  بلاک[1] = بلاک[0] + abbreviate_duplicate_بلاکونه * بایټونه_فی_بلاک;

  intmax_t current_دننه_واټن = n_بایټونه_منزل_skip;

  افعل
    {
      intmax_t needed_bound
        = پای_ټکی_دننه_واټن < 0 ? INTMAX_MAX : پای_ټکی_دننه_واټن - current_دننه_واټن;
      إذا (needed_bound <= 0)
        {
          n_بایټونه_read = 0;
          اكسر;
        }
      idx_t n_needed = MIN (بایټونه_فی_بلاک, needed_bound);
      ok &= read_بلاک (n_needed, بلاک[idx], &n_بایټونه_read);
      إذا (n_بایټونه_read < بایټونه_فی_بلاک)
        اكسر;
      affirm (n_بایټونه_read == بایټونه_فی_بلاک);
      write_بلاک (current_دننه_واټن, n_بایټونه_read,
                   بلاک[!idx], بلاک[idx]);
      إذا (ferror (stdout))
        ok = false;
      current_دننه_واټن += n_بایټونه_read;
      idx = !idx;
    }
  طالما (ok);

  إذا (n_بایټونه_read > 0)
    {
      صحيح l_c_m = get_lcm ();

      /* Ensure zero-byte padding up to the smallest multiple of l_c_m that
         is at least as large as n_bytes_read.  */
      idx_t بایټونه_منزل_write = (n_بایټونه_read + l_c_m - 1
                              - (n_بایټونه_read + l_c_m - 1) % l_c_m);

      memset (بلاک[idx] + n_بایټونه_read, 0, بایټونه_منزل_write - n_بایټونه_read);
      write_بلاک (current_دننه_واټن, n_بایټونه_read, بلاک[!idx], بلاک[idx]);
      current_دننه_واټن += n_بایټونه_read;
    }

  إذا (ok || current_دننه_واټن)
    format_پته (current_دننه_واټن, '\n');

  إذا (0 <= پای_ټکی_دننه_واټن && پای_ټکی_دننه_واټن <= current_دننه_واټن)
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
dump_مزي_ګانې (فراغ)
{
  idx_t bufsize = MAX (100, string_min + 1);
  حرف *buf = ximalloc (bufsize);
  intmax_t پته = n_بایټونه_منزل_skip;
  bool ok = true;

  طالما (true)
    {
      idx_t i = 0;
      صحيح c = 1;  /* Init to 1 so can distinguish if NUL read.  */

      إذا (0 <= پای_ټکی_دننه_واټن
          && (پای_ټکی_دننه_واټن < string_min || پای_ټکی_دننه_واټن - string_min < پته))
        اكسر;

      /* Store consecutive printable characters to BUF.  */
      طالما (! (0 <= پای_ټکی_دننه_واټن && پای_ټکی_دننه_واټن <= پته))
        {
          إذا (i == bufsize - 1)
            buf = xpalloc (buf, &bufsize, 1, -1, حجم *buf);
          ok &= read_char (&c);
          إذا (c < 0)
            {
              حرر (buf);
              أرجع ok;
            }
          پته++;
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

      format_پته (پته - i, ' ');

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
      توری_چاپ_کړه ('\n');
    }

  /* We reach this point only if we search through
     (max_bytes_to_format - string_min) bytes before reaching EOF.  */

  حرر (buf);

  ok &= check_and_close (0);
  أرجع ok;
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  صحيح n_files;
  صحيح l_c_m;
  idx_t desired_پلنوالی = 0;
  bool modern = false;
  bool ok = true;
  idx_t پلنوالی_فی_بلاک = 0;
  ساكن حرف ثابت multipliers[] = "bEGKkMmPQRTYZ0";

  /* The maximum number of bytes that will be formatted.
     If negative, there is no limit.  */
  intmax_t max_بایټونه_منزل_format = -1;

  /* The old-style 'pseudo starting address' to be printed in parentheses
     after any true address.  */
  intmax_t pseudo_پیل IF_LINT ( = 0);

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
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
              format_پته = format_پته_std;
              پته_بنسټ = 10;
              پته_pad_len = 7;
              اكسر;
            حالة 'o':
              format_پته = format_پته_std;
              پته_بنسټ = 8;
              پته_pad_len = 7;
              اكسر;
            حالة 'x':
              format_پته = format_پته_std;
              پته_بنسټ = 16;
              پته_pad_len = 6;
              اكسر;
            حالة 'n':
              format_پته = format_پته_none;
              پته_pad_len = 0;
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
          s_err = xstr2nonneg (optarg, 0, &n_بایټونه_منزل_skip, multipliers);
          إذا (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          اكسر;

        حالة 'N':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &max_بایټونه_منزل_format, multipliers);
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
              إذا (s_err == LONGINT_OK && ckd_جمع_کړه (&i, tmp, 1))
                s_err = LONGINT_ډکېدنه;
              إذا (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
              string_min = tmp;
            }
          flag_dump_مزي_ګانې = true;
          اكسر;

        حالة 't':
          modern = true;
          ok &= decode_format_string (optarg);
          اكسر;

        حالة 'v':
          modern = true;
          abbreviate_duplicate_بلاکونه = false;
          اكسر;

        حالة TRADITIONAL_OPTION:
          traditional = true;
          اكسر;

        حالة ENDIAN_OPTION:
          بدل (XARGMATCH ("--endian", optarg, endian_دلیلونه, endian_types))
            {
            حالة endian_big:
              ننوت_بدلون = BYTE_ORDER != BIG_ENDIAN;
              اكسر;
            حالة endian_little:
              ننوت_بدلون = BYTE_ORDER != LITTLE_ENDIAN;
              اكسر;
            }
          اكسر;

          /* The next several cases map the traditional format
             specification options to the corresponding modern format
             specs.  GNU od accepts any combination of old- and
             new-style options.  Format specification options accumulate.
             The obsolescent and undocumented formats are compatible
             with FreeBSD 4.10 od.  */

#تعریف_کړه CASE_OLD_ARG(old_char,new_string)		\
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

#تعریف_لغوه CASE_OLD_ARG

        حالة 'w':
          modern = true;
          إذا (optarg == عدم)
            {
              desired_پلنوالی = 32;
            }
          وإلا
            {
              intmax_t w_tmp;
              s_err = xstr2nonneg (optarg, 10, &w_tmp, "");
              إذا (s_err == LONGINT_OK)
                {
                  إذا (ckd_جمع_کړه (&desired_پلنوالی, w_tmp, 0))
                    s_err = LONGINT_ډکېدنه;
                  وإلا إذا (desired_پلنوالی == 0)
                    s_err = LONGINT_INVALID;
                }
              إذا (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
            }
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
          اكسر;
        }
    }

  إذا (!ok)
    أرجع EXIT_FAILURE;

  إذا (flag_dump_مزي_ګانې && n_specs > 0)
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
              && parse_old_دننه_واټن (argv[optind], &o1))
            {
              n_بایټونه_منزل_skip = o1;
              --n_files;
              ++argv;
            }
          اكسر;

        حالة 2:
          إذا ((traditional || argv[optind + 1][0] == '+'
               || c_isdigit (argv[optind + 1][0]))
              && parse_old_دننه_واټن (argv[optind + 1], &o2))
            {
              إذا (traditional && parse_old_دننه_واټن (argv[optind], &o1))
                {
                  n_بایټونه_منزل_skip = o1;
                  flag_pseudo_پیل = true;
                  pseudo_پیل = o2;
                  argv += 2;
                  n_files -= 2;
                }
              وإلا
                {
                  n_بایټونه_منزل_skip = o2;
                  --n_files;
                  argv[optind + 1] = argv[optind];
                  ++argv;
                }
            }
          اكسر;

        حالة 3:
          إذا (traditional
              && parse_old_دننه_واټن (argv[optind + 1], &o1)
              && parse_old_دننه_واټن (argv[optind + 2], &o2))
            {
              n_بایټونه_منزل_skip = o1;
              flag_pseudo_پیل = true;
              pseudo_پیل = o2;
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

  إذا (flag_pseudo_پیل)
    {
      إذا (format_پته == format_پته_none)
        {
          پته_بنسټ = 8;
          پته_pad_len = 7;
          format_پته = format_پته_paren;
        }
      وإلا
        format_پته = format_پته_label;
    }

  إذا (0 <= max_بایټونه_منزل_format
      && ckd_جمع_کړه (&پای_ټکی_دننه_واټن, n_بایټونه_منزل_skip, max_بایټونه_منزل_format))
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
  ok = open_بل_file ();
  إذا (دننه_بهیر == عدم)
    اذهب cleanup;

  /* skip over any unwanted header bytes */
  ok &= skip (n_بایټونه_منزل_skip);
  إذا (دننه_بهیر == عدم)
    اذهب cleanup;

  pseudo_دننه_واټن = (flag_pseudo_پیل ? pseudo_پیل - n_بایټونه_منزل_skip : 0);

  /* Compute output block length.  */
  l_c_m = get_lcm ();

  إذا (desired_پلنوالی != 0)
    {
      إذا (desired_پلنوالی % l_c_m == 0)
        بایټونه_فی_بلاک = desired_پلنوالی;
      وإلا
        {
          error (0, 0, _("warning: invalid width %td; using %d instead"),
                 desired_پلنوالی, l_c_m);
          بایټونه_فی_بلاک = l_c_m;
        }
    }
  وإلا
    {
      إذا (l_c_m < DEFAULT_بایټونه_فی_بلاک)
        بایټونه_فی_بلاک = (DEFAULT_بایټونه_فی_بلاک
                           - DEFAULT_بایټونه_فی_بلاک % l_c_m);
      وإلا
        بایټونه_فی_بلاک = l_c_m;
    }

  /* Compute padding necessary to align output block.  */
  لكل (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_فی_بلاک = بایټونه_فی_بلاک / پلنوالی_بایټونه[spec[i].اندازه];
      إذا (pad_at_ډکېدنه (fields_فی_بلاک))
        error (EXIT_FAILURE, 0, _("%td is too large"), desired_پلنوالی);
      idx_t بلاک_پلنوالی = (spec[i].ډګر_پلنوالی + 1) * fields_فی_بلاک;
      إذا (پلنوالی_فی_بلاک < بلاک_پلنوالی)
        پلنوالی_فی_بلاک = بلاک_پلنوالی;
    }
  لكل (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_فی_بلاک = بایټونه_فی_بلاک / پلنوالی_بایټونه[spec[i].اندازه];
      idx_t بلاک_پلنوالی = spec[i].ډګر_پلنوالی * fields_فی_بلاک;
      spec[i].pad_پلنوالی = پلنوالی_فی_بلاک - بلاک_پلنوالی;
    }

#که_تعریف_شوی DEBUG
  چاپ_کړه ("lcm=%d, width_per_block=%td\n", l_c_m, پلنوالی_فی_بلاک);
  لكل (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_فی_بلاک = بایټونه_فی_بلاک / پلنوالی_بایټونه[spec[i].اندازه];
      affirm (بایټونه_فی_بلاک % پلنوالی_بایټونه[spec[i].اندازه] == 0);
      affirm (1 <= spec[i].pad_پلنوالی / fields_فی_بلاک);
      چاپ_کړه ("%td: fmt=\"%s\" in_width=%d out_width=%d pad=%td\n",
              i, spec[i].fmt_string, پلنوالی_بایټونه[spec[i].اندازه],
              spec[i].ډګر_پلنوالی, spec[i].pad_پلنوالی);
    }
#د_که_پای

  ok &= (flag_dump_مزي_ګانې ? dump_مزي_ګانې () : dump ());

cleanup:

  إذا (have_read_stdin && fclose (stdin) < 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
