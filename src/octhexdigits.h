ساکن اندرونی bool _GL_ATTRIBUTE_CONST
isoct (حرف c)
{
  واپس '0' <= c && c <= '7';
}
ساکن اندرونی صحیح _GL_ATTRIBUTE_CONST
fromoct (حرف c)
{
  واپس c - '0';
}
ساکن اندرونی صحیح _GL_ATTRIBUTE_CONST
fromhex (حرف c)
{
  واپس ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
