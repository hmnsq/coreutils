अटल अंतरभूत bool _GL_ATTRIBUTE_CONST
isoct (अक्षर c)
{
  वापस '0' <= c && c <= '7';
}
अटल अंतरभूत पूर्णांक _GL_ATTRIBUTE_CONST
fromoct (अक्षर c)
{
  वापस c - '0';
}
अटल अंतरभूत पूर्णांक _GL_ATTRIBUTE_CONST
fromhex (अक्षर c)
{
  वापस ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
