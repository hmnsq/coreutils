/*
   BLAKE2 reference source code package - b2sum tool

   Copyright 2012, Samuel Neves <sneves@dei.uc.pt>.  You may use this under the
   terms of the CC0, the OpenSSL Licence, or the Apache Public License 2.0, at
   your option.  The terms of these licenses can be found at:

   - CC0 1.0 Universal : https://creativecommons.org/publicdomain/zero/1.0
   - OpenSSL license   : https://www.openssl.org/source/license.html
   - Apache 2.0        : https://www.apache.org/licenses/LICENSE-2.0

   More information about the BLAKE2 hash function can be found at
   https://blake2.net.
*/

#جے_تعریف_شدہ HAVE_CONFIG_H
# include <config.h>
#جے_انت

#ضمن <stdio.h>
#ضمن <stdlib.h>

#ضمن "blake2.h"

#إذا 0
/* This will help compatibility with coreutils */
صحيح blake2s_دھارا( FILE *دھارا, فراغ *resstream, size_t outbytes )
{
  صحيح ret = -1;
  size_t جوڑ, n;
  blake2s_state S[1];
  ساكن ثابت size_t بفر_لمائی = 32768;
  uint8_t *بفر = ( uint8_t * )حافظہ_دیو( بفر_لمائی );

  إذا( !بفر ) أرجع -1;

  blake2s_init( S, outbytes );

  طالما( 1 )
  {
    جوڑ = 0;

    طالما( 1 )
    {
      n = fread( بفر + جوڑ, 1, بفر_لمائی - جوڑ, دھارا );
      جوڑ += n;

      إذا( بفر_لمائی == جوڑ )
        اكسر;

      إذا( 0 == n )
      {
        إذا( ferror( دھارا ) )
          اذهب cleanup_بفر;

        اذهب final_process;
      }

      إذا( feof( دھارا ) )
        اذهب final_process;
    }

    blake2s_update( S, بفر, بفر_لمائی );
  }

final_process:;

  إذا( جوڑ > 0 ) blake2s_update( S, بفر, جوڑ );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_بفر:
  حرر( بفر );
  أرجع ret;
}
#جے_انت

صحيح blake2b_دھارا( FILE *دھارا, فراغ *resstream, size_t outbytes )
{
  صحيح ret = -1;
  size_t جوڑ, n;
  blake2b_state S[1];
  ساكن ثابت size_t بفر_لمائی = 32768;
  uint8_t *بفر = ( uint8_t * )حافظہ_دیو( بفر_لمائی );

  إذا( !بفر ) أرجع -1;

  blake2b_init( S, outbytes );

  طالما( 1 )
  {
    جوڑ = 0;

    طالما( 1 )
    {
      n = fread( بفر + جوڑ, 1, بفر_لمائی - جوڑ, دھارا );
      جوڑ += n;

      إذا( بفر_لمائی == جوڑ )
        اكسر;

      إذا( 0 == n )
      {
        إذا( ferror( دھارا ) )
          اذهب cleanup_بفر;

        اذهب final_process;
      }

      إذا( feof( دھارا ) )
        اذهب final_process;
    }

    blake2b_update( S, بفر, بفر_لمائی );
  }

final_process:;

  إذا( جوڑ > 0 ) blake2b_update( S, بفر, جوڑ );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_بفر:
  حرر( بفر );
  أرجع ret;
}

#إذا 0

#ضمن <errno.h>
#ضمن <getopt.h>
#ضمن <string.h>
#ضمن <unistd.h>

صحيح blake2sp_دھارا( FILE *دھارا, فراغ *resstream, size_t outbytes )
{
  صحيح ret = -1;
  size_t جوڑ, n;
  blake2sp_state S[1];
  ساكن ثابت size_t بفر_لمائی = 16 * ( 1UL << 20 );
  uint8_t *بفر = ( uint8_t * )حافظہ_دیو( بفر_لمائی );

  إذا( !بفر ) أرجع -1;

  blake2sp_init( S, outbytes );

  طالما( 1 )
  {
    جوڑ = 0;

    طالما( 1 )
    {
      n = fread( بفر + جوڑ, 1, بفر_لمائی - جوڑ, دھارا );
      جوڑ += n;

      إذا( بفر_لمائی == جوڑ )
        اكسر;

      إذا( 0 == n )
      {
        إذا( ferror( دھارا ) )
          اذهب cleanup_بفر;

        اذهب final_process;
      }

      إذا( feof( دھارا ) )
        اذهب final_process;
    }

    blake2sp_update( S, بفر, بفر_لمائی );
  }

final_process:;

  إذا( جوڑ > 0 ) blake2sp_update( S, بفر, جوڑ );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_بفر:
  حرر( بفر );
  أرجع ret;
}


