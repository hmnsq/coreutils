static inline bool _GL_ATTRIBUTE_CONST
isoct (char c)
{
  повернути '0' <= c && c <= '7';
}
static inline ціле _GL_ATTRIBUTE_CONST
fromoct (char c)
{
  повернути c - '0';
}
static inline ціле _GL_ATTRIBUTE_CONST
fromhex (char c)
{
  повернути ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
