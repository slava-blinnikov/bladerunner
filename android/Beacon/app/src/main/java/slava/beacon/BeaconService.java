package slava.beacon;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.location.Location;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Binder;
import android.os.Bundle;
import android.os.Environment;
import android.os.IBinder;
import android.preference.PreferenceManager;
import android.support.v7.app.NotificationCompat;
import android.telephony.TelephonyManager;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;

import static slava.beacon.Constants.*;

public class BeaconService extends Service implements LocationSourceListener, BeaconStateListener
{
	private LocationsSource m_locations_source = null;
	private Beacon m_beacon = null;
	private BroadcastReceiver m_receiver = null;
	private BeaconServiceBinder m_binder;
	private int m_tracking_timeout;
	private boolean m_gps_enabled;
	private boolean m_network_enabled;
	private boolean m_phone_idle;
	private ArrayList<String> m_watchers = null;

	public class BeaconServiceBinder extends Binder
	{
		BeaconService getService()
		{
			return BeaconService.this;
		}
	}

	@Override
	public IBinder onBind(Intent intent)
	{
		return m_binder;
	}

	@Override
	public void onCreate()
	{
		Log.i(LOG_TAG, "Service: onCreate");
		m_binder = new BeaconServiceBinder();
		m_watchers = new ArrayList<>();
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId)
	{
		Log.i(LOG_TAG, "Service: onStartCommand");
		if (m_beacon != null)
		{
			Log.i(LOG_TAG, "Service: beacon already runs. try to stop...");
			m_beacon.stop();
			m_beacon = null;
			m_locations_source.stop();
			m_locations_source = null;
			m_watchers.clear();
			Log.i(LOG_TAG, "Service: success.");
		}
		m_receiver = null;
		m_tracking_timeout = 15;
		m_gps_enabled = false;
		m_network_enabled = false;
		m_phone_idle = true;
		Resources res = getResources();
		SharedPreferences prefs = getSharedPreferences(APP_PREFERENCES, MODE_PRIVATE);
		String host = prefs.getString(res.getString(R.string.server_host_preference_key), res.getString(R.string.text_default_host));
		int port = prefs.getInt(res.getString(R.string.server_port_preference_key), Integer.parseInt(res.getString(R.string.text_default_port)));
		String crew = prefs.getString(res.getString(R.string.crew_name_preference_key), null);
		String member = prefs.getString(res.getString(R.string.member_name_preference_key), null);
		String password = prefs.getString(res.getString(R.string.member_password_preference_key), null);
		int ava_changetime = prefs.getInt(AVA_CHANGETIME_PREFERENCE_KEY, 0);
		if (crew != null && member != null && password != null)
		{
			m_locations_source = new LocationsSource(this);
			m_beacon = new Beacon(this);
			m_beacon.setAddress(host, port);
			m_beacon.setAuthorization(crew, member, password);
			m_beacon.setAvaChangetime(ava_changetime);
			m_receiver = new BroadcastReceiver()
			{
				@Override
				public void onReceive(Context context, Intent intent)
				{
					String action = intent.getAction();
					if (action != null)
					{
						if (action.equals(TelephonyManager.ACTION_PHONE_STATE_CHANGED))
						{
							Bundle extras = intent.getExtras();
							if (extras != null)
							{
								String state = extras.getString(TelephonyManager.EXTRA_STATE);
								if (state != null)
								{
									if (state.equals(TelephonyManager.EXTRA_STATE_IDLE))
									{
										onPhoneIdle();
									} else
									{
										onPhoneCalling();
									}
								}
							}
						} else if (action.equals(ConnectivityManager.CONNECTIVITY_ACTION))
						{
							ConnectivityManager connectivityManager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
							NetworkInfo activeNetInfo = connectivityManager.getActiveNetworkInfo();
							if (activeNetInfo != null && activeNetInfo.isConnected())
							{
								onNetworkEnabled();
							} else
							{
								onNetworkDisabled();
							}
						}
					}
				}
			};

			registerReceiver(m_receiver, new IntentFilter(TelephonyManager.ACTION_PHONE_STATE_CHANGED));
			registerReceiver(m_receiver, new IntentFilter(ConnectivityManager.CONNECTIVITY_ACTION));
			m_gps_enabled = m_locations_source.isProviderEnabled();
			ConnectivityManager connectivityManager = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
			NetworkInfo activeNetInfo = connectivityManager.getActiveNetworkInfo();
			m_network_enabled = activeNetInfo != null && activeNetInfo.isConnected();
			if (m_network_enabled && m_gps_enabled && m_phone_idle)
			{
				Log.i(LOG_TAG, "Service: start beacon.");
				m_beacon.start();
			}

			Intent notification_intent = new Intent(this, MainActivity.class);
			PendingIntent content_intent = PendingIntent.getActivity(this, 0, notification_intent, PendingIntent.FLAG_CANCEL_CURRENT);
			NotificationCompat.Builder notification_builder = (NotificationCompat.Builder) new NotificationCompat.Builder(this)
					.setContentIntent(content_intent)
					.setSmallIcon(R.drawable.beacon_off)
					.setContentTitle(getString(R.string.app_name))
					.setContentText(getString(R.string.watching_finished_caption))
					.setAutoCancel(true);
			startForeground(777, notification_builder.build());

			Intent i = new Intent(SERVICE_STATE_BROADCAST_ACTION);
			i.putExtra(PARAM_SERVICE_STATE_EVENT, SERVICE_EVENT_START);
			sendBroadcast(i);
		}
		else stopSelf();
		return Service.START_STICKY;
	}

