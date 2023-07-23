package slava.beacon;

public class Constants
{
	public static final String LOG_TAG = "myLogs";

	public static final String APP_PREFERENCES = "beacon_app_settings";
	public static final String SERVICE_STATE_PREFERENCE_KEY = "pref_start_key";
	public static final String AVA_CHANGETIME_PREFERENCE_KEY = "pref_ava_changetime_password_key";
	public static final String SERVICE_STATE_BROADCAST_ACTION = "slava.beacon.service.state";
	public static final String PARAM_SERVICE_STATE_EVENT = "state_event";
	public static final String PARAM_TRACKING_PERMISSION = "tracking_allowed";
	public static final String PARAM_RECORDING_PERMISSION = "recording_allowed";
	public static final String PARAM_WATCHER_NAME = "watcher_name";
	public static final String PARAM_WATCHER_STATE_EVENT = "watcher_state";
	public static final int UNKNOWN_SERVICE_EVENT = 0;
	public static final int SERVICE_EVENT_START = 1;
	public static final int SERVICE_EVENT_AUTHORIZED = 2;
	public static final int SERVICE_EVENT_CONFIG = 3;
	public static final int SERVICE_EVENT_AVATAR = 4;
	public static final int SERVICE_EVENT_DISCONNECTED = 5;
	public static final int SERVICE_EVENT_START_BEEPING = 6;
	public static final int SERVICE_EVENT_STOP_BEEPING = 7;
	public static final int SERVICE_EVENT_STOP = 8;
	public static final int WATCHER_EVENT_CONNECTED = 1;
	public static final int WATCHER_EVENT_DISCONNECTED = 0;
	public static final String APP_DATA_PATH = "/bladerunner/beacon";
	public static final String AVATAR_FILENAME = "beacon_avatar.png";
}
