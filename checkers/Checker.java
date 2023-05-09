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

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.net.DatagramSocket;
import java.net.ServerSocket;

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
    private Color color;

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
    public boolean isWhite() {
        return color == Color.white;
    }
}

class Chooser {
    public Image chooser;
    private Integer width;
    private Integer height;

    public Chooser(Integer wd, Integer he) {
        chooser = Toolkit.getDefaultToolkit().getImage("./twoPC1.gif");
        width = wd;
        height = he;
    }
    public void draw(Graphics g, Checker checker) {
        g.drawImage(chooser, 0, 100, width, height / 4, checker);  
        g.setColor(Color.red);
        g.drawRect(150, 400, 200, 100);
        g.drawRect(450, 400, 200, 100);
        g.setColor(Color.black);
        g.fillRect(151, 401, 199, 99);
        g.fillRect(451, 401, 199, 99);
        g.setColor(Color.white);
        g.setFont(new Font("Arial", Font.BOLD, 40));
        g.drawString("Server", 175, 460);
        g.drawString("Client", 485, 460);
    }
    public boolean isServer(int x, int y) {
        return x > 150 && x < 350 
            && y > 400 && y < 500;
    }
    public boolean isClient(int x, int y) {
        return x > 450 && x < 650 
            && y > 400 && y < 500;
    }
}

class Port {
    public Image chooser;
    private Integer width;
    private Integer height;
    private boolean isServer;
    private String ip;
    private String port;
    private boolean printIp;

    public Port(Integer wd, Integer he, boolean isServer2) {
        chooser = Toolkit.getDefaultToolkit().getImage("./twoPC1.gif");
        width = wd;
        height = he;
        this.isServer = isServer2;
        ip = isServer2 ? getIp() : "";
        port = isServer2 ? "11000" : "";
        printIp = true;
    }
    public String getIP() {
        return ip;
    }
    public String getPort() {
        return port;
    }
    private String getIp () {
        String out = "ERROR";
        try (final DatagramSocket datagramSocket = new DatagramSocket()) {
            datagramSocket.connect(InetAddress.getByName("8.8.8.8"), 12345);
            out = datagramSocket.getLocalAddress().getHostAddress();
        }
        catch (UnknownHostException e) {
            System.out.println(e.getMessage());
        }
        catch (IOException e) {
            System.out.println(e.getMessage());
        }
        return out;
    }
    public void draw(Graphics g, Checker checker) {
        g.drawImage(chooser, 0, 100, width, height / 4, checker);  
        if (isServer)
            drawServer(g);
        else
            drawClient(g);
    }
    public void drawServer(Graphics g) {
        g.setColor(Color.red);
        g.drawRect(150, 400, 200, 100);
        g.drawRect(450, 400, 200, 100);
        g.drawRect(50, 600, 400, 100);
        g.drawRect(550, 600, 200, 100);
        g.setColor(Color.black);
        g.fillRect(151, 401, 199, 99);
        g.fillRect(451, 401, 199, 99);
        g.fillRect(51, 601, 399, 99);
        g.fillRect(551, 601, 199, 99);
        g.setColor(Color.white);
        g.setFont(new Font("Arial", Font.BOLD, 40));
        g.drawString("IP", 225, 460);
        g.drawString("Port", 500, 460);
        g.drawString(ip, 100, 660);
        g.drawString(port, 580, 660);
    }
    public void drawClient(Graphics g) {
        g.setColor(Color.red);
        g.drawRect(150, 400, 200, 100);
        g.drawRect(450, 400, 200, 100);
        g.drawRect(50, 600, 400, 100);
        g.drawRect(550, 600, 200, 100);
        g.setColor(Color.black);
        g.fillRect(151, 401, 199, 99);
        g.fillRect(451, 401, 199, 99);
        g.fillRect(51, 601, 399, 99);
        g.fillRect(551, 601, 199, 99);
        g.setColor(Color.white);
        g.setFont(new Font("Arial", Font.BOLD, 40));
        g.drawString("IP", 225, 460);
        g.drawString("Port", 500, 460);
        g.drawString(ip, 100, 660);
        g.drawString(port, 580, 660);
    }
    public boolean update(char sym) {
        if (!isServer) {
            if (printIp) {
                switch(sym) {
                    case 10: // Enter
                        printIp = false;
                        break;
                    case 8: // Backspace
                        ip = ip.length() == 0 ?
                            "" :
                            ip.substring(0, ip.length() - 1);
                        break;
                    default:
                        ip += sym;
                        break;
                }
            } else {
                switch(sym) {
                    case 10: // Enter
                        return true;
                    case 8: // Backspace
                        port = port.length() == 0 ?
                            "" :
                            port.substring(0, port.length() - 1);
                        break;
                    default:
                        port += sym;
                        break;
                }
            }
        }
        return false;
    }
}

