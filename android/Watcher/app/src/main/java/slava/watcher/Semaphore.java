package slava.watcher;

public class Semaphore
{
	private int count = 0;
	public synchronized void lock()
	{
		if (count <= 0)
		{
			count = 0;
			try
			{
				wait();
			}
			catch (InterruptedException ignored){}
		}
		else count--;
	}

	public synchronized void signal()
	{
		count++;
		notify();
	}
}
