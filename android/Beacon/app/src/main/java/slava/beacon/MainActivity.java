package slava.beacon;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;
import android.content.res.Resources;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Environment;
import android.os.IBinder;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;
import android.widget.ToggleButton;

import java.io.File;

import static slava.beacon.Constants.*;

public class MainActivity extends AppCompatActivity implements OnSharedPreferenceChangeListener
{
	private ServiceConnection m_srv_conn = null;
	private BeaconService m_service = null;
	private ToggleButton m_btn_start;
	private TextView m_member;
	private BroadcastReceiver m_service_state_receiver = null;

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		//Log.i(LOG_TAG, "MainActivity: onCreate()");
		super.onCreate(savedInstanceState);

		if (m_srv_conn == null)
		{
			m_service = null;
			m_srv_conn = new ServiceConnection()
			{
				@Override
				public void onServiceConnected(ComponentName name, IBinder binder)
				{
					//Log.i(LOG_TAG, "MainActivity: connect to service.");
					m_service = ((BeaconService.BeaconServiceBinder) binder).getService();
					Drawable img_left = getResources().getDrawable(R.drawable.service_state_control);
					Drawable img_right = m_service.isBeeping() ? getResources().getDrawable(R.drawable.beacon_on) : getResources().getDrawable(R.drawable.beacon_off);
					m_btn_start.setCompoundDrawablesWithIntrinsicBounds(img_left, null, img_right, null);
					m_btn_start.setChecked(m_service != null);
					if (m_service.isAuthorized()) m_member.setTextColor(getResources().getColor(R.color.authorized_color));
					else m_member.setTextColor(getResources().getColor(R.color.not_authorized_color));
				}

				@Override
				public void onServiceDisconnected(ComponentName name)
				{
					//Log.i(LOG_TAG, "MainActivity: disconnect from service.");
					m_service = null;
					Drawable img_left = getResources().getDrawable(R.drawable.service_state_control);
					Drawable img_right = getResources().getDrawable(R.drawable.beacon_off);
					m_btn_start.setCompoundDrawablesWithIntrinsicBounds(img_left, null, img_right, null);
					m_btn_start.setChecked(false);
					m_member.setTextColor(getResources().getColor(R.color.not_authorized_color));
				}
			};
		}
		if (m_service_state_receiver == null)
		{
			m_service_state_receiver = new BroadcastReceiver()
			{
				@Override
				public void onReceive(Context context, Intent intent)
				{
					int service_event = intent.getIntExtra(PARAM_SERVICE_STATE_EVENT, UNKNOWN_SERVICE_EVENT);
					switch (service_event)
					{
						case SERVICE_EVENT_START:
							onServiceStart();
							break;
						case SERVICE_EVENT_AUTHORIZED:
							onBeaconAuthorized();
							break;
						case SERVICE_EVENT_AVATAR:
							updateAvatar();
							break;
						case SERVICE_EVENT_DISCONNECTED:
							onBeaconDisconnected();
							break;
						case SERVICE_EVENT_START_BEEPING:
							onBeaconStartBeeping();
							break;
						case SERVICE_EVENT_STOP_BEEPING:
							onBeaconStopBeeping();
							break;
						case SERVICE_EVENT_STOP:
							onServiceStop();
							break;
					}
				}
			};
		}

		setContentView(R.layout.activity_main);

		m_btn_start = (ToggleButton)findViewById(R.id.buttonStart);
		m_member = (TextView)findViewById(R.id.txvMember);

