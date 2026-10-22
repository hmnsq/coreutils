ساكن مضمن bool _GL_ATTRIBUTE_CONST
isoct (حرف c)
{
  ܦܢܐ '0' <= c && c <= '7';
}
ساكن مضمن صحيح _GL_ATTRIBUTE_CONST
fromoct (حرف c)
{
  ܦܢܐ c - '0';
}
ساكن مضمن صحيح _GL_ATTRIBUTE_CONST
fromhex (حرف c)
{
  ܦܢܐ ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
