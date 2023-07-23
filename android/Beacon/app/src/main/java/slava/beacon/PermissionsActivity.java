package slava.beacon;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.graphics.drawable.Drawable;
import android.os.IBinder;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import static slava.beacon.Constants.*;

public class PermissionsActivity extends AppCompatActivity
{
	private ServiceConnection m_srv_conn = null;
	private BeaconService m_service = null;
	private BroadcastReceiver m_service_state_receiver = null;
	private Button m_btn_tracking;
	private Button m_btn_recording;

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		//Log.i(LOG_TAG, "PermissionsActivity: onCreate()");
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_permissions);

		m_btn_tracking = (Button)findViewById(R.id.btnTracking);
		m_btn_recording = (Button)findViewById(R.id.btnRecording);

		if (m_service_state_receiver == null)
		{
			m_service_state_receiver = new BroadcastReceiver()
			{
				@Override
				public void onReceive(Context context, Intent intent)
				{
					boolean is_tracking;
					boolean is_recording;
					Drawable img;
					int service_event = intent.getIntExtra(PARAM_SERVICE_STATE_EVENT, UNKNOWN_SERVICE_EVENT);
					switch (service_event)
					{
						case SERVICE_EVENT_AUTHORIZED:
							m_btn_tracking.setEnabled(true);
							m_btn_recording.setEnabled(true);
						case SERVICE_EVENT_CONFIG:
							is_tracking = intent.getBooleanExtra(PARAM_TRACKING_PERMISSION, false);
							is_recording = intent.getBooleanExtra(PARAM_RECORDING_PERMISSION, false);
							img = is_tracking ? getResources().getDrawable(R.drawable.button_allow) : getResources().getDrawable(R.drawable.button_forbid);
							m_btn_tracking.setCompoundDrawablesWithIntrinsicBounds(null, null, img, null);
							img = is_recording ? getResources().getDrawable(R.drawable.button_allow) : getResources().getDrawable(R.drawable.button_forbid);
							m_btn_recording.setCompoundDrawablesWithIntrinsicBounds(null, null, img, null);
							break;
						case SERVICE_EVENT_DISCONNECTED:
							disableButtons();
							break;
					}
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
					//Log.i(LOG_TAG, "PermissionsActivity: connect to service.");
					m_service = ((BeaconService.BeaconServiceBinder) binder).getService();
					if (m_service.isAuthorized())
					{
						m_btn_tracking.setEnabled(true);
						m_btn_recording.setEnabled(true);
						Drawable img;
						img = m_service.isTrackingAllowed() ? getResources().getDrawable(R.drawable.button_allow) : getResources().getDrawable(R.drawable.button_forbid);
						m_btn_tracking.setCompoundDrawablesWithIntrinsicBounds(null, null, img, null);
						img = m_service.isRecordingAllowed() ? getResources().getDrawable(R.drawable.button_allow) : getResources().getDrawable(R.drawable.button_forbid);
						m_btn_recording.setCompoundDrawablesWithIntrinsicBounds(null, null, img, null);
					}
					else disableButtons();
				}

				@Override
				public void onServiceDisconnected(ComponentName name)
				{
					//Log.i(LOG_TAG, "PermissionsActivity: disconnect from service.");
					m_service = null;
					disableButtons();
				}
			};
		}
	}

	private void disableButtons()
	{
		Drawable img = getResources().getDrawable(R.drawable.button_forbid);
		m_btn_tracking.setCompoundDrawablesWithIntrinsicBounds(null, null, img, null);
		m_btn_recording.setCompoundDrawablesWithIntrinsicBounds(null, null, img, null);
		m_btn_tracking.setEnabled(false);
		m_btn_recording.setEnabled(false);
	}

	@Override
	protected void onStart()
	{
		//Log.i(LOG_TAG, "PermissionsActivity: onStart()");
		super.onStart();
		if (m_srv_conn != null) bindService(new Intent(this, BeaconService.class), m_srv_conn, 0);
		if (m_service_state_receiver != null) registerReceiver(m_service_state_receiver, new IntentFilter(SERVICE_STATE_BROADCAST_ACTION));
	}

	@Override
	protected void onStop()
	{
		//Log.i(LOG_TAG, "PermissionsActivity: onStop()");
		super.onStop();
		if (m_srv_conn != null) unbindService(m_srv_conn);
		if (m_service_state_receiver != null) unregisterReceiver(m_service_state_receiver);
	}

	public void switchTrackingState(View view)
	{
		if (m_service != null)
		{
			m_service.setTrackingPermission(!m_service.isTrackingAllowed());
		}
	}

	public void switchRecordingState(View view)
	{
		if (m_service != null)
		{
			m_service.setRecordingPermission(!m_service.isRecordingAllowed());
		}
	}
}
