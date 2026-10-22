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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ضمن <stdio.h>
#ضمن <stdlib.h>

#ضمن "blake2.h"

#إذا 0
/* This will help compatibility with coreutils */
صحيح blake2s_stream( FILE *stream, فراغ *resstream, size_t outbytes )
{
  صحيح ret = -1;
  size_t sum, n;
  blake2s_state S[1];
  ساكن ثابت size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  إذا( !buffer ) أرجع -1;

  blake2s_init( S, outbytes );

  طالما( 1 )
  {
    sum = 0;

    طالما( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      إذا( buffer_length == sum )
        اكسر;

      إذا( 0 == n )
      {
        إذا( ferror( stream ) )
          اذهب cleanup_buffer;

        اذهب final_process;
      }

      إذا( feof( stream ) )
        اذهب final_process;
    }

    blake2s_update( S, buffer, buffer_length );
  }

final_process:;

  إذا( sum > 0 ) blake2s_update( S, buffer, sum );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  حرر( buffer );
  أرجع ret;
}
#endif

صحيح blake2b_stream( FILE *stream, فراغ *resstream, size_t outbytes )
{
  صحيح ret = -1;
  size_t sum, n;
  blake2b_state S[1];
  ساكن ثابت size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  إذا( !buffer ) أرجع -1;

  blake2b_init( S, outbytes );

  طالما( 1 )
  {
    sum = 0;

    طالما( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      إذا( buffer_length == sum )
        اكسر;

      إذا( 0 == n )
      {
        إذا( ferror( stream ) )
          اذهب cleanup_buffer;

        اذهب final_process;
      }

      إذا( feof( stream ) )
        اذهب final_process;
    }

    blake2b_update( S, buffer, buffer_length );
  }

final_process:;

  إذا( sum > 0 ) blake2b_update( S, buffer, sum );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  حرر( buffer );
  أرجع ret;
}

#إذا 0

#ضمن <errno.h>
#ضمن <getopt.h>
#ضمن <string.h>
#ضمن <unistd.h>

صحيح blake2sp_stream( FILE *stream, فراغ *resstream, size_t outbytes )
{
  صحيح ret = -1;
  size_t sum, n;
  blake2sp_state S[1];
  ساكن ثابت size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  إذا( !buffer ) أرجع -1;

  blake2sp_init( S, outbytes );

  طالما( 1 )
  {
    sum = 0;

    طالما( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      إذا( buffer_length == sum )
        اكسر;

      إذا( 0 == n )
      {
        إذا( ferror( stream ) )
          اذهب cleanup_buffer;

        اذهب final_process;
      }

      إذا( feof( stream ) )
        اذهب final_process;
    }

    blake2sp_update( S, buffer, buffer_length );
  }

final_process:;

  إذا( sum > 0 ) blake2sp_update( S, buffer, sum );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  حرر( buffer );
  أرجع ret;
}


صحيح blake2bp_stream( FILE *stream, فراغ *resstream, size_t outbytes )
{
  صحيح ret = -1;
  size_t sum, n;
  blake2bp_state S[1];
  ساكن ثابت size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  إذا( !buffer ) أرجع -1;

  blake2bp_init( S, outbytes );

  طالما( 1 )
  {
    sum = 0;

    طالما( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      إذا( buffer_length == sum )
        اكسر;

      إذا( 0 == n )
      {
        إذا( ferror( stream ) )
          اذهب cleanup_buffer;

        اذهب final_process;
      }

      إذا( feof( stream ) )
        اذهب final_process;
    }

    blake2bp_update( S, buffer, buffer_length );
  }

final_process:;

  إذا( sum > 0 ) blake2bp_update( S, buffer, sum );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  حرر( buffer );
  أرجع ret;
}

تعريفنوع صحيح ( *blake2fn )( FILE *, فراغ *, size_t );


ساكن فراغ usage( حرف **argv, صحيح errcode )
{
  FILE *out = errcode ? stderr : stdout;
  fprintf( out, "Usage: %s [OPTION]... [FILE]...\n", argv[0] );
  fprintf( out, "\n" );
  fprintf( out, "With no FILE, or when FILE is -, read standard input.\n" );
  fprintf( out, "\n" );
  fprintf( out, "  -a <algo>    hash algorithm (blake2b is default): \n"
                "               [blake2b|blake2s|blake2bp|blake2sp]\n" );
  fprintf( out, "  -l <length>  digest length in bits, must not exceed the maximum for\n"
                "               the selected algorithm and must be a multiple of 8\n" );
  fprintf( out, "  --tag        create a BSD-style checksum\n" );
  fprintf( out, "  --help       display this help and exit\n" );
  exit( errcode );
}


صحيح main( صحيح argc, حرف **argv )
{
  blake2fn blake2_stream = blake2b_stream;
  unsigned long maxbytes = BLAKE2B_OUTBYTES;
  ثابت حرف *algorithm = "BLAKE2b";
  unsigned long outbytes = 0;
  unsigned حرف hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  صحيح c, i;
  opterr = 1;

  طالما( 1 )
  {
    صحيح option_index = 0;
    حرف *end = عدم;
    unsigned long outbits;
    ساكن بنية option long_options[] = {
      { "help",  no_argument, 0,  0  },
      { "tag",   no_argument, 0,  0  },
      { عدم, 0, عدم, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_index );
    إذا( c == -1 ) اكسر;
    بدل( c )
    {
    حالة 'a':
      إذا( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_stream = blake2b_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      وإلا إذا ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_stream = blake2s_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      وإلا إذا ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_stream = blake2bp_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      وإلا إذا ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_stream = blake2sp_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      وإلا
      {
        printf( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      اكسر;

    حالة 'l':
      outbits = strtoul(optarg, &end, 10);
      إذا( !end || *end != '\0' || outbits % 8 != 0)
      {
        printf( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      اكسر;

    حالة 0:
      إذا( 0 == strcmp( "help", long_options[option_index].name ) )
        usage( argv, 0 );
      وإلا إذا( 0 == strcmp( "tag", long_options[option_index].name ) )
        bsdstyle = true;
      اكسر;

    حالة '?':
      usage( argv, 1 );
      اكسر;
    }
  }

  إذا(outbytes > maxbytes)
  {
    printf( "Invalid length argument: %lu\n", outbytes * 8 );
    printf( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
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

    إذا( blake2_stream( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    وإلا
    {
      size_t j;
      إذا( bsdstyle )
      {
        إذا( outbytes < maxbytes )
          printf( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        وإلا
          printf( "%s (%s) = ", algorithm, argv[i] );
      }

      لكل( j = 0; j < outbytes; ++j )
        printf( "%02x", hash[j] );

      إذا( bsdstyle )
        printf( "\n" );
      وإلا
        printf( "  %s\n", argv[i] );
    }

    إذا( f == stdin )
      clearerr( f );
    وإلا إذا( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  أرجع 0;
}
#endif
