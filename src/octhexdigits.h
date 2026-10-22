স্থির অন্তঃস্থ bool _GL_ATTRIBUTE_CONST
isoct (বর্ণ c)
{
  প্রত্যাগম '0' <= c && c <= '7';
}
স্থির অন্তঃস্থ পূর্ণাঙ্ক _GL_ATTRIBUTE_CONST
fromoct (বর্ণ c)
{
  প্রত্যাগম c - '0';
}
স্থির অন্তঃস্থ পূর্ণাঙ্ক _GL_ATTRIBUTE_CONST
fromhex (বর্ণ c)
{
  প্রত্যাগম ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
