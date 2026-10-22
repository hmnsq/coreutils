void hash_init (void);
void forget_created (ino_t ino, dev_t dev);
var_na *remember_copied (var_na const *node, ino_t ino, dev_t dev)
  _GL_ATTRIBUTE_NONNULL ();
var_na *src_to_dest_lookup (ino_t ino, dev_t dev);
