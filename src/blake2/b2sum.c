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

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CONFIG_H
# include <config.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "blake2.h"

#ਜੇ 0
/* This will help compatibility with coreutils */
ਪੂਰਨ blake2s_ਧਾਰਾ( FILE *ਧਾਰਾ, ਖਾਲੀ *resstream, size_t outbytes )
{
  ਪੂਰਨ ret = -1;
  size_t ਜੋੜ, n;
  blake2s_state S[1];
  ਸਥਿਰ ਅਚਲ size_t ਬਫਰ_ਲੰਮਾਈ = 32768;
  uint8_t *ਬਫਰ = ( uint8_t * )ਸਮ੍ਰਿਤੀ_ਦਿਓ( ਬਫਰ_ਲੰਮਾਈ );

  ਜੇ( !ਬਫਰ ) ਵਾਪਸ -1;

  blake2s_init( S, outbytes );

  ਜਦੋਂ( 1 )
  {
    ਜੋੜ = 0;

    ਜਦੋਂ( 1 )
    {
      n = fread( ਬਫਰ + ਜੋੜ, 1, ਬਫਰ_ਲੰਮਾਈ - ਜੋੜ, ਧਾਰਾ );
      ਜੋੜ += n;

      ਜੇ( ਬਫਰ_ਲੰਮਾਈ == ਜੋੜ )
        ਰੋਕੋ;

      ਜੇ( 0 == n )
      {
        ਜੇ( ferror( ਧਾਰਾ ) )
          ਜਾਓ cleanup_ਬਫਰ;

        ਜਾਓ final_process;
      }

      ਜੇ( feof( ਧਾਰਾ ) )
        ਜਾਓ final_process;
    }

    blake2s_update( S, ਬਫਰ, ਬਫਰ_ਲੰਮਾਈ );
  }

final_process:;

  ਜੇ( ਜੋੜ > 0 ) blake2s_update( S, ਬਫਰ, ਜੋੜ );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_ਬਫਰ:
  ਮੁਕਤ( ਬਫਰ );
  ਵਾਪਸ ret;
}
#ਜੇ_ਅੰਤ

ਪੂਰਨ blake2b_ਧਾਰਾ( FILE *ਧਾਰਾ, ਖਾਲੀ *resstream, size_t outbytes )
{
  ਪੂਰਨ ret = -1;
  size_t ਜੋੜ, n;
  blake2b_state S[1];
  ਸਥਿਰ ਅਚਲ size_t ਬਫਰ_ਲੰਮਾਈ = 32768;
  uint8_t *ਬਫਰ = ( uint8_t * )ਸਮ੍ਰਿਤੀ_ਦਿਓ( ਬਫਰ_ਲੰਮਾਈ );

  ਜੇ( !ਬਫਰ ) ਵਾਪਸ -1;

  blake2b_init( S, outbytes );

  ਜਦੋਂ( 1 )
  {
    ਜੋੜ = 0;

    ਜਦੋਂ( 1 )
    {
      n = fread( ਬਫਰ + ਜੋੜ, 1, ਬਫਰ_ਲੰਮਾਈ - ਜੋੜ, ਧਾਰਾ );
      ਜੋੜ += n;

      ਜੇ( ਬਫਰ_ਲੰਮਾਈ == ਜੋੜ )
        ਰੋਕੋ;

      ਜੇ( 0 == n )
      {
        ਜੇ( ferror( ਧਾਰਾ ) )
          ਜਾਓ cleanup_ਬਫਰ;

        ਜਾਓ final_process;
      }

      ਜੇ( feof( ਧਾਰਾ ) )
        ਜਾਓ final_process;
    }

    blake2b_update( S, ਬਫਰ, ਬਫਰ_ਲੰਮਾਈ );
  }

final_process:;

  ਜੇ( ਜੋੜ > 0 ) blake2b_update( S, ਬਫਰ, ਜੋੜ );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_ਬਫਰ:
  ਮੁਕਤ( ਬਫਰ );
  ਵਾਪਸ ret;
}

#ਜੇ 0

#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>

