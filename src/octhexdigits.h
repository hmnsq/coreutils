സ്ഥിതം വരിയിൽ bool _GL_ATTRIBUTE_CONST
isoct (അക്ഷരം c)
{
  തിരികെ '0' <= c && c <= '7';
}
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ _GL_ATTRIBUTE_CONST
fromoct (അക്ഷരം c)
{
  തിരികെ c - '0';
}
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ _GL_ATTRIBUTE_CONST
fromhex (അക്ഷരം c)
{
  തിരികെ ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
