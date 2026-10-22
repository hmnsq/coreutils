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

#اگر_تعریف_شده HAVE_CONFIG_H
# include <config.h>
#پایان_اگر

#ضمن <stdio.h>
#ضمن <stdlib.h>

#ضمن "blake2.h"

#إذا 0
/* This will help compatibility with coreutils */
صحيح blake2s_جریان( FILE *جریان, فراغ *resstream, size_t outbytes )
{
  صحيح ret = -1;
  size_t جمع, n;
  blake2s_state S[1];
  ساكن ثابت size_t میانگیر_درازی = 32768;
  uint8_t *میانگیر = ( uint8_t * )حافظه_بده( میانگیر_درازی );

  إذا( !میانگیر ) أرجع -1;

  blake2s_init( S, outbytes );

  طالما( 1 )
  {
    جمع = 0;

    طالما( 1 )
    {
      n = fread( میانگیر + جمع, 1, میانگیر_درازی - جمع, جریان );
      جمع += n;

      إذا( میانگیر_درازی == جمع )
        اكسر;

      إذا( 0 == n )
      {
        إذا( ferror( جریان ) )
          اذهب cleanup_میانگیر;

        اذهب final_process;
      }

      إذا( feof( جریان ) )
        اذهب final_process;
    }

    blake2s_update( S, میانگیر, میانگیر_درازی );
  }

final_process:;

  إذا( جمع > 0 ) blake2s_update( S, میانگیر, جمع );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_میانگیر:
  حرر( میانگیر );
  أرجع ret;
}
#پایان_اگر

صحيح blake2b_جریان( FILE *جریان, فراغ *resstream, size_t outbytes )
{
  صحيح ret = -1;
  size_t جمع, n;
  blake2b_state S[1];
  ساكن ثابت size_t میانگیر_درازی = 32768;
  uint8_t *میانگیر = ( uint8_t * )حافظه_بده( میانگیر_درازی );

  إذا( !میانگیر ) أرجع -1;

  blake2b_init( S, outbytes );

  طالما( 1 )
  {
    جمع = 0;

    طالما( 1 )
    {
      n = fread( میانگیر + جمع, 1, میانگیر_درازی - جمع, جریان );
      جمع += n;

      إذا( میانگیر_درازی == جمع )
        اكسر;

      إذا( 0 == n )
      {
        إذا( ferror( جریان ) )
          اذهب cleanup_میانگیر;

        اذهب final_process;
      }

      إذا( feof( جریان ) )
        اذهب final_process;
    }

    blake2b_update( S, میانگیر, میانگیر_درازی );
  }

final_process:;

  إذا( جمع > 0 ) blake2b_update( S, میانگیر, جمع );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_میانگیر:
  حرر( میانگیر );
  أرجع ret;
}

#إذا 0

#ضمن <errno.h>
#ضمن <getopt.h>
#ضمن <string.h>
#ضمن <unistd.h>

صحيح blake2sp_جریان( FILE *جریان, فراغ *resstream, size_t outbytes )
{
  صحيح ret = -1;
  size_t جمع, n;
  blake2sp_state S[1];
  ساكن ثابت size_t میانگیر_درازی = 16 * ( 1UL << 20 );
  uint8_t *میانگیر = ( uint8_t * )حافظه_بده( میانگیر_درازی );

  إذا( !میانگیر ) أرجع -1;

  blake2sp_init( S, outbytes );

  طالما( 1 )
  {
    جمع = 0;

    طالما( 1 )
    {
      n = fread( میانگیر + جمع, 1, میانگیر_درازی - جمع, جریان );
      جمع += n;

      إذا( میانگیر_درازی == جمع )
        اكسر;

      إذا( 0 == n )
      {
        إذا( ferror( جریان ) )
          اذهب cleanup_میانگیر;

        اذهب final_process;
      }

      إذا( feof( جریان ) )
        اذهب final_process;
    }

    blake2sp_update( S, میانگیر, میانگیر_درازی );
  }

final_process:;

  إذا( جمع > 0 ) blake2sp_update( S, میانگیر, جمع );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_میانگیر:
  حرر( میانگیر );
  أرجع ret;
}


