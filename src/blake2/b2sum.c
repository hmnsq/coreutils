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

#include <stdio.h>
#include <stdlib.h>

#include "blake2.h"

#ប្រសិនបើ 0
/* This will help compatibility with coreutils */
ចំនួនគត់ blake2s_stream( FILE *stream, void *resstream, size_t outbytes )
{
  ចំនួនគត់ ret = -1;
  size_t sum, n;
  blake2s_state S[1];
  static const size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ប្រសិនបើ( !buffer ) ត្រឡប់ -1;

  blake2s_init( S, outbytes );

  ខណៈពេល( 1 )
  {
    sum = 0;

    ខណៈពេល( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ប្រសិនបើ( buffer_length == sum )
        bha_nga;

      ប្រសិនបើ( 0 == n )
      {
        ប្រសិនបើ( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      ប្រសិនបើ( feof( stream ) )
        goto final_process;
    }

    blake2s_update( S, buffer, buffer_length );
  }

final_process:;

  ប្រសិនបើ( sum > 0 ) blake2s_update( S, buffer, sum );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  ត្រឡប់ ret;
}
#endif

ចំនួនគត់ blake2b_stream( FILE *stream, void *resstream, size_t outbytes )
{
  ចំនួនគត់ ret = -1;
  size_t sum, n;
  blake2b_state S[1];
  static const size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ប្រសិនបើ( !buffer ) ត្រឡប់ -1;

  blake2b_init( S, outbytes );

  ខណៈពេល( 1 )
  {
    sum = 0;

    ខណៈពេល( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ប្រសិនបើ( buffer_length == sum )
        bha_nga;

      ប្រសិនបើ( 0 == n )
      {
        ប្រសិនបើ( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      ប្រសិនបើ( feof( stream ) )
        goto final_process;
    }

    blake2b_update( S, buffer, buffer_length );
  }

final_process:;

  ប្រសិនបើ( sum > 0 ) blake2b_update( S, buffer, sum );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  ត្រឡប់ ret;
}

#ប្រសិនបើ 0

#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>

ចំនួនគត់ blake2sp_stream( FILE *stream, void *resstream, size_t outbytes )
{
  ចំនួនគត់ ret = -1;
  size_t sum, n;
  blake2sp_state S[1];
  static const size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ប្រសិនបើ( !buffer ) ត្រឡប់ -1;

  blake2sp_init( S, outbytes );

  ខណៈពេល( 1 )
  {
    sum = 0;

    ខណៈពេល( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ប្រសិនបើ( buffer_length == sum )
        bha_nga;

      ប្រសិនបើ( 0 == n )
      {
        ប្រសិនបើ( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      ប្រសិនបើ( feof( stream ) )
        goto final_process;
    }

    blake2sp_update( S, buffer, buffer_length );
  }

final_process:;

  ប្រសិនបើ( sum > 0 ) blake2sp_update( S, buffer, sum );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  ត្រឡប់ ret;
}


ចំនួនគត់ blake2bp_stream( FILE *stream, void *resstream, size_t outbytes )
{
  ចំនួនគត់ ret = -1;
  size_t sum, n;
  blake2bp_state S[1];
  static const size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ប្រសិនបើ( !buffer ) ត្រឡប់ -1;

  blake2bp_init( S, outbytes );

  ខណៈពេល( 1 )
  {
    sum = 0;

    ខណៈពេល( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ប្រសិនបើ( buffer_length == sum )
        bha_nga;

      ប្រសិនបើ( 0 == n )
      {
        ប្រសិនបើ( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      ប្រសិនបើ( feof( stream ) )
        goto final_process;
    }

    blake2bp_update( S, buffer, buffer_length );
  }

final_process:;

  ប្រសិនបើ( sum > 0 ) blake2bp_update( S, buffer, sum );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  ត្រឡប់ ret;
}

typedef ចំនួនគត់ ( *blake2fn )( FILE *, void *, size_t );


static void usage( var_na **argv, ចំនួនគត់ errcode )
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


ចំនួនគត់ main( ចំនួនគត់ argc, var_na **argv )
{
  blake2fn blake2_stream = blake2b_stream;
  unsigned long maxbytes = BLAKE2B_OUTBYTES;
  const var_na *algorithm = "BLAKE2b";
  unsigned long outbytes = 0;
  unsigned var_na hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  ចំនួនគត់ c, i;
  opterr = 1;

  ខណៈពេល( 1 )
  {
    ចំនួនគត់ option_index = 0;
    var_na *end = NULL;
    unsigned long outbits;
    static sa_nracanaa option long_options[] = {
      { "help",  no_argument, 0,  0  },
      { "tag",   no_argument, 0,  0  },
      { NULL, 0, NULL, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_index );
    ប្រសិនបើ( c == -1 ) bha_nga;
    switch( c )
    {
    pak_sa 'a':
      ប្រសិនបើ( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_stream = blake2b_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      បើមិនដូច្នេះ ប្រសិនបើ ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_stream = blake2s_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      បើមិនដូច្នេះ ប្រសិនបើ ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_stream = blake2bp_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      បើមិនដូច្នេះ ប្រសិនបើ ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_stream = blake2sp_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      បើមិនដូច្នេះ
      {
        printf( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      bha_nga;

    pak_sa 'l':
      outbits = strtoul(optarg, &end, 10);
      ប្រសិនបើ( !end || *end != '\0' || outbits % 8 != 0)
      {
        printf( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      bha_nga;

    pak_sa 0:
      ប្រសិនបើ( 0 == strcmp( "help", long_options[option_index].name ) )
        usage( argv, 0 );
      បើមិនដូច្នេះ ប្រសិនបើ( 0 == strcmp( "tag", long_options[option_index].name ) )
        bsdstyle = true;
      bha_nga;

    pak_sa '?':
      usage( argv, 1 );
      bha_nga;
    }
  }

  ប្រសិនបើ(outbytes > maxbytes)
  {
    printf( "Invalid length argument: %lu\n", outbytes * 8 );
    printf( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  បើមិនដូច្នេះ ប្រសិនបើ( outbytes == 0 )
    outbytes = maxbytes;

  ប្រសិនបើ( optind == argc )
    argv[argc++] = (var_na *) "-";

  សម្រាប់( i = optind; i < argc; ++i )
  {
    FILE *f = NULL;
    ប្រសិនបើ( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = stdin;
    បើមិនដូច្នេះ
      f = fopen( argv[i], "rb" );

    ប្រសិនបើ( !f )
    {
      fprintf( stderr, "Could not open `%s': %s\n", argv[i], strerror( errno ) );
      continue;
    }

    ប្រសិនបើ( blake2_stream( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    បើមិនដូច្នេះ
    {
      size_t j;
      ប្រសិនបើ( bsdstyle )
      {
        ប្រសិនបើ( outbytes < maxbytes )
          printf( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        បើមិនដូច្នេះ
          printf( "%s (%s) = ", algorithm, argv[i] );
      }

      សម្រាប់( j = 0; j < outbytes; ++j )
        printf( "%02x", hash[j] );

      ប្រសិនបើ( bsdstyle )
        printf( "\n" );
      បើមិនដូច្នេះ
        printf( "  %s\n", argv[i] );
    }

    ប្រសិនបើ( f == stdin )
      clearerr( f );
    បើមិនដូច្នេះ ប្រសិនបើ( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  ត្រឡប់ 0;
}
#endif