	@Override
	public void onDestroy()
	{
		Log.i(LOG_TAG, "Service: onDestroy");
		if (m_receiver != null)
		{
			unregisterReceiver(m_receiver);
			m_receiver = null;
		}
		if (m_beacon != null)
		{
			m_beacon.stop();
			m_beacon = null;
			m_locations_source.stop();
			m_locations_source = null;
		}
		m_watchers.clear();
		m_watchers = null;
		Intent i = new Intent(SERVICE_STATE_BROADCAST_ACTION);
		i.putExtra(PARAM_SERVICE_STATE_EVENT, SERVICE_EVENT_STOP);
		sendBroadcast(i);
	}

	@Override
	public void onLocationChanged(Location location, boolean ok)
	{
		double latitude, longitude, speed;
		if (location != null)
		{
			latitude = location.getLatitude();
			longitude = location.getLongitude();
			speed = location.getSpeed();
		}
		else
		{
			latitude = 0;
			longitude = 0;
			speed = 0;
		}
		m_beacon.sendLocation(latitude, longitude, speed, ok);
	}

	@Override
	public void onLocationProviderEnabled()
	{
		//Log.i(LOG_TAG, "Service: onLocationProviderEnabled");
		if (!m_gps_enabled)
		{
			m_gps_enabled = true;
			if (m_network_enabled && m_phone_idle) m_beacon.start();
		}
	}

	@Override
	public void onLocationProviderDisabled()
	{
		//Log.i(LOG_TAG, "Service: onLocationProviderDisabled");
		if (m_gps_enabled)
		{
			m_gps_enabled = false;
			m_beacon.stop();
		}
	}

	public void onPhoneCalling()
	{
		//Log.i(LOG_TAG, "Service: onPhoneCalling");
		if (m_phone_idle)
		{
			m_phone_idle = false;
			m_locations_source.stop();
			m_beacon.stop();
		}
	}

	public void onPhoneIdle()
	{
		//Log.i(LOG_TAG, "Service: onPhoneIdle");
		if (!m_phone_idle)
		{
			m_phone_idle = true;
			if (m_gps_enabled && m_network_enabled) m_beacon.start();
		}
	}

	public void onNetworkEnabled()
	{
		//Log.i(LOG_TAG, "Service: onNetworkEnabled");
		if (!m_network_enabled)
		{
			m_network_enabled = true;
			if (m_gps_enabled && m_phone_idle) m_beacon.start();
		}
	}

	public void onNetworkDisabled()
	{
		//Log.i(LOG_TAG, "Service: onNetworkDisabled");
		if (m_network_enabled)
		{
			m_network_enabled = false;
			m_beacon.stop();
		}
	}

