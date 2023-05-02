package src;

import java.applet.*;  
import java.awt.*;  
import java.awt.event.*;


public class Button {

    private boolean is_busy;
    private int x;
    private int y;
    private Color color;

    public Button(Color color) {
        is_busy = false;
        x = 0;
        y = 0;
        this.color = color;
    }
    public Button(Color color, int x, int y) {
        is_busy = false;
        this.x = x;
        this.y = y;
        this.color = color;
    }
    public Button(Button button) {
        this.x = button.x;
        this.y = button.y;
        this.is_busy = button.is_busy;
        this.color = button.color;
    }
    
    public void update() {
        is_busy = false;
    }
    public void show() {
        is_busy = true;
    }
    public void update(int x, int y) {
        is_busy = true;
        this.x = x;
        this.y = y;
    }
    public void draw(Graphics g) {
        if (is_busy) {
            g.setColor(color);
            g.fillRect(11 + 100 * y, 11 + 100 * x, 99, 99);
        }
    }
    public boolean isBusy() {
        return is_busy;
    }
    public boolean isCoord (int x, int y) {
        return this.x == x && this.y == y;
    }
    public int getX () {
        return this.x;
    }
    public int getY () {
        return this.y;
    }
}