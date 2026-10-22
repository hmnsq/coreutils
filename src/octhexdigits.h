static inline bool _GL_ATTRIBUTE_CONST
isoct (char c)
{
  вернуть '0' <= c && c <= '7';
}
static inline целое _GL_ATTRIBUTE_CONST
fromoct (char c)
{
  вернуть c - '0';
}
static inline целое _GL_ATTRIBUTE_CONST
fromhex (char c)
{
  вернуть ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
