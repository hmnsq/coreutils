स्थिर अन्तःस्थ bool _GL_ATTRIBUTE_CONST
isoct (वर्ण c)
{
  पटिनिवत्त '0' <= c && c <= '7';
}
स्थिर अन्तःस्थ पूर्णाङ्क _GL_ATTRIBUTE_CONST
fromoct (वर्ण c)
{
  पटिनिवत्त c - '0';
}
स्थिर अन्तःस्थ पूर्णाङ्क _GL_ATTRIBUTE_CONST
fromhex (वर्ण c)
{
  पटिनिवत्त ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
