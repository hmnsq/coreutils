static inline bool _GL_ATTRIBUTE_CONST
isoct (char c)
{
  返回 '0' <= c && c <= '7';
}
static inline 整数 _GL_ATTRIBUTE_CONST
fromoct (char c)
{
  返回 c - '0';
}
static inline 整数 _GL_ATTRIBUTE_CONST
fromhex (char c)
{
  返回 ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
