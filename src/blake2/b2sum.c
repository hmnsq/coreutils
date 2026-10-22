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

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>

#அந்தர்பாவ "blake2.h"

#எனில் 0
/* This will help compatibility with coreutils */
முழு blake2s_stream( FILE *stream, வெறுமை *resstream, size_t outbytes )
{
  முழு ret = -1;
  size_t sum, n;
  blake2s_state S[1];
  நிலையான மாறிலி size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  எனில்( !buffer ) திரும்பு -1;

  blake2s_init( S, outbytes );

  போது( 1 )
  {
    sum = 0;

    போது( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      எனில்( buffer_length == sum )
        நிறுத்து;

      எனில்( 0 == n )
      {
        எனில்( ferror( stream ) )
          செல் cleanup_buffer;

        செல் final_process;
      }

      எனில்( feof( stream ) )
        செல் final_process;
    }

    blake2s_update( S, buffer, buffer_length );
  }

final_process:;

  எனில்( sum > 0 ) blake2s_update( S, buffer, sum );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  விடுவி( buffer );
  திரும்பு ret;
}
#endif

முழு blake2b_stream( FILE *stream, வெறுமை *resstream, size_t outbytes )
{
  முழு ret = -1;
  size_t sum, n;
  blake2b_state S[1];
  நிலையான மாறிலி size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  எனில்( !buffer ) திரும்பு -1;

  blake2b_init( S, outbytes );

  போது( 1 )
  {
    sum = 0;

    போது( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      எனில்( buffer_length == sum )
        நிறுத்து;

      எனில்( 0 == n )
      {
        எனில்( ferror( stream ) )
          செல் cleanup_buffer;

        செல் final_process;
      }

      எனில்( feof( stream ) )
        செல் final_process;
    }

    blake2b_update( S, buffer, buffer_length );
  }

final_process:;

  எனில்( sum > 0 ) blake2b_update( S, buffer, sum );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  விடுவி( buffer );
  திரும்பு ret;
}

#எனில் 0

#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <unistd.h>

முழு blake2sp_stream( FILE *stream, வெறுமை *resstream, size_t outbytes )
{
  முழு ret = -1;
  size_t sum, n;
  blake2sp_state S[1];
  நிலையான மாறிலி size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  எனில்( !buffer ) திரும்பு -1;

  blake2sp_init( S, outbytes );

  போது( 1 )
  {
    sum = 0;

    போது( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      எனில்( buffer_length == sum )
        நிறுத்து;

      எனில்( 0 == n )
      {
        எனில்( ferror( stream ) )
          செல் cleanup_buffer;

        செல் final_process;
      }

      எனில்( feof( stream ) )
        செல் final_process;
    }

    blake2sp_update( S, buffer, buffer_length );
  }

final_process:;

  எனில்( sum > 0 ) blake2sp_update( S, buffer, sum );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  விடுவி( buffer );
  திரும்பு ret;
}


முழு blake2bp_stream( FILE *stream, வெறுமை *resstream, size_t outbytes )
{
  முழு ret = -1;
  size_t sum, n;
  blake2bp_state S[1];
  நிலையான மாறிலி size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  எனில்( !buffer ) திரும்பு -1;

  blake2bp_init( S, outbytes );

  போது( 1 )
  {
    sum = 0;

    போது( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      எனில்( buffer_length == sum )
        நிறுத்து;

      எனில்( 0 == n )
      {
        எனில்( ferror( stream ) )
          செல் cleanup_buffer;

        செல் final_process;
      }

      எனில்( feof( stream ) )
        செல் final_process;
    }

    blake2bp_update( S, buffer, buffer_length );
  }

final_process:;

  எனில்( sum > 0 ) blake2bp_update( S, buffer, sum );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  விடுவி( buffer );
  திரும்பு ret;
}

வகைவரை முழு ( *blake2fn )( FILE *, வெறுமை *, size_t );


நிலையான வெறுமை usage( எழுத்து **argv, முழு errcode )
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


முழு முதன்மை( முழு argc, எழுத்து **argv )
{
  blake2fn blake2_stream = blake2b_stream;
  unsigned long maxbytes = BLAKE2B_OUTBYTES;
  மாறிலி எழுத்து *algorithm = "BLAKE2b";
  unsigned long outbytes = 0;
  unsigned எழுத்து hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  முழு c, i;
  opterr = 1;

  போது( 1 )
  {
    முழு option_index = 0;
    எழுத்து *end = வெற்று;
    unsigned long outbits;
    நிலையான கட்டமைப்பு option long_options[] = {
      { "help",  no_argument, 0,  0  },
      { "tag",   no_argument, 0,  0  },
      { வெற்று, 0, வெற்று, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_index );
    எனில்( c == -1 ) நிறுத்து;
    தேர்வு( c )
    {
    நிலை 'a':
      எனில்( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_stream = blake2b_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      இல்லையெனில் எனில் ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_stream = blake2s_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      இல்லையெனில் எனில் ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_stream = blake2bp_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      இல்லையெனில் எனில் ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_stream = blake2sp_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      இல்லையெனில்
      {
        printf( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      நிறுத்து;

    நிலை 'l':
      outbits = strtoul(optarg, &end, 10);
      எனில்( !end || *end != '\0' || outbits % 8 != 0)
      {
        printf( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      நிறுத்து;

    நிலை 0:
      எனில்( 0 == strcmp( "help", long_options[option_index].name ) )
        usage( argv, 0 );
      இல்லையெனில் எனில்( 0 == strcmp( "tag", long_options[option_index].name ) )
        bsdstyle = true;
      நிறுத்து;

    நிலை '?':
      usage( argv, 1 );
      நிறுத்து;
    }
  }

  எனில்(outbytes > maxbytes)
  {
    printf( "Invalid length argument: %lu\n", outbytes * 8 );
    printf( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  இல்லையெனில் எனில்( outbytes == 0 )
    outbytes = maxbytes;

  எனில்( optind == argc )
    argv[argc++] = (எழுத்து *) "-";

  க்கு( i = optind; i < argc; ++i )
  {
    FILE *f = வெற்று;
    எனில்( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = stdin;
    இல்லையெனில்
      f = fopen( argv[i], "rb" );

    எனில்( !f )
    {
      fprintf( stderr, "Could not open `%s': %s\n", argv[i], strerror( errno ) );
      தொடர்;
    }

    எனில்( blake2_stream( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    இல்லையெனில்
    {
      size_t j;
      எனில்( bsdstyle )
      {
        எனில்( outbytes < maxbytes )
          printf( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        இல்லையெனில்
          printf( "%s (%s) = ", algorithm, argv[i] );
      }

      க்கு( j = 0; j < outbytes; ++j )
        printf( "%02x", hash[j] );

      எனில்( bsdstyle )
        printf( "\n" );
      இல்லையெனில்
        printf( "  %s\n", argv[i] );
    }

    எனில்( f == stdin )
      clearerr( f );
    இல்லையெனில் எனில்( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  திரும்பு 0;
}
#endif
