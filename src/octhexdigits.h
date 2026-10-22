ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool _GL_ATTRIBUTE_CONST
isoct (ಅಕ್ಷರ c)
{
  ಹಿಂತಿರುಗಿ '0' <= c && c <= '7';
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ _GL_ATTRIBUTE_CONST
fromoct (ಅಕ್ಷರ c)
{
  ಹಿಂತಿರುಗಿ c - '0';
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ _GL_ATTRIBUTE_CONST
fromhex (ಅಕ್ಷರ c)
{
  ಹಿಂತಿರುಗಿ ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
