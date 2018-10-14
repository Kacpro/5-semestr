package main;


import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

class Buffer
{
    private Integer size;
    private Integer numberOfElements;

    private final Lock lock = new ReentrantLock();
    private final Condition notFull = lock.newCondition();
    private final Condition notEmpty = lock.newCondition();

    private Double avgConsumerTime;
    private Double avgProducerTime;
    private Integer numberOfConsumptions;
    private Integer numberOfProductions;

    public Buffer(Integer size)
    {
        this.size = size;
        this.numberOfElements = 0;
        this.numberOfConsumptions = 0;
        this.numberOfProductions = 0;
        this.avgConsumerTime = 0.0;
        this.avgProducerTime = 0.0;
    }

    public void putElements(Integer numberOfElements) throws InterruptedException
    {
        lock.lock();
        Long start = System.nanoTime();
        while (numberOfElements + this.numberOfElements > size)
        {
            notFull.await();
        }
        Long end = System.nanoTime();
        avgProducerTime = (avgProducerTime * numberOfProductions + (end - start))/(numberOfProductions + 1);
        numberOfProductions++;
        this.numberOfElements += numberOfElements;
        notEmpty.signalAll();
        lock.unlock();
    }

    public void takeElements(Integer numberOfElements) throws InterruptedException
    {
        lock.lock();
        Long start = System.nanoTime();
        while (this.numberOfElements < numberOfElements)
        {
            notEmpty.await();
        }
        Long end = System.nanoTime();
        avgConsumerTime = (avgConsumerTime * numberOfConsumptions + (end - start))/(numberOfConsumptions + 1);
        numberOfConsumptions++;
        this.numberOfElements -= numberOfElements;
        notFull.signalAll();
        lock.unlock();
    }

    public void printAvgs()
    {
        System.out.println("Consumer avg: " + avgConsumerTime);
        System.out.println("Producer avg: " + avgProducerTime);
        avgProducerTime = 0.0;
        avgConsumerTime = 0.0;
        numberOfProductions = 0;
        numberOfConsumptions = 0;
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
            Integer numberOfElements = ThreadLocalRandom.current().nextInt(1, maxNumberOfElements);
            try
            {
//                System.out.println("Taking: " + numberOfElements);
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
            Integer numberOfElements = ThreadLocalRandom.current().nextInt(1, maxNumberOfElements);
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

        Integer numberOfConsumers = 10;
        Integer numberOfProducers = 10;
        Integer bufferSize = 20;

        Buffer buffer = new Buffer(bufferSize);

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
            Thread.sleep(1000);
            buffer.printAvgs();
        }

    }
}
