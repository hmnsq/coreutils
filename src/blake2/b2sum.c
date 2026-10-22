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

#شامل <stdio.h>
#شامل <stdlib.h>

#شامل "blake2.h"

#اگر 0
/* This will help compatibility with coreutils */
صحیح blake2s_stream( FILE *stream, تهی *resstream, size_t outbytes )
{
  صحیح ret = -1;
  size_t sum, n;
  blake2s_state S[1];
  ایستا ثابت size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  اگر( !buffer ) بازگشت -1;

  blake2s_init( S, outbytes );

  وقتی( 1 )
  {
    sum = 0;

    وقتی( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      اگر( buffer_length == sum )
        بشکن;

      اگر( 0 == n )
      {
        اگر( ferror( stream ) )
          برو cleanup_buffer;

        برو final_process;
      }

      اگر( feof( stream ) )
        برو final_process;
    }

    blake2s_update( S, buffer, buffer_length );
  }

final_process:;

  اگر( sum > 0 ) blake2s_update( S, buffer, sum );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  آزاد( buffer );
  بازگشت ret;
}
#endif

صحیح blake2b_stream( FILE *stream, تهی *resstream, size_t outbytes )
{
  صحیح ret = -1;
  size_t sum, n;
  blake2b_state S[1];
  ایستا ثابت size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  اگر( !buffer ) بازگشت -1;

  blake2b_init( S, outbytes );

  وقتی( 1 )
  {
    sum = 0;

    وقتی( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      اگر( buffer_length == sum )
        بشکن;

      اگر( 0 == n )
      {
        اگر( ferror( stream ) )
          برو cleanup_buffer;

        برو final_process;
      }

      اگر( feof( stream ) )
        برو final_process;
    }

    blake2b_update( S, buffer, buffer_length );
  }

final_process:;

  اگر( sum > 0 ) blake2b_update( S, buffer, sum );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  آزاد( buffer );
  بازگشت ret;
}

#اگر 0

#شامل <errno.h>
#شامل <getopt.h>
#شامل <string.h>
#شامل <unistd.h>

صحیح blake2sp_stream( FILE *stream, تهی *resstream, size_t outbytes )
{
  صحیح ret = -1;
  size_t sum, n;
  blake2sp_state S[1];
  ایستا ثابت size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  اگر( !buffer ) بازگشت -1;

  blake2sp_init( S, outbytes );

  وقتی( 1 )
  {
    sum = 0;

    وقتی( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      اگر( buffer_length == sum )
        بشکن;

      اگر( 0 == n )
      {
        اگر( ferror( stream ) )
          برو cleanup_buffer;

        برو final_process;
      }

      اگر( feof( stream ) )
        برو final_process;
    }

    blake2sp_update( S, buffer, buffer_length );
  }

final_process:;

  اگر( sum > 0 ) blake2sp_update( S, buffer, sum );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  آزاد( buffer );
  بازگشت ret;
}


صحیح blake2bp_stream( FILE *stream, تهی *resstream, size_t outbytes )
{
  صحیح ret = -1;
  size_t sum, n;
  blake2bp_state S[1];
  ایستا ثابت size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  اگر( !buffer ) بازگشت -1;

  blake2bp_init( S, outbytes );

  وقتی( 1 )
  {
    sum = 0;

    وقتی( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      اگر( buffer_length == sum )
        بشکن;

      اگر( 0 == n )
      {
        اگر( ferror( stream ) )
          برو cleanup_buffer;

        برو final_process;
      }

      اگر( feof( stream ) )
        برو final_process;
    }

    blake2bp_update( S, buffer, buffer_length );
  }

final_process:;

  اگر( sum > 0 ) blake2bp_update( S, buffer, sum );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  آزاد( buffer );
  بازگشت ret;
}

تعریفنوع صحیح ( *blake2fn )( FILE *, تهی *, size_t );


ایستا تهی usage( نویسه **argv, صحیح errcode )
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


صحیح اصلی( صحیح argc, نویسه **argv )
{
  blake2fn blake2_stream = blake2b_stream;
  unsigned long maxbytes = BLAKE2B_OUTBYTES;
  ثابت نویسه *algorithm = "BLAKE2b";
  unsigned long outbytes = 0;
  unsigned نویسه hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  صحیح c, i;
  opterr = 1;

  وقتی( 1 )
  {
    صحیح option_index = 0;
    نویسه *end = پوچ;
    unsigned long outbits;
    ایستا ساختار option long_options[] = {
      { "help",  no_argument, 0,  0  },
      { "tag",   no_argument, 0,  0  },
      { پوچ, 0, پوچ, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_index );
    اگر( c == -1 ) بشکن;
    انتخاب( c )
    {
    حالت 'a':
      اگر( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_stream = blake2b_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      وگرنه اگر ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_stream = blake2s_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      وگرنه اگر ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_stream = blake2bp_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      وگرنه اگر ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_stream = blake2sp_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      وگرنه
      {
        printf( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      بشکن;

    حالت 'l':
      outbits = strtoul(optarg, &end, 10);
      اگر( !end || *end != '\0' || outbits % 8 != 0)
      {
        printf( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      بشکن;

    حالت 0:
      اگر( 0 == strcmp( "help", long_options[option_index].name ) )
        usage( argv, 0 );
      وگرنه اگر( 0 == strcmp( "tag", long_options[option_index].name ) )
        bsdstyle = true;
      بشکن;

    حالت '?':
      usage( argv, 1 );
      بشکن;
    }
  }

  اگر(outbytes > maxbytes)
  {
    printf( "Invalid length argument: %lu\n", outbytes * 8 );
    printf( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  وگرنه اگر( outbytes == 0 )
    outbytes = maxbytes;

  اگر( optind == argc )
    argv[argc++] = (نویسه *) "-";

  برای( i = optind; i < argc; ++i )
  {
    FILE *f = پوچ;
    اگر( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = stdin;
    وگرنه
      f = fopen( argv[i], "rb" );

    اگر( !f )
    {
      fprintf( stderr, "Could not open `%s': %s\n", argv[i], strerror( errno ) );
      ادامه;
    }

    اگر( blake2_stream( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    وگرنه
    {
      size_t j;
      اگر( bsdstyle )
      {
        اگر( outbytes < maxbytes )
          printf( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        وگرنه
          printf( "%s (%s) = ", algorithm, argv[i] );
      }

      برای( j = 0; j < outbytes; ++j )
        printf( "%02x", hash[j] );

      اگر( bsdstyle )
        printf( "\n" );
      وگرنه
        printf( "  %s\n", argv[i] );
    }

    اگر( f == stdin )
      clearerr( f );
    وگرنه اگر( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  بازگشت 0;
}
#endif
