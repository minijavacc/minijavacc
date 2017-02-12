class PhiConstVisiter {

  public static void main(String[] args) {
    PhiConstVisiter phi = new PhiConstVisiter();
    phi.doo(10);
  }
  
  public void doo(int i) {
    while (i > 0) {
      System.out.println(i);
      i = i - 1;
    }
  }

}
