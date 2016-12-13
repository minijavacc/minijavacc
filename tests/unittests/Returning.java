class Returning {
  public static void main(String[] args) {
    Returning r = new Returning();
    System.out.println(r.foo1(1));
  }
  
  public int foo1(int a) {
    return 1;
    return 2;
  }
}