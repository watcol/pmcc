int main() {
  // Logical Operations
  return !(1 && ((1 < 2) == (1 <= 1))) || !(0 && ((1 > 2) != (1 >= 1)));
}