	@Override
	public void connected(boolean tracking_permission, boolean recording_permission, int tracking_timeout)
	{
		m_watchers.clear();
		m_tracking_timeout = tracking_timeout;
		Intent intent = new Intent(SERVICE_STATE_BROADCAST_ACTION);
		intent.putExtra(PARAM_SERVICE_STATE_EVENT, SERVICE_EVENT_AUTHORIZED);
		intent.putExtra(PARAM_TRACKING_PERMISSION, tracking_permission);
		intent.putExtra(PARAM_RECORDING_PERMISSION, recording_permission);
		sendBroadcast(intent);
	}

	@Override
	public void configured(boolean tracking_permission, boolean recording_permission, int tracking_timeout)
	{
		if (m_tracking_timeout != tracking_timeout)
		{
			m_tracking_timeout = tracking_timeout;
			if (m_locations_source.isActive())
			{
				boolean ok = !m_locations_source.isPaused();
				m_locations_source.stop();
				m_locations_source.start(m_tracking_timeout);
				if (ok) m_locations_source.resume();
			}
		}
		Intent intent = new Intent(SERVICE_STATE_BROADCAST_ACTION);
		intent.putExtra(PARAM_SERVICE_STATE_EVENT, SERVICE_EVENT_CONFIG);
		intent.putExtra(PARAM_TRACKING_PERMISSION, tracking_permission);
		intent.putExtra(PARAM_RECORDING_PERMISSION, recording_permission);
		sendBroadcast(intent);
	}

