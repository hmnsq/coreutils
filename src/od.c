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

#شامل <config.h>

#شامل <endian.h>
#شامل <float.h>
#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>
#شامل "system.h"
#شامل "argmatch.h"
#شامل "assure.h"
#شامل "c-ctype.h"
#شامل "ftoastr.h"
#شامل "quote.h"
#شامل "stat-size.h"
#شامل "xbinary-io.h"
#شامل "xprintf.h"
#شامل "xstrtol.h"
#شامل "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "od"

#تعریف_کرو AUTHORS proper_name ("Jim Meyering")

/* The default number of input bytes per output line.  */
#تعریف_کرو DEFAULT_بائٹیں_فی_بلاک 16

#اگر FLOAT16_SUPPORTED
  /* Available since clang 6 (2018), and gcc 7 (2017).  */
  قسمتعریف _Float16 float16;
#ورنہ
# define FLOAT16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  قسمتعریف اعشاری float16;
#اگر_ختم

#اگر BF16_SUPPORTED
  /* Available since clang 11 (2020), and gcc 13 (2023). */
  قسمتعریف __bf16 bfloat16;
#ورنہ
# define BF16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  قسمتعریف اعشاری bfloat16;
#اگر_ختم

شمار سائز_spec
  {
    NO_سائز = 0,
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
    N_سائز_SPECS
  };

شمار output_format
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

شمار { MAX_INTEGRAL_TYPE_چوڑائی = UINTMAX_چوڑائی };

/* The maximum number of bytes needed for a format string, including
   the trailing nul.  Each format string expects a variable amount of
   padding (guaranteed to be at least 1 plus the field width), then an
   element that will be formatted in the field.  */
شمار
  {
    FMT_بائٹیں_ALLOCATED =
      حجم "%*.%dlld" - حجم "%d" + INT_STRLEN_BOUND (صحیح) + 1
  };

/* Ensure that our choice for FMT_BYTES_ALLOCATED is reasonable.  */
static_assert (MAX_INTEGRAL_TYPE_چوڑائی <= 3 * 99);

/* The type of a print function.  FIELDS is the number of fields per
   line, BLANK is the number of fields to leave blank.  WIDTH is width
   of one field, excluding leading space, and PAD is total pad to
   divide among FIELDS.  PAD is at least as large as FIELDS.  */
قسمتعریف خالی (*print_function_type)
  (idx_t fields, idx_t blank, خالی ثابت *ڈیٹا,
   حرف ثابت *fmt, صحیح چوڑائی, idx_t pad);

/* Each output format specification (from '-t spec' or from
   old-style options) is represented by one of these structures.  */
ساخت tspec
  {
    شمار output_format fmt;
    شمار سائز_spec سائز; /* Type of input object.  */
    print_function_type print_function;
    حرف fmt_string[FMT_بائٹیں_ALLOCATED]; /* Of the style "%*d".  */
    bool hexl_منوال_trailer;
    صحیح میدان_چوڑائی; /* Minimum width of a field, excluding leading space.  */
    idx_t pad_چوڑائی; /* Total padding to be divided among fields.  */
  };

/* Convert enum size_spec to the size of the named type.  */
ساکن ثابت صحیح چوڑائی_بائٹیں[] =
{
  -1,
  حجم (غیرعلامتی حرف),
#اگر UCHAR_MAX < USHRT_MAX
  حجم (غیرعلامتی مختصر صحیح),
#اگر_ختم
#اگر USHRT_MAX < UINT_MAX
  حجم (غیرعلامتی صحیح),
#اگر_ختم
#اگر UINT_MAX < ULONG_MAX
  حجم (غیرعلامتی طویل صحیح),
#اگر_ختم
#اگر ULONG_MAX < ULLONG_MAX
  حجم (غیرعلامتی طویل طویل صحیح),
#اگر_ختم
#اگر ULLONG_MAX < UINTMAX_MAX
  حجم (uintmax_t),
#اگر_ختم
#اگر BF16_SUPPORTED
  حجم (bfloat16),
#وگرنہ_اگر FLOAT16_SUPPORTED
  حجم (float16),
#اگر_ختم
#اگر FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
  حجم (اعشاری),
#اگر_ختم
  حجم (دوہرا),
#اگر DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
  حجم (طویل دوہرا),
#اگر_ختم
};

/* Ensure that for each member of 'enum size_spec' there is an
   initializer in the width_bytes array.  */
static_assert (countof (چوڑائی_بائٹیں) == N_سائز_SPECS);

/* Names for some non-printing characters.  */
ساکن حرف ثابت charname[33][4] =
{
  "nul", "soh", "stx", "etx", "eot", "enq", "ack", "bel",
  "bs", "ht", "nl", "vt", "ff", "cr", "so", "si",
  "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb",
  "can", "em", "sub", "esc", "fs", "gs", "rs", "us",
  "sp"
};

/* Address base (8, 10 or 16).  */
ساکن صحیح پتہ_بنیاد = 8;

/* The number of octal digits required to represent the largest
   address value.  */
شمار { MAX_پتہ_لمبائی = ((INTMAX_چوڑائی - 1) / 3
                             + ((INTMAX_چوڑائی - 1) % 3 != 0)) };

/* Width of a normal address.  */
ساکن صحیح پتہ_pad_len = 7;

/* Minimum length when detecting --strings.  */
ساکن idx_t string_min;

/* True when in --strings mode.  */
ساکن bool flag_dump_لڑیاں;

/* True if we should recognize the older non-option arguments
   that specified at most one file and optional arguments specifying
   offset and pseudo-start address.  */
ساکن bool traditional;

/* True if an old-style 'pseudo-address' was specified.  */
ساکن bool flag_pseudo_آغاز;

/* The difference between the old-style pseudo starting address and
   the number of bytes to skip.  */
ساکن intmax_t pseudo_انحراف;

/* Function that accepts an address and an optional following char,
   and prints the address and char to stdout.  */
ساکن خالی format_پتہ_std (intmax_t, حرف);
ساکن خالی (*format_پتہ) (intmax_t, حرف) = format_پتہ_std;

/* The number of input bytes to skip before formatting and writing.  */
ساکن intmax_t n_بائٹیں_مقصد_skip;

/* The offset of the first byte after the last byte to be formatted.
   If negative, there is no limit.  */
ساکن intmax_t انتہا_انحراف = -1;

/* When true and two or more consecutive blocks are equal, format
   only the first block and output an asterisk alone on the following
   line to indicate that identical blocks have been elided.  */
ساکن bool abbreviate_duplicate_بلاکیں = true;

/* An array of specs describing how to format each input block.  */
ساکن ساخت tspec *spec;

/* The number of format specs.  */
ساکن idx_t n_specs;

/* The allocated length of SPEC.  */
ساکن idx_t n_specs_allocated;

/* The number of input bytes formatted per output line.  It must be
   a multiple of the least common multiple of the sizes associated with
   the specified output types.  It should be as large as possible, but
   no larger than 16 -- unless specified with the -w option.  */
ساکن idx_t بائٹیں_فی_بلاک;

/* Human-readable representation of *file_list (for error messages).
   It differs from file_list[-1] only when file_list[-1] is "-".  */
ساکن حرف ثابت *داخلہ_filename;

/* A null-terminated list of the file-arguments from the command line.  */
ساکن حرف ثابت *ثابت *file_list;

/* Initializer for file_list if no file-arguments
   were specified on the command line.  */
ساکن حرف ثابت *ثابت default_file_list[] = {"-", تہی};

/* The input stream associated with the current file.  */
ساکن FILE *اندر_دھارا;

/* If true, at least one of the files we read was standard input.  */
ساکن bool have_read_stdin;

