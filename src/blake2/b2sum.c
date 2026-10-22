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

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>

#അന്തര്ഭാവ "blake2.h"

#എങ്കിൽ 0
/* This will help compatibility with coreutils */
പൂർണ്ണസംഖ്യ blake2s_stream( FILE *stream, ശൂന്യം *resstream, size_t outbytes )
{
  പൂർണ്ണസംഖ്യ ret = -1;
  size_t sum, n;
  blake2s_state S[1];
  സ്ഥിതം സ്ഥിരം size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  എങ്കിൽ( !buffer ) തിരികെ -1;

  blake2s_init( S, outbytes );

  ആകുമ്പോൾ( 1 )
  {
    sum = 0;

    ആകുമ്പോൾ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      എങ്കിൽ( buffer_length == sum )
        നിർത്തുക;

      എങ്കിൽ( 0 == n )
      {
        എങ്കിൽ( ferror( stream ) )
          പോകുക cleanup_buffer;

        പോകുക final_process;
      }

      എങ്കിൽ( feof( stream ) )
        പോകുക final_process;
    }

    blake2s_update( S, buffer, buffer_length );
  }

final_process:;

  എങ്കിൽ( sum > 0 ) blake2s_update( S, buffer, sum );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  സ്വതന്ത്രം( buffer );
  തിരികെ ret;
}
#endif

പൂർണ്ണസംഖ്യ blake2b_stream( FILE *stream, ശൂന്യം *resstream, size_t outbytes )
{
  പൂർണ്ണസംഖ്യ ret = -1;
  size_t sum, n;
  blake2b_state S[1];
  സ്ഥിതം സ്ഥിരം size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  എങ്കിൽ( !buffer ) തിരികെ -1;

  blake2b_init( S, outbytes );

  ആകുമ്പോൾ( 1 )
  {
    sum = 0;

    ആകുമ്പോൾ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      എങ്കിൽ( buffer_length == sum )
        നിർത്തുക;

      എങ്കിൽ( 0 == n )
      {
        എങ്കിൽ( ferror( stream ) )
          പോകുക cleanup_buffer;

        പോകുക final_process;
      }

      എങ്കിൽ( feof( stream ) )
        പോകുക final_process;
    }

    blake2b_update( S, buffer, buffer_length );
  }

final_process:;

  എങ്കിൽ( sum > 0 ) blake2b_update( S, buffer, sum );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  സ്വതന്ത്രം( buffer );
  തിരികെ ret;
}

#എങ്കിൽ 0

#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <unistd.h>

പൂർണ്ണസംഖ്യ blake2sp_stream( FILE *stream, ശൂന്യം *resstream, size_t outbytes )
{
  പൂർണ്ണസംഖ്യ ret = -1;
  size_t sum, n;
  blake2sp_state S[1];
  സ്ഥിതം സ്ഥിരം size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  എങ്കിൽ( !buffer ) തിരികെ -1;

  blake2sp_init( S, outbytes );

  ആകുമ്പോൾ( 1 )
  {
    sum = 0;

    ആകുമ്പോൾ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      എങ്കിൽ( buffer_length == sum )
        നിർത്തുക;

      എങ്കിൽ( 0 == n )
      {
        എങ്കിൽ( ferror( stream ) )
          പോകുക cleanup_buffer;

        പോകുക final_process;
      }

      എങ്കിൽ( feof( stream ) )
        പോകുക final_process;
    }

    blake2sp_update( S, buffer, buffer_length );
  }

final_process:;

  എങ്കിൽ( sum > 0 ) blake2sp_update( S, buffer, sum );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  സ്വതന്ത്രം( buffer );
  തിരികെ ret;
}


പൂർണ്ണസംഖ്യ blake2bp_stream( FILE *stream, ശൂന്യം *resstream, size_t outbytes )
{
  പൂർണ്ണസംഖ്യ ret = -1;
  size_t sum, n;
  blake2bp_state S[1];
  സ്ഥിതം സ്ഥിരം size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  എങ്കിൽ( !buffer ) തിരികെ -1;

  blake2bp_init( S, outbytes );

  ആകുമ്പോൾ( 1 )
  {
    sum = 0;

    ആകുമ്പോൾ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      എങ്കിൽ( buffer_length == sum )
        നിർത്തുക;

      എങ്കിൽ( 0 == n )
      {
        എങ്കിൽ( ferror( stream ) )
          പോകുക cleanup_buffer;

        പോകുക final_process;
      }

      എങ്കിൽ( feof( stream ) )
        പോകുക final_process;
    }

    blake2bp_update( S, buffer, buffer_length );
  }

