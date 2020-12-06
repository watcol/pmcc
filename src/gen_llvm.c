#include"teal.h"

char* types[13] = {
  "unknown",
  "i8",   "i16",   "i32",   "i64",
  "i8*",  "i16*",  "i32*",  "i64*",
  "i8**", "i16**", "i32**", "i64**"
};

void llputty(int ty) {
  if(ty == TY_UNKNOWN) {
    panic("Unknown type.");
  }

    put(types[ty]);
}

int llrefty(int ty) {
  if(ty <= TY_UNKNOWN) panic("Unknown type.");
  if(ty >= TY_I8_REF_REF) panic("Can't refer the reference of reference.");

  return ty + 4;
}

int llderefty(int ty) {
  if(ty <= TY_UNKNOWN) panic("Unknwon type.");
  if(ty <= TY_I64) panic("Can't derefer the entity type.");

  return ty - 4;
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

void llputvar(int id) {
  putc('%');
  putnum(id);
}

int llalign(int ty) {
  if(ty == TY_I8) return 1;
  else if(ty == TY_I16) return 2;
  else if(ty == TY_I32) return 4;
  else if(ty == TY_I64) return 8;

  panic("Unknown type.");
  return 0;
}

void llputalign(int ty) {
  put("align ");
  putnum(llalign(ty));
}

void llretn(int ty, int val) {
  put("  ret ");
  llputty(ty);
  putc(' ');
  putnum(val);
  putc('\n');
}

void llalloca(int var) {
  int ty = llderefty(lvar_type(var));
  put("  ");
  llputvar(var);
  put(" = alloca ");
  llputty(ty);
  put(", ");
  llputalign(ty);
  putc('\n');
}
