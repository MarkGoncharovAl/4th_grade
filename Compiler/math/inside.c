int checkPixel(int ij, int i_1j, int ij_1, int i1j, int ij1, int speed) {
  return ij + (i_1j + ij_1 + i1j + ij1 - 4 * ij) / speed;
}