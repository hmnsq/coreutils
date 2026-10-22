স্থির অন্তঃরেখ bool _GL_ATTRIBUTE_CONST
isoct (অক্ষর c)
{
  ফেরত '0' <= c && c <= '7';
}
স্থির অন্তঃরেখ পূর্ণসংখ্যা _GL_ATTRIBUTE_CONST
fromoct (অক্ষর c)
{
  ফেরত c - '0';
}
স্থির অন্তঃরেখ পূর্ণসংখ্যা _GL_ATTRIBUTE_CONST
fromhex (অক্ষর c)
{
  ফেরত ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
