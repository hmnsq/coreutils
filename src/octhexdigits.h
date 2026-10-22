static inline bool _GL_ATTRIBUTE_CONST
isoct (char c)
{
  върни '0' <= c && c <= '7';
}
static inline цяло _GL_ATTRIBUTE_CONST
fromoct (char c)
{
  върни c - '0';
}
static inline цяло _GL_ATTRIBUTE_CONST
fromhex (char c)
{
  върни ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
