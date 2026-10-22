ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool _GL_ATTRIBUTE_CONST
isoct (ଅକ୍ଷର c)
{
  ଫେରନ୍ତୁ '0' <= c && c <= '7';
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା _GL_ATTRIBUTE_CONST
fromoct (ଅକ୍ଷର c)
{
  ଫେରନ୍ତୁ c - '0';
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା _GL_ATTRIBUTE_CONST
fromhex (ଅକ୍ଷର c)
{
  ଫେରନ୍ତୁ ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