final_process:;

  എങ്കിൽ( sum > 0 ) blake2bp_update( S, buffer, sum );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  സ്വതന്ത്രം( buffer );
  തിരികെ ret;
}

തരനിർവചനം പൂർണ്ണസംഖ്യ ( *blake2fn )( FILE *, ശൂന്യം *, size_t );


സ്ഥിതം ശൂന്യം usage( അക്ഷരം **argv, പൂർണ്ണസംഖ്യ errcode )
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


പൂർണ്ണസംഖ്യ പ്രധാന( പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv )
{
  blake2fn blake2_stream = blake2b_stream;
  unsigned long maxbytes = BLAKE2B_OUTBYTES;
  സ്ഥിരം അക്ഷരം *algorithm = "BLAKE2b";
  unsigned long outbytes = 0;
  unsigned അക്ഷരം hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  പൂർണ്ണസംഖ്യ c, i;
  opterr = 1;

  ആകുമ്പോൾ( 1 )
  {
    പൂർണ്ണസംഖ്യ option_index = 0;
    അക്ഷരം *end = ശൂന്യ;
    unsigned long outbits;
    സ്ഥിതം ഘടന option long_options[] = {
      { "help",  no_argument, 0,  0  },
      { "tag",   no_argument, 0,  0  },
      { ശൂന്യ, 0, ശൂന്യ, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_index );
    എങ്കിൽ( c == -1 ) നിർത്തുക;
    തിരഞ്ഞെടുപ്പ്( c )
    {
    അവസ്ഥ 'a':
      എങ്കിൽ( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_stream = blake2b_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      അല്ലെങ്കിൽ എങ്കിൽ ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_stream = blake2s_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      അല്ലെങ്കിൽ എങ്കിൽ ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_stream = blake2bp_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      അല്ലെങ്കിൽ എങ്കിൽ ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_stream = blake2sp_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      അല്ലെങ്കിൽ
      {
        printf( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      നിർത്തുക;

    അവസ്ഥ 'l':
      outbits = strtoul(optarg, &end, 10);
      എങ്കിൽ( !end || *end != '\0' || outbits % 8 != 0)
      {
        printf( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      നിർത്തുക;

    അവസ്ഥ 0:
      എങ്കിൽ( 0 == strcmp( "help", long_options[option_index].name ) )
        usage( argv, 0 );
      അല്ലെങ്കിൽ എങ്കിൽ( 0 == strcmp( "tag", long_options[option_index].name ) )
        bsdstyle = true;
      നിർത്തുക;

    അവസ്ഥ '?':
      usage( argv, 1 );
      നിർത്തുക;
    }
  }

  എങ്കിൽ(outbytes > maxbytes)
  {
    printf( "Invalid length argument: %lu\n", outbytes * 8 );
    printf( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  അല്ലെങ്കിൽ എങ്കിൽ( outbytes == 0 )
    outbytes = maxbytes;

  എങ്കിൽ( optind == argc )
    argv[argc++] = (അക്ഷരം *) "-";

  വേണ്ടി( i = optind; i < argc; ++i )
  {
    FILE *f = ശൂന്യ;
    എങ്കിൽ( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = stdin;
    അല്ലെങ്കിൽ
      f = fopen( argv[i], "rb" );

    എങ്കിൽ( !f )
    {
      fprintf( stderr, "Could not open `%s': %s\n", argv[i], strerror( errno ) );
      തുടരുക;
    }

    എങ്കിൽ( blake2_stream( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    അല്ലെങ്കിൽ
    {
      size_t j;
      എങ്കിൽ( bsdstyle )
      {
        എങ്കിൽ( outbytes < maxbytes )
          printf( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        അല്ലെങ്കിൽ
          printf( "%s (%s) = ", algorithm, argv[i] );
      }

      വേണ്ടി( j = 0; j < outbytes; ++j )
        printf( "%02x", hash[j] );

      എങ്കിൽ( bsdstyle )
        printf( "\n" );
      അല്ലെങ്കിൽ
        printf( "  %s\n", argv[i] );
    }

    എങ്കിൽ( f == stdin )
      clearerr( f );
    അല്ലെങ്കിൽ എങ്കിൽ( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  തിരികെ 0;
}
#endif
