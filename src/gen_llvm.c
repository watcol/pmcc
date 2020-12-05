#include"teal.h"

char* types[5] = {"unknown", "i8", "i16", "i32", "i64"};

void llputty(int ty) {
  if(ty == TY_UNKNOWN) {
    panic("Unknown type.");
  }

  put(types[ty]);
}

void llfunc_begin(char* name, int ret, int* args, int argc) {
  put("define ");
  llputty(ret);
  put(" @");
  put(name);
  putc('(');

  int c = 0;
  while(c < argc) {
    llputty(args[c]);
    put(" %");
    putnum(c);

    if(c != argc-1) put(", ");
    c++;
  }

  puts(") {");
}

void llfunc_end() {
  puts("}\n");
}

void llbb_begin(char* name) {
  put(name);
  puts(":");
}

void llbb_end() {
  putc('\n');
}

void llinstn(char* in, int ty, int val) {
  put("  ");
  put(in);
  putc(' ');
  llputty(ty);
  putc(' ');
  putnum(val);
  putc('\n');
}
