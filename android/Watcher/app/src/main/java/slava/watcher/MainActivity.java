package slava.watcher;

import android.util.Log;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.ActionBar;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.view.SubMenu;
import android.webkit.WebView;
import android.widget.CheckBox;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import static slava.watcher.Constants.*;

public class MainActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener, WatcherStateListener
{
    public class Member
    {
        public int db_id;
        public String name;
        public String note;
        public boolean tracking_permission;
        public boolean recording_permission;
        public boolean online;
        public MenuItem menu_item;
    }
    private Member[] m_members;

    private Watcher m_watcher = new Watcher(this);
    private WebView m_map;
    private int m_current_member;
    private int m_crew_ava_changetime;
    private Menu m_nav_menu;
    private CheckBox m_chb_crew;
    private TextView m_txt_speed;
    private String m_speed_template;
    private double m_latitude;
    private double m_longitude;
    private long m_speed;
    private Runnable m_rnb_set_location = new Runnable()
    {
        @Override
        public void run()
        {
            m_map.loadUrl("javascript:moveMarker(" + m_latitude + "," + m_longitude + ")");
            m_txt_speed.setText(String.format(m_speed_template, m_speed));
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar)findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        ActionBar ab = getSupportActionBar();
        if (ab != null) ab.setDisplayShowTitleEnabled(false);

        DrawerLayout drawer = (DrawerLayout)findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.setDrawerListener(toggle);
        toggle.syncState();

        NavigationView navigationView = (NavigationView)findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);
        navigationView.setItemIconTintList(null);

        m_map = (WebView)findViewById(R.id.webView);
        m_map.getSettings().setJavaScriptEnabled(true);
        m_map.loadUrl("file:///android_asset/index.yandex.html");

        Resources res = getResources();
        SharedPreferences prefs = getSharedPreferences(APP_PREFERENCES, MODE_PRIVATE);
        String host = prefs.getString(res.getString(R.string.server_host_preference_key), res.getString(R.string.text_default_host));
        int port = prefs.getInt(res.getString(R.string.server_port_preference_key), Integer.parseInt(res.getString(R.string.text_default_port)));
        m_watcher.setAddress(host, port);
        String crew_name = prefs.getString(res.getString(R.string.crew_name_preference_key), "blinnikov");
        String crew_watcher = prefs.getString(res.getString(R.string.crew_watcher_preference_key), "test");
        String password = prefs.getString(res.getString(R.string.crew_password_preference_key), "qwerty");
        m_watcher.setAuthorization(crew_name, password, crew_watcher);
        m_current_member = prefs.getInt(res.getString(R.string.current_member_pref_key), 3);
        m_crew_ava_changetime = prefs.getInt(res.getString(R.string.crew_ava_changetime_preference_key), 0);

        Drawable img;
        LinearLayout ll = (LinearLayout)navigationView.getHeaderView(0);
        img = getAvaDrawable(Watcher.CREW_CLIENT_TYPE, 0);
        if (img != null)
        {
            ImageView imgCrew = (ImageView)ll.findViewById(R.id.imgCrew);
            if (imgCrew != null) imgCrew.setImageDrawable(img);
        }
        m_chb_crew = (CheckBox) ll.findViewById(R.id.chbCrew);
        if (m_chb_crew != null) m_chb_crew.setText(crew_name);

        if (m_current_member >= 0)
        {
            img = getAvaDrawable(Watcher.MEMBER_CLIENT_TYPE, m_current_member);
            if (img != null)
            {
                ImageView imgCurrMember = (ImageView)findViewById(R.id.imgCurrMember);
                imgCurrMember.setImageDrawable(img);
            }
        }
        m_txt_speed = (TextView)findViewById(R.id.txtCurrSpeed);
        m_speed_template = res.getString(R.string.speed_template);

        m_nav_menu = navigationView.getMenu();
        m_members = null;

