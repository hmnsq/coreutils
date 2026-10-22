स्थिर ओळीत bool _GL_ATTRIBUTE_CONST
isoct (अक्षर c)
{
  परत '0' <= c && c <= '7';
}
स्थिर ओळीत पूर्णांक _GL_ATTRIBUTE_CONST
fromoct (अक्षर c)
{
  परत c - '0';
}
स्थिर ओळीत पूर्णांक _GL_ATTRIBUTE_CONST
fromhex (अक्षर c)
{
  परत ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
