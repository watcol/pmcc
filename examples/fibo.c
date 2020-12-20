int fibo(int i) {
  if(i <= 0) {
    return -1;
  } else if(i == 1 || i == 2) {
    return 1;
  } else {
    return fibo(i-1) + fibo(i-2);
  }
}

int main() {
  return fibo(10);
}
