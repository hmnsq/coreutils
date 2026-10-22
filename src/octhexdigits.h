ساكن مضمن bool _GL_ATTRIBUTE_CONST
isoct (حرف c)
{
  החזר '0' <= c && c <= '7';
}
ساكن مضمن صحيح _GL_ATTRIBUTE_CONST
fromoct (حرف c)
{
  החזר c - '0';
}
ساكن مضمن صحيح _GL_ATTRIBUTE_CONST
fromhex (حرف c)
{
  החזר ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