	@Override
	public void avatarChanged(int change_time, byte[] data, int data_size)
	{
		boolean ok = false;
		if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED))
		{
			File sd_path = Environment.getExternalStorageDirectory();
			sd_path = new File(sd_path.getAbsolutePath() + APP_DATA_PATH);
			sd_path.mkdirs();
			if (sd_path.isDirectory())
			{
				File sd_file = new File(sd_path, AVATAR_FILENAME);
				try
				{
					FileOutputStream fout = new FileOutputStream(sd_file);
					fout.write(data, 9, data_size - 9);
					fout.close();
					SharedPreferences prefs = getSharedPreferences(APP_PREFERENCES, Service.MODE_PRIVATE);
					SharedPreferences.Editor editor = prefs.edit();
					editor.putInt(AVA_CHANGETIME_PREFERENCE_KEY, change_time);
					editor.apply();
					ok = true;
					//Log.d(LOG_TAG, "Service: Avatar saved on SD-card - " + sd_file.getAbsolutePath());
				}
				catch (IOException ignored){}
			}
		}
		else
		{
			try
			{
				FileOutputStream fout = openFileOutput(AVATAR_FILENAME, Service.MODE_PRIVATE);
				fout.write(data, 9, data_size - 9);
				fout.close();
				ok = true;
				//Log.d(LOG_TAG, "Service: Avatar saved on internal memory - " + AVATAR_FILENAME);
			} catch (IOException ignored){}
		}
		if (ok)
		{
			Intent intent = new Intent(SERVICE_STATE_BROADCAST_ACTION);
			intent.putExtra(PARAM_SERVICE_STATE_EVENT, SERVICE_EVENT_AVATAR);
			sendBroadcast(intent);
		}
	}

	@Override
	public void disconnected()
	{
		if (m_locations_source.isActive()) notifyOff();
		m_locations_source.stop();
		m_watchers.clear();
		Intent intent = new Intent(SERVICE_STATE_BROADCAST_ACTION);
		intent.putExtra(PARAM_SERVICE_STATE_EVENT, SERVICE_EVENT_DISCONNECTED);
		sendBroadcast(intent);
	}

	private void notifyOn()
	{
		m_locations_source.start(m_tracking_timeout);
		if (m_locations_source.isProviderEnabled()) m_locations_source.resume();

		Resources res = getResources();
		SharedPreferences sp = PreferenceManager.getDefaultSharedPreferences(this);
		boolean need_notify = sp.getBoolean(res.getString(R.string.start_beeping_notify_preference_key), false);
		if (need_notify)
		{
			String melody = sp.getString(res.getString(R.string.start_beeping_ringtone_preference_key), "");
			Intent notification_intent = new Intent(this, MainActivity.class);
			PendingIntent content_intent = PendingIntent.getActivity(this, 0, notification_intent, PendingIntent.FLAG_CANCEL_CURRENT);
			NotificationCompat.Builder notification_builder = (NotificationCompat.Builder) new NotificationCompat.Builder(this)
					.setContentIntent(content_intent)
					.setSmallIcon(R.drawable.beacon_on)
					.setContentTitle(getString(R.string.app_name))
					.setContentText(getString(R.string.watching_started_caption) + (new SimpleDateFormat(" HH:mm:ss").format(System.currentTimeMillis())))
					.setSound(Uri.parse(melody))
					.setAutoCancel(true);
			NotificationManager notification_manager = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
			notification_manager.notify(777, notification_builder.build());
		}
		Intent intent = new Intent(SERVICE_STATE_BROADCAST_ACTION);
		intent.putExtra(PARAM_SERVICE_STATE_EVENT, SERVICE_EVENT_START_BEEPING);
		sendBroadcast(intent);
	}

	private void notifyOff()
	{
		Resources res = getResources();
		SharedPreferences sp = PreferenceManager.getDefaultSharedPreferences(this);
		boolean need_notify = sp.getBoolean(res.getString(R.string.stop_beeping_notify_preference_key), false);
		if (need_notify)
		{
			String melody = sp.getString(res.getString(R.string.stop_beeping_ringtone_preference_key), "");
			Intent notification_intent = new Intent(this, MainActivity.class);
			PendingIntent content_intent = PendingIntent.getActivity(this, 0, notification_intent, PendingIntent.FLAG_CANCEL_CURRENT);
			NotificationCompat.Builder notification_builder = (NotificationCompat.Builder) new NotificationCompat.Builder(this)
					.setContentIntent(content_intent)
					.setSmallIcon(R.drawable.beacon_off)
					.setContentTitle(getString(R.string.app_name))
					.setContentText(getString(R.string.watching_finished_caption) + (new SimpleDateFormat(" HH:mm:ss").format(System.currentTimeMillis())))
					.setSound(Uri.parse(melody))
					.setAutoCancel(true);
			NotificationManager notification_manager = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
			notification_manager.notify(777, notification_builder.build());
		}
		Intent intent = new Intent(SERVICE_STATE_BROADCAST_ACTION);
		intent.putExtra(PARAM_SERVICE_STATE_EVENT, SERVICE_EVENT_STOP_BEEPING);
		sendBroadcast(intent);
	}

	@Override
	public void beaconOn()
	{
		notifyOn();
	}

	@Override
	public void beaconOff()
	{
		if (m_locations_source.isActive()) notifyOff();
		m_watchers.clear();
		m_locations_source.stop();
	}

	@Override
	public void watcherConnected(String watcher)
	{
		m_watchers.add(watcher);
		Intent intent = new Intent(SERVICE_STATE_BROADCAST_ACTION);
		intent.putExtra(PARAM_WATCHER_STATE_EVENT, WATCHER_EVENT_CONNECTED);
		intent.putExtra(PARAM_WATCHER_NAME, watcher);
		sendBroadcast(intent);
	}

	@Override
	public void watcherDisconnected(String watcher)
	{
		m_watchers.remove(watcher);
		Intent intent = new Intent(SERVICE_STATE_BROADCAST_ACTION);
		intent.putExtra(PARAM_WATCHER_STATE_EVENT, WATCHER_EVENT_DISCONNECTED);
		intent.putExtra(PARAM_WATCHER_NAME, watcher);
		sendBroadcast(intent);
	}

	public boolean isAuthorized()
	{
		return m_beacon != null && m_beacon.isAuthorized();
	}

	public boolean isBeeping()
	{
		return m_beacon != null && m_beacon.isBeeping();
	}

	public boolean isTrackingAllowed()
	{
		return m_beacon != null && m_beacon.isTrackingAllowed();
	}

	public boolean isRecordingAllowed()
	{
		return m_beacon != null && m_beacon.isRecordingAllowed();
	}

	public void setTrackingPermission(boolean ok)
	{
		if (m_beacon != null) m_beacon.setTrackingPermission(ok);
	}

	public void setRecordingPermission(boolean ok)
	{
		if (m_beacon != null) m_beacon.setRecordingPermission(ok);
	}

	public ArrayList<String> getWatchers()
	{
		return m_watchers;
	}
}
