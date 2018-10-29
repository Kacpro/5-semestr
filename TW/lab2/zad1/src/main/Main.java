package main;

import java.util.LinkedList;
import java.util.List;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;


abstract class Buffer
{
    protected Integer size;
    protected Integer numberOfElements;

    protected final Lock lock = new ReentrantLock();

    protected SortedMap<Integer, Long> consumerTimes;
    protected SortedMap<Integer, Long> producerTimes;
    protected List<Integer> numberOfConsumptions;
    protected List<Integer> numberOfProductions;

    protected Integer DIV = 100;


    public void printAvgs()
    {
        for (int i = 0; i<(size/2)/ DIV; i++)
        {
            long buf = consumerTimes.get(i) == null ? 0 : consumerTimes.get(i);
            System.out.print((buf / numberOfConsumptions.get(i)) + " ");
        }
        for (int i = 0; i<(size/2)/ DIV; i++)
        {
            long buf = producerTimes.get(i) == null ? 0 : producerTimes.get(i);
            System.out.print((buf / numberOfProductions.get(i)) + " ");
        }
        System.out.println();
    }

    public abstract void putElements(Integer numberOfElements) throws InterruptedException;
    public abstract void takeElements(Integer numberOfElements) throws InterruptedException;
}



class Buffer1 extends Buffer
{
    private final Condition notFull = lock.newCondition();
    private final Condition notEmpty = lock.newCondition();


    public Buffer1(Integer size)
    {
        this.size = size;
        this.numberOfElements = 0;

        consumerTimes = new TreeMap<>();
        producerTimes = new TreeMap<>();
        numberOfProductions = new LinkedList<>();
        numberOfConsumptions = new LinkedList<>();

        for (int i = 0; i<(size/2)/ DIV; i++)
        {
            numberOfProductions.add(1);
            numberOfConsumptions.add(1);
        }
    }

    public void putElements(Integer numberOfElements) throws InterruptedException
    {
        lock.lock();
        numberOfConsumptions.set((numberOfElements - 1) / DIV, numberOfConsumptions.get((numberOfElements - 1) / DIV) != null ? (numberOfConsumptions.get((numberOfElements - 1) / DIV) + 1) : 1);
        Long start = System.nanoTime();
        while (numberOfElements + this.numberOfElements > size)
        {

            notFull.await();
        }
        Long end = System.nanoTime();
        this.numberOfElements += numberOfElements;
        notEmpty.signalAll();
        lock.unlock();

        producerTimes.merge((numberOfElements - 1) / DIV, end - start, (a, b) -> a+b);
    }

    public void takeElements(Integer numberOfElements) throws InterruptedException
    {
        lock.lock();
        numberOfProductions.set((numberOfElements - 1) / DIV, numberOfProductions.get((numberOfElements-1) / DIV) != null ? (numberOfProductions.get((numberOfElements - 1) / DIV) + 1) : 1);
        Long start = System.nanoTime();
        while (this.numberOfElements < numberOfElements)
        {
            notEmpty.await();
        }
        Long end = System.nanoTime();

        this.numberOfElements -= numberOfElements;
        notFull.signalAll();
        lock.unlock();

        consumerTimes.merge((numberOfElements - 1) / DIV, end - start, (a, b) -> a+b);
    }
}



class Buffer2 extends Buffer
{
    private final Condition firstProd = lock.newCondition();
    private final Condition firstCons = lock.newCondition();
    private final Condition restProd = lock.newCondition();
    private final Condition restCons = lock.newCondition();

    private boolean firstProdEmpty;
    private boolean firstConsEmpty;


    public Buffer2(Integer size)
    {
        this.size = size;
        this.numberOfElements = 0;

        consumerTimes = new TreeMap<>();
        producerTimes = new TreeMap<>();
        numberOfProductions = new LinkedList<>();
        numberOfConsumptions = new LinkedList<>();
        firstProdEmpty = true;
        firstConsEmpty = true;

        for (int i = 0; i<(size/2)/ DIV; i++)
        {
            numberOfProductions.add(1);
            numberOfConsumptions.add(1);
        }
    }

