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

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>

#અન્તર્ભાવ "blake2.h"

#જો 0
/* This will help compatibility with coreutils */
પૂર્ણાંક blake2s_stream( FILE *stream, શૂન્ય *resstream, size_t outbytes )
{
  પૂર્ણાંક ret = -1;
  size_t sum, n;
  blake2s_state S[1];
  સ્થિર અચલ size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  જો( !buffer ) પાછા -1;

  blake2s_init( S, outbytes );

  જ્યારે( 1 )
  {
    sum = 0;

    જ્યારે( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      જો( buffer_length == sum )
        તોડો;

      જો( 0 == n )
      {
        જો( ferror( stream ) )
          જાઓ cleanup_buffer;

        જાઓ final_process;
      }

      જો( feof( stream ) )
        જાઓ final_process;
    }

    blake2s_update( S, buffer, buffer_length );
  }

final_process:;

  જો( sum > 0 ) blake2s_update( S, buffer, sum );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  મુક્ત( buffer );
  પાછા ret;
}
#endif

પૂર્ણાંક blake2b_stream( FILE *stream, શૂન્ય *resstream, size_t outbytes )
{
  પૂર્ણાંક ret = -1;
  size_t sum, n;
  blake2b_state S[1];
  સ્થિર અચલ size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  જો( !buffer ) પાછા -1;

  blake2b_init( S, outbytes );

  જ્યારે( 1 )
  {
    sum = 0;

    જ્યારે( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      જો( buffer_length == sum )
        તોડો;

      જો( 0 == n )
      {
        જો( ferror( stream ) )
          જાઓ cleanup_buffer;

        જાઓ final_process;
      }

      જો( feof( stream ) )
        જાઓ final_process;
    }

    blake2b_update( S, buffer, buffer_length );
  }

final_process:;

  જો( sum > 0 ) blake2b_update( S, buffer, sum );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  મુક્ત( buffer );
  પાછા ret;
}

#જો 0

#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <unistd.h>

પૂર્ણાંક blake2sp_stream( FILE *stream, શૂન્ય *resstream, size_t outbytes )
{
  પૂર્ણાંક ret = -1;
  size_t sum, n;
  blake2sp_state S[1];
  સ્થિર અચલ size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  જો( !buffer ) પાછા -1;

  blake2sp_init( S, outbytes );

  જ્યારે( 1 )
  {
    sum = 0;

    જ્યારે( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      જો( buffer_length == sum )
        તોડો;

      જો( 0 == n )
      {
        જો( ferror( stream ) )
          જાઓ cleanup_buffer;

        જાઓ final_process;
      }

      જો( feof( stream ) )
        જાઓ final_process;
    }

    blake2sp_update( S, buffer, buffer_length );
  }

final_process:;

  જો( sum > 0 ) blake2sp_update( S, buffer, sum );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  મુક્ત( buffer );
  પાછા ret;
}


પૂર્ણાંક blake2bp_stream( FILE *stream, શૂન્ય *resstream, size_t outbytes )
{
  પૂર્ણાંક ret = -1;
  size_t sum, n;
  blake2bp_state S[1];
  સ્થિર અચલ size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  જો( !buffer ) પાછા -1;

  blake2bp_init( S, outbytes );

  જ્યારે( 1 )
  {
    sum = 0;

    જ્યારે( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      જો( buffer_length == sum )
        તોડો;

      જો( 0 == n )
      {
        જો( ferror( stream ) )
          જાઓ cleanup_buffer;

        જાઓ final_process;
      }

      જો( feof( stream ) )
        જાઓ final_process;
    }

    blake2bp_update( S, buffer, buffer_length );
  }

final_process:;

  જો( sum > 0 ) blake2bp_update( S, buffer, sum );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  મુક્ત( buffer );
  પાછા ret;
}

પ્રકારવ્યાખ્યા પૂર્ણાંક ( *blake2fn )( FILE *, શૂન્ય *, size_t );


સ્થિર શૂન્ય usage( અક્ષર **argv, પૂર્ણાંક errcode )
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


પૂર્ણાંક મુખ્ય( પૂર્ણાંક argc, અક્ષર **argv )
{
  blake2fn blake2_stream = blake2b_stream;
  unsigned long maxbytes = BLAKE2B_OUTBYTES;
  અચલ અક્ષર *algorithm = "BLAKE2b";
  unsigned long outbytes = 0;
  unsigned અક્ષર hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  પૂર્ણાંક c, i;
  opterr = 1;

  જ્યારે( 1 )
  {
    પૂર્ણાંક option_index = 0;
    અક્ષર *end = ખાલી;
    unsigned long outbits;
    સ્થિર સંરચના option long_options[] = {
      { "help",  no_argument, 0,  0  },
      { "tag",   no_argument, 0,  0  },
      { ખાલી, 0, ખાલી, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_index );
    જો( c == -1 ) તોડો;
    પસંદગી( c )
    {
    સ્થિતિ 'a':
      જો( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_stream = blake2b_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      નહીંતર જો ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_stream = blake2s_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      નહીંતર જો ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_stream = blake2bp_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      નહીંતર જો ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_stream = blake2sp_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      નહીંતર
      {
        printf( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      તોડો;

    સ્થિતિ 'l':
      outbits = strtoul(optarg, &end, 10);
      જો( !end || *end != '\0' || outbits % 8 != 0)
      {
        printf( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      તોડો;

    સ્થિતિ 0:
      જો( 0 == strcmp( "help", long_options[option_index].name ) )
        usage( argv, 0 );
      નહીંતર જો( 0 == strcmp( "tag", long_options[option_index].name ) )
        bsdstyle = true;
      તોડો;

    સ્થિતિ '?':
      usage( argv, 1 );
      તોડો;
    }
  }

  જો(outbytes > maxbytes)
  {
    printf( "Invalid length argument: %lu\n", outbytes * 8 );
    printf( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  નહીંતર જો( outbytes == 0 )
    outbytes = maxbytes;

  જો( optind == argc )
    argv[argc++] = (અક્ષર *) "-";

  માટે( i = optind; i < argc; ++i )
  {
    FILE *f = ખાલી;
    જો( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = stdin;
    નહીંતર
      f = fopen( argv[i], "rb" );

    જો( !f )
    {
      fprintf( stderr, "Could not open `%s': %s\n", argv[i], strerror( errno ) );
      ચાલુ;
    }

    જો( blake2_stream( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    નહીંતર
    {
      size_t j;
      જો( bsdstyle )
      {
        જો( outbytes < maxbytes )
          printf( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        નહીંતર
          printf( "%s (%s) = ", algorithm, argv[i] );
      }

      માટે( j = 0; j < outbytes; ++j )
        printf( "%02x", hash[j] );

      જો( bsdstyle )
        printf( "\n" );
      નહીંતર
        printf( "  %s\n", argv[i] );
    }

    જો( f == stdin )
      clearerr( f );
    નહીંતર જો( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  પાછા 0;
}
#endif
