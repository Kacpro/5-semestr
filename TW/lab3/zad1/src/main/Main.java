package main;


import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

class Mandelbrot extends JFrame {

    private BufferedImage I;

    public Mandelbrot() {
        super("Mandelbrot Set");
        List<Callable<Integer>> tasks = new LinkedList<>();

        ExecutorService executor = Executors.newWorkStealingPool(16);
        setBounds(100, 100, 800, 600);
        setResizable(false);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        I = new BufferedImage(getWidth(), getHeight(), BufferedImage.TYPE_INT_RGB);

        long start = System.nanoTime();

        for (int y = 0; y < getHeight(); y++) {
            for (int x = 0; x < getWidth(); x++) {
                tasks.add(new Calculator(x, y, I));
            }
        }

        try
        {
            executor.invokeAll(tasks);
            long end = System.nanoTime();
            System.out.println((end - start)/1e9);
        }
        catch (InterruptedException e)
        {
            e.printStackTrace();
        }
    }


    @Override
    public void paint(Graphics g) {
        g.drawImage(I, 0, 0, this);
    }

    public static void main() {
        new Mandelbrot().setVisible(true);
    }
}


class Calculator implements Callable<Integer>
{
    private double zx, zy, cX, cY, tmp;
    private final int MAX_ITER = 20000;
    private final double ZOOM = 150;
    private BufferedImage I;
    private int x, y;

    public Calculator(int x, int y, BufferedImage I)
    {
        this.x = x;
        this.y = y;
        this.I = I;
    }

    @Override
    public Integer call() {
        zx = zy = 0;
        cX = (x - 400) / ZOOM;
        cY = (y - 300) / ZOOM;
        int iter = MAX_ITER;
        while (zx * zx + zy * zy < 4 && iter > 0) {
            tmp = zx * zx - zy * zy + cX;
            zy = 2.0 * zx * zy + cY;
            zx = tmp;
            iter--;
        }
        I.setRGB(x, y, iter | (iter << 8));


        return 0;
    }
}


public class Main
{
    public static void main(String[] args){
        Mandelbrot.main();
    }
}
