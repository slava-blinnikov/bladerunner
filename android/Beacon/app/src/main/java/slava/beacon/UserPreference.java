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

public class UserPreference extends DialogPreference
{
	private EditText m_edtCrewName;
	private EditText m_edtMemberName;
	private EditText m_edtPassword;

	public UserPreference(Context context, AttributeSet attrs)
	{
		super(context, attrs);
	}

	@Override
	protected View onCreateDialogView()
	{
		LayoutInflater inflater = (LayoutInflater) getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		View view = inflater.inflate(R.layout.user_dialog, null);
		Resources res = getContext().getResources();
		SharedPreferences prefs = getContext().getSharedPreferences(APP_PREFERENCES, Context.MODE_PRIVATE);
		String crewName = prefs.getString(res.getString(R.string.crew_name_preference_key), "");
		String memberName = prefs.getString(res.getString(R.string.member_name_preference_key), "");
		String password = prefs.getString(res.getString(R.string.member_password_preference_key), "");
		m_edtCrewName = (EditText) view.findViewById(R.id.edtCrewName);
		m_edtCrewName.setText(crewName);
		m_edtMemberName = (EditText) view.findViewById(R.id.edtMemberName);
		m_edtMemberName.setText(memberName);
		m_edtPassword = (EditText) view.findViewById(R.id.edtPassword);
		m_edtPassword.setText(password);
		return view;
	}

	@Override
	protected void onDialogClosed(boolean positiveResult)
	{
		super.onDialogClosed(positiveResult);
		if (positiveResult)
		{
			Resources res = getContext().getResources();
			String crewName = m_edtCrewName.getText().toString();
			String memberName = m_edtMemberName.getText().toString();
			String password = m_edtPassword.getText().toString();
			SharedPreferences prefs = getContext().getSharedPreferences(APP_PREFERENCES, Context.MODE_PRIVATE);
			SharedPreferences.Editor editor = prefs.edit();
			editor.putString(res.getString(R.string.crew_name_preference_key), crewName);
			editor.putString(res.getString(R.string.member_name_preference_key), memberName);
			editor.putString(res.getString(R.string.member_password_preference_key), password);
			editor.putInt(AVA_CHANGETIME_PREFERENCE_KEY, 0);
			if (editor.commit()) notifyChanged();
		}
	}

	@Override
	public CharSequence getSummary()
	{
		Resources res = getContext().getResources();
		SharedPreferences prefs = getContext().getSharedPreferences(APP_PREFERENCES, Context.MODE_PRIVATE);
		String crewName = prefs.getString(res.getString(R.string.crew_name_preference_key), "");
		String memberName = prefs.getString(res.getString(R.string.member_name_preference_key), "");
		return crewName + "." + memberName;
	}
}