package slava.beacon;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import java.util.ArrayList;

import static slava.beacon.Constants.*;

public class WatchersActivity extends AppCompatActivity
{
	private ServiceConnection m_srv_conn = null;
	private BeaconService m_service = null;
	private BroadcastReceiver m_service_state_receiver = null;
	private ListView m_watchers = null;
	private ArrayAdapter<String> m_adapter = null;

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		//Log.i(LOG_TAG, "WatchersActivity: onCreate()");
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_watchers);
		m_watchers = (ListView) findViewById(R.id.lvWatchers);

		if (m_service_state_receiver == null)
		{
			m_service_state_receiver = new BroadcastReceiver()
			{
				@Override
				public void onReceive(Context context, Intent intent)
				{
					if (m_adapter != null) m_adapter.notifyDataSetChanged();
				}
			};
		}

		if (m_srv_conn == null)
		{
			m_service = null;
			m_srv_conn = new ServiceConnection()
			{
				@Override
				public void onServiceConnected(ComponentName name, IBinder binder)
				{
					//Log.i(LOG_TAG, "WatchersActivity: connect to service.");
					m_service = ((BeaconService.BeaconServiceBinder) binder).getService();
					ArrayList<String> w = m_service.getWatchers();
					//Log.i(LOG_TAG, "WatchersActivity: watchers count = " + w.size());
					m_adapter = new ArrayAdapter<>(WatchersActivity.this, android.R.layout.simple_list_item_1, w);
					m_watchers.setAdapter(m_adapter);
					m_adapter.notifyDataSetChanged();
				}

				@Override
				public void onServiceDisconnected(ComponentName name)
				{
					//Log.i(LOG_TAG, "WatchersActivity: disconnect from service.");
					m_service = null;
					m_adapter = null;
					m_watchers.setAdapter(null);
				}
			};
		}
	}

	@Override
	protected void onStart()
	{
		//Log.i(LOG_TAG, "WatchersActivity: onStart()");
		super.onStart();
		if (m_srv_conn != null) bindService(new Intent(this, BeaconService.class), m_srv_conn, 0);
		if (m_service_state_receiver != null) registerReceiver(m_service_state_receiver, new IntentFilter(SERVICE_STATE_BROADCAST_ACTION));
	}

	@Override
	protected void onStop()
	{
		//Log.i(LOG_TAG, "WatchersActivity: onStop()");
		super.onStop();
		if (m_srv_conn != null) unbindService(m_srv_conn);
		if (m_service_state_receiver != null) unregisterReceiver(m_service_state_receiver);
	}
}
