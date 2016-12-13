class Ifs {
  public static void main(String[] args) {
    Ifs i = new Ifs();
  }
  
  public int foo1(int a) {
    if (a == 3) {
      return 42;
    }
    
    if (a == 4) {
      return 23;
    }
    
    return -1;
  }
}