صحيح blake2bp_جریان( FILE *جریان, فراغ *resstream, size_t outbytes )
{
  صحيح ret = -1;
  size_t جمع, n;
  blake2bp_state S[1];
  ساكن ثابت size_t میانگیر_درازی = 16 * ( 1UL << 20 );
  uint8_t *میانگیر = ( uint8_t * )حافظه_بده( میانگیر_درازی );

  إذا( !میانگیر ) أرجع -1;

  blake2bp_init( S, outbytes );

  طالما( 1 )
  {
    جمع = 0;

    طالما( 1 )
    {
      n = fread( میانگیر + جمع, 1, میانگیر_درازی - جمع, جریان );
      جمع += n;

      إذا( میانگیر_درازی == جمع )
        اكسر;

      إذا( 0 == n )
      {
        إذا( ferror( جریان ) )
          اذهب cleanup_میانگیر;

        اذهب final_process;
      }

      إذا( feof( جریان ) )
        اذهب final_process;
    }

    blake2bp_update( S, میانگیر, میانگیر_درازی );
  }

final_process:;

  إذا( جمع > 0 ) blake2bp_update( S, میانگیر, جمع );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_میانگیر:
  حرر( میانگیر );
  أرجع ret;
}

تعريفنوع صحيح ( *blake2fn )( FILE *, فراغ *, size_t );


ساكن فراغ usage( حرف **argv, صحيح errcode )
{
  FILE *بیرون = errcode ? stderr : stdout;
  fprintf( بیرون, "Usage: %s [OPTION]... [FILE]...\n", argv[0] );
  fprintf( بیرون, "\n" );
  fprintf( بیرون, "With no FILE, or when FILE is -, read standard input.\n" );
  fprintf( بیرون, "\n" );
  fprintf( بیرون, "  -a <algo>    hash algorithm (blake2b is default): \n"
                "               [blake2b|blake2s|blake2bp|blake2sp]\n" );
  fprintf( بیرون, "  -l <length>  digest length in bits, must not exceed the maximum for\n"
                "               the selected algorithm and must be a multiple of 8\n" );
  fprintf( بیرون, "  --tag        create a BSD-style checksum\n" );
  fprintf( بیرون, "  --help       display this help and exit\n" );
  خروج( errcode );
}


صحيح اصلی( صحيح argc, حرف **argv )
{
  blake2fn blake2_جریان = blake2b_جریان;
  بیعلامت دراز maxbytes = BLAKE2B_OUTBYTES;
  ثابت حرف *algorithm = "BLAKE2b";
  بیعلامت دراز outbytes = 0;
  بیعلامت حرف hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  صحيح c, i;
  opterr = 1;

  طالما( 1 )
  {
    صحيح option_شاخص = 0;
    حرف *انتها = عدم;
    بیعلامت دراز outbits;
    ساكن بنية option long_options[] = {
      { "help",  no_آرگومان, 0,  0  },
      { "tag",   no_آرگومان, 0,  0  },
      { عدم, 0, عدم, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_شاخص );
    إذا( c == -1 ) اكسر;
    بدل( c )
    {
    حالة 'a':
      إذا( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_جریان = blake2b_جریان;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      وإلا إذا ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_جریان = blake2s_جریان;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      وإلا إذا ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_جریان = blake2bp_جریان;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      وإلا إذا ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_جریان = blake2sp_جریان;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      وإلا
      {
        چاپ_کن( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      اكسر;

    حالة 'l':
      outbits = strtoul(optarg, &انتها, 10);
      إذا( !انتها || *انتها != '\0' || outbits % 8 != 0)
      {
        چاپ_کن( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      اكسر;

    حالة 0:
      إذا( 0 == strcmp( "help", long_options[option_شاخص].name ) )
        usage( argv, 0 );
      وإلا إذا( 0 == strcmp( "tag", long_options[option_شاخص].name ) )
        bsdstyle = true;
      اكسر;

    حالة '?':
      usage( argv, 1 );
      اكسر;
    }
  }

  إذا(outbytes > maxbytes)
  {
    چاپ_کن( "Invalid length argument: %lu\n", outbytes * 8 );
    چاپ_کن( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
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

    إذا( blake2_جریان( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    وإلا
    {
      size_t j;
      إذا( bsdstyle )
      {
        إذا( outbytes < maxbytes )
          چاپ_کن( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        وإلا
          چاپ_کن( "%s (%s) = ", algorithm, argv[i] );
      }

      لكل( j = 0; j < outbytes; ++j )
        چاپ_کن( "%02x", hash[j] );

      إذا( bsdstyle )
        چاپ_کن( "\n" );
      وإلا
        چاپ_کن( "  %s\n", argv[i] );
    }

    إذا( f == stdin )
      clearerr( f );
    وإلا إذا( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  أرجع 0;
}
#پایان_اگر
