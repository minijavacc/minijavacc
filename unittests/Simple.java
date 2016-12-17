class Simple {
  public static void main(String[] args) {
  }
  
  public int foo(int a) {
  int b = 1;
  
    if (a == 3) {
      b = 42;
    }
    
    if (a == 4) {
      b = 23;
    }
    
    return a + b;
  }
}