صحيح blake2bp_دھارا( FILE *دھارا, فراغ *resstream, size_t outbytes )
{
  صحيح ret = -1;
  size_t جوڑ, n;
  blake2bp_state S[1];
  ساكن ثابت size_t بفر_لمائی = 16 * ( 1UL << 20 );
  uint8_t *بفر = ( uint8_t * )حافظہ_دیو( بفر_لمائی );

  إذا( !بفر ) أرجع -1;

  blake2bp_init( S, outbytes );

  طالما( 1 )
  {
    جوڑ = 0;

    طالما( 1 )
    {
      n = fread( بفر + جوڑ, 1, بفر_لمائی - جوڑ, دھارا );
      جوڑ += n;

      إذا( بفر_لمائی == جوڑ )
        اكسر;

      إذا( 0 == n )
      {
        إذا( ferror( دھارا ) )
          اذهب cleanup_بفر;

        اذهب final_process;
      }

      إذا( feof( دھارا ) )
        اذهب final_process;
    }

    blake2bp_update( S, بفر, بفر_لمائی );
  }

final_process:;

  إذا( جوڑ > 0 ) blake2bp_update( S, بفر, جوڑ );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_بفر:
  حرر( بفر );
  أرجع ret;
}

تعريفنوع صحيح ( *blake2fn )( FILE *, فراغ *, size_t );


ساكن فراغ usage( حرف **argv, صحيح errcode )
{
  FILE *باہرلا = errcode ? stderr : stdout;
  fprintf( باہرلا, "Usage: %s [OPTION]... [FILE]...\n", argv[0] );
  fprintf( باہرلا, "\n" );
  fprintf( باہرلا, "With no FILE, or when FILE is -, read standard input.\n" );
  fprintf( باہرلا, "\n" );
  fprintf( باہرلا, "  -a <algo>    hash algorithm (blake2b is default): \n"
                "               [blake2b|blake2s|blake2bp|blake2sp]\n" );
  fprintf( باہرلا, "  -l <length>  digest length in bits, must not exceed the maximum for\n"
                "               the selected algorithm and must be a multiple of 8\n" );
  fprintf( باہرلا, "  --tag        create a BSD-style checksum\n" );
  fprintf( باہرلا, "  --help       display this help and exit\n" );
  باہر_نکلو( errcode );
}


صحيح مکھ( صحيح argc, حرف **argv )
{
  blake2fn blake2_دھارا = blake2b_دھارا;
  بنانشانی لما maxbytes = BLAKE2B_OUTBYTES;
  ثابت حرف *algorithm = "BLAKE2b";
  بنانشانی لما outbytes = 0;
  بنانشانی حرف hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  صحيح c, i;
  opterr = 1;

  طالما( 1 )
  {
    صحيح option_اشاریہ = 0;
    حرف *سرا = عدم;
    بنانشانی لما outbits;
    ساكن بنية option long_options[] = {
      { "help",  no_دلیل, 0,  0  },
      { "tag",   no_دلیل, 0,  0  },
      { عدم, 0, عدم, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_اشاریہ );
    إذا( c == -1 ) اكسر;
    بدل( c )
    {
    حالة 'a':
      إذا( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_دھارا = blake2b_دھارا;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      وإلا إذا ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_دھارا = blake2s_دھارا;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      وإلا إذا ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_دھارا = blake2bp_دھارا;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      وإلا إذا ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_دھارا = blake2sp_دھارا;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      وإلا
      {
        لکھو( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      اكسر;

    حالة 'l':
      outbits = strtoul(optarg, &سرا, 10);
      إذا( !سرا || *سرا != '\0' || outbits % 8 != 0)
      {
        لکھو( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      اكسر;

    حالة 0:
      إذا( 0 == strcmp( "help", long_options[option_اشاریہ].name ) )
        usage( argv, 0 );
      وإلا إذا( 0 == strcmp( "tag", long_options[option_اشاریہ].name ) )
        bsdstyle = true;
      اكسر;

    حالة '?':
      usage( argv, 1 );
      اكسر;
    }
  }

  إذا(outbytes > maxbytes)
  {
    لکھو( "Invalid length argument: %lu\n", outbytes * 8 );
    لکھو( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  وإلا إذا( outbytes == 0 )
    outbytes = maxbytes;

  إذا( optind == argc )
    argv[argc++] = (حرف *) "-";

  لكل( i = optind; i < argc; ++i )
  {
    FILE *f = عدم;
    إذا( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = stdin;
    وإلا
      f = fopen( argv[i], "rb" );

    إذا( !f )
    {
      fprintf( stderr, "Could not open `%s': %s\n", argv[i], strerror( errno ) );
      استمر;
    }

    إذا( blake2_دھارا( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    وإلا
    {
      size_t j;
      إذا( bsdstyle )
      {
        إذا( outbytes < maxbytes )
          لکھو( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        وإلا
          لکھو( "%s (%s) = ", algorithm, argv[i] );
      }

      لكل( j = 0; j < outbytes; ++j )
        لکھو( "%02x", hash[j] );

      إذا( bsdstyle )
        لکھو( "\n" );
      وإلا
        لکھو( "  %s\n", argv[i] );
    }

    إذا( f == stdin )
      clearerr( f );
    وإلا إذا( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  أرجع 0;
}
#جے_انت