    public void putElements(Integer numberOfElements) throws InterruptedException {
        lock.lock();
        numberOfProductions.set((numberOfElements - 1) / DIV, numberOfProductions.get((numberOfElements - 1) / DIV) != null ? (numberOfProductions.get((numberOfElements - 1) / DIV) + 1) : 1);
        Long start = System.nanoTime();
        if (!firstProdEmpty) restProd.await();
        while (numberOfElements + this.numberOfElements > size)
        {
            firstProdEmpty = false;
            firstProd.await();
            firstProdEmpty = true;
        }
        Long end = System.nanoTime();
        this.numberOfElements += numberOfElements;

        restProd.signal();
        firstCons.signal();

        lock.unlock();
        producerTimes.merge((numberOfElements - 1) / DIV, end - start, (a, b) -> a+b);
    }

    public void takeElements(Integer numberOfElements) throws InterruptedException
    {
        lock.lock();
        numberOfConsumptions.set((numberOfElements - 1) / DIV, numberOfConsumptions.get((numberOfElements-1) / DIV) != null ? (numberOfConsumptions.get((numberOfElements - 1) / DIV) + 1) : 1);
        Long start = System.nanoTime();

        if (!firstConsEmpty) restCons.await();
        while (this.numberOfElements < numberOfElements)
        {
            firstConsEmpty = false;
            firstCons.await();
            firstConsEmpty = true;
        }
        Long end = System.nanoTime();
        this.numberOfElements -= numberOfElements;

        restCons.signal();
        firstProd.signal();

        lock.unlock();
        consumerTimes.merge((numberOfElements - 1) / DIV, end - start, (a, b) -> a+b);
    }
}



class Consumer extends Thread
{
    private Integer maxNumberOfElements;
    private Buffer buffer;

    public Consumer(Integer maxNumberOfElements, Buffer buffer)
    {
        this.maxNumberOfElements = maxNumberOfElements;
        this.buffer = buffer;
    }

    public void run()
    {
        while (true)
        {
            Integer numberOfElements = ThreadLocalRandom.current().nextInt(1, maxNumberOfElements+1);
//            Integer numberOfElements = (int)Math.floor(Math.pow(ThreadLocalRandom.current().nextDouble(), 4) * maxNumberOfElements + 1);
            try
            {
                buffer.takeElements(numberOfElements);
            }
            catch (InterruptedException e)
            {
                e.printStackTrace();
            }
        }
    }
}


class Producer extends Thread
{
    private Integer maxNumberOfElements;
    private Buffer buffer;

    public Producer(Integer maxNumberOfElements, Buffer buffer)
    {
        this.maxNumberOfElements = maxNumberOfElements;
        this.buffer = buffer;
    }

    public void run()
    {
        while (true)
        {
           Integer numberOfElements = ThreadLocalRandom.current().nextInt(1, maxNumberOfElements+1);
//            Integer numberOfElements = (int)Math.floor(Math.pow(ThreadLocalRandom.current().nextDouble(), 4) * maxNumberOfElements + 1);
            try
            {
                buffer.putElements(numberOfElements);
            }
            catch (InterruptedException e)
            {
                e.printStackTrace();
            }
        }
    }
}




public class Main
{
    public static void main(String[] args) throws InterruptedException {
        List<Thread> threads = new LinkedList<>();

        Integer numberOfConsumers = 1000;
        Integer numberOfProducers = 1000;
        Integer bufferSize = 20000;

        Buffer1 buffer = new Buffer1(bufferSize);

        for (int i=0; i<numberOfConsumers; i++)
        {
            threads.add(new Consumer(bufferSize/2, buffer));
        }

        for (int i=0; i<numberOfProducers; i++)
        {
            threads.add(new Producer(bufferSize/2, buffer));
        }

        for (Thread thread : threads)
        {
            thread.start();
        }

        while (true)
        {
            Thread.sleep(5000);
            buffer.printAvgs();
        }

    }
}
