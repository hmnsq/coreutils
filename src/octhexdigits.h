static anta_hstha bool _GL_ATTRIBUTE_CONST
isoct (var_na c)
{
  ᬯᬮᬶ '0' <= c && c <= '7';
}
static anta_hstha puur_naa_nka _GL_ATTRIBUTE_CONST
fromoct (var_na c)
{
  ᬯᬮᬶ c - '0';
}
static anta_hstha puur_naa_nka _GL_ATTRIBUTE_CONST
fromhex (var_na c)
{
  ᬯᬮᬶ ('a' <= c && c <= 'f' ? c - 'a' + 10
          : 'A' <= c && c <= 'F' ? c - 'A' + 10 : c - '0');
}
