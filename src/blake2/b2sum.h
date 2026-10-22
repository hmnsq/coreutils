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

पूर्णाङ्क blake2b_प्रवाहः (FILE *प्रवाहः, शून्य *resstream, size_t outbytes)
  _GL_ATTRIBUTE_NONNULL ((1));
प्रकारनिर्देश पूर्णाङ्क ( *blake2fn )( FILE *, शून्य *, size_t );
#परिभाषय BLAKE2S_OUTBYTES 32
#परिभाषय BLAKE2B_OUTBYTES 64
