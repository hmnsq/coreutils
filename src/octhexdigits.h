ਸਥਿਰ ਸਤਰਅੰਦਰ bool _GL_ATTRIBUTE_CONST
isoct (ਅੱਖਰ c)
{
  ਵਾਪਸ '0' <= c && c <= '7';
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ _GL_ATTRIBUTE_CONST
fromoct (ਅੱਖਰ c)
{
  ਵਾਪਸ c - '0';
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ _GL_ATTRIBUTE_CONST
fromhex (ਅੱਖਰ c)
{
  ਵਾਪਸ ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
