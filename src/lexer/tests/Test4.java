class Test4 {

	public static void main(String[] args) {
		while (5 > 3) {
			if (!false) {
				Test4 t = new Test4();
				int f = 10;
				while (t.whilee(f)) {
					t.whilee(f * 5);
				}
			}
		}
	}

	public boolean whilee(int f) {
		return f < 7;
	}

}