/* Map the size in bytes to a type identifier.
   When two types have the same machine layout:
     - Prefer unsigned int to other types, as its format is shorter.
     - Prefer unsigned long to higher-ranked types, as it is older.
     - Prefer uintmax_t to unsigned long long int; this wins if %lld
       does not work but %jd does (e.g., MS-Windows).  */
ساکن شمار سائز_spec ثابت integral_type_سائز[] =
  {
#اگر UCHAR_MAX < USHRT_MAX
    [حجم (غیرعلامتی حرف)] = CHAR,
#اگر_ختم
#اگر USHRT_MAX < UINT_MAX
    [حجم (غیرعلامتی مختصر صحیح)] = SHORT,
#اگر_ختم
    [حجم (غیرعلامتی صحیح)] = INT,
#اگر UINT_MAX < ULONG_MAX
    [حجم (غیرعلامتی طویل صحیح)] = LONG,
#اگر_ختم
#اگر ULONG_MAX < ULLONG_MAX && ULLONG_MAX < UINTMAX_MAX
    [حجم (غیرعلامتی طویل طویل صحیح)] = LONG_LONG,
#اگر_ختم
#اگر ULONG_MAX < UINTMAX_MAX
    [حجم (uintmax_t)] = INTMAX,
#اگر_ختم
  };

/* Map the size in bytes to a floating type identifier.
   When two types have the same machine layout:
     - Prefer double to the other types, as its format is shorter.  */
ساکن شمار سائز_spec ثابت fp_type_سائز[] =
  {
#اگر FLOAT16_SUPPORTED
    [حجم (float16)] = FLOAT_HALF,
#وگرنہ_اگر BF16_SUPPORTED
    [حجم (bfloat16)] = FLOAT_HALF,
#اگر_ختم
#اگر FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
    [حجم (اعشاری)] = FLOAT_SINGLE,
#اگر_ختم
    [حجم (دوہرا)] = FLOAT_DOUBLE,
#اگر DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
    [حجم (طویل دوہرا)] = FLOAT_LONG_DOUBLE,
#اگر_ختم
  };

/* Use native endianness by default.  */
ساکن bool داخلہ_تبادلہ;

ساکن حرف ثابت short_options[] = "A:aBbcDdeFfHhIij:LlN:OoS:st:vw::Xx";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  TRADITIONAL_OPTION = CHAR_MAX + 1,
  ENDIAN_OPTION,
};

شمار endian_type
{
  endian_little,
  endian_big
};

ساکن حرف ثابت *ثابت endian_دلائل[] =
{
  "little", "big", تہی
};

ساکن شمار endian_type ثابت endian_types[] =
{
  endian_little, endian_big
};

