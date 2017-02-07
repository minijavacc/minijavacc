class Simple {
  public static void main(String[] args) {
    Simple s = new Simple();
    s.attr = new Attr();
    s.attr.anInt = 5;
    
    s.foo(1);
    
    System.out.println(s.attr.anInt);
  }
  
  public Attr attr;
  
  public void foo(int x) {
    if (x < 50) {
      this.attr.anInt = 23;
    } else {
      this.attr.anInt = 42;
    }
  }

}


class Attr {
  public int anInt;
}
