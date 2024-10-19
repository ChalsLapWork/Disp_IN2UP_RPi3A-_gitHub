

struct _VFD_{
 union _Config{
  unsigned char byte1;
  struct _Bits{
    unsigned char init_VFD:1;
    unsigned char init_menu:1;
    unsigned char x0:1;
    unsigned char x0:1;
    unsigned char x0:1;
    unsigned char x0:1;
    unsigned char x0:1;
    }bits;
  }config;
};