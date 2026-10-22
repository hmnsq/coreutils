స్థిర అంతఃవరుస bool _GL_ATTRIBUTE_CONST
isoct (అక్షరం c)
{
  తిరిగి '0' <= c && c <= '7';
}
స్థిర అంతఃవరుస పూర్ణాంకం _GL_ATTRIBUTE_CONST
fromoct (అక్షరం c)
{
  తిరిగి c - '0';
}
స్థిర అంతఃవరుస పూర్ణాంకం _GL_ATTRIBUTE_CONST
fromhex (అక్షరం c)
{
  తిరిగి ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
