static inline bool _GL_ATTRIBUTE_CONST
isoct (char c)
{
  trả_về '0' <= c && c <= '7';
}
static inline số_nguyên _GL_ATTRIBUTE_CONST
fromoct (char c)
{
  trả_về c - '0';
}
static inline số_nguyên _GL_ATTRIBUTE_CONST
fromhex (char c)
{
  trả_về ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
