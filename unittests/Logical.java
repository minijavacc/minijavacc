class Logical {
  public static void main(String[] args) {
    Logical l = new Logical();
    if (l.foo1(3, 5)) {
      System.out.println(1);
    } else {
      System.out.println(2);
    }
  }
  
  public boolean foo1(int a, int b) {
    return ((a == 5) || (a == 3)) && (a == b);
  }
}