class Sock {
    private Socket socket;
    private ObjectInputStream ois;
    private ObjectOutputStream oos;
    private ServerSocket server;
    private boolean is_server;
    public boolean isServer() {
        return is_server;
    }
    public Sock (String ip, Integer port, boolean is_server) {
        if (is_server) {
            try {
                server = new ServerSocket(port);
                socket = server.accept();
            } catch (IOException e) {
                System.out.println("Creating sock server : " + e.getMessage());
            }
        } else { // is_client
            try {
                socket = new Socket(ip, port);
            } catch (IOException e) {
                System.out.println("Creating sock client : " + e.getMessage());
            } 
        }
        System.out.println("Created sockets");
        this.is_server = is_server;
    }
    public Integer readData() {
        Integer message = 0;
        try {
            ois = new ObjectInputStream(socket.getInputStream());
            message = (Integer) ois.readObject();
            System.out.println("Message: " + message);
        } catch (IOException | ClassNotFoundException e) {
            System.out.println("Can't get " + e.getMessage());
        }
        return message;
    }
    public void sendData(Integer data) {
        try {
            oos = new ObjectOutputStream(socket.getOutputStream());
            oos.writeObject(data);
            System.out.println("Printing: " + data);
        } catch (IOException e) {
            System.out.println("Can't get " + e.getMessage());
        }
    }
}

enum Stage {
    CHOOSER, 
    PORT,
    GAME,
    END
}

public class Checker extends Applet 
    implements KeyListener, MouseListener {  
    
    Action act;
    Image final_image;
    Chooser chooser;
    Port port;
    Stage stage;
    Sock socket;

    public void init() {
        stage = Stage.CHOOSER;
        act = new Action();
        addKeyListener( this );
        addMouseListener( this );
        final_image = Toolkit.getDefaultToolkit().getImage("./black.gif");
        chooser = new Chooser(getWidth(), getHeight());
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
        switch(stage) {
        case CHOOSER:
            chooser.draw(g, this);
            break;
        case PORT:
            port.draw(g, this);
            break;
        case GAME:
            if (act.checkEnd()) 
                g.drawImage(final_image, 0, 0, getWidth(), getHeight(), this);  
            else {
                paintField(g);
                act.draw(g);
                if (act.isWhite() ^ socket.isServer()) {
                    System.out.println("Wait for");
                    Integer data = socket.readData();
                    act.update(data / 10, data % 10);
                }
            }
            break;
        default:
            break;
        }
        super.paint(g);
    }  

    public void keyPressed( KeyEvent e ) { }
    public void keyReleased( KeyEvent e ) { }
    public void keyTyped( KeyEvent e ) {
       char c = e.getKeyChar();
       if ( c != KeyEvent.CHAR_UNDEFINED ) {
            processKey(c);
            repaint();
            e.consume();
       }
    }
    private void processKey(char sym) {
        switch (stage) {
            case PORT:
                if (port.update(sym)) {
                    socket = new Sock(port.getIP(), Integer.parseInt(port.getPort()), false);
                    stage = Stage.GAME;
                }
                break;
            default:
                break;
        }
    }
 
    public void mouseEntered( MouseEvent e ) { }
    public void mouseExited( MouseEvent e ) { }
    public void mousePressed( MouseEvent e ) { }
    public void mouseReleased( MouseEvent e ) { }
    public void mouseClicked( MouseEvent e ) {
        int x = e.getX();
        int y = e.getY();
        processMouse(x, y);
        repaint();
        e.consume();
    }

    private void processMouse(int x, int y) {
        switch (stage) {
            case CHOOSER:
                if (chooser.isServer(x, y)) {
                    port = new Port(getWidth(), getHeight(), true);
                    port.draw(getGraphics(), this);
                    socket = new Sock(port.getIP(), Integer.parseInt(port.getPort()), true);
                    stage = Stage.GAME;
                }
                else if (chooser.isClient(x, y)) {
                    port = new Port(getWidth(), getHeight(), false);
                    stage = Stage.PORT;
                }
                break;
            case GAME:
                if (!(act.isWhite() ^ socket.isServer())) {
                    System.out.println("Sending");
                    act.update((x - 10) / 100, (y - 10) / 100);
                    socket.sendData((x - 10) / 100 * 10 + (y - 10) / 100);
                }
                break;
            default:
                break;
        }
    }
  
  
}  

/* 
<applet code="Checker.class" width="300" height="300"> 
</applet> 
*/  