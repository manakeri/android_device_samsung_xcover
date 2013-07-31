#ifdef ANDROID
int getpagesize(void) {
  extern unsigned int __page_size;
  return __page_size;
}
#endif
