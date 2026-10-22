ایستا درونخطی bool _GL_ATTRIBUTE_CONST
isoct (نویسه c)
{
  بازگشت '0' <= c && c <= '7';
}
ایستا درونخطی صحیح _GL_ATTRIBUTE_CONST
fromoct (نویسه c)
{
  بازگشت c - '0';
}
ایستا درونخطی صحیح _GL_ATTRIBUTE_CONST
fromhex (نویسه c)
{
  بازگشت ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
