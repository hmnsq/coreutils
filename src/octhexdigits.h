static inline bool _GL_ATTRIBUTE_CONST
isoct (char c)
{
  반환 '0' <= c && c <= '7';
}
static inline 정수 _GL_ATTRIBUTE_CONST
fromoct (char c)
{
  반환 c - '0';
}
static inline 정수 _GL_ATTRIBUTE_CONST
fromhex (char c)
{
  반환 ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
