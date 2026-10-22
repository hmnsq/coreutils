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

#अगर_घोषित HAVE_CONFIG_H
# include <config.h>
#पूर्ण_अगर

#समावेश <stdio.h>
#समावेश <stdlib.h>

#समावेश "blake2.h"

#अगर 0
/* This will help compatibility with coreutils */
पूर्णांक blake2s_stream( खाता *stream, व्योम *resstream, माप_प्रकार outbytes )
{
  पूर्णांक ret = -1;
  माप_प्रकार योग, n;
  blake2s_state S[1];
  अटल स्थिर माप_प्रकार बफर_लंबाई = 32768;
  uint8_t *बफर = ( uint8_t * )दो_स्मृति( बफर_लंबाई );

  अगर( !बफर ) वापस -1;

  blake2s_init( S, outbytes );

  जबतक( 1 )
  {
    योग = 0;

    जबतक( 1 )
    {
      n = ख_पढ़ो( बफर + योग, 1, बफर_लंबाई - योग, stream );
      योग += n;

      अगर( बफर_लंबाई == योग )
        अवरोध;

      अगर( 0 == n )
      {
        अगर( ख_त्रुटि( stream ) )
          जाओ cleanup_बफर;

        जाओ final_process;
      }

      अगर( ख_पूर्ण( stream ) )
        जाओ final_process;
    }

    blake2s_update( S, बफर, बफर_लंबाई );
  }

final_process:;

  अगर( योग > 0 ) blake2s_update( S, बफर, योग );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_बफर:
  मुक्त( बफर );
  वापस ret;
}
#पूर्ण_अगर

पूर्णांक blake2b_stream( खाता *stream, व्योम *resstream, माप_प्रकार outbytes )
{
  पूर्णांक ret = -1;
  माप_प्रकार योग, n;
  blake2b_state S[1];
  अटल स्थिर माप_प्रकार बफर_लंबाई = 32768;
  uint8_t *बफर = ( uint8_t * )दो_स्मृति( बफर_लंबाई );

  अगर( !बफर ) वापस -1;

  blake2b_init( S, outbytes );

  जबतक( 1 )
  {
    योग = 0;

    जबतक( 1 )
    {
      n = ख_पढ़ो( बफर + योग, 1, बफर_लंबाई - योग, stream );
      योग += n;

      अगर( बफर_लंबाई == योग )
        अवरोध;

      अगर( 0 == n )
      {
        अगर( ख_त्रुटि( stream ) )
          जाओ cleanup_बफर;

        जाओ final_process;
      }

      अगर( ख_पूर्ण( stream ) )
        जाओ final_process;
    }

    blake2b_update( S, बफर, बफर_लंबाई );
  }

final_process:;

  अगर( योग > 0 ) blake2b_update( S, बफर, योग );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_बफर:
  मुक्त( बफर );
  वापस ret;
}

#अगर 0

#समावेश <errno.h>
#समावेश <getopt.h>
#समावेश <string.h>
#समावेश <unistd.h>

पूर्णांक blake2sp_stream( खाता *stream, व्योम *resstream, माप_प्रकार outbytes )
{
  पूर्णांक ret = -1;
  माप_प्रकार योग, n;
  blake2sp_state S[1];
  अटल स्थिर माप_प्रकार बफर_लंबाई = 16 * ( 1UL << 20 );
  uint8_t *बफर = ( uint8_t * )दो_स्मृति( बफर_लंबाई );

  अगर( !बफर ) वापस -1;

  blake2sp_init( S, outbytes );

  जबतक( 1 )
  {
    योग = 0;

    जबतक( 1 )
    {
      n = ख_पढ़ो( बफर + योग, 1, बफर_लंबाई - योग, stream );
      योग += n;

      अगर( बफर_लंबाई == योग )
        अवरोध;

      अगर( 0 == n )
      {
        अगर( ख_त्रुटि( stream ) )
          जाओ cleanup_बफर;

        जाओ final_process;
      }

      अगर( ख_पूर्ण( stream ) )
        जाओ final_process;
    }

    blake2sp_update( S, बफर, बफर_लंबाई );
  }

final_process:;

  अगर( योग > 0 ) blake2sp_update( S, बफर, योग );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_बफर:
  मुक्त( बफर );
  वापस ret;
}


पूर्णांक blake2bp_stream( खाता *stream, व्योम *resstream, माप_प्रकार outbytes )
{
  पूर्णांक ret = -1;
  माप_प्रकार योग, n;
  blake2bp_state S[1];
  अटल स्थिर माप_प्रकार बफर_लंबाई = 16 * ( 1UL << 20 );
  uint8_t *बफर = ( uint8_t * )दो_स्मृति( बफर_लंबाई );

  अगर( !बफर ) वापस -1;

  blake2bp_init( S, outbytes );

  जबतक( 1 )
  {
    योग = 0;

    जबतक( 1 )
    {
      n = ख_पढ़ो( बफर + योग, 1, बफर_लंबाई - योग, stream );
      योग += n;

      अगर( बफर_लंबाई == योग )
        अवरोध;

      अगर( 0 == n )
      {
        अगर( ख_त्रुटि( stream ) )
          जाओ cleanup_बफर;

        जाओ final_process;
      }

      अगर( ख_पूर्ण( stream ) )
        जाओ final_process;
    }

    blake2bp_update( S, बफर, बफर_लंबाई );
  }

final_process:;

  अगर( योग > 0 ) blake2bp_update( S, बफर, योग );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_बफर:
  मुक्त( बफर );
  वापस ret;
}

