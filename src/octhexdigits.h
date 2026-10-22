நிலையான உள்வரி bool _GL_ATTRIBUTE_CONST
isoct (எழுத்து c)
{
  திரும்பு '0' <= c && c <= '7';
}
நிலையான உள்வரி முழு _GL_ATTRIBUTE_CONST
fromoct (எழுத்து c)
{
  திரும்பு c - '0';
}
நிலையான உள்வரி முழு _GL_ATTRIBUTE_CONST
fromhex (எழுத்து c)
{
  திரும்பு ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
