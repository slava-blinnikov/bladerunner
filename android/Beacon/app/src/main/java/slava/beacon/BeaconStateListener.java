package slava.beacon;

public interface BeaconStateListener
{
	void connected(boolean tracking_permission, boolean recording_permission, int tracking_timeout);
	void configured(boolean tracking_permission, boolean recording_permission, int tracking_timeout);
	void avatarChanged(int change_time, byte[] data, int data_size);
	void disconnected();
	void beaconOn();
	void beaconOff();
	void watcherConnected(String watcher);
	void watcherDisconnected(String watcher);
}