प्रकार पूर्णांक ( *blake2fn )( खाता *, व्योम *, माप_प्रकार );


अटल व्योम usage( अक्षर **argv, पूर्णांक errcode )
{
  खाता *out = errcode ? मानक_त्रुटि : मानक_निकास;
  ख_लिखो( out, "Usage: %s [OPTION]... [FILE]...\n", argv[0] );
  ख_लिखो( out, "\n" );
  ख_लिखो( out, "With no FILE, or when FILE is -, read standard input.\n" );
  ख_लिखो( out, "\n" );
  ख_लिखो( out, "  -a <algo>    hash algorithm (blake2b is default): \n"
                "               [blake2b|blake2s|blake2bp|blake2sp]\n" );
  ख_लिखो( out, "  -l <length>  digest length in bits, must not exceed the maximum for\n"
                "               the selected algorithm and must be a multiple of 8\n" );
  ख_लिखो( out, "  --tag        create a BSD-style checksum\n" );
  ख_लिखो( out, "  --help       display this help and exit\n" );
  निकास( errcode );
}


पूर्णांक मुख्य( पूर्णांक argc, अक्षर **argv )
{
  blake2fn blake2_stream = blake2b_stream;
  अचिन्हित दीर्घ maxbytes = BLAKE2B_OUTBYTES;
  स्थिर अक्षर *algorithm = "BLAKE2b";
  अचिन्हित दीर्घ outbytes = 0;
  अचिन्हित अक्षर hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  पूर्णांक c, i;
  opterr = 1;

  जबतक( 1 )
  {
    पूर्णांक option_सूचकांक = 0;
    अक्षर *end = शून्य;
    अचिन्हित दीर्घ outbits;
    अटल काष्ठा option long_options[] = {
      { "help",  no_argument, 0,  0  },
      { "tag",   no_argument, 0,  0  },
      { शून्य, 0, शून्य, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_सूचकांक );
    अगर( c == -1 ) अवरोध;
    चयन( c )
    {
    हाल 'a':
      अगर( 0 == म_भेद( optarg, "blake2b" ) )
      {
        blake2_stream = blake2b_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      अन्यथा अगर ( 0 == म_भेद( optarg, "blake2s" ) )
      {
        blake2_stream = blake2s_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      अन्यथा अगर ( 0 == म_भेद( optarg, "blake2bp" ) )
      {
        blake2_stream = blake2bp_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      अन्यथा अगर ( 0 == म_भेद( optarg, "blake2sp" ) )
      {
        blake2_stream = blake2sp_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      अन्यथा
      {
        म_लिखो( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      अवरोध;

    हाल 'l':
      outbits = म_से_अदीर्घ(optarg, &end, 10);
      अगर( !end || *end != '\0' || outbits % 8 != 0)
      {
        म_लिखो( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      अवरोध;

    हाल 0:
      अगर( 0 == म_भेद( "help", long_options[option_सूचकांक].name ) )
        usage( argv, 0 );
      अन्यथा अगर( 0 == म_भेद( "tag", long_options[option_सूचकांक].name ) )
        bsdstyle = true;
      अवरोध;

    हाल '?':
      usage( argv, 1 );
      अवरोध;
    }
  }

  अगर(outbytes > maxbytes)
  {
    म_लिखो( "Invalid length argument: %lu\n", outbytes * 8 );
    म_लिखो( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  अन्यथा अगर( outbytes == 0 )
    outbytes = maxbytes;

  अगर( optind == argc )
    argv[argc++] = (अक्षर *) "-";

  क्रम( i = optind; i < argc; ++i )
  {
    खाता *f = शून्य;
    अगर( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = मानक_निवेश;
    अन्यथा
      f = ख_खोलो( argv[i], "rb" );

    अगर( !f )
    {
      ख_लिखो( मानक_त्रुटि, "Could not open `%s': %s\n", argv[i], म_त्रुटि( त्रुटि_सं ) );
      जारी;
    }

    अगर( blake2_stream( f, hash, outbytes ) < 0 )
    {
      ख_लिखो( मानक_त्रुटि, "Failed to hash `%s'\n", argv[i] );
    }
    अन्यथा
    {
      माप_प्रकार j;
      अगर( bsdstyle )
      {
        अगर( outbytes < maxbytes )
          म_लिखो( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        अन्यथा
          म_लिखो( "%s (%s) = ", algorithm, argv[i] );
      }

      क्रम( j = 0; j < outbytes; ++j )
        म_लिखो( "%02x", hash[j] );

      अगर( bsdstyle )
        म_लिखो( "\n" );
      अन्यथा
        म_लिखो( "  %s\n", argv[i] );
    }

    अगर( f == मानक_निवेश )
      साफ_त्रुटि( f );
    अन्यथा अगर( ख_बंद( f ) != 0 )
      ख_लिखो( मानक_त्रुटि, "Could not close `%s': %s\n", argv[i], म_त्रुटि( त्रुटि_सं ) );
  }

  वापस 0;
}
#पूर्ण_अगर
