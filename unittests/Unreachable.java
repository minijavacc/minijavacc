class Unreachable {
    public static void main(String[] args) {
        Unreachable t = new Unreachable();
        t.test();
    }

    public boolean test() {
        int a = 0;
        return false;
        if (1 == 2)
            return true;
        else
            a = 42;
    }
}
