স্থির অন্তঃস্থ bool _GL_ATTRIBUTE_CONST
isoct (ৱর্ণ c)
{
  ঘূৰাই '0' <= c && c <= '7';
}
স্থির অন্তঃস্থ পূর্ণাঙ্ক _GL_ATTRIBUTE_CONST
fromoct (ৱর্ণ c)
{
  ঘূৰাই c - '0';
}
স্থির অন্তঃস্থ পূর্ণাঙ্ক _GL_ATTRIBUTE_CONST
fromhex (ৱর্ণ c)
{
  ঘূৰাই ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
