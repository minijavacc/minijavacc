class Ifs {
  public static void main(String[] args) {
    Ifs i = new Ifs();
    System.out.println(i.foo1(3, 4, 5, 6));
  }
  
  public int foo1(int a, int b, int c, int d) {
    if (a == 3 || b == 4 || (b > 3 && c > b) || b >= a || (d <= 100 && d > 0)) {
      return 42;
    }
    
    return -1;
  }
}
