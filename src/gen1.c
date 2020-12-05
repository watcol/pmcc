#include"teal.h"

char* types[5] = {"unknown", "i8", "i16", "i32", "i64"};

void putty(int ty) {
  if(ty == TY_UNKNOWN) {
    panic("Unknown type.");
  }

  put(types[ty]);
}

void func_begin(char* name, int ret, int* args, int argc) {
  put("define ");
  putty(ret);
  put(" @");
  put(name);
  putc('(');

  int c = 0;
  while(c < argc) {
    putty(args[c]);
    put(" %");
    putnum(c);

    if(c != argc-1) put(", ");
    c++;
  }

  puts(") {");
}

void func_end() {
  puts("}\n");
}

void bb_begin(char* name) {
  put(name);
  puts(":");
}

void bb_end() {
  putc('\n');
}

void instn(char* in, int ty, int val) {
  put("  ");
  put(in);
  putc(' ');
  putty(ty);
  putc(' ');
  putnum(val);
  putc('\n');
}
