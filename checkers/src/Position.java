package src;

import java.awt.*;  

public class Position
{
    private boolean is_available;
    private boolean is_busy;
    private Color color;
    private int x;
    private int y;

    public Position(int x, int y) {
        this.is_available = ((x + y) % 2 == 0);
        this.is_busy = false;
        this.x = x;
        this.y = y;
    }
    public void update() {
        this.is_busy = false;
    }
    public void update(Color color) {
        this.is_busy = true;
        this.color = color;
    }
    public void draw(Graphics g) {
        if (!is_available || !is_busy)
            return;
        g.setColor(color);
        g.fillOval(20 + 100 * y, 20 + 100 * x, 80, 80);
        g.drawOval(20 + 100 * y, 20 + 100 * x, 80, 80);
    }
    public boolean isWhite() {
        return color == Color.white;
    }
    public boolean isBusy() {
        return is_busy;
    }
    public Color getColor() {
        return color;
    }
}