ਪੂਰਨ blake2sp_ਧਾਰਾ( FILE *ਧਾਰਾ, ਖਾਲੀ *resstream, size_t outbytes )
{
  ਪੂਰਨ ret = -1;
  size_t ਜੋੜ, n;
  blake2sp_state S[1];
  ਸਥਿਰ ਅਚਲ size_t ਬਫਰ_ਲੰਮਾਈ = 16 * ( 1UL << 20 );
  uint8_t *ਬਫਰ = ( uint8_t * )ਸਮ੍ਰਿਤੀ_ਦਿਓ( ਬਫਰ_ਲੰਮਾਈ );

  ਜੇ( !ਬਫਰ ) ਵਾਪਸ -1;

  blake2sp_init( S, outbytes );

  ਜਦੋਂ( 1 )
  {
    ਜੋੜ = 0;

    ਜਦੋਂ( 1 )
    {
      n = fread( ਬਫਰ + ਜੋੜ, 1, ਬਫਰ_ਲੰਮਾਈ - ਜੋੜ, ਧਾਰਾ );
      ਜੋੜ += n;

      ਜੇ( ਬਫਰ_ਲੰਮਾਈ == ਜੋੜ )
        ਰੋਕੋ;

      ਜੇ( 0 == n )
      {
        ਜੇ( ferror( ਧਾਰਾ ) )
          ਜਾਓ cleanup_ਬਫਰ;

        ਜਾਓ final_process;
      }

      ਜੇ( feof( ਧਾਰਾ ) )
        ਜਾਓ final_process;
    }

    blake2sp_update( S, ਬਫਰ, ਬਫਰ_ਲੰਮਾਈ );
  }

final_process:;

  ਜੇ( ਜੋੜ > 0 ) blake2sp_update( S, ਬਫਰ, ਜੋੜ );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_ਬਫਰ:
  ਮੁਕਤ( ਬਫਰ );
  ਵਾਪਸ ret;
}


