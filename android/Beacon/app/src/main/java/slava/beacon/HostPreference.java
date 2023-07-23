package slava.beacon;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.preference.DialogPreference;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.EditText;

import static slava.beacon.Constants.*;

public class HostPreference extends DialogPreference
{
	private EditText m_edtHostName;
	private EditText m_edtPort;

	public HostPreference(Context context, AttributeSet attrs)
	{
		super(context, attrs);
	}

	@Override
	protected View onCreateDialogView()
	{
		LayoutInflater inflater = (LayoutInflater)getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		View view = inflater.inflate(R.layout.host_dialog, null);
		Resources res = getContext().getResources();
		SharedPreferences prefs = getContext().getSharedPreferences(APP_PREFERENCES, Context.MODE_PRIVATE);
		String hostName = prefs.getString(res.getString(R.string.server_host_preference_key), res.getString(R.string.text_default_host));
		int hostPort = prefs.getInt(res.getString(R.string.server_port_preference_key), Integer.parseInt(res.getString(R.string.text_default_port)));
		m_edtHostName = (EditText)view.findViewById(R.id.edtHostName);
		m_edtHostName.setText(hostName);
		m_edtPort = (EditText)view.findViewById(R.id.edtPort);
		m_edtPort.setText(String.valueOf(hostPort));
		return view;
	}

	@Override
	protected void onDialogClosed(boolean positiveResult)
	{
		super.onDialogClosed(positiveResult);
		if (positiveResult)
		{
			Resources res = getContext().getResources();
			String hostName = m_edtHostName.getText().toString();
			String s = m_edtPort.getText().toString();
			int hostPort;
			try
			{
				hostPort = Integer.parseInt(s);
			}
			catch (NumberFormatException ne)
			{
				hostPort = Integer.parseInt(res.getString(R.string.text_default_port));
			}

			SharedPreferences prefs = getContext().getSharedPreferences(APP_PREFERENCES, Context.MODE_PRIVATE);
			SharedPreferences.Editor editor = prefs.edit();
			editor.putString(res.getString(R.string.server_host_preference_key), hostName);
			editor.putInt(res.getString(R.string.server_port_preference_key), hostPort);
			if (editor.commit()) notifyChanged();
		}
	}

	@Override
	public CharSequence getSummary()
	{
		Resources res = getContext().getResources();
		SharedPreferences prefs = getContext().getSharedPreferences(APP_PREFERENCES, Context.MODE_PRIVATE);
		String hostName = prefs.getString(res.getString(R.string.server_host_preference_key), res.getString(R.string.text_default_host));
		int hostPort = prefs.getInt(res.getString(R.string.server_port_preference_key), Integer.parseInt(res.getString(R.string.text_default_port)));
		return hostName + ":" + hostPort;
	}
}
