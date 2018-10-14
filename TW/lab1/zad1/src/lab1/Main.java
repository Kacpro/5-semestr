package lab1;

class Counter
{
    private int value;

    public void incValue()
    {
        value++;
    }

    public void decValue()
    {
        value--;
    }

    public int getValue()
    {
        return value;
    }
}


class Test {

    class IncThread extends Thread
    {
        Counter counter;

        public IncThread(Counter counter)
        {
            this.counter = counter;
        }

        public void run()
        {
            for (int i=0; i<100000000; i++)
            {
                synchronized(counter)
                {
                    counter.incValue();
                }
            }
        }
    }

    class DecThread extends Thread
    {
        Counter counter;

        public DecThread(Counter counter)
        {
            this.counter = counter;
        }

        public void run()
        {
            for (int i=0; i<100000000; i++)
            {
                synchronized (counter)
                {
                    counter.decValue();
                }
            }
        }
    }

    public void runTest() throws InterruptedException
    {
        Counter counter = new Counter();
        Thread incThread = new IncThread(counter);
        Thread decThread = new DecThread(counter);

        incThread.start();
        decThread.start();

        incThread.join();
        decThread.join();

        System.out.println(counter.getValue());
    }

}

public class Main
{
    public static void main(String[] args) throws InterruptedException
    {
        Test tester = new Test();
        tester.runTest();
    }
}