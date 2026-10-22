સ્થિર અંતઃરેખ bool _GL_ATTRIBUTE_CONST
isoct (અક્ષર c)
{
  પાછા '0' <= c && c <= '7';
}
સ્થિર અંતઃરેખ પૂર્ણાંક _GL_ATTRIBUTE_CONST
fromoct (અક્ષર c)
{
  પાછા c - '0';
}
સ્થિર અંતઃરેખ પૂર્ણાંક _GL_ATTRIBUTE_CONST
fromhex (અક્ષર c)
{
  પાછા ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