		Resources res = getResources();
		SharedPreferences prefs = getSharedPreferences(APP_PREFERENCES, MODE_PRIVATE);
		String crew_name = prefs.getString(res.getString(R.string.crew_name_preference_key), null);
		String member_name = prefs.getString(res.getString(R.string.member_name_preference_key), null);
		String device_name;
		if (crew_name != null && member_name != null) device_name = crew_name + "." + member_name;
			else device_name = getResources().getString(R.string.no_member_name_caption);
		m_member.setText(device_name);
		int ava_changetime = prefs.getInt(AVA_CHANGETIME_PREFERENCE_KEY, 0);
		if (ava_changetime > 0) updateAvatar();
		prefs.registerOnSharedPreferenceChangeListener(this);
	}

	@Override
	public void onSharedPreferenceChanged(SharedPreferences prefs, String key)
	{
		Resources res = getResources();
		String crew_key = res.getString(R.string.crew_name_preference_key);
		String member_key = res.getString(R.string.member_name_preference_key);
		if (key.equals(crew_key) || key.equals(member_key))
		{
			String device_name;
			String crew_name = prefs.getString(crew_key, null);
			String member_name = prefs.getString(member_key, null);
			if (crew_name != null && member_name != null) device_name = crew_name + "." + member_name;
				else device_name = getResources().getString(R.string.no_member_name_caption);
			m_member.setText(device_name);
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu)
	{
		getMenuInflater().inflate(R.menu.menu_main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item)
	{
		int id = item.getItemId();
		switch (id)
		{
			case R.id.action_settings:
				startActivity(new Intent(this, SettingsActivity.class));
				return true;
			case R.id.action_watchers:
				startActivity(new Intent(this, WatchersActivity.class));
				return true;
			case R.id.action_permissions:
				startActivity(new Intent(this, PermissionsActivity.class));
				return true;
		}
		return super.onOptionsItemSelected(item);
	}

	@Override
	protected void onStart()
	{
		//Log.i(LOG_TAG, "MainActivity: onStart()");
		super.onStart();
		if (m_srv_conn != null) bindService(new Intent(this, BeaconService.class), m_srv_conn, 0);
		if (m_service_state_receiver != null) registerReceiver(m_service_state_receiver, new IntentFilter(SERVICE_STATE_BROADCAST_ACTION));
	}

	@Override
	protected void onStop()
	{
		//Log.i(LOG_TAG, "MainActivity: onStop()");
		super.onStop();
		if (m_srv_conn != null) unbindService(m_srv_conn);
		if (m_service_state_receiver != null) unregisterReceiver(m_service_state_receiver);
	}

	public void startButtonPressed(View btn)
	{
		//Log.i(LOG_TAG, "MainActivity: startButtonPressed()");
		SharedPreferences prefs = getSharedPreferences(APP_PREFERENCES, Context.MODE_PRIVATE);
		SharedPreferences.Editor editor = prefs.edit();
		if (m_btn_start.isChecked())
		{
			startService(new Intent(this, BeaconService.class));
			editor.putBoolean(SERVICE_STATE_PREFERENCE_KEY, true);
		}
		else
		{
			stopService(new Intent(this, BeaconService.class));
			editor.putBoolean(SERVICE_STATE_PREFERENCE_KEY, false);
		}
		editor.apply();
	}

	private void updateAvatar()
	{
		if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED))
		{
			File sd_path = Environment.getExternalStorageDirectory();
			sd_path = new File(sd_path.getAbsolutePath() + APP_DATA_PATH);
			sd_path.mkdirs();
			if (sd_path.isDirectory())
			{
				Drawable img = Drawable.createFromPath(sd_path.getAbsolutePath() + "/" + AVATAR_FILENAME);
				m_member.setCompoundDrawablesWithIntrinsicBounds(null, img, null, null);
			}
		}
		else
		{
			Drawable img = Drawable.createFromPath(AVATAR_FILENAME);
			m_member.setCompoundDrawablesWithIntrinsicBounds(null, img, null, null);
		}
	}

	private void onServiceStart()
	{
		m_btn_start.setChecked(true);
	}

	private void onBeaconAuthorized()
	{
		m_member.setTextColor(getResources().getColor(R.color.authorized_color));
	}

	private void onBeaconStartBeeping()
	{
		Drawable img_left = getResources().getDrawable(R.drawable.service_state_control);
		Drawable img_right = getResources().getDrawable(R.drawable.beacon_on);
		m_btn_start.setCompoundDrawablesWithIntrinsicBounds(img_left, null, img_right, null);
	}

	private void onBeaconStopBeeping()
	{
		Drawable img_left = getResources().getDrawable(R.drawable.service_state_control);
		Drawable img_right = getResources().getDrawable(R.drawable.beacon_off);
		m_btn_start.setCompoundDrawablesWithIntrinsicBounds(img_left, null, img_right, null);
	}

	private void onBeaconDisconnected()
	{
		m_member.setTextColor(getResources().getColor(R.color.not_authorized_color));
	}

	private void onServiceStop()
	{
		m_btn_start.setChecked(false);
	}
}