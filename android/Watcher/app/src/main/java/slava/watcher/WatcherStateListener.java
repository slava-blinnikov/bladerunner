package slava.watcher;

public interface WatcherStateListener
{
	void connected();
	void disconnected();
	int findAvatar(byte client_type, int db_id);
	void avatarChanged(byte client_type, int db_id, int change_time, byte[] data, int data_size);
	void membersListChanged(int[] members);
	void memberChanged(int db_id, String member, String note, int ava_changetime, boolean tracking, boolean recording, int tracking_timeout);
	void membersOnlineChanged(int[] members);
	void memberConnected(int db_id);
	void memberDisconnected(int db_id);
	void memberBeaconStateChanged(int db_id, byte state);
	void memberLocationChanged(int db_id, int time, double latitude, double longitude, double speed, boolean gps);
	void memberTrackingPermissionChanged(int db_id, byte state);
	void memberRecordingPermissionChanged(int db_id, byte state);
}
