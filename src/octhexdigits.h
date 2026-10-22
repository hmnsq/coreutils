static inline bool _GL_ATTRIBUTE_CONST
isoct (char c)
{
  врати '0' <= c && c <= '7';
}
static inline цео _GL_ATTRIBUTE_CONST
fromoct (char c)
{
  врати c - '0';
}
static inline цео _GL_ATTRIBUTE_CONST
fromhex (char c)
{
  врати ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
