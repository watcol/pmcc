#include"teal.h"

char* regs[41] = {
  "unknown",
   "al", "dil", "sil",  "dl",  "cl", "r8b", "r9b", "spl", "bpl",  "bl",
   "ax",  "di",  "si",  "dx",  "cx", "r8w", "r9w",  "sp",  "bp",  "bx",
  "eax", "edi", "esi", "edx", "ecx", "r8d", "r9d", "esp", "ebp", "ebx",
  "rax", "rdi", "rsi", "rdx", "rcx",  "r8",  "r9", "rsp", "rbp", "rbx",
};

void putreg(int i) {
  if (i > 0 && i <= 40) {
    put(regs[i]);
  } else {
    eputs("Unknown operand type.");
    sys_exit(1);
  }
}

int conv_type(int reg, int type) {
  if(reg == REG_UNKNOWN) {
    return REG_UNKNOWN;
  }

  reg %= 10;

  if(reg == 0) {
    reg = 10;
  }

  int c = 0;
  if(type == TY_CHAR) {
    c = 0;
  } else if(type == TY_SHORT) {
    c = 1;
  } else if(type == TY_INT) {
    c = 2;
  } else if(type == TY_LONG) {
    c = 3;
  }

  return reg + c*10;
}

int reg_type(int i) {
  if(i > 0 && i <= 10) {
    return TY_CHAR;
  } else if(i > 10 && i <= 20) {
    return TY_SHORT;
  } else if(i > 20 && i <= 30) {
    return TY_INT;
  } else if(i > 30 && i <= 40) {
    return TY_LONG;
  } else {
    return TY_UNKNOWN;
  }
}

void putmem(int id) {
  int type = get_type(id);
  int offset = get_offset(id);
  if(type == TY_UNKNOWN) {
    eputs("Undeclared variable.");
    sys_exit(1);
  }

  if(type == TY_CHAR) {
    put("BYTE");
  } else if(type == TY_SHORT) {
    put("WORD");
  } else if(type == TY_INT) {
    put("DWORD");
  } else if(type == TY_LONG) {
    put("QWORD");
  } else {
    eputs("Unknown type.");
    sys_exit(1);
  }

  put(" ptr [rbp-");
  putnum(offset);
  put("]");
}

void inst(char* in) {
  put("  ");
  put(in);
  putc('\n');
}

void instr(char* in, int val) {
  put("  ");
  put(in);
  putc(' ');
  putreg(val);
  putc('\n');
}

void instm(char* in, int val) {
  put("  ");
  put(in);
  putc(' ');
  putmem(val);
  putc('\n');
}

void instl(char* in) {
  put("  ");
  put(in);
  putc(' ');
  lex_put();
  putc('\n');

}

void instn(char* in, int val) {
  put("  ");
  put(in);
  putc(' ');
  putnum(val);
  putc('\n');
}

void instrr(char* in, int dst, int src) {
  put("  ");
  put(in);
  putc(' ');
  putreg(dst);
  put(", ");
  putreg(src);
  putc('\n');
}

void instrm(char* in, int dst, int src) {
  put("  ");
  put(in);
  putc(' ');
  putreg(dst);
  put(", ");
  putmem(src);
  putc('\n');
}

void instrl(char* in, int dst) {
  put("  ");
  put(in);
  putc(' ');
  putreg(dst);
  put(", ");
  lex_put();
  putc('\n');
}

void instrn(char* in, int dst, int src) {
  put("  ");
  put(in);
  putc(' ');
  putreg(dst);
  put(", ");
  putnum(src);
  putc('\n');
}

void instmr(char* in, int dst, int src) {
  put("  ");
  put(in);
  putc(' ');
  putmem(dst);
  put(", ");
  putreg(src);
  putc('\n');
}

void instmm(char* in, int dst, int src) {
  put("  ");
  put(in);
  putc(' ');
  putmem(dst);
  put(", ");
  putmem(src);
  putc('\n');
}

void instml(char* in, int dst) {
  put("  ");
  put(in);
  putc(' ');
  putmem(dst);
  put(", ");
  lex_put();
  putc('\n');
}

void instmn(char* in, int dst, int src) {
  put("  ");
  put(in);
  putc(' ');
  putmem(dst);
  put(", ");
  putnum(src);
  putc('\n');
}