ساکن ساخت option ثابت long_options[] =
{
  {"skip-bytes", required_دلیل, تہی, 'j'},
  {"address-radix", required_دلیل, تہی, 'A'},
  {"read-bytes", required_دلیل, تہی, 'N'},
  {"format", required_دلیل, تہی, 't'},
  {"output-duplicates", no_دلیل, تہی, 'v'},
  {"strings", optional_دلیل, تہی, 'S'},
  {"traditional", no_دلیل, تہی, TRADITIONAL_OPTION},
  {"width", optional_دلیل, تہی, 'w'},
  {"endian", required_دلیل, تہی, ENDIAN_OPTION },

  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [-abcdfilosx]... [FILE] [[+]انحراف_2[.][b]]\n\
  or:  %s --traditional [OPTION]... [FILE] [[+]انحراف_2[.][b] [+][LABEL][.][b]]\n\
"),
              پروگرام_name, پروگرام_name, پروگرام_name);
      fputs (_("\n\
Write an unambiguous representation, octal بائٹیں by طےشدہ,\n\
کا FILE مقصد standard output.  With مزید than one FILE دلیل,\n\
concatenate them اندر the listed ترتیب مقصد form the داخلہ.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
If پہلا and دوسرا call formats both apply, the دوسرا format is assumed\n\
اگر the آخری عملیہ_2 begins with + or (اگر there are 2 operands) a digit.\n\
An انحراف_2 عملیہ_2 means -j انحراف_2.  LABEL is the pseudo-پتہ\n\
at پہلا byte printed, incremented when dump is progressing.\n\
For انحراف_2 and LABEL, a 0x or 0X سابقہ indicates hexadecimal;\n\
suffixes may be . برائے octal and b برائے multiply by 512.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --پتہ-radix=RADIX\n\
         output format برائے file offsets;\n\
         RADIX is one کا [doxn], برائے Decimal, Octal, Hex or None\n\
"));
      oputs (_("\
      --endian={big|little}\n\
         تبادلہ داخلہ بائٹیں according the specified ترتیب\n\
"));
      oputs (_("\
  -j, --skip-بائٹیں=بائٹیں_2\n\
         skip بائٹیں_2 داخلہ بائٹیں پہلا\n\
"));
      oputs (_("\
  -N, --اقرأ-بائٹیں=بائٹیں_2\n\
         حد_بندی dump مقصد بائٹیں_2 داخلہ بائٹیں\n\
"));
      oputs (_("\
  -S بائٹیں_2, --لڑیاں[=بائٹیں_2]\n\
         show only NUL terminated لڑیاں\n\
         کا at کم_سے_کم بائٹیں_2 (طےشدہ 3) printable characters\n\
"));
      oputs (_("\
  -t, --format=TYPE\n\
         select output format or formats\n\
"));
      oputs (_("\
  -v, --output-duplicates\n\
         کرو not use * مقصد mark سطر suppression\n\
"));
      oputs (_("\
  -w[بائٹیں_2], --چوڑائی[=بائٹیں_2]\n\
         output بائٹیں_2 بائٹیں فی output سطر;\n\
         32 is implied when بائٹیں_2 is not specified\n\
"));
      oputs (_("\
      --traditional\n\
         accept arguments اندر تیسرا form above\n\
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
  -b   same as -t o1, select octal بائٹیں\n\
"));
      oputs (_("\
  -c   same as -t c,  select printable characters or backslash escapes\n\
"));
      oputs (_("\
  -d   same as -t u2, select غیرعلامتی decimal 2-byte units\n\
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
TYPE is made اوپر کا one or مزید کا these specifications:\n\
  a          named character, ignoring بلند-ترتیب ثنائی_ہندسہ\n\
  c          printable character or backslash escape\n\
"), stdout);
      fputs (_("\
  d[سائز_2]    علامتی decimal, سائز_2 بائٹیں فی integer\n\
  f[سائز_2]    floating نقطہ, سائز_2 بائٹیں فی اعشاری\n\
  o[سائز_2]    octal, سائز_2 بائٹیں فی integer\n\
  u[سائز_2]    غیرعلامتی decimal, سائز_2 بائٹیں فی integer\n\
  x[سائز_2]    hexadecimal, سائز_2 بائٹیں فی integer\n\
"), stdout);
      fputs (_("\
\n\
سائز_2 is a نمبر.  For TYPE اندر [doux], سائز_2 may also be C برائے\n\
حجم(حرف), S برائے حجم(مختصر), I برائے حجم(صحیح) or L برائے\n\
حجم(طویل).  If TYPE is f, سائز_2 may also be B برائے Brain 16 ثنائی_ہندسہ,\n\
H برائے Half precision اعشاری, F برائے حجم(اعشاری), D برائے حجم(دوہرا),\n\
or L برائے حجم(طویل دوہرا).\n\
"), stdout);
      fputs (_("\
\n\
Adding a z suffix مقصد any type displays printable characters at the انتہا کا\n\
each output سطر.\n\
"), stdout);
      fputs (_("\
\n\
\n\
بائٹیں_2 is hex with 0x or 0X سابقہ, and may have a multiplier suffix:\n\
  b    512\n\
  KB   1000\n\
  K    1024\n\
  MB   1000*1000\n\
  M    1024*1024\n\
and so on برائے G, T, P, E, Z, Y, R, Q.\n\
ثنائی_2 prefixes can be مستعمل, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Define the print functions.  */

/* Print N spaces, where 0 <= N.
   Do not rely on printf ("%*s", N, "") as N may exceed INT_MAX.  */
ساکن خالی
print_n_spaces (intmax_t n)
{
  برائے (; 0 < n; n--)
    حرف_لکھو (' ');
}

/* If there are FIELDS fields, return the total padding up to the
   start of field I, where I < FIELDS.  PAD is the total padding for
   all fields.  The result equals (PAD * I) / FIELDS, except it does
   not suffer from internal overflow.  */
ساکن idx_t
pad_at (idx_t fields, idx_t i, idx_t pad)
{
  /* This implementation assumes that (FIELDS - 1)^2 does not overflow
     intmax_t, an assumption checked by pad_at_overflow.  */
  intmax_t m = pad % fields;
  واپس pad / fields * i + m * i / fields;
}
ساکن bool
pad_at_حد_سے_تجاوز (idx_t fields)
{
  intmax_t حاصل_ضرب;
  واپس ckd_mul (&حاصل_ضرب, fields - 1, fields - 1);
}

#تعریف_کرو PRINT_FIELDS(N, T, FMT_STRING_DECL, ACTION)                     \
ساکن خالی                                                             \
N (idx_t fields, idx_t blank, خالی ثابت *بلاک,			\
   FMT_STRING_DECL, صحیح چوڑائی, idx_t pad)				\
{                                                                       \
  T ثابت *p = بلاک;                                                   \
  idx_t pad_بقایا = pad;						\
  برائے (idx_t i = fields; blank < i; i--)				\
    {                                                                   \
      idx_t اگلا_pad = pad_at (fields, i - 1, pad);			\
      صحیح adjusted_چوڑائی = pad_بقایا - اگلا_pad + چوڑائی;            \
      T x;                                                              \
      اگر (داخلہ_تبادلہ && حجم (T) > 1)                                 \
        {                                                               \
          اتحاد {                                                       \
            T x;                                                        \
            حرف b[حجم (T)];                                         \
          } u;                                                          \
          برائے (idx_t j = 0; j < حجم (T); j++)			\
            u.b[j] = ((حرف ثابت *) p)[حجم (T) - 1 - j];            \
          x = u.x;                                                      \
        }                                                               \
      ورنہ                                                              \
        x = *p;                                                         \
      p++;                                                              \
      ACTION;                                                           \
      pad_بقایا = اگلا_pad;                                         \
    }                                                                   \
}

#تعریف_کرو PRINT_TYPE(N, T)                                                \
  PRINT_FIELDS (N, T, حرف ثابت *fmt_string,                           \
                xprintf (fmt_string, adjusted_چوڑائی, x))

#تعریف_کرو PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
  PRINT_FIELDS (N, T, MAYBE_غیر_مستعمل حرف ثابت *fmt_string,              \
                حرف buf[BUFSIZE];                                      \
                FTOASTR (buf, حجم buf, 0, 0, x);                     \
                xprintf ("%*s", adjusted_چوڑائی, buf))

PRINT_TYPE (print_s_char, علامتی حرف)
PRINT_TYPE (print_char, غیرعلامتی حرف)
PRINT_TYPE (print_s_short, مختصر صحیح)
PRINT_TYPE (print_short, غیرعلامتی مختصر صحیح)
PRINT_TYPE (print_int, غیرعلامتی صحیح)
PRINT_TYPE (print_long, غیرعلامتی طویل صحیح)
PRINT_TYPE (print_long_long, غیرعلامتی طویل طویل صحیح)
PRINT_TYPE (print_intmax, uintmax_t)

PRINT_FLOATTYPE (print_bfloat, bfloat16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_halffloat, float16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_float, اعشاری, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_double, دوہرا, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_long_double, طویل دوہرا, ldtoastr, LDBL_BUFSIZE_BOUND)

#تعریف_منسوخ PRINT_TYPE
#تعریف_منسوخ PRINT_FLOATTYPE

ساکن خالی
dump_hexl_منوال_trailer (idx_t n_بائٹیں, حرف ثابت *بلاک)
{
  fputs ("  >", stdout);
  برائے (idx_t i = n_بائٹیں; i > 0; i--)
    {
      غیرعلامتی حرف c = *بلاک++;
      غیرعلامتی حرف c2 = (isprint (c) ? c : '.');
      حرف_لکھو (c2);
    }
  حرف_لکھو ('<');
}

ساکن خالی
print_named_ascii (idx_t fields, idx_t blank, خالی ثابت *بلاک,
                   MAYBE_غیر_مستعمل حرف ثابت *غیر_مستعمل_fmt_string,
                   صحیح چوڑائی, idx_t pad)
{
  غیرعلامتی حرف ثابت *p = بلاک;
  idx_t pad_بقایا = pad;
  برائے (idx_t i = fields; blank < i; i--)
    {
      صحیح masked_c = *p++ & 0x7f;
      حرف ثابت *s;
      حرف buf[2];

      اگر (masked_c == 127)
        s = "del";
      ورنہ اگر (masked_c <= 040)
        s = charname[masked_c];
      ورنہ
        {
          buf[0] = masked_c;
          buf[1] = 0;
          s = buf;
        }

      idx_t اگلا_pad = pad_at (fields, i - 1, pad);
      صحیح adjusted_چوڑائی = pad_بقایا - اگلا_pad + چوڑائی;
      xprintf ("%*s", adjusted_چوڑائی, s);
      pad_بقایا = اگلا_pad;
    }
}

ساکن خالی
print_ascii (idx_t fields, idx_t blank, خالی ثابت *بلاک,
             MAYBE_غیر_مستعمل حرف ثابت *غیر_مستعمل_fmt_string, صحیح چوڑائی,
             idx_t pad)
{
  غیرعلامتی حرف ثابت *p = بلاک;
  idx_t pad_بقایا = pad;
  برائے (idx_t i = fields; blank < i; i--)
    {
      غیرعلامتی حرف c = *p++;
      حرف ثابت *s;
      حرف buf[4];

      انتخاب (c)
        {
        حالت '\0':
          s = "\\0";
          توڑو;

        حالت '\a':
          s = "\\a";
          توڑو;

        حالت '\b':
          s = "\\b";
          توڑو;

        حالت '\f':
          s = "\\f";
          توڑو;

        حالت '\n':
          s = "\\n";
          توڑو;

        حالت '\r':
          s = "\\r";
          توڑو;

        حالت '\t':
          s = "\\t";
          توڑو;

        حالت '\v':
          s = "\\v";
          توڑو;

        طےشدہ:
          sprintf (buf, (isprint (c) ? "%c" : "%03o"), c);
          s = buf;
        }

      idx_t اگلا_pad = pad_at (fields, i - 1, pad);
      صحیح adjusted_چوڑائی = pad_بقایا - اگلا_pad + چوڑائی;
      xprintf ("%*s", adjusted_چوڑائی, s);
      pad_بقایا = اگلا_pad;
    }
}

/* Convert a null-terminated (possibly zero-length) string S to an
   int value.  If S points to a non-digit set *P to S,
   *VAL to 0, and return true.  Otherwise, accumulate the integer value of
   the string of digits.  If the string of digits represents a value
   larger than INT_MAX, don't modify *VAL or *P and return false.
   Otherwise, advance *P to the first non-digit after S, set *VAL to
   the result of the conversion and return true.  */

ساکن bool
simple_strtoi (حرف ثابت *s, حرف ثابت **p, صحیح *val)
{
  صحیح مجموع;

  برائے (مجموع = 0; c_isdigit (*s); s++)
    اگر (ckd_mul (&مجموع, مجموع, 10) || ckd_جوڑو (&مجموع, مجموع, *s - '0'))
      واپس false;
  *p = s;
  *val = مجموع;
  واپس true;
}

/* Return a printf format for SPEC, given that remaining arguments are
   formats for int, long, long long, and intmax_t, respectively.  */
ساکن حرف ثابت *
ispec_مقصد_format (شمار سائز_spec سائز_spec,
                 حرف ثابت *int_format,
                 حرف ثابت *long_format,
                 حرف ثابت *long_long_format,
                 حرف ثابت *intmax_format)
{
  واپس (LONG < INTMAX && سائز_spec == INTMAX ? intmax_format
          : LONG < LONG_LONG && سائز_spec == LONG_LONG ? long_long_format
          : INT < LONG && سائز_spec == LONG ? long_format
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

ساکن bool ATTRIBUTE_NONNULL ()
decode_one_format (حرف ثابت *s_orig, حرف ثابت *s, حرف ثابت **اگلا,
                   ساخت tspec *tspec)
{
  شمار سائز_spec سائز_spec;
  صحیح سائز;
  شمار output_format fmt;
  print_function_type print_function;
  حرف ثابت *p;
  حرف c;
  صحیح میدان_چوڑائی;

  انتخاب (*s)
    {
    حالت 'd':
    حالت 'o':
    حالت 'u':
    حالت 'x':
      c = *s;
      ++s;
      انتخاب (*s)
        {
        حالت 'C':
          ++s;
          سائز = حجم (غیرعلامتی حرف);
          توڑو;

        حالت 'S':
          ++s;
          سائز = حجم (غیرعلامتی مختصر صحیح);
          توڑو;

        حالت 'I':
          ++s;
          سائز = حجم (غیرعلامتی صحیح);
          توڑو;

        حالت 'L':
          ++s;
          سائز = حجم (غیرعلامتی طویل صحیح);
          توڑو;

        طےشدہ:
          اگر (! simple_strtoi (s, &p, &سائز))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              واپس false;
            }
          اگر (p == s)
            سائز = حجم (غیرعلامتی صحیح);
          ورنہ
            {
              اگر (countof (integral_type_سائز) <= سائز
                  || integral_type_سائز[سائز] == NO_سائز)
                {
                  error (0, 0, _("invalid type string %s;\nthis system"
                                 " doesn't provide a %d-byte integral type"),
                         quote (s_orig), سائز);
                  واپس false;
                }
              s = p;
            }
          توڑو;
        }

      سائز_spec = integral_type_سائز[سائز];

      انتخاب (c)
        {
        حالت 'd':
          fmt = SIGNED_DECIMAL;
          میدان_چوڑائی = INT_بٹ_STRLEN_BOUND (CHAR_BIT * سائز - 1) + 1;
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_مقصد_format (سائز_spec, "d", "ld", "lld", "jd"));
          توڑو;

        حالت 'o':
          fmt = OCTAL;
          میدان_چوڑائی = (CHAR_BIT * سائز + 2) / 3;
          sprintf (tspec->fmt_string, "%%*.%d%s", میدان_چوڑائی,
                   ispec_مقصد_format (سائز_spec, "o", "lo", "llo", "jo"));
          توڑو;

        حالت 'u':
          fmt = UNSIGNED_DECIMAL;
          میدان_چوڑائی = INT_بٹ_STRLEN_BOUND (CHAR_BIT * سائز);
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_مقصد_format (سائز_spec, "u", "lu", "llu", "ju"));
          توڑو;

        حالت 'x':
          fmt = HEXADECIMAL;
          میدان_چوڑائی = (CHAR_BIT * سائز + 3) / 4;
          sprintf (tspec->fmt_string, "%%*.%d%s", میدان_چوڑائی,
                   ispec_مقصد_format (سائز_spec, "x", "lx", "llx", "jx"));
          توڑو;

        طےشدہ:
          affirm (false);
        }

      /* Prefer INT, prefer LONG to longer types,
         and prefer INTMAX to LONG_LONG.  */
      print_function
        = (سائز_spec == INT
           ? print_int
           : SHORT < INT && سائز_spec == SHORT
           ? (fmt == SIGNED_DECIMAL ? print_s_short : print_short)
           : CHAR < SHORT && سائز_spec == CHAR
           ? (fmt == SIGNED_DECIMAL ? print_s_char : print_char)
           : INT < LONG && سائز_spec == LONG
           ? print_long
           : LONG < INTMAX && سائز_spec == INTMAX
           ? print_intmax
           : LONG < LONG_LONG && LONG_LONG < INTMAX && سائز_spec == LONG_LONG
           ? print_long_long
           : (affirm (false), (print_function_type) تہی));
      توڑو;

    حالت 'f':
      fmt = FLOATING_نقطہ;
      ++s;
      انتخاب (*s)
        {
        حالت 'B':
          ++s;
          fmt = BFLOATING_نقطہ;
          سائز = حجم (bfloat16);
          توڑو;

        حالت 'H':
          ++s;
          fmt = HFLOATING_نقطہ;
          سائز = حجم (float16);
          توڑو;

        حالت 'F':
          ++s;
          سائز = حجم (اعشاری);
          توڑو;

        حالت 'D':
          ++s;
          سائز = حجم (دوہرا);
          توڑو;

        حالت 'L':
          ++s;
          سائز = حجم (طویل دوہرا);
          توڑو;

        طےشدہ:
          اگر (! simple_strtoi (s, &p, &سائز))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              واپس false;
            }
          اگر (p == s)
            سائز = حجم (دوہرا);
          ورنہ
            {
              اگر (countof (fp_type_سائز) <= سائز
                  || fp_type_سائز[سائز] == NO_سائز
                  || (! FLOAT16_SUPPORTED && BF16_SUPPORTED
                      && سائز == حجم (bfloat16)))
                {
                  error (0, 0,
                         _("invalid type string %s;\n"
                           "this system doesn't provide a %d-byte"
                           " floating point type"),
                         quote (s_orig), سائز);
                  واپس false;
                }
              s = p;
            }
          توڑو;
        }
      سائز_spec = fp_type_سائز[سائز];

      اگر ((! FLOAT16_SUPPORTED && fmt == HFLOATING_نقطہ)
          || (! BF16_SUPPORTED && fmt == BFLOATING_نقطہ))
      {
        error (0, 0,
               _("this system doesn't provide a %s floating point type"),
               quote (s_orig));
        واپس false;
      }

      {
        ساخت lconv ثابت *locale = localeconv ();
        idx_t decimal_نقطہ_len =
          (locale->decimal_نقطہ[0] ? لڑی_لمبائی (locale->decimal_نقطہ) : 1);

        اگر (سائز_spec == FLOAT_DOUBLE)
          {
            print_function = print_double;
            میدان_چوڑائی = DBL_STRLEN_BOUND_L (decimal_نقطہ_len);
          }
        ورنہ اگر (FLOAT_SINGLE < FLOAT_DOUBLE && سائز_spec == FLOAT_SINGLE)
          {
            print_function = print_float;
            میدان_چوڑائی = FLT_STRLEN_BOUND_L (decimal_نقطہ_len);
          }
        ورنہ اگر (FLOAT_HALF < FLOAT_SINGLE && سائز_spec == FLOAT_HALF)
          {
            print_function = fmt == BFLOATING_نقطہ
                             ? print_bfloat : print_halffloat;
            میدان_چوڑائی = FLT_STRLEN_BOUND_L (decimal_نقطہ_len);
          }
        ورنہ اگر (FLOAT_DOUBLE < FLOAT_LONG_DOUBLE
                 && سائز_spec == FLOAT_LONG_DOUBLE)
          {
            print_function = print_long_double;
            میدان_چوڑائی = LDBL_STRLEN_BOUND_L (decimal_نقطہ_len);
          }
        ورنہ
          affirm (false);

        توڑو;
      }

    حالت 'a':
      ++s;
      fmt = NAMED_CHARACTER;
      سائز_spec = CHAR;
      print_function = print_named_ascii;
      میدان_چوڑائی = 3;
      توڑو;

    حالت 'c':
      ++s;
      fmt = CHARACTER;
      سائز_spec = CHAR;
      print_function = print_ascii;
      میدان_چوڑائی = 3;
      توڑو;

    طےشدہ:
      error (0, 0, _("invalid character '%c' in type string %s"),
             *s, quote (s_orig));
      واپس false;
    }

  tspec->سائز = سائز_spec;
  tspec->fmt = fmt;
  tspec->print_function = print_function;

  tspec->میدان_چوڑائی = میدان_چوڑائی;
  tspec->hexl_منوال_trailer = (*s == 'z');
  اگر (tspec->hexl_منوال_trailer)
    s++;

  *اگلا = s;
  واپس true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM and the global string INPUT_FILENAME to the
   first one that can be successfully opened. Modify FILE_LIST to
   reference the next filename in the list.  A file name of "-" is
   interpreted as standard input.  If any file open fails, give an error
   message and return false.  */

ساکن bool
open_اگلا_file (خالی)
{
  bool ok = true;

  کرو
    {
      داخلہ_filename = *file_list;
      اگر (داخلہ_filename == تہی)
        واپس ok;
      ++file_list;

      اگر (streq (داخلہ_filename, "-"))
        {
          داخلہ_filename = _("standard input");
          اندر_دھارا = stdin;
          have_read_stdin = true;
          xset_ثنائی_منوال (STDIN_FILENO, O_ثنائی);
        }
      ورنہ
        {
          اندر_دھارا = fopen (داخلہ_filename, (O_ثنائی ? "rb" : "r"));
          اگر (اندر_دھارا == تہی)
            {
              error (0, errno, "%s", quotef (داخلہ_filename));
              ok = false;
            }
        }
    }
  جب (اندر_دھارا == تہی);

  اگر (0 <= انتہا_انحراف && !flag_dump_لڑیاں)
    setvbuf (اندر_دھارا, تہی, _IONBF, 0);

  واپس ok;
}

/* Test whether there have been errors on in_stream, and close it if
   it is not standard input.  Return false if there has been an error
   on in_stream or stdout; return true otherwise.  This function will
   report more than one error only if both a read and a write error
   have occurred.  IN_ERRNO, if nonzero, is the error number
   corresponding to the most recent action for IN_STREAM.  */

ساکن bool
check_and_close (صحیح اندر_errno)
{
  bool ok = true;

  اگر (اندر_دھارا != تہی)
    {
      اگر (!ferror (اندر_دھارا))
        اندر_errno = 0;
      اگر (streq (file_list[-1], "-"))
        clearerr (اندر_دھارا);
      ورنہ اگر (fclose (اندر_دھارا) != 0 && !اندر_errno)
        اندر_errno = errno;
      اگر (اندر_errno)
        {
          error (0, اندر_errno, "%s", quotef (داخلہ_filename));
          ok = false;
        }

      اندر_دھارا = تہی;
    }

  اگر (ferror (stdout))
    {
      error (0, 0, _("write error"));
      ok = false;
    }

  واپس ok;
}

/* Decode the modern od format string S.  Append the decoded
   representation to the global array SPEC, reallocating SPEC if
   necessary.  Return true if S is valid.  */

ساکن bool ATTRIBUTE_NONNULL ()
decode_format_string (حرف ثابت *s)
{
  حرف ثابت *s_orig = s;

  جب (*s != '\0')
    {
      حرف ثابت *اگلا;

      اگر (n_specs_allocated <= n_specs)
        spec = xpalloc (spec, &n_specs_allocated, 1, -1, حجم *spec);

      اگر (! decode_one_format (s_orig, s, &اگلا, &spec[n_specs]))
        واپس false;

      affirm (s != اگلا);
      s = اگلا;
      ++n_specs;
    }

  واپس true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM to position N_SKIP in the concatenation of
   those files.  If any file operation fails or if there are fewer than
   N_SKIP bytes in the combined input, give an error message and return
   false.  When possible, use seek rather than read operations to
   advance IN_STREAM.  */

ساکن bool
skip (intmax_t n_skip)
{
  bool ok = true;
  صحیح اندر_errno = 0;

  اگر (n_skip == 0)
    واپس true;

  جب (اندر_دھارا != تہی)	/* EOF.  */
    {
      ساخت stat file_stats;

      /* First try seeking.  For large offsets, this extra work is
         worthwhile.  If the offset is below some threshold it may be
         more efficient to move the pointer by reading.  There are two
         issues when trying to seek:
           - the file must be seekable.
           - before seeking to the specified position, make sure
             that the new position is in the current file.
             Try to do that by getting file's size using fstat.
             But that will work only for regular files.  */

      اگر (fstat (fileno (اندر_دھارا), &file_stats) == 0)
        {
          /* The st_size field is valid for regular files.
             If the number of bytes left to skip is larger than
             the size of the current file, we can decrement n_skip
             and go on to the next file.  Skip this optimization also
             when st_size is no greater than the block size, because
             some kernels report nonsense small file sizes for
             proc-like file systems.  */
          اگر (S_ISREG (file_stats.st_منوال)
              && STP_BLKSIZE (&file_stats) < file_stats.st_سائز)
            {
              اگر (file_stats.st_سائز < n_skip)
                n_skip -= file_stats.st_سائز;
              ورنہ
                {
                  اگر (fseeko (اندر_دھارا, n_skip, SEEK_CUR) != 0)
                    {
                      اندر_errno = errno;
                      ok = false;
                    }
                  n_skip = 0;
                }
            }

          ورنہ اگر (! (S_ISREG (file_stats.st_منوال)
                      || S_TYPEISSHM (&file_stats)
                      || S_TYPEISTMO (&file_stats))
                   && fseeko (اندر_دھارا, n_skip, SEEK_CUR) == 0)
            n_skip = 0;

          /* If it's neither a seekable file with unusable size, nor a
             regular file so large it can't be in a proc-like file system,
             position the file pointer by reading.  */

          ورنہ
            {
              حرف buf[BUFSIZ];
              idx_t n_بائٹیں_read, n_بائٹیں_مقصد_read = BUFSIZ;

              جب (0 < n_skip)
                {
                  اگر (n_skip < n_بائٹیں_مقصد_read)
                    n_بائٹیں_مقصد_read = n_skip;
                  n_بائٹیں_read = fread (buf, 1, n_بائٹیں_مقصد_read, اندر_دھارا);
                  n_skip -= n_بائٹیں_read;
                  اگر (n_بائٹیں_read != n_بائٹیں_مقصد_read)
                    {
                      اگر (ferror (اندر_دھارا))
                        {
                          اندر_errno = errno;
                          ok = false;
                          n_skip = 0;
                          توڑو;
                        }
                      اگر (feof (اندر_دھارا))
                        توڑو;
                    }
                }
            }

          اگر (n_skip == 0)
            توڑو;
        }

      ورنہ   /* cannot fstat() file */
        {
          error (0, errno, "%s", quotef (داخلہ_filename));
          ok = false;
        }

      ok &= check_and_close (اندر_errno);

      ok &= open_اگلا_file ();
    }

  اگر (n_skip != 0)
    error (EXIT_FAILURE, 0, _("cannot skip past end of combined input"));

  واپس ok;
}

ساکن خالی
format_پتہ_none (MAYBE_غیر_مستعمل intmax_t پتہ,
                     MAYBE_غیر_مستعمل حرف c)
{
}

ساکن خالی
format_پتہ_std (intmax_t پتہ, حرف c)
{
  حرف buf[MAX_پتہ_لمبائی + 2];
  حرف *p = buf + حجم buf;
  حرف ثابت *pbound;

  *--p = '\0';
  *--p = c;
  pbound = p - پتہ_pad_len;

  /* Use a special case of the code for each base.  This is measurably
     faster than generic code.  */
  انتخاب (پتہ_بنیاد)
    {
    حالت 8:
      کرو
        *--p = '0' + (پتہ & 7);
      جب ((پتہ >>= 3) != 0);
      توڑو;

    حالت 10:
      کرو
        *--p = '0' + (پتہ % 10);
      جب ((پتہ /= 10) != 0);
      توڑو;

    حالت 16:
      کرو
        *--p = "0123456789abcdef"[پتہ & 15];
      جب ((پتہ >>= 4) != 0);
      توڑو;
    }

  جب (pbound < p)
    *--p = '0';

  fputs (p, stdout);
}

ساکن خالی
format_پتہ_paren (intmax_t پتہ, حرف c)
{
  حرف_لکھو ('(');
  format_پتہ_std (پتہ, ')');
  اگر (c)
    حرف_لکھو (c);
}

ساکن خالی
format_پتہ_label (intmax_t پتہ, حرف c)
{
  format_پتہ_std (پتہ, ' ');

  intmax_t addr;
  اگر (ckd_جوڑو (&addr, پتہ, pseudo_انحراف))
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

ساکن خالی
write_بلاک (intmax_t current_انحراف, idx_t n_بائٹیں,
             حرف ثابت *پچھلا_بلاک, حرف ثابت *curr_بلاک)
{
  ساکن bool پہلا = true;
  ساکن bool پچھلا_pair_equal = false;

  اگر (abbreviate_duplicate_بلاکیں
      && !پہلا && n_بائٹیں == بائٹیں_فی_بلاک
      && memeq (پچھلا_بلاک, curr_بلاک, بائٹیں_فی_بلاک))
    {
      اگر (پچھلا_pair_equal)
        {
          /* The two preceding blocks were equal, and the current
             block is the same as the last one, so print nothing.  */
        }
      ورنہ
        {
          لکھو ("*\n");
          پچھلا_pair_equal = true;
        }
    }
  ورنہ
    {
      پچھلا_pair_equal = false;
      برائے (idx_t i = 0; i < n_specs; i++)
        {
          صحیح datum_چوڑائی = چوڑائی_بائٹیں[spec[i].سائز];
          idx_t fields_فی_بلاک = بائٹیں_فی_بلاک / datum_چوڑائی;
          idx_t blank_fields = (بائٹیں_فی_بلاک - n_بائٹیں) / datum_چوڑائی;
          اگر (i == 0)
            format_پتہ (current_انحراف, '\0');
          ورنہ
            print_n_spaces (پتہ_pad_len);
          (*spec[i].print_function) (fields_فی_بلاک, blank_fields,
                                     curr_بلاک, spec[i].fmt_string,
                                     spec[i].میدان_چوڑائی, spec[i].pad_چوڑائی);
          اگر (spec[i].hexl_منوال_trailer)
            {
              /* Space-pad out to full line width, then dump the trailer.  */
              صحیح میدان_چوڑائی = spec[i].میدان_چوڑائی;
              برائے (idx_t f = 0; f < blank_fields; f++)
                print_n_spaces (میدان_چوڑائی);
              idx_t pad_چوڑائی = pad_at (fields_فی_بلاک, blank_fields,
                                        spec[i].pad_چوڑائی);
              print_n_spaces (pad_چوڑائی);
              dump_hexl_منوال_trailer (n_بائٹیں, curr_بلاک);
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

ساکن bool
read_char (صحیح *c)
{
  bool ok = true;

  *c = EOF;

  جب (اندر_دھارا && (*c = getc (اندر_دھارا)) < 0)
    {
      ok &= check_and_close (errno);
      ok &= open_اگلا_file ();
    }

  واپس ok;
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

ساکن bool
read_بلاک (idx_t n, حرف *بلاک, idx_t *n_بائٹیں_اندر_بفر)
{
  bool ok = true;

  affirm (0 < n && n <= بائٹیں_فی_بلاک);

  *n_بائٹیں_اندر_بفر = 0;

  جب (اندر_دھارا != تہی)	/* EOF.  */
    {
      idx_t n_needed = n - *n_بائٹیں_اندر_بفر;
      idx_t n_read = fread (بلاک + *n_بائٹیں_اندر_بفر,
                            1, n_needed, اندر_دھارا);

      *n_بائٹیں_اندر_بفر += n_read;

      اگر (n_read == n_needed)
        توڑو;

      ok &= check_and_close (errno);

      ok &= open_اگلا_file ();
    }

  واپس ok;
}

/* Return the least common multiple of the sizes associated
   with the format specs.  */

ATTRIBUTE_خالص
ساکن صحیح
get_lcm (خالی)
{
  صحیح l_c_m = 1;

  برائے (idx_t i = 0; i < n_specs; i++)
    l_c_m = lcm (l_c_m, چوڑائی_بائٹیں[spec[i].سائز]);
  واپس l_c_m;
}

/* Act like xstrtoimax (NPTR, NULL, BASE, VAL, VALID_SUFFIXES),
   except reject negative values, and *VAL may be set if
   LONGINT_INVALID is returned.  */
ساکن strtol_error
xstr2nonneg (حرف ثابت *محدود nptr, صحیح بنیاد_2, intmax_t *val,
             حرف ثابت *محدود valid_suffixes)
{
  strtol_error s_err = xstrtoimax (nptr, تہی, بنیاد_2, val, valid_suffixes);
  واپس s_err != LONGINT_INVALID && *val < 0 ? LONGINT_INVALID : s_err;
}

/* If STR is a valid traditional offset specification with an optional
   leading '+', and can be represented in intmax_t, return true and
   set *OFFSET to the offset it denotes.  If it is valid but cannot be
   represented, diagnose the problem and exit.  Otherwise return false
   and possibly set *OFFSET.  */

ساکن bool
parse_old_انحراف (حرف *str, intmax_t *انحراف)
{
  /* Skip over any leading '+'.  */
  حرف *s = str + (str[0] == '+');

  اگر (! c_isdigit (s[0]))
    واپس false;

  /* Determine the radix for S.  If there is a '.', followed first by
     optional 'b' or (undocumented) 'B' and then by end of string,
     it's decimal, otherwise, if the string begins with '0X' or '0x',
     it's hexadecimal, else octal.  */
  حرف *dot = strchr (s, '.');
  اگر (dot && dot[(dot[1] == 'b' || dot[1] == 'B') + 1])
    dot = تہی;
  صحیح radix = dot ? 10 : s[0] == '0' && (s[1] == 'x' || s[1] == 'X') ? 16 : 8;

  اگر (dot)
    {
      /* Temporarily remove the '.' from the decimal string.  */
      dot[0] = dot[1];
      dot[1] = '\0';
    }

  شمار strtol_error s_err = xstr2nonneg (s, radix, انحراف, "bB");

  اگر (dot)
    {
      /* Restore the decimal string's original value.  */
      dot[1] = dot[0];
      dot[0] = '.';
    }

  اگر (s_err == LONGINT_حد_سے_تجاوز)
    error (EXIT_FAILURE, ERANGE, "%s", quotef (str));
  واپس s_err == LONGINT_OK;
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

ساکن bool
dump (خالی)
{
  حرف *بلاک[2];
  bool idx = false;
  bool ok = true;
  idx_t n_بائٹیں_read;

  بلاک[0] = xinmalloc (1 + abbreviate_duplicate_بلاکیں, بائٹیں_فی_بلاک);
  بلاک[1] = بلاک[0] + abbreviate_duplicate_بلاکیں * بائٹیں_فی_بلاک;

  intmax_t current_انحراف = n_بائٹیں_مقصد_skip;

  کرو
    {
      intmax_t needed_bound
        = انتہا_انحراف < 0 ? INTMAX_MAX : انتہا_انحراف - current_انحراف;
      اگر (needed_bound <= 0)
        {
          n_بائٹیں_read = 0;
          توڑو;
        }
      idx_t n_needed = MIN (بائٹیں_فی_بلاک, needed_bound);
      ok &= read_بلاک (n_needed, بلاک[idx], &n_بائٹیں_read);
      اگر (n_بائٹیں_read < بائٹیں_فی_بلاک)
        توڑو;
      affirm (n_بائٹیں_read == بائٹیں_فی_بلاک);
      write_بلاک (current_انحراف, n_بائٹیں_read,
                   بلاک[!idx], بلاک[idx]);
      اگر (ferror (stdout))
        ok = false;
      current_انحراف += n_بائٹیں_read;
      idx = !idx;
    }
  جب (ok);

  اگر (n_بائٹیں_read > 0)
    {
      صحیح l_c_m = get_lcm ();

      /* Ensure zero-byte padding up to the smallest multiple of l_c_m that
         is at least as large as n_bytes_read.  */
      idx_t بائٹیں_مقصد_write = (n_بائٹیں_read + l_c_m - 1
                              - (n_بائٹیں_read + l_c_m - 1) % l_c_m);

      memset (بلاک[idx] + n_بائٹیں_read, 0, بائٹیں_مقصد_write - n_بائٹیں_read);
      write_بلاک (current_انحراف, n_بائٹیں_read, بلاک[!idx], بلاک[idx]);
      current_انحراف += n_بائٹیں_read;
    }

  اگر (ok || current_انحراف)
    format_پتہ (current_انحراف, '\n');

  اگر (0 <= انتہا_انحراف && انتہا_انحراف <= current_انحراف)
    ok &= check_and_close (0);

  آزاد (بلاک[0]);

  واپس ok;
}

/* STRINGS mode.  Find each "string constant" in the input.
   A string constant is a run of at least STRING_MIN
   printable characters terminated by a NUL or END_OFFSET.
   Based on a function written by Richard Stallman for a
   traditional version of od.  Return true if successful.  */

ساکن bool
dump_لڑیاں (خالی)
{
  idx_t bufsize = MAX (100, string_min + 1);
  حرف *buf = ximalloc (bufsize);
  intmax_t پتہ = n_بائٹیں_مقصد_skip;
  bool ok = true;

  جب (true)
    {
      idx_t i = 0;
      صحیح c = 1;  /* Init to 1 so can distinguish if NUL read.  */

      اگر (0 <= انتہا_انحراف
          && (انتہا_انحراف < string_min || انتہا_انحراف - string_min < پتہ))
        توڑو;

      /* Store consecutive printable characters to BUF.  */
      جب (! (0 <= انتہا_انحراف && انتہا_انحراف <= پتہ))
        {
          اگر (i == bufsize - 1)
            buf = xpalloc (buf, &bufsize, 1, -1, حجم *buf);
          ok &= read_char (&c);
          اگر (c < 0)
            {
              آزاد (buf);
              واپس ok;
            }
          پتہ++;
          buf[i++] = c;
          اگر (c == '\0')
            توڑو;		/* Print this string.  */
          اگر (! isprint (c))
            {
              c = -1;		/* Give up on this string.  */
              توڑو;
            }
        }

      اگر (c < 0 || i - !c < string_min)
        جاری;

      buf[i] = 0;

      /* If we get here, the string is all printable, so print it.  */

      format_پتہ (پتہ - i, ' ');

      برائے (i = 0; (c = buf[i]); i++)
        {
          انتخاب (c)
            {
            حالت '\a':
              fputs ("\\a", stdout);
              توڑو;

            حالت '\b':
              fputs ("\\b", stdout);
              توڑو;

            حالت '\f':
              fputs ("\\f", stdout);
              توڑو;

            حالت '\n':
              fputs ("\\n", stdout);
              توڑو;

            حالت '\r':
              fputs ("\\r", stdout);
              توڑو;

            حالت '\t':
              fputs ("\\t", stdout);
              توڑو;

            حالت '\v':
              fputs ("\\v", stdout);
              توڑو;

            طےشدہ:
              putc (c, stdout);
            }
        }
      حرف_لکھو ('\n');
    }

  /* We reach this point only if we search through
     (max_bytes_to_format - string_min) bytes before reaching EOF.  */

  آزاد (buf);

  ok &= check_and_close (0);
  واپس ok;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح n_files;
  صحیح l_c_m;
  idx_t desired_چوڑائی = 0;
  bool modern = false;
  bool ok = true;
  idx_t چوڑائی_فی_بلاک = 0;
  ساکن حرف ثابت multipliers[] = "bEGKkMmPQRTYZ0";

  /* The maximum number of bytes that will be formatted.
     If negative, there is no limit.  */
  intmax_t max_بائٹیں_مقصد_format = -1;

  /* The old-style 'pseudo starting address' to be printed in parentheses
     after any true address.  */
  intmax_t pseudo_آغاز IF_LINT ( = 0);

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب (true)
    {
      intmax_t tmp;
      شمار strtol_error s_err;
      صحیح oi = -1;
      صحیح c = getopt_long (argc, argv, short_options, long_options, &oi);
      اگر (c == -1)
        توڑو;

      انتخاب (c)
        {
        حالت 'A':
          modern = true;
          انتخاب (optarg[0])
            {
            حالت 'd':
              format_پتہ = format_پتہ_std;
              پتہ_بنیاد = 10;
              پتہ_pad_len = 7;
              توڑو;
            حالت 'o':
              format_پتہ = format_پتہ_std;
              پتہ_بنیاد = 8;
              پتہ_pad_len = 7;
              توڑو;
            حالت 'x':
              format_پتہ = format_پتہ_std;
              پتہ_بنیاد = 16;
              پتہ_pad_len = 6;
              توڑو;
            حالت 'n':
              format_پتہ = format_پتہ_none;
              پتہ_pad_len = 0;
              توڑو;
            طےشدہ:
              error (EXIT_FAILURE, 0,
                     _("invalid output address radix '%c';"
                       " it must be one character from [doxn]"),
                     optarg[0]);
              توڑو;
            }
          توڑو;

        حالت 'j':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &n_بائٹیں_مقصد_skip, multipliers);
          اگر (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          توڑو;

        حالت 'N':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &max_بائٹیں_مقصد_format, multipliers);
          اگر (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          توڑو;

        حالت 'S':
          modern = true;
          اگر (optarg == تہی)
            string_min = 3;
          ورنہ
            {
              s_err = xstr2nonneg (optarg, 0, &tmp, multipliers);
              /* The minimum string length + 1 must fit in idx_t,
                 since we may allocate a buffer of this size + 1.  */
              idx_t i;
              اگر (s_err == LONGINT_OK && ckd_جوڑو (&i, tmp, 1))
                s_err = LONGINT_حد_سے_تجاوز;
              اگر (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
              string_min = tmp;
            }
          flag_dump_لڑیاں = true;
          توڑو;

        حالت 't':
          modern = true;
          ok &= decode_format_string (optarg);
          توڑو;

        حالت 'v':
          modern = true;
          abbreviate_duplicate_بلاکیں = false;
          توڑو;

        حالت TRADITIONAL_OPTION:
          traditional = true;
          توڑو;

        حالت ENDIAN_OPTION:
          انتخاب (XARGMATCH ("--endian", optarg, endian_دلائل, endian_types))
            {
            حالت endian_big:
              داخلہ_تبادلہ = BYTE_ORDER != BIG_ENDIAN;
              توڑو;
            حالت endian_little:
              داخلہ_تبادلہ = BYTE_ORDER != LITTLE_ENDIAN;
              توڑو;
            }
          توڑو;

          /* The next several cases map the traditional format
             specification options to the corresponding modern format
             specs.  GNU od accepts any combination of old- and
             new-style options.  Format specification options accumulate.
             The obsolescent and undocumented formats are compatible
             with FreeBSD 4.10 od.  */

#تعریف_کرو CASE_OLD_ARG(old_char,new_string)		\
        حالت old_char:					\
          ok &= decode_format_string (new_string);	\
          توڑو

          CASE_OLD_ARG ('a', "a");
          CASE_OLD_ARG ('b', "o1");
          CASE_OLD_ARG ('c', "c");
          CASE_OLD_ARG ('D', "u4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('d', "u2");
        حالت 'F': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('e', "fD"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('f', "fF");
        حالت 'X': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('H', "x4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('i', "dI");
        حالت 'I': حالت 'L': /* obsolescent and undocumented aliases */
          CASE_OLD_ARG ('l', "dL");
          CASE_OLD_ARG ('O', "o4"); /* obsolescent and undocumented */
        حالت 'B': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('o', "o2");
          CASE_OLD_ARG ('s', "d2");
        حالت 'h': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('x', "x2");

#تعریف_منسوخ CASE_OLD_ARG

        حالت 'w':
          modern = true;
          اگر (optarg == تہی)
            {
              desired_چوڑائی = 32;
            }
          ورنہ
            {
              intmax_t w_tmp;
              s_err = xstr2nonneg (optarg, 10, &w_tmp, "");
              اگر (s_err == LONGINT_OK)
                {
                  اگر (ckd_جوڑو (&desired_چوڑائی, w_tmp, 0))
                    s_err = LONGINT_حد_سے_تجاوز;
                  ورنہ اگر (desired_چوڑائی == 0)
                    s_err = LONGINT_INVALID;
                }
              اگر (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
            }
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
          توڑو;
        }
    }

  اگر (!ok)
    واپس EXIT_FAILURE;

  اگر (flag_dump_لڑیاں && n_specs > 0)
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

  اگر (!modern || traditional)
    {
      intmax_t o1;
      intmax_t o2;

      انتخاب (n_files)
        {
        حالت 1:
          اگر ((traditional || argv[optind][0] == '+')
              && parse_old_انحراف (argv[optind], &o1))
            {
              n_بائٹیں_مقصد_skip = o1;
              --n_files;
              ++argv;
            }
          توڑو;

        حالت 2:
          اگر ((traditional || argv[optind + 1][0] == '+'
               || c_isdigit (argv[optind + 1][0]))
              && parse_old_انحراف (argv[optind + 1], &o2))
            {
              اگر (traditional && parse_old_انحراف (argv[optind], &o1))
                {
                  n_بائٹیں_مقصد_skip = o1;
                  flag_pseudo_آغاز = true;
                  pseudo_آغاز = o2;
                  argv += 2;
                  n_files -= 2;
                }
              ورنہ
                {
                  n_بائٹیں_مقصد_skip = o2;
                  --n_files;
                  argv[optind + 1] = argv[optind];
                  ++argv;
                }
            }
          توڑو;

        حالت 3:
          اگر (traditional
              && parse_old_انحراف (argv[optind + 1], &o1)
              && parse_old_انحراف (argv[optind + 2], &o2))
            {
              n_بائٹیں_مقصد_skip = o1;
              flag_pseudo_آغاز = true;
              pseudo_آغاز = o2;
              argv[optind + 2] = argv[optind];
              argv += 2;
              n_files -= 2;
            }
          توڑو;
        }

      اگر (traditional && 1 < n_files)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
          error (0, 0, "%s",
                 _("compatibility mode supports at most one file"));
          usage (EXIT_FAILURE);
        }
    }

  اگر (flag_pseudo_آغاز)
    {
      اگر (format_پتہ == format_پتہ_none)
        {
          پتہ_بنیاد = 8;
          پتہ_pad_len = 7;
          format_پتہ = format_پتہ_paren;
        }
      ورنہ
        format_پتہ = format_پتہ_label;
    }

  اگر (0 <= max_بائٹیں_مقصد_format
      && ckd_جوڑو (&انتہا_انحراف, n_بائٹیں_مقصد_skip, max_بائٹیں_مقصد_format))
    error (EXIT_FAILURE, 0, _("skip-bytes + read-bytes is too large"));

  اگر (n_specs == 0)
    decode_format_string ("oS");

  اگر (n_files > 0)
    {
      /* Set the global pointer FILE_LIST so that it
         references the first file-argument on the command-line.  */

      file_list = (حرف ثابت *ثابت *) &argv[optind];
    }
  ورنہ
    {
      /* No files were listed on the command line.
         Set the global pointer FILE_LIST so that it
         references the null-terminated list of one name: "-".  */

      file_list = default_file_list;
    }

  /* open the first input file */
  ok = open_اگلا_file ();
  اگر (اندر_دھارا == تہی)
    جاؤ cleanup;

  /* skip over any unwanted header bytes */
  ok &= skip (n_بائٹیں_مقصد_skip);
  اگر (اندر_دھارا == تہی)
    جاؤ cleanup;

  pseudo_انحراف = (flag_pseudo_آغاز ? pseudo_آغاز - n_بائٹیں_مقصد_skip : 0);

  /* Compute output block length.  */
  l_c_m = get_lcm ();

  اگر (desired_چوڑائی != 0)
    {
      اگر (desired_چوڑائی % l_c_m == 0)
        بائٹیں_فی_بلاک = desired_چوڑائی;
      ورنہ
        {
          error (0, 0, _("warning: invalid width %td; using %d instead"),
                 desired_چوڑائی, l_c_m);
          بائٹیں_فی_بلاک = l_c_m;
        }
    }
  ورنہ
    {
      اگر (l_c_m < DEFAULT_بائٹیں_فی_بلاک)
        بائٹیں_فی_بلاک = (DEFAULT_بائٹیں_فی_بلاک
                           - DEFAULT_بائٹیں_فی_بلاک % l_c_m);
      ورنہ
        بائٹیں_فی_بلاک = l_c_m;
    }

  /* Compute padding necessary to align output block.  */
  برائے (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_فی_بلاک = بائٹیں_فی_بلاک / چوڑائی_بائٹیں[spec[i].سائز];
      اگر (pad_at_حد_سے_تجاوز (fields_فی_بلاک))
        error (EXIT_FAILURE, 0, _("%td is too large"), desired_چوڑائی);
      idx_t بلاک_چوڑائی = (spec[i].میدان_چوڑائی + 1) * fields_فی_بلاک;
      اگر (چوڑائی_فی_بلاک < بلاک_چوڑائی)
        چوڑائی_فی_بلاک = بلاک_چوڑائی;
    }
  برائے (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_فی_بلاک = بائٹیں_فی_بلاک / چوڑائی_بائٹیں[spec[i].سائز];
      idx_t بلاک_چوڑائی = spec[i].میدان_چوڑائی * fields_فی_بلاک;
      spec[i].pad_چوڑائی = چوڑائی_فی_بلاک - بلاک_چوڑائی;
    }

#اگر_تعریف_شدہ DEBUG
  لکھو ("lcm=%d, width_per_block=%td\n", l_c_m, چوڑائی_فی_بلاک);
  برائے (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_فی_بلاک = بائٹیں_فی_بلاک / چوڑائی_بائٹیں[spec[i].سائز];
      affirm (بائٹیں_فی_بلاک % چوڑائی_بائٹیں[spec[i].سائز] == 0);
      affirm (1 <= spec[i].pad_چوڑائی / fields_فی_بلاک);
      لکھو ("%td: fmt=\"%s\" in_width=%d out_width=%d pad=%td\n",
              i, spec[i].fmt_string, چوڑائی_بائٹیں[spec[i].سائز],
              spec[i].میدان_چوڑائی, spec[i].pad_چوڑائی);
    }
#اگر_ختم

  ok &= (flag_dump_لڑیاں ? dump_لڑیاں () : dump ());

cleanup:

  اگر (have_read_stdin && fclose (stdin) < 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
