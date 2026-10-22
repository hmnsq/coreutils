ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool _GL_ATTRIBUTE_CONST
isoct (ವರ್ಣ c)
{
  ಪ್ರತ್ಯಾಗಮ '0' <= c && c <= '7';
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ _GL_ATTRIBUTE_CONST
fromoct (ವರ್ಣ c)
{
  ಪ್ರತ್ಯಾಗಮ c - '0';
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ _GL_ATTRIBUTE_CONST
fromhex (ವರ್ಣ c)
{
  ಪ್ರತ್ಯಾಗಮ ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
