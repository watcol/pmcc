#include"parse.h"

void panicParse(char* at) {
  ePutStr("Parse failed (at \"");
  ePutStr(at);
  ePutStrLn("\")");
  sysExit(1);
}

void program() {
  while(func()) {};
}

void parse() {
  program();

  if(!atEof()) {
    ePutStrLn("WARN: Unread characters are remain.");
  }
}
