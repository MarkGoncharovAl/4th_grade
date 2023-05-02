package src;

import java.applet.*;  
import java.awt.*;  
import java.awt.event.*;
import src.Button;
import src.Position;
import src.Coord;
import src.Status;
import java.util.Vector;

public class Way {

    private Button button;
    private boolean is_ready = false;
    public Vector<Coord> vector;

    private boolean check(int x, int y) {
        return x >= 0 
            && x <= 7
            && y >= 0
            && y <= 7;
    }
    public Way() {
        vector = new Vector<Coord>();
        button = new Button(Color.yellow);
    }
    public Way(Way way) {
        vector = new Vector<Coord>(way.vector);
        button = new Button(way.button);
        //for (Coord elem : way.vector)
        //    vector.add(elem);
    }
    private boolean isBusy(int x, int y, Position[][] pos) {
        return check(x, y) && pos[x][y].isBusy();
    }
    private boolean isAval(int x, int y, Position[][] pos) {
        return check(x, y) && !pos[x][y].isBusy();
    }
    public boolean update(Vector<Coord> old_vector, Color curColor, int x, int y, int stepX, int stepY, Position[][] pos) {
        vector = new Vector<Coord>(old_vector);
        if (isBusy(x + stepX, y + stepY, pos)
            && curColor != pos[x + stepX][y + stepY].getColor() 
            && isAval(x + 2 * stepX, y + 2 * stepY, pos)) {
                button.update(x + 2 * stepX, y + 2 * stepY);
                is_ready = true;
                vector.add(new Coord(x + stepX, y + stepY));
                return true;
        }
        button.update();
        is_ready = false;
        return false;
    }
    public Status update(Color curColor, int x, int y, int stepX, int stepY, Position[][] pos) {
        if (isAval(x + stepX, y + stepY, pos)) {
                button.update(x + stepX, y + stepY);
                is_ready = true;
                return Status.Neighboor;
        }
        if (isBusy(x + stepX, y + stepY, pos)
            && curColor != pos[x + stepX][y + stepY].getColor() 
            && isAval(x + 2 * stepX, y + 2 * stepY, pos)) {
                button.update(x + 2 * stepX, y + 2 * stepY);
                is_ready = true;
                vector.add(new Coord(x + stepX, y + stepY));
                return Status.StepAval;
        }
        button.update();
        is_ready = false;
        return Status.BusyAll;
    }
    public void update() {
        button.update();
        vector.clear();
    }
    public void run(Position[][] pos) {
        for (int i = 0; i < vector.size(); ++i) {
            pos[vector.get(i).x][vector.get(i).y].update();
        }
    }
    public boolean isBusy() {
        return button.isBusy();
    }
    public boolean isCoord (int x, int y) {
        return button.isCoord(x, y);
    }
    public void draw(Graphics g) {
        button.draw(g);
    }
}