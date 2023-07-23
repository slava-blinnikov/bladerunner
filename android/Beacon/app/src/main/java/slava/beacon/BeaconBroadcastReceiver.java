package slava.beacon;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.util.Log;
import android.widget.Toast;

import static slava.beacon.Constants.*;

public class BeaconBroadcastReceiver extends BroadcastReceiver
{
	@Override
	public void onReceive(Context context, Intent intent)
	{
		String action = intent.getAction();
		Log.i(LOG_TAG, "BeaconBroadcastReceiver: receive on load intent with action " + action);

		Toast.makeText(context, "Booting Completed", Toast.LENGTH_LONG).show();

		if (action != null && action.equals(Intent.ACTION_BOOT_COMPLETED))
		{
			SharedPreferences prefs = context.getSharedPreferences(APP_PREFERENCES, Context.MODE_PRIVATE);
			boolean start = prefs.getBoolean(SERVICE_STATE_PREFERENCE_KEY, false);
			if (start)
			{
				context.startService(new Intent(context, BeaconService.class));
				Log.i(LOG_TAG, "BeaconBroadcastReceiver: on load start");
			}
		}
	}
}