ਪੂਰਨ blake2bp_ਧਾਰਾ( FILE *ਧਾਰਾ, ਖਾਲੀ *resstream, size_t outbytes )
{
  ਪੂਰਨ ret = -1;
  size_t ਜੋੜ, n;
  blake2bp_state S[1];
  ਸਥਿਰ ਅਚਲ size_t ਬਫਰ_ਲੰਮਾਈ = 16 * ( 1UL << 20 );
  uint8_t *ਬਫਰ = ( uint8_t * )ਸਮ੍ਰਿਤੀ_ਦਿਓ( ਬਫਰ_ਲੰਮਾਈ );

  ਜੇ( !ਬਫਰ ) ਵਾਪਸ -1;

  blake2bp_init( S, outbytes );

  ਜਦੋਂ( 1 )
  {
    ਜੋੜ = 0;

    ਜਦੋਂ( 1 )
    {
      n = fread( ਬਫਰ + ਜੋੜ, 1, ਬਫਰ_ਲੰਮਾਈ - ਜੋੜ, ਧਾਰਾ );
      ਜੋੜ += n;

      ਜੇ( ਬਫਰ_ਲੰਮਾਈ == ਜੋੜ )
        ਰੋਕੋ;

      ਜੇ( 0 == n )
      {
        ਜੇ( ferror( ਧਾਰਾ ) )
          ਜਾਓ cleanup_ਬਫਰ;

        ਜਾਓ final_process;
      }

      ਜੇ( feof( ਧਾਰਾ ) )
        ਜਾਓ final_process;
    }

    blake2bp_update( S, ਬਫਰ, ਬਫਰ_ਲੰਮਾਈ );
  }

final_process:;

  ਜੇ( ਜੋੜ > 0 ) blake2bp_update( S, ਬਫਰ, ਜੋੜ );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_ਬਫਰ:
  ਮੁਕਤ( ਬਫਰ );
  ਵਾਪਸ ret;
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ ( *blake2fn )( FILE *, ਖਾਲੀ *, size_t );


ਸਥਿਰ ਖਾਲੀ usage( ਅੱਖਰ **argv, ਪੂਰਨ errcode )
{
  FILE *ਬਾਹਰਲਾ = errcode ? stderr : stdout;
  fprintf( ਬਾਹਰਲਾ, "Usage: %s [OPTION]... [FILE]...\n", argv[0] );
  fprintf( ਬਾਹਰਲਾ, "\n" );
  fprintf( ਬਾਹਰਲਾ, "With no FILE, or when FILE is -, read standard input.\n" );
  fprintf( ਬਾਹਰਲਾ, "\n" );
  fprintf( ਬਾਹਰਲਾ, "  -a <algo>    hash algorithm (blake2b is default): \n"
                "               [blake2b|blake2s|blake2bp|blake2sp]\n" );
  fprintf( ਬਾਹਰਲਾ, "  -l <length>  digest length in bits, must not exceed the maximum for\n"
                "               the selected algorithm and must be a multiple of 8\n" );
  fprintf( ਬਾਹਰਲਾ, "  --tag        create a BSD-style checksum\n" );
  fprintf( ਬਾਹਰਲਾ, "  --help       display this help and exit\n" );
  ਬਾਹਰ_ਨਿਕਲੋ( errcode );
}


ਪੂਰਨ ਮੁੱਖ( ਪੂਰਨ argc, ਅੱਖਰ **argv )
{
  blake2fn blake2_ਧਾਰਾ = blake2b_ਧਾਰਾ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ maxbytes = BLAKE2B_OUTBYTES;
  ਅਚਲ ਅੱਖਰ *algorithm = "BLAKE2b";
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ outbytes = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  ਪੂਰਨ c, i;
  opterr = 1;

  ਜਦੋਂ( 1 )
  {
    ਪੂਰਨ option_ਸੂਚਕਾਂਕ = 0;
    ਅੱਖਰ *ਸਿਰਾ = ਨਿਰਾ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ outbits;
    ਸਥਿਰ ਬਣਤਰ option long_options[] = {
      { "help",  no_ਦਲੀਲ, 0,  0  },
      { "tag",   no_ਦਲੀਲ, 0,  0  },
      { ਨਿਰਾ, 0, ਨਿਰਾ, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_ਸੂਚਕਾਂਕ );
    ਜੇ( c == -1 ) ਰੋਕੋ;
    ਚੋਣ( c )
    {
    ਹਾਲਤ 'a':
      ਜੇ( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_ਧਾਰਾ = blake2b_ਧਾਰਾ;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      ਨਹੀਂਤਾਂ ਜੇ ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_ਧਾਰਾ = blake2s_ਧਾਰਾ;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      ਨਹੀਂਤਾਂ ਜੇ ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_ਧਾਰਾ = blake2bp_ਧਾਰਾ;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      ਨਹੀਂਤਾਂ ਜੇ ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_ਧਾਰਾ = blake2sp_ਧਾਰਾ;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      ਨਹੀਂਤਾਂ
      {
        ਲਿਖੋ( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      ਰੋਕੋ;

    ਹਾਲਤ 'l':
      outbits = strtoul(optarg, &ਸਿਰਾ, 10);
      ਜੇ( !ਸਿਰਾ || *ਸਿਰਾ != '\0' || outbits % 8 != 0)
      {
        ਲਿਖੋ( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      ਰੋਕੋ;

    ਹਾਲਤ 0:
      ਜੇ( 0 == strcmp( "help", long_options[option_ਸੂਚਕਾਂਕ].name ) )
        usage( argv, 0 );
      ਨਹੀਂਤਾਂ ਜੇ( 0 == strcmp( "tag", long_options[option_ਸੂਚਕਾਂਕ].name ) )
        bsdstyle = true;
      ਰੋਕੋ;

    ਹਾਲਤ '?':
      usage( argv, 1 );
      ਰੋਕੋ;
    }
  }

  ਜੇ(outbytes > maxbytes)
  {
    ਲਿਖੋ( "Invalid length argument: %lu\n", outbytes * 8 );
    ਲਿਖੋ( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  ਨਹੀਂਤਾਂ ਜੇ( outbytes == 0 )
    outbytes = maxbytes;

  ਜੇ( optind == argc )
    argv[argc++] = (ਅੱਖਰ *) "-";

  ਲਈ( i = optind; i < argc; ++i )
  {
    FILE *f = ਨਿਰਾ;
    ਜੇ( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = stdin;
    ਨਹੀਂਤਾਂ
      f = fopen( argv[i], "rb" );

    ਜੇ( !f )
    {
      fprintf( stderr, "Could not open `%s': %s\n", argv[i], strerror( errno ) );
      ਜਾਰੀ;
    }

    ਜੇ( blake2_ਧਾਰਾ( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    ਨਹੀਂਤਾਂ
    {
      size_t j;
      ਜੇ( bsdstyle )
      {
        ਜੇ( outbytes < maxbytes )
          ਲਿਖੋ( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        ਨਹੀਂਤਾਂ
          ਲਿਖੋ( "%s (%s) = ", algorithm, argv[i] );
      }

      ਲਈ( j = 0; j < outbytes; ++j )
        ਲਿਖੋ( "%02x", hash[j] );

      ਜੇ( bsdstyle )
        ਲਿਖੋ( "\n" );
      ਨਹੀਂਤਾਂ
        ਲਿਖੋ( "  %s\n", argv[i] );
    }

    ਜੇ( f == stdin )
      clearerr( f );
    ਨਹੀਂਤਾਂ ਜੇ( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ
