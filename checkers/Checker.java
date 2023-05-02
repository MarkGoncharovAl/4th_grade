//First.java  
import java.applet.*;  
import java.awt.*;  
import java.awt.event.*;

import src.Button;
import src.Position;
import src.Way;
import src.Status;
import src.Coord;

import java.util.Vector;
import java.util.Collections;

class Click {
    private Button main;
    private Vector<Way> ways;
    private Vector<Vector<Button>> possible;

    private boolean check(int x, int y) {
        return x >= 0 
            && x <= 7
            && y >= 0
            && y <= 7;
    }
    public Click() {
        main = new Button(Color.red);
        ways = new Vector<Way>();
        possible = new Vector<Vector<Button>>();
        for (int i = 0; i < 8; ++i) {
            possible.add(new Vector<>());
            for (int j = 0; j < 8; ++j)
                possible.get(i).add(new Button(Color.blue, i, j));
        }
    }
    private boolean checkPos(int x, int y, Position[][] pos, Color col) {
        if (!pos[x][y].isBusy() || pos[x][y].getColor() != col)
            return false;
        Coord[] cs = { new Coord(1, 1), new Coord(1, -1), new Coord(-1, 1), new Coord(-1, -1)};
        for (Coord c : cs) {
            if (check(x + c.x, y + c.y) 
                && !pos[x + c.x][y + c.y].isBusy())
                    return true;
            if (check(x + c.x, y + c.y)
                && pos[x + c.x][y + c.y].getColor() != col 
                && check(x + 2 * c.x, y + 2 * c.y) 
                && !pos[x + 2 * c.x][y + 2 * c.y].isBusy())
                return true;
        }
        return false;
    }

    public void update(Position[][] pos, Color col) {
        ways.clear();
        main.update();
        for (Way way : ways)
            way.update();
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                Button but = possible.get(i).get(j);
                if (checkPos(i, j, pos, col))
                    but.show();
                else
                    but.update();
            }
        }
    }
    private void updateWay(Vector<Coord> vector, Color col, Position[][] pos, int x, int y, int stepX, int stepY) {
        Vector<Coord> cs = new Vector<Coord>(); 
        if (stepX != -1 || stepY != -1)
            cs.add(new Coord(1, 1));
        if (stepX != -1 || stepY != 1)
            cs.add(new Coord(1, -1));
        if (stepX != 1 || stepY != -1)
            cs.add(new Coord(-1, 1));
        if (stepX != 1 || stepY != 1)
            cs.add(new Coord(-1, -1));
        for (Coord coord : cs) {
            Way new_way = new Way();
            if (new_way.update(vector, col, x, y, coord.x, coord.y, pos)) {
                updateWay(vector, col, pos, x + 2 * coord.x, y + 2 * coord.y, coord.x, coord.y);
            }
            ways.add(new_way);
        }
    }
    private void updateWay(Color col, Position[][] pos, int x, int y, int stepX, int stepY) {
        Way new_way = new Way();
        Status stat = new_way.update(col, x, y, stepX, stepY, pos);
        if (stat == Status.StepAval) {
            Vector<Coord> vector = new_way.vector;
            updateWay(vector, col, pos, x + 2 * stepX, y + 2 * stepY, stepX, stepY);
        }
        ways.add(new_way);
    }
    private void checkWays() {
        Vector<Integer> sizes = new Vector<Integer>();
        for (Way way : ways)
            sizes.add(way.vector.size());
        Integer m = Collections.max(sizes);
        for (int i = 0; i < ways.size(); ++i)
            if (ways.get(i).vector.size() != m)
                ways.get(i).update();
    }
    public void update(int x, int y, Position[][] pos) {
        ways.clear();
        main.update(x, y);
        Color curColor = pos[x][y].getColor();
        int stepX = pos[x][y].isWhite() ? 1 : -1;
        Vector<Coord> vector = new Vector<>();
        updateWay(curColor, pos, x, y, stepX, 1);
        updateWay(curColor, pos, x, y, stepX, -1);
        updateWay(curColor, pos, x, y, -stepX, 1);
        updateWay(curColor, pos, x, y, -stepX, -1);
        checkWays();
    }
    public void draw(Graphics g) {
        for (Way way : ways)
            way.draw(g);   
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j) 
                possible.get(i).get(j).draw(g);
        main.draw(g);
    }
    public boolean is_chosen() {
        boolean out = false;
        for (Way way : ways) 
            out = out || way.isBusy(); 
        return main.isBusy() && out;
    }
    public boolean is_available(int x, int y) {
        boolean out = false;
        for (Way way : ways) 
            out = out || (way.isCoord(x, y) && way.isBusy()); 
        return is_chosen() && out;
    }
    public Button get_main() {
        return main;
    }
    public Way find_way(int x, int y) {
        for (Way way : ways) {
            if (way.isCoord(x, y))
                return way;
        }
        return ways.get(0);
    }
    public void run(int x, int y, Position[][] pos, Color col) {
        int new_x = main.getX();
        int new_y = main.getY();
        find_way(x, y).run(pos);
        pos[x][y].update(pos[new_x][new_y].getColor());
        pos[new_x][new_y].update();
        update(pos, col);
    }
}

class Action {
    public Position[][] pos;
    public Click click;
    Color color;

    private void switchUser() {
        color = 
            (color == (Color.white)) 
            ? Color.black 
            : Color.white;
    }

    public Action() {
        color = Color.white;
        click = new Click();
        this.pos = new Position[8][8];
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                this.pos[i][j] = new Position(i, j);

        for (int i = 0; i < 4; ++i) {
            this.pos[0][i * 2].update(Color.white);
            this.pos[1][1 + i * 2].update(Color.white);
            this.pos[2][i * 2].update(Color.white);
            this.pos[5][1 + i * 2].update(Color.black);
            this.pos[6][i * 2].update(Color.black);
            this.pos[7][1 + i * 2].update(Color.black);
        }
        click.update(pos, color);
    }
    public void draw(Graphics g) {
        click.draw(g);
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                pos[i][j].draw(g);
    }

    public void update(int y, int x) {
        if (click.is_available(x, y)) {
            switchUser();
            click.run(x, y, pos, color);
        } else {
            // update click
            if (pos[x][y].isBusy() && pos[x][y].getColor() == color) 
                click.update(x, y, pos);
            else
                click.update(pos, color);
        }
    }

    public boolean checkEnd() {
        Integer white = 0;
        Integer black = 0;
        for (Position[] poss : pos)
            for (Position posss : poss)
                if (posss.isBusy()) {
                    if (posss.isWhite())
                        ++white;
                    else
                        ++black;
                }
        return white == 0 || black == 0;
    }
}

public class Checker extends Applet 
    implements KeyListener, MouseListener {  
    
    Action act;
    Image final_image;

    public void init() {
        act = new Action();
        addKeyListener( this );
        addMouseListener( this );
        final_image = Toolkit.getDefaultToolkit().getImage("/home/mark/Study/8/checkers/black.gif");

    }
    public void start() {
    }
    public void stop() {
    }
    public void destroy() {
    }

    private void paintField(Graphics g) {
        g.setColor(Color.black);  
        g.drawRect(10, 10, 800, 800);
        for (int i = 0; i < 7; ++i) {
            g.drawLine(110 + i * 100, 10, 110 + i * 100, 810);
            g.drawLine(10, 110 + i * 100, 810, 110 + i * 100);
        }
    }
    public void paint(Graphics g) {  
        if (act.checkEnd()) 
            g.drawImage(final_image, 0, 0, getWidth(), getHeight(), this);  
        else {
        paintField(g);
        act.draw(g);
        }
        super.paint(g);
    }  

    public void keyPressed( KeyEvent e ) { }
    public void keyReleased( KeyEvent e ) { }
    public void keyTyped( KeyEvent e ) {
       char c = e.getKeyChar();
       if ( c != KeyEvent.CHAR_UNDEFINED ) {
        // action
          repaint();
          e.consume();
       }
    }
 
    public void mouseEntered( MouseEvent e ) { }
    public void mouseExited( MouseEvent e ) { }
    public void mousePressed( MouseEvent e ) { }
    public void mouseReleased( MouseEvent e ) { }
    public void mouseClicked( MouseEvent e ) {
        int x = e.getX();
        int y = e.getY();
        act.update((x - 10) / 100, (y - 10) / 100);
        repaint();
        e.consume();
    }
  
}  

/* 
<applet code="Checker.class" width="300" height="300"> 
</applet> 
*/  