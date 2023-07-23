package slava.beacon;

import android.location.Location;

public interface LocationSourceListener
{
	void onLocationChanged(Location location, boolean ok);
	void onLocationProviderEnabled();
	void onLocationProviderDisabled();
}
