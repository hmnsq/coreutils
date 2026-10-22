ساكن مضمن bool _GL_ATTRIBUTE_CONST
isoct (حرف c)
{
  أرجع '0' <= c && c <= '7';
}
ساكن مضمن صحيح _GL_ATTRIBUTE_CONST
fromoct (حرف c)
{
  أرجع c - '0';
}
ساكن مضمن صحيح _GL_ATTRIBUTE_CONST
fromhex (حرف c)
{
  أرجع ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
