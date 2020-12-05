#include"teal.h"

char* types[5] = {"unknown", "i8", "i16", "i32", "i64"};

void putty(int ty) {
  if(ty == TY_UNKNOWN) {
    panic("Unknown type.");
  }

  put(types[ty]);
}
