package src;

public class Coord {
    public int x;
    public int y;
    public Coord() {
        x = -1;
        y = -1;
    }
    public Coord(int ax, int ay) {
        this.x = ax;
        this.y = ay;
    }
    public Coord(Coord new_coord) {
        x = new_coord.x;
        y = new_coord.y;
    }
}
