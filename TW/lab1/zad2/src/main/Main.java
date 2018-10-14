package main;

class Tester
{
    class Printer
    {
        public Integer expected;

        public Printer(Integer expected)
        {
            this.expected = expected;
        }

        public void print(Integer value)
        {
            System.out.println(value);
        }
    }

    class PrinterThread extends Thread
    {
        private Integer value;
        private Printer printer;

        public PrinterThread(Integer value, Printer printer)
        {
            this.value = value;
            this.printer = printer;
        }

        public void run()
        {
            for (int i=0; i<10; i++)
            {
                synchronized (printer)
                {
                    while (!printer.expected.equals(value))
                    {
                        try
                        {
                            printer.wait();
                        }
                        catch (InterruptedException e)
                        {
                            e.printStackTrace();
                        }
                    }
                    printer.print(value);
                    printer.expected = (value % 3) + 1;
                    printer.notifyAll();
                }
            }
        }
    }

    public void runTests()
    {
        Printer printer = new Printer(1);
        Thread onePrinter = new PrinterThread(1, printer);
        Thread twoPrinter = new PrinterThread(2, printer);
        Thread threePrinter = new PrinterThread(3, printer);

        onePrinter.start();
        twoPrinter.start();
        threePrinter.start();

        try
        {
            onePrinter.join();
            twoPrinter.join();
            threePrinter.join();
        }
        catch (InterruptedException e)
        {
            e.printStackTrace();
        }

    }
}

public class Main
{
    public static void main(String[] args)
    {
        Tester tester = new Tester();
        tester.runTests();
    }
}