        Log.d(LOG_TAG, "[Activity] onCreate(" + (savedInstanceState == null) + ")");
    }

    @Override
    protected void onStart()
    {
        super.onStart();
        m_watcher.start();
        Log.d(LOG_TAG, "[Activity] onStart()");
    }

    @Override
    protected void onStop()
    {
        super.onStop();
        m_watcher.stop();
        Log.d(LOG_TAG, "[Activity] onStop()");
    }

    @Override
    public void onBackPressed()
    {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START))
        {
            drawer.closeDrawer(GravityCompat.START);
        }
        else
        {
            super.onBackPressed();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu)
    {
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        int id = item.getItemId();
        if (id == R.id.action_settings)
        {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item)
    {
        // Handle navigation view item clicks here.

        int id = item.getItemId();
        if (id >= 0)
        {
            if (m_current_member != id)
            {
                if (m_current_member >= 0) m_watcher.requestStopMemberBeacon(m_current_member);
                m_current_member = id;
                ImageView imgCurrMember = (ImageView) findViewById(R.id.imgCurrMember);
                Drawable img = getAvaDrawable(Watcher.MEMBER_CLIENT_TYPE, m_current_member);
                if (img == null) img = getResources().getDrawable(R.drawable.no_member_avatar);
                imgCurrMember.setImageDrawable(img);
                m_txt_speed.setText("");
                m_watcher.requestStartMemberBeacon(m_current_member);

                Resources res = getResources();
                SharedPreferences prefs = getSharedPreferences(APP_PREFERENCES, MODE_PRIVATE);
                SharedPreferences.Editor editor = prefs.edit();
                editor.putInt(res.getString(R.string.current_member_pref_key), id);
                editor.apply();
            }
        }
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }

    @Override
    public void connected()
    {
        Log.d(LOG_TAG, "[StateListener] Connected.");
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                m_chb_crew.setChecked(true);
            }
        });
    }

    @Override
    public void disconnected()
    {
        Log.d(LOG_TAG, "[StateListener] Disconnected.");
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                m_chb_crew.setChecked(false);
            }
        });
    }

    @Override
    public int findAvatar(byte client_type, int db_id)
    {
        //Log.d(LOG_TAG, "[StateListener] Find avatar: client=" + client_type + "; id=" + db_id);
        if (client_type == Watcher.MEMBER_CLIENT_TYPE)
        {
            Resources res = getResources();
            SharedPreferences prefs = getSharedPreferences(APP_PREFERENCES, MODE_PRIVATE);
            String key = String.format(res.getString(R.string.member_ava_changetime_template_preference_key), db_id);
            return prefs.getInt(key, 0);
        }
        else return m_crew_ava_changetime;
    }

    @Override
    public void avatarChanged(byte client_type, int db_id, int change_time, byte[] data, int data_size)
    {
        Resources res = getResources();
        String fn;
        if (client_type == Watcher.MEMBER_CLIENT_TYPE)
            fn = String.format(res.getString(R.string.member_ava_filename_template), db_id);
        else fn = res.getString(R.string.crew_ava_filename);

        FileOutputStream fout = null;
        if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED))
        {
            File sd_path = Environment.getExternalStorageDirectory();
            sd_path = new File(sd_path.getAbsolutePath() + APP_DATA_PATH);
            sd_path.mkdirs();
            if (sd_path.isDirectory())
            {
                File sd_file = new File(sd_path, fn);
                try
                {
                    fout = new FileOutputStream(sd_file);
                }
                catch (IOException ignored){}
            }
        }
        else
        {
            try
            {
                fout = openFileOutput(fn, MODE_PRIVATE);
            }
            catch (IOException ignored){}
        }
        boolean ok = false;
        if (fout != null)
        {
            try
            {
                fout.write(data, 9, data_size - 9);
                fout.close();
                ok = true;
            }
            catch (IOException ignored){}
        }
        if (ok)
        {
            SharedPreferences prefs = getSharedPreferences(APP_PREFERENCES, MODE_PRIVATE);
            SharedPreferences.Editor editor = prefs.edit();
            String key;
            if (client_type == Watcher.MEMBER_CLIENT_TYPE)
            {
                key = String.format(res.getString(R.string.member_ava_changetime_template_preference_key), db_id);
            }
            else
            {
                key = res.getString(R.string.crew_ava_changetime_preference_key);
                m_crew_ava_changetime = change_time;
            }
            editor.putInt(key, change_time);
            editor.apply();
            Log.d(LOG_TAG, "[StateListener] " +
                    (client_type == Watcher.MEMBER_CLIENT_TYPE ? "Member" : "Crew") +
                    " avatar saved. db_id=" + db_id);
        }
    }

    private Drawable getAvaDrawable(int client_type, int db_id)
    {
        Resources res = getResources();
        String afn;
        if (client_type == Watcher.MEMBER_CLIENT_TYPE)
            afn = String.format(res.getString(R.string.member_ava_filename_template), db_id);
        else afn = res.getString(R.string.crew_ava_filename);

        String fn = null;
        if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED))
        {
            File sd_path = Environment.getExternalStorageDirectory();
            sd_path = new File(sd_path.getAbsolutePath() + APP_DATA_PATH);
            sd_path.mkdirs();
            if (sd_path.isDirectory()) fn = sd_path.getAbsolutePath() + "/" + afn;
        }
        else fn = afn;
        if (fn != null)
        {
            File file = new File(fn);
            if (file.exists())
            {
                Drawable img = Drawable.createFromPath(fn);
                return img;
            }
        }
        return null;
    }

    @Override
    public void membersListChanged(int[] members)
    {
        m_members = null;
        if (members != null)
        {
            m_members = new Member[members.length];
            for (int i = 0; i < members.length; i++)
            {
                Member member = new Member();
                member.db_id = members[i];
                member.online = false;
                member.recording_permission = false;
                member.tracking_permission = false;
                member.menu_item = null;
                m_members[i] = member;
            }
        }
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                m_nav_menu.clear();
                /*SubMenu smActions = m_nav_menu.addSubMenu("test");
                smActions.add(Menu.NONE, -1, Menu.NONE, "test_01");
                smActions.add(Menu.NONE, -2, Menu.NONE, "test_01");*/
                if (m_members != null)
                {
                    SubMenu smMembers = m_nav_menu.addSubMenu(R.string.members_list_title);
                    Member member;
                    for (int i = 0; i < m_members.length; i++)
                    {
                        member = m_members[i];
                        MenuItem itm = smMembers.add(Menu.NONE, member.db_id, i, R.string.unknown_member_title);
                        member.menu_item = itm;
                        if (member.online) itm.setIcon(R.drawable.online);
                            else itm.setIcon(R.drawable.offline);
                        itm.setActionView(R.layout.member_item);
                        Drawable img = getAvaDrawable(Watcher.MEMBER_CLIENT_TYPE, member.db_id);
                        if (img != null)
                        {
                            LinearLayout ll = (LinearLayout)itm.getActionView();
                            ImageView imgv = (ImageView)ll.findViewById(R.id.imgMember);
                            imgv.setImageDrawable(img);
                        }
                    }
                }
            }
        });
    }

    @Override
    public void memberChanged(int db_id, String member, String note, int ava_changetime, boolean tracking, boolean recording, int tracking_timeout)
    {
        //Log.d(LOG_TAG, "[StateListener] Member changed: id=" + db_id + "; name=\"" + member + "\"; note=\"" + note + "\"; ava_time=" + ava_changetime + "; tracking=" + tracking + "; recording=" + recording + "; tracking_timeout=" + tracking_timeout);
        int idx = -1;
        Member mbr;
        for (int i = 0; i < m_members.length; i++)
        {
            mbr = m_members[i];
            if (mbr.db_id == db_id)
            {
                mbr.name = member;
                mbr.note = note;
                mbr.tracking_permission = tracking;
                mbr.recording_permission = recording;
                idx = i;
                break;
            }
        }
        final int mbr_index = idx;
        if (mbr_index >= 0)
        {
            runOnUiThread(new Runnable()
            {
                @Override
                public void run()
                {
                    Member mbr = m_members[mbr_index];
                    mbr.menu_item.setTitle(mbr.name);
                }
            });
        }
    }

    @Override
    public void membersOnlineChanged(int[] members)
    {
        Member mbr;
        if (members != null)
        {
            if (m_current_member >= 0)
            {
                for (int i = 0; i < members.length; i++)
                {
                    if (members[i] == m_current_member)
                    {
                        m_watcher.requestStartMemberBeacon(m_current_member);
                        break;
                    }
                }
            }
            boolean ok;
            for (int i = 0; i < m_members.length; i++)
            {
                mbr = m_members[i];
                ok = false;
                for (int j = 0; j < members.length; j++)
                {
                    if (members[j] == mbr.db_id)
                    {
                        ok = true;
                        break;
                    }
                }
                mbr.online = ok;
            }
        }
        else
        {
            for (int i = 0; i < m_members.length; i++) m_members[i].online = false;
        }
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                Member mbr;
                for (int i = 0; i < m_members.length; i++)
                {
                    mbr = m_members[i];
                    if (mbr.online) mbr.menu_item.setIcon(R.drawable.online);
                        else mbr.menu_item.setIcon(R.drawable.offline);
                }
            }
        });
    }

    @Override
    public void memberConnected(int db_id)
    {
        //Log.d(LOG_TAG, "[StateListener] Member connected: id=" + db_id);
        if (m_current_member == db_id) m_watcher.requestStartMemberBeacon(m_current_member);
        int idx = -1;
        Member mbr;
        for (int i = 0; i < m_members.length; i++)
        {
            mbr = m_members[i];
            if (mbr.db_id == db_id)
            {
                mbr.online = true;
                idx = i;
                break;
            }
        }
        final int mbr_index = idx;
        if (mbr_index >= 0)
        {
            runOnUiThread(new Runnable()
            {
                @Override
                public void run()
                {
                    m_members[mbr_index].menu_item.setIcon(R.drawable.online);
                }
            });
        }
    }

    @Override
    public void memberDisconnected(int db_id)
    {
        //Log.d(LOG_TAG, "[StateListener] Member disconnected: id=" + db_id);
        int idx = -1;
        Member mbr;
        for (int i = 0; i < m_members.length; i++)
        {
            mbr = m_members[i];
            if (mbr.db_id == db_id)
            {
                mbr.online = false;
                idx = i;
                break;
            }
        }
        final int mbr_index = idx;
        if (mbr_index >= 0)
        {
            runOnUiThread(new Runnable()
            {
                @Override
                public void run()
                {
                    m_members[mbr_index].menu_item.setIcon(R.drawable.offline);
                }
            });
        }
    }

    @Override
    public void memberBeaconStateChanged(int db_id, byte state)
    {
        //Log.d(LOG_TAG, "[StateListener] Member beacon state changed: id=" + db_id + "; state=" + state);
    }

    @Override
    public void memberLocationChanged(int db_id, int time, double latitude, double longitude, double speed, boolean gps)
    {
        if (m_current_member == db_id)
        {
            m_latitude = latitude;
            m_longitude = longitude;
            m_speed = Math.round(speed * 3.6);
            runOnUiThread(m_rnb_set_location);
        }
        //Log.d(LOG_TAG, "[StateListener] Member location changed: id=" + db_id + "; time=" + time + "; latitude=" + latitude + "; longitude=" + longitude + "; speed=" + speed + "; gps=" + gps);
    }

    @Override
    public void memberTrackingPermissionChanged(int db_id, byte state)
    {
    }

    @Override
    public void memberRecordingPermissionChanged(int db_id, byte state)
    {
    }
}
