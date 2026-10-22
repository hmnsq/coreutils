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

#ꦪꦼꦤ꧀ 0
/* This will help compatibility with coreutils */
puur_naa_nka blake2s_stream( FILE *stream, void *resstream, size_t outbytes )
{
  puur_naa_nka ret = -1;
  size_t sum, n;
  blake2s_state S[1];
  static const size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ꦪꦼꦤ꧀( !buffer ) ꦮꦭꦶ -1;

  blake2s_init( S, outbytes );

  ꦱꦼꦢꦼꦭ( 1 )
  {
    sum = 0;

    ꦱꦼꦢꦼꦭ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ꦪꦼꦤ꧀( buffer_length == sum )
        bha_nga;

      ꦪꦼꦤ꧀( 0 == n )
      {
        ꦪꦼꦤ꧀( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      ꦪꦼꦤ꧀( feof( stream ) )
        goto final_process;
    }

    blake2s_update( S, buffer, buffer_length );
  }

final_process:;

  ꦪꦼꦤ꧀( sum > 0 ) blake2s_update( S, buffer, sum );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  ꦮꦭꦶ ret;
}
#endif

puur_naa_nka blake2b_stream( FILE *stream, void *resstream, size_t outbytes )
{
  puur_naa_nka ret = -1;
  size_t sum, n;
  blake2b_state S[1];
  static const size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ꦪꦼꦤ꧀( !buffer ) ꦮꦭꦶ -1;

  blake2b_init( S, outbytes );

  ꦱꦼꦢꦼꦭ( 1 )
  {
    sum = 0;

    ꦱꦼꦢꦼꦭ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ꦪꦼꦤ꧀( buffer_length == sum )
        bha_nga;

      ꦪꦼꦤ꧀( 0 == n )
      {
        ꦪꦼꦤ꧀( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      ꦪꦼꦤ꧀( feof( stream ) )
        goto final_process;
    }

    blake2b_update( S, buffer, buffer_length );
  }

final_process:;

  ꦪꦼꦤ꧀( sum > 0 ) blake2b_update( S, buffer, sum );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  ꦮꦭꦶ ret;
}

#ꦪꦼꦤ꧀ 0

#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>

puur_naa_nka blake2sp_stream( FILE *stream, void *resstream, size_t outbytes )
{
  puur_naa_nka ret = -1;
  size_t sum, n;
  blake2sp_state S[1];
  static const size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ꦪꦼꦤ꧀( !buffer ) ꦮꦭꦶ -1;

  blake2sp_init( S, outbytes );

  ꦱꦼꦢꦼꦭ( 1 )
  {
    sum = 0;

    ꦱꦼꦢꦼꦭ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ꦪꦼꦤ꧀( buffer_length == sum )
        bha_nga;

      ꦪꦼꦤ꧀( 0 == n )
      {
        ꦪꦼꦤ꧀( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      ꦪꦼꦤ꧀( feof( stream ) )
        goto final_process;
    }

    blake2sp_update( S, buffer, buffer_length );
  }

final_process:;

  ꦪꦼꦤ꧀( sum > 0 ) blake2sp_update( S, buffer, sum );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  ꦮꦭꦶ ret;
}


puur_naa_nka blake2bp_stream( FILE *stream, void *resstream, size_t outbytes )
{
  puur_naa_nka ret = -1;
  size_t sum, n;
  blake2bp_state S[1];
  static const size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ꦪꦼꦤ꧀( !buffer ) ꦮꦭꦶ -1;

  blake2bp_init( S, outbytes );

  ꦱꦼꦢꦼꦭ( 1 )
  {
    sum = 0;

    ꦱꦼꦢꦼꦭ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ꦪꦼꦤ꧀( buffer_length == sum )
        bha_nga;

      ꦪꦼꦤ꧀( 0 == n )
      {
        ꦪꦼꦤ꧀( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      ꦪꦼꦤ꧀( feof( stream ) )
        goto final_process;
    }

    blake2bp_update( S, buffer, buffer_length );
  }

final_process:;

  ꦪꦼꦤ꧀( sum > 0 ) blake2bp_update( S, buffer, sum );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  ꦮꦭꦶ ret;
}

typedef puur_naa_nka ( *blake2fn )( FILE *, void *, size_t );


static void usage( var_na **argv, puur_naa_nka errcode )
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


puur_naa_nka main( puur_naa_nka argc, var_na **argv )
{
  blake2fn blake2_stream = blake2b_stream;
  unsigned long maxbytes = BLAKE2B_OUTBYTES;
  const var_na *algorithm = "BLAKE2b";
  unsigned long outbytes = 0;
  unsigned var_na hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  puur_naa_nka c, i;
  opterr = 1;

  ꦱꦼꦢꦼꦭ( 1 )
  {
    puur_naa_nka option_index = 0;
    var_na *end = NULL;
    unsigned long outbits;
    static sa_nracanaa option long_options[] = {
      { "help",  no_argument, 0,  0  },
      { "tag",   no_argument, 0,  0  },
      { NULL, 0, NULL, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_index );
    ꦪꦼꦤ꧀( c == -1 ) bha_nga;
    switch( c )
    {
    pak_sa 'a':
      ꦪꦼꦤ꧀( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_stream = blake2b_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      ꦭꦶꦪ ꦪꦼꦤ꧀ ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_stream = blake2s_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      ꦭꦶꦪ ꦪꦼꦤ꧀ ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_stream = blake2bp_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      ꦭꦶꦪ ꦪꦼꦤ꧀ ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_stream = blake2sp_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      ꦭꦶꦪ
      {
        printf( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      bha_nga;

    pak_sa 'l':
      outbits = strtoul(optarg, &end, 10);
      ꦪꦼꦤ꧀( !end || *end != '\0' || outbits % 8 != 0)
      {
        printf( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      bha_nga;

    pak_sa 0:
      ꦪꦼꦤ꧀( 0 == strcmp( "help", long_options[option_index].name ) )
        usage( argv, 0 );
      ꦭꦶꦪ ꦪꦼꦤ꧀( 0 == strcmp( "tag", long_options[option_index].name ) )
        bsdstyle = true;
      bha_nga;

    pak_sa '?':
      usage( argv, 1 );
      bha_nga;
    }
  }

  ꦪꦼꦤ꧀(outbytes > maxbytes)
  {
    printf( "Invalid length argument: %lu\n", outbytes * 8 );
    printf( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  ꦭꦶꦪ ꦪꦼꦤ꧀( outbytes == 0 )
    outbytes = maxbytes;

  ꦪꦼꦤ꧀( optind == argc )
    argv[argc++] = (var_na *) "-";

  ꦏꦁꦒꦺꦴ( i = optind; i < argc; ++i )
  {
    FILE *f = NULL;
    ꦪꦼꦤ꧀( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = stdin;
    ꦭꦶꦪ
      f = fopen( argv[i], "rb" );

    ꦪꦼꦤ꧀( !f )
    {
      fprintf( stderr, "Could not open `%s': %s\n", argv[i], strerror( errno ) );
      continue;
    }

    ꦪꦼꦤ꧀( blake2_stream( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    ꦭꦶꦪ
    {
      size_t j;
      ꦪꦼꦤ꧀( bsdstyle )
      {
        ꦪꦼꦤ꧀( outbytes < maxbytes )
          printf( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        ꦭꦶꦪ
          printf( "%s (%s) = ", algorithm, argv[i] );
      }

      ꦏꦁꦒꦺꦴ( j = 0; j < outbytes; ++j )
        printf( "%02x", hash[j] );

      ꦪꦼꦤ꧀( bsdstyle )
        printf( "\n" );
      ꦭꦶꦪ
        printf( "  %s\n", argv[i] );
    }

    ꦪꦼꦤ꧀( f == stdin )
      clearerr( f );
    ꦭꦶꦪ ꦪꦼꦤ꧀( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  ꦮꦭꦶ 0;
}
#endif
