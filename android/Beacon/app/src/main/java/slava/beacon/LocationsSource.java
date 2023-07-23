package slava.beacon;

import android.content.ContextWrapper;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.Looper;

import java.util.Timer;
import java.util.TimerTask;

//import static slava.beacon.Constants.*;

public class LocationsSource implements LocationListener, Runnable
{
	private int m_timeout;
	private Thread m_thread;
	private Looper m_looper;
	private Timer m_timer;
	private LocationsQueue m_queue;
	private LocationManager m_location_manager;
	private LocationSourceListener m_listener;
	private final static int LOCATIONS_QUEUE_CAPACITY = 10;

	private class LocationsQueue
	{
		private Location[] m_locations;
		private int m_top_index;
		private int m_capacity;
		private int m_bottom_index;
		private int m_size;
		public LocationsQueue(int capacity)
		{
			if (capacity > 0) m_locations = new Location[capacity]; else m_locations = null;
			m_capacity = capacity;
			m_size = 0;
			m_top_index = -1;
			m_bottom_index = -1;
		}

		public synchronized void offer(Location location)
		{
			if (m_size < m_capacity) m_size++;
			m_top_index = (m_top_index + 1) % m_capacity;
			m_bottom_index = (m_top_index - m_size + m_capacity + 1) % m_capacity;
			m_locations[m_top_index] = location;
		}

		public synchronized Location poll()
		{
			if (m_size > 0)
			{
				Location l = m_locations[m_bottom_index];
				m_locations[m_bottom_index] = null;
				m_size--;
				if (m_size > 0)
				{
					m_bottom_index = (m_bottom_index + 1) % m_capacity;
				}
				else
				{
					m_size = 0;
					m_top_index = -1;
					m_bottom_index = -1;
				}
				return l;
			}
			return null;
		}

		public synchronized void clear()
		{
			m_size = 0;
			m_top_index = -1;
			m_bottom_index = -1;
			for (int i = 0; i < m_locations.length; i++) m_locations[i] = null;
		}
	}

	public LocationsSource(ContextWrapper context)
	{
		m_timeout = 15;
		m_listener = (LocationSourceListener)context;
		m_location_manager = (LocationManager)context.getSystemService(ContextWrapper.LOCATION_SERVICE);
		m_thread = null;
		m_looper = null;
		m_timer = null;
		m_queue = new LocationsQueue(LOCATIONS_QUEUE_CAPACITY);
	}

	public boolean isProviderEnabled()
	{
		return m_location_manager != null && m_location_manager.isProviderEnabled(LocationManager.GPS_PROVIDER);
	}

	public void pause()
	{
		if (m_timer != null)
		{
			m_timer.cancel();
			m_timer.purge();
			m_timer = null;
			m_queue.clear();
		}
	}

	public void resume()
	{
		if (m_timer == null)
		{
			long t = m_timeout * 1000;
			m_timer = new Timer();
			m_timer.schedule(new TimerTask()
			{
				@Override
				public void run()
				{
					Location location = m_queue.poll();
					boolean ok = location != null;
					if (!ok) location = m_location_manager.getLastKnownLocation(LocationManager.GPS_PROVIDER);
					/*if (location != null)
					{
						Log.i(LOG_TAG, "Location source: send location(" + location.getLongitude() + "; " + location.getLatitude() + "; " + location.getSpeed() + ")");
					}
					else
					{
						Log.i(LOG_TAG, "Location source: unknown location");
					}*/
					m_listener.onLocationChanged(location, ok);
				}
			}, t, t);
		}
	}

	public void start(int timeout)
	{
		//Log.i(LOG_TAG, "Location source: start.");
		if (m_location_manager != null)
		{
			m_timeout = timeout;
			m_thread = new Thread(this);
			m_thread.start();
		}
	}

	public void stop()
	{
		//Log.i(LOG_TAG, "Location source: stop.");
		if (m_location_manager != null) m_location_manager.removeUpdates(this);
		if (m_timer != null)
		{
			m_timer.cancel();
			m_timer.purge();
			m_timer = null;
			//Log.i(LOG_TAG, "Location source: timer stop.");
		}
		if (m_looper != null)
		{
			m_looper.quit();
			m_looper = null;
			//Log.i(LOG_TAG, "Location source: looper stop.");
		}
		if (m_thread != null)
		{
			if (m_thread.isAlive())
			{
				try
				{
					m_thread.join();
				}
				catch (InterruptedException ignored){}
			}
			m_thread = null;
			//Log.i(LOG_TAG, "Location source: thread stop.");
		}
	}

	public boolean isActive()
	{
		return m_looper != null;
	}

	public boolean isPaused()
	{
		return m_timer == null;
	}

	@Override
	public void onLocationChanged(Location location)
	{
		m_queue.offer(location);
	}

	@Override
	public void onStatusChanged(String provider, int status, Bundle extras){}

	@Override
	public void onProviderEnabled(String provider)
	{
		//Log.i(LOG_TAG, "Location source: " + provider + " enabled");
		resume();
		m_listener.onLocationProviderEnabled();
	}

	@Override
	public void onProviderDisabled(String provider)
	{
		//Log.i(LOG_TAG, "Location source: " + provider + " disabled");
		pause();
		m_listener.onLocationProviderDisabled();
	}

	@Override
	public void run()
	{
		//Log.i(LOG_TAG, "Location source: start GPS loop.");
		Looper.prepare();
		m_queue.clear();
		m_looper = Looper.myLooper();
		m_location_manager.requestLocationUpdates(LocationManager.GPS_PROVIDER, m_timeout * 1000, 0, this, m_looper);
		Looper.loop();
		m_looper = null;
		m_queue.clear();
		//Log.i(LOG_TAG, "Location source: finish GPS loop.");
	}
}
