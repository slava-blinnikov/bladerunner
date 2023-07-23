package slava.watcher;

import android.util.Log;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.util.Arrays;
import java.util.Timer;
import java.util.TimerTask;

import static slava.watcher.Constants.*;

public class Watcher implements Runnable
{
    private final static int PACKET_MAX_SIZE = 512;
    private final static int PAYLOAD_MAX_SIZE = 33;
    private final static String PACKET_MARKER = "#&#&_bladerunner_packet";
    private final static int PACKET_MARKER_LENGTH = PACKET_MARKER.length();
    public final static int CREW_CLIENT_TYPE = 2;
    public final static int MEMBER_CLIENT_TYPE = 3;
    private final static int QUERY_UNKNOWN = 0,
                            QUERY_CLIENT_HEARTBEAT = 1,
                            QUERY_SERVER_HEARTBEAT = 2,
                            QUERY_AUTHORIZATION = 3,
                            QUERY_AUTHORIZATION_OK = 4,
                            QUERY_AVATAR_CHANGETIME = 5,
                            QUERY_AVATAR = 6,
                            QUERY_MEMBERS_LIST = 7,
                            QUERY_MEMBER = 8,
                            QUERY_MEMBERS_ONLINE = 9,
                            QUERY_MEMBER_CONNECTED = 10,
                            QUERY_MEMBER_DISCONNECTED = 11,
                            QUERY_START_BEACON = 14,
                            QUERY_STOP_BEACON = 15,
                            QUERY_BEACON_STATE = 16,
                            QUERY_LOCATION = 17,
                            QUERY_TRACKING_PERMISSION = 19,
                            QUERY_RECORDING_PERMISSION = 20;
    private final static int BUFFER_SIZE = PACKET_MAX_SIZE + PACKET_MARKER_LENGTH + 50;

    private final static int TCP_CLIENT_CONNECTING = 0;
    private final static int TCP_CLIENT_ONLINE_WORK = 1;
    private boolean m_is_active;
    private int m_tcp_status;
    private Thread m_thread;
    private final Object m_synch;
    private String m_host;
    private int m_port;
    private String m_crew;
    private String m_password;
    private String m_watcher;
    private Socket m_socket;
    private BeaconQueryDispatcher m_qdispatcher;
    private InputStream m_input;
    private OutputStream m_output;
    private byte[] m_buffer;
    private int[] m_crc32_table;
    private byte[] m_query_template;
    private byte[] m_payload;
    private byte[] m_ava_bytes;
    private byte[] m_cmp_marker;
    private byte[] m_packet_marker_bytes;
    private int m_buffer_index;
    private int m_payload_index;
    private int m_packet_index;
    private int m_current_query_type;
    private long m_data_length;
    private int m_crc32;
    private boolean m_is_last_packet;
    private int m_db_id;
    private int m_heartbeat_timeout;
    private WatcherStateListener m_listener;
    private HeartbeatSource m_heartbeat_source;
    private long m_last_read;

    private class BeaconQuery
    {
        private int m_query;
        private byte[] m_payload;
        BeaconQuery(int query, byte[] payload)
        {
            m_query = query;
            m_payload = payload;
        }

        public int getQuery()
        {
            return m_query;
        }

        public byte[] getPayload()
        {
            return m_payload;
        }
    }

    private class BeaconQueriesQueue
    {
        private BeaconQuery[] m_queries;
        private int m_top_index;
        private int m_capacity;
        private int m_bottom_index;
        private int m_size;
        public BeaconQueriesQueue(int capacity)
        {
            if (capacity > 0) m_queries = new BeaconQuery[capacity]; else m_queries = null;
            m_capacity = capacity;
            m_size = 0;
            m_top_index = -1;
            m_bottom_index = -1;
        }

        public synchronized void offer(BeaconQuery query)
        {
            if (m_size < m_capacity) m_size++;
            m_top_index = (m_top_index + 1) % m_capacity;
            m_bottom_index = (m_top_index - m_size + m_capacity + 1) % m_capacity;
            m_queries[m_top_index] =query;
        }

        public synchronized BeaconQuery poll()
        {
            if (m_size > 0)
            {
                BeaconQuery q = m_queries[m_bottom_index];
                m_queries[m_bottom_index] = null;
                m_size--;
                if (m_size > 0)
                {
                    m_bottom_index = (m_bottom_index + 1) % m_capacity;
                }
                else
                {
                    m_size = 0;
                    m_top_index = -1;
                    m_bottom_index = -1;
                }
                return q;
            }
            return null;
        }

        public synchronized void clear()
        {
            m_size = 0;
            m_top_index = -1;
            m_bottom_index = -1;
            for (int i = 0; i < m_queries.length; i++) m_queries[i] = null;
        }
    }

    private class BeaconQueryDispatcher implements Runnable
    {
        private boolean m_running;
        private Thread m_thread;
        private Semaphore m_synch;
        BeaconQueriesQueue m_queries;

        public BeaconQueryDispatcher()
        {
            m_running = false;
            m_thread = null;
            m_synch = null;
            m_queries = null;
        }

        public void start()
        {
            if (!m_running)
            {
                m_synch = new Semaphore();
                m_queries = new BeaconQueriesQueue(50);
                m_thread = new Thread(this);
                m_thread.start();
            }
        }

        public void stop()
        {
            if (m_running)
            {
                m_running = false;
                m_queries.clear();
                m_synch.signal();
                if (m_thread.isAlive())
                {
                    try
                    {
                        m_thread.join();
                    }
                    catch (InterruptedException ignored){}
                }
                m_thread = null;
                m_synch = null;
                m_queries = null;
            }
        }

        public boolean isActive()
        {
            return m_running;
        }

        public synchronized void sendQuery(int query, byte[] payload)
        {
            if (m_running && query >= 0)
            {
                m_queries.offer(new BeaconQuery(query, payload));
                m_synch.signal();
            }
        }

        @Override
        public void run()
        {
            BeaconQuery q;
            m_running = true;
            while (m_running)
            {
                m_synch.lock();
                if (m_running)
                {
                    q = m_queries.poll();
                    if (q != null)
                    {
                        try
                        {
                            send(q.getQuery(), q.getPayload());
                            //Log.d(LOG_TAG, "[Watcher] Query " + q.getQuery() + " sended.");
                        }
                        catch (IOException e)
                        {
                            if (m_socket != null && !m_socket.isClosed())
                            {
                                try
                                {
                                    m_socket.close();
                                }
                                catch (IOException ignored){}
                            }
                        }
                    }
                }
            }
        }
    }

    private class HeartbeatSource
    {
        private Timer m_timer = null;
        private boolean m_is_active = false;

        public void start()
        {
            if (!m_is_active)
            {
                m_is_active = true;
                long t = m_heartbeat_timeout * 1000;
                m_timer = new Timer();
                m_timer.schedule(new TimerTask()
                {
                    @Override
                    public void run()
                    {
                        if (System.currentTimeMillis() - m_last_read >= m_heartbeat_timeout * 1000)
                        {
                            if (m_qdispatcher.isActive())
                            {
                                //Log.d(LOG_TAG, "[Watcher] Client heartbeat.");
                                m_qdispatcher.sendQuery(QUERY_CLIENT_HEARTBEAT, null);
                            }
                        }
                    }
                }, t, t);
            }
        }

        public void stop()
        {
            if (m_is_active)
            {
                m_is_active = false;
                m_timer.cancel();
                m_timer = null;
            }
        }
    }

    public Watcher(WatcherStateListener listener)
    {
        m_synch = new Object();
        m_listener = listener;
        m_heartbeat_timeout = 10;
        m_socket = null;
        m_thread = null;
        m_buffer = new byte[BUFFER_SIZE];
        m_payload = new byte[PAYLOAD_MAX_SIZE];
        m_ava_bytes = null;
        m_buffer_index = 0;
        m_payload_index = 0;
        m_packet_index = -1;
        m_db_id = -1;
        m_current_query_type = QUERY_UNKNOWN;
        m_cmp_marker = new byte[PACKET_MARKER_LENGTH];

        m_packet_marker_bytes = PACKET_MARKER.getBytes();
        /*try
        {
            m_packet_marker_bytes = PACKET_MARKER.getBytes("UTF-8");
        }
        catch (UnsupportedEncodingException ignored){}*/
        m_crc32_table = new int[256];
        m_input = null;
        m_output = null;
        m_query_template = new byte[BUFFER_SIZE];
        System.arraycopy(m_packet_marker_bytes, 0, m_query_template, 0, PACKET_MARKER_LENGTH);
        int crc;
        for (int i = 0; i < 256; i++)
        {
            crc = i;
            for (int j = 0; j < 8; j++)
            {
                crc = ((crc & 1) != 0) ? ((crc >>> 1) ^ 0xEDB88320) : (crc >>> 1);
            }
            m_crc32_table[i] = crc;
        }
        m_qdispatcher = new BeaconQueryDispatcher();
        m_heartbeat_source = new HeartbeatSource();
    }

    public void setAddress(String host, int port)
    {
        m_host = host;
        m_port = port;
    }

    public void setAuthorization(String crew, String password, String watcher)
    {
        m_crew = crew;
        m_password = password;
        m_watcher = watcher;
    }

    private long bytesToLong(byte[] array, int pos)
    {
        long ret;
        int i = pos + 8;
        ret = 0x00000000000000FF & array[--i];
        ret <<= 8;
        ret |= 0x00000000000000FF & array[--i];
        ret <<= 8;
        ret |= 0x00000000000000FF & array[--i];
        ret <<= 8;
        ret |= 0x00000000000000FF & array[--i];
        ret <<= 8;
        ret |= 0x00000000000000FF & array[--i];
        ret <<= 8;
        ret |= 0x00000000000000FF & array[--i];
        ret <<= 8;
        ret |= 0x00000000000000FF & array[--i];
        ret <<= 8;
        ret |= 0x00000000000000FF & array[--i];
        return ret;
    }

    private int bytesToInt(byte[] array, int pos)
    {
        int ret;
        int i = pos + 4;
        ret = 0x000000FF & array[--i];
        ret <<= 8;
        ret |= 0x000000FF & array[--i];
        ret <<= 8;
        ret |= 0x000000FF & array[--i];
        ret <<= 8;
        ret |= 0x000000FF & array[--i];
        return ret;
    }

    private short bytesToShort(byte[] array, int pos)
    {
        short ret;
        int i = pos + 2;
        ret = (short)(0x00FF & array[--i]);
        ret <<= 8;
        ret |= 0x00FF & array[--i];
        return ret;
    }

    private boolean processPacketHeader()
    {
        m_is_last_packet = false;
        int bpos = m_packet_index + PACKET_MARKER_LENGTH + 1;
        if (bpos + 3 < m_buffer_index)
        {
            byte cf = m_buffer[bpos++];
            if ((cf & 0x01) == 0) //is first packet
            {
                if (bpos + 14 < m_buffer_index)
                {
                    m_data_length = bytesToLong(m_buffer, bpos);
                    bpos += 8;
                    m_crc32 = bytesToInt(m_buffer, bpos);
                    bpos += 4;
                    m_payload_index = 0;
                }
            }
            short packet_size = bytesToShort(m_buffer, bpos);
            bpos += 2;
            if (bpos < m_buffer_index)
            {
                int ni = bpos + packet_size;
                if (ni <= m_buffer_index)
                {
                    System.arraycopy(m_buffer, bpos, m_payload, m_payload_index, packet_size);
                    m_payload_index += packet_size;
                    removePacket(ni);
                    m_is_last_packet = (cf & 0x02) != 0;
                    return true;
                }
            }
        }
        return false;
    }

    private boolean processAvatarHeader()
    {
        m_is_last_packet = false;
        int bpos = m_packet_index + PACKET_MARKER_LENGTH + 1;
        if (bpos + 3 < m_buffer_index)
        {
            byte cf = m_buffer[bpos++];
            if ((cf & 0x01) == 0) //is first packet
            {
                if (bpos + 14 < m_buffer_index)
                {
                    m_data_length = bytesToLong(m_buffer, bpos);
                    bpos += 8;
                    m_crc32 = bytesToInt(m_buffer, bpos);
                    bpos += 4;
                    m_ava_bytes = new byte[(int)m_data_length];
                    m_payload_index = 0;
                }
            }
            short packet_size = bytesToShort(m_buffer, bpos);
            bpos += 2;
            if (bpos < m_buffer_index)
            {
                int ni = bpos + packet_size;
                if (ni <= m_buffer_index)
                {
                    System.arraycopy(m_buffer, bpos, m_ava_bytes, m_payload_index, packet_size);
                    m_payload_index += packet_size;
                    removePacket(ni);
                    m_is_last_packet = (cf & 0x02) != 0;
                    return true;
                }
            }
        }
        return false;
    }

    private int crc32(byte[] buf, int len)
    {
        int idx;
        int  crc = 0xFFFFFFFF;
        for (int i = 0; i < len; i++)
        {
            idx = (crc ^ buf[i]) & 0x000000FF;
            crc = m_crc32_table[idx] ^ (crc >>> 8);
        }
        return ~crc;
    }

    private int crc32(byte[] buf)
    {
        return crc32(buf, buf.length);
    }

    private void removePacket(int idx)
    {
        if (idx < m_buffer_index)
        {
            int new_idx = 0;
            for (int i = idx; i < m_buffer_index;) m_buffer[new_idx++] = m_buffer[i++];
        }
        m_buffer_index -= idx;
        m_packet_index = -1;
    }

    private int findStartMarker()
    {
        if (m_buffer_index >= PACKET_MARKER_LENGTH + 1)
        {
            int lim = m_buffer_index - PACKET_MARKER_LENGTH;
            for (int idx = 0; idx <= lim; idx++)
            {
                System.arraycopy(m_buffer, idx, m_cmp_marker, 0, PACKET_MARKER_LENGTH);
                if (Arrays.equals(m_cmp_marker, m_packet_marker_bytes))
                {
                    m_packet_index = idx;
                    return (0x000000FF & m_buffer[idx + PACKET_MARKER_LENGTH]);
                }
            }
        }
        return QUERY_UNKNOWN;
    }

    private int longToBytes(long value, byte[] arr, int idx)
    {
        arr[idx++] = (byte)(value & 0x00000000000000FF);
        value >>>= 8;
        arr[idx++] = (byte)(value & 0x00000000000000FF);
        value >>>= 8;
        arr[idx++] = (byte)(value & 0x00000000000000FF);
        value >>>= 8;
        arr[idx++] = (byte)(value & 0x00000000000000FF);
        value >>>= 8;
        arr[idx++] = (byte)(value & 0x00000000000000FF);
        value >>>= 8;
        arr[idx++] = (byte)(value & 0x00000000000000FF);
        value >>>= 8;
        arr[idx++] = (byte)(value & 0x00000000000000FF);
        value >>>= 8;
        arr[idx++] = (byte)(value & 0x00000000000000FF);
        return idx;
    }

    private int intToBytes(int value, byte[] arr, int idx)
    {
        arr[idx++] = (byte)(value & 0x000000FF);
        value >>>= 8;
        arr[idx++] = (byte)(value & 0x000000FF);
        value >>>= 8;
        arr[idx++] = (byte)(value & 0x000000FF);
        value >>>= 8;
        arr[idx++] = (byte)(value & 0x000000FF);
        return idx;
    }

    private int shortToBytes(short value, byte[] arr, int idx)
    {
        arr[idx++] = (byte)(value & 0x00FF);
        value >>>= 8;
        arr[idx++] = (byte)(value & 0x00FF);
        return idx;
    }

    private void send(int query_type, byte[] payload) throws IOException
    {
        m_query_template[PACKET_MARKER_LENGTH] = (byte)query_type;
        if (payload != null)
        {
            long payload_size = payload.length;
            short packet_size;
            int ni, wi;
            int i = 0;
            boolean cnt;
            do
            {
                wi = PACKET_MARKER_LENGTH + 1;
                ni = i +  PACKET_MAX_SIZE;
                cnt = ni < payload_size;
                if (i > 0)
                {
                    m_query_template[wi++] = (byte)(cnt ? 1 : 3); //carry flag
                }
                else
                {   //first packet
                    m_query_template[wi++] = (byte)(cnt ? 0 : 2); //carry flag
                    wi = longToBytes(payload_size, m_query_template, wi);
                    wi = intToBytes(crc32(payload), m_query_template, wi);
                }
                packet_size =  (short)(cnt ? PACKET_MAX_SIZE : payload_size - i);
                wi = shortToBytes(packet_size, m_query_template, wi);
                System.arraycopy(payload, i, m_query_template, wi, packet_size);
                m_output.write(m_query_template, 0, wi + packet_size);
                i = ni;
            }
            while (cnt);
        }
        else
        {
            m_output.write(m_query_template, 0, PACKET_MARKER_LENGTH + 1);
        }
        m_output.flush();
    }

    private void sendAuthorization()
    {
        int len = 7 + m_crew.length() + m_watcher.length() + m_password.length();
        byte[] payload = new byte[len];
        int wi = 0;
        payload[wi++] = (byte)CREW_CLIENT_TYPE;
        byte[] s;

        len = m_crew.length();
        wi = shortToBytes((short)len, payload, wi);
        s = m_crew.getBytes();
        System.arraycopy(s, 0, payload, wi, len);
        wi += len;

        len = m_watcher.length();
        wi = shortToBytes((short)len, payload, wi);
        s = m_watcher.getBytes();
        System.arraycopy(s, 0, payload, wi, len);
        wi += len;

        len = m_password.length();
        wi = shortToBytes((short)len, payload, wi);
        s = m_password.getBytes();
        System.arraycopy(s, 0, payload, wi, len);

        //Log.d(LOG_TAG, "[Watcher] Try authorized as crew=\"" + m_crew + "\"; watcher=\"" + m_watcher + "\"; password=\"" + m_password + "\"");

        try
        {
            send(QUERY_AUTHORIZATION, payload);
            //Log.d(LOG_TAG, "[Watcher] Authorization sended.");
        }
        catch (IOException e)
        {
            //Log.d(LOG_TAG, "[Watcher] Authorization send failed" + e.toString());
        }
    }

    private boolean onQueryAuthorization()
    {
        //Log.d(LOG_TAG, "[Watcher] Server authorization request...");
        removePacket(m_packet_index + PACKET_MARKER_LENGTH + 1);
        sendAuthorization();
        m_current_query_type = QUERY_UNKNOWN;
        return true;
    }

    private boolean onQueryAuthorizationOk()
    {
        if (processPacketHeader())
        {
            if (m_is_last_packet && m_data_length == m_payload_index)
            {
                if (m_crc32 == crc32(m_payload, m_payload_index))
                {
                    //Log.d(LOG_TAG, "[Watcher] Authorization success.");
                    m_db_id = bytesToInt(m_payload, 0);
                    m_heartbeat_timeout = bytesToInt(m_payload, 4);
                    byte[] payload = new byte[5];
                    payload[0] = (byte)CREW_CLIENT_TYPE;
                    intToBytes(m_db_id, payload, 1);
                    m_qdispatcher.sendQuery(QUERY_AVATAR_CHANGETIME, payload);
                    m_qdispatcher.sendQuery(QUERY_MEMBERS_LIST, null);
                    m_heartbeat_source.start();
                    m_listener.connected();
                }
            }
            m_current_query_type = QUERY_UNKNOWN;
            return true;
        }
        return false;
    }

    private boolean onQueryClientHeartbeat()
    {
        removePacket(m_packet_index + PACKET_MARKER_LENGTH + 1);
        m_current_query_type = QUERY_UNKNOWN;
        return true;
    }

    private boolean onQueryServerHeartbeat()
    {
        removePacket(m_packet_index + PACKET_MARKER_LENGTH + 1);
        m_qdispatcher.sendQuery(QUERY_SERVER_HEARTBEAT, null);
        //Log.d(LOG_TAG, "[Watcher] Server heartbeat.");
        m_current_query_type = QUERY_UNKNOWN;
        return true;
    }

    private boolean onQueryAvatarChangeTime()
    {
        if (processPacketHeader())
        {
            if (m_is_last_packet && m_data_length == m_payload_index)
            {
                if (m_crc32 == crc32(m_payload, m_payload_index))
                {
                    byte client_type = m_payload[0];
                    int db_id = bytesToInt(m_payload, 1);
                    int ava_changetime = bytesToInt(m_payload, 5);
                    if (ava_changetime != m_listener.findAvatar(client_type, db_id))
                    {
                        byte[] payload = new byte[5];
                        System.arraycopy(m_payload, 0, payload, 0, 5);
                        m_qdispatcher.sendQuery(QUERY_AVATAR, payload);
                    }
                }
            }
            m_current_query_type = QUERY_UNKNOWN;
            return true;
        }
        return false;
    }

    private boolean onQueryAvatar()
    {
        if (processAvatarHeader())
        {
            if (m_is_last_packet && m_payload_index > 9 && m_data_length == m_payload_index)
            {
                if (m_crc32 == crc32(m_ava_bytes, m_payload_index))
                {
                    byte client_type = m_ava_bytes[0];
                    int db_id = bytesToInt(m_ava_bytes, 1);
                    int ava_changetime = bytesToInt(m_ava_bytes, 5);
                    m_listener.avatarChanged(client_type, db_id, ava_changetime, m_ava_bytes, m_payload_index);
                    m_ava_bytes = null;
                }
            }
            m_current_query_type = QUERY_UNKNOWN;
            return true;
        }
        return false;
    }

    private boolean onQueryMembersList()
    {
        if (processPacketHeader())
        {
            if (m_is_last_packet && m_data_length == m_payload_index)
            {
                if (m_crc32 == crc32(m_payload, m_payload_index))
                {
                    short count = bytesToShort(m_payload, 0);
                    if (count > 0)
                    {
                        int[] members = new int[count];
                        int idx = 2;
                        for (short i = 0; i < count; i++)
                        {
                            members[i] = bytesToInt(m_payload, idx);
                            idx += 4;
                        }
                        m_listener.membersListChanged(members);
                        for (short i = 0; i < count; i++)
                        {
                            byte[] payload = new byte[4];
                            intToBytes(members[i], payload, 0);
                            m_qdispatcher.sendQuery(QUERY_MEMBER, payload);
                        }
                        m_qdispatcher.sendQuery(QUERY_MEMBERS_ONLINE, null);
                    }
                    else m_listener.membersListChanged(null);
                }
            }
            m_current_query_type = QUERY_UNKNOWN;
            return true;
        }
        return false;
    }

    private boolean onQueryMember()
    {
        if (processPacketHeader())
        {
            if (m_is_last_packet && m_data_length == m_payload_index)
            {
                if (m_crc32 == crc32(m_payload, m_payload_index))
                {
                    int idx = 0;
                    int db_id = bytesToInt(m_payload, idx);
                    idx += 4;
                    int ava_changetime = bytesToInt(m_payload, idx);
                    idx += 4;
                    boolean tracking_permission = m_payload[idx++] != 0;
                    boolean recording_permission = m_payload[idx++] != 0;
                    int tracking_timeout = bytesToInt(m_payload, idx);
                    idx += 4;
                    short len = bytesToShort(m_payload, idx);
                    idx += 2;
                    String member_name = "";
                    if (len > 0) member_name = new String(m_payload, idx, len);
                    idx += len;
                    len = bytesToShort(m_payload, idx);
                    idx += 2;
                    String member_note = "";
                    if (len > 0) member_note = new String(m_payload, idx, len);
                    boolean need_ava = ava_changetime != m_listener.findAvatar((byte)MEMBER_CLIENT_TYPE, db_id);
                    m_listener.memberChanged(db_id, member_name, member_note, ava_changetime, tracking_permission, recording_permission, tracking_timeout);
                    if (need_ava)
                    {
                        byte[] payload = new byte[5];
                        payload[0] = (byte)MEMBER_CLIENT_TYPE;
                        intToBytes(db_id, payload, 1);
                        m_qdispatcher.sendQuery(QUERY_AVATAR, payload);
                    }
                }
            }
            m_current_query_type = QUERY_UNKNOWN;
            return true;
        }
        return false;
    }

    private boolean onQueryMembersOnline()
    {
        if (processPacketHeader())
        {
            if (m_is_last_packet && m_data_length == m_payload_index)
            {
                if (m_crc32 == crc32(m_payload, m_payload_index))
                {
                    short count = bytesToShort(m_payload, 0);
                    if (count > 0)
                    {
                        int[] members = new int[count];
                        int idx = 2;
                        for (short i = 0; i < count; i++)
                        {
                            members[i] = bytesToInt(m_payload, idx);
                            idx += 4;
                        }
                        m_listener.membersOnlineChanged(members);
                    }
                    else m_listener.membersOnlineChanged(null);
                }
            }
            m_current_query_type = QUERY_UNKNOWN;
            return true;
        }
        return false;
    }

    private boolean onQueryMemberConnected()
    {
        if (processPacketHeader())
        {
            if (m_is_last_packet && m_data_length == m_payload_index)
            {
                if (m_crc32 == crc32(m_payload, m_payload_index))
                {
                    int db_id = bytesToInt(m_payload, 0);
                    m_listener.memberConnected(db_id);
                }
            }
            m_current_query_type = QUERY_UNKNOWN;
            return true;
        }
        return false;
    }

    private boolean onQueryMemberDisconnected()
    {
        if (processPacketHeader())
        {
            if (m_is_last_packet && m_data_length == m_payload_index)
            {
                if (m_crc32 == crc32(m_payload, m_payload_index))
                {
                    int db_id = bytesToInt(m_payload, 0);
                    m_listener.memberDisconnected(db_id);
                }
            }
            m_current_query_type = QUERY_UNKNOWN;
            return true;
        }
        return false;
    }

    private boolean onQueryBeaconState()
    {
        if (processPacketHeader())
        {
            if (m_is_last_packet && m_data_length == m_payload_index)
            {
                if (m_crc32 == crc32(m_payload, m_payload_index))
                {
                    int db_id = bytesToInt(m_payload, 0);
                    byte state = m_payload[4];
                    m_listener.memberBeaconStateChanged(db_id, state);
                }
            }
            m_current_query_type = QUERY_UNKNOWN;
            return true;
        }
        return false;
    }

    private boolean onQueryLocation()
    {
        if (processPacketHeader())
        {
            if (m_is_last_packet && m_data_length == m_payload_index)
            {
                if (m_crc32 == crc32(m_payload, m_payload_index))
                {
                    int idx = 0;
                    int db_id = bytesToInt(m_payload, idx);
                    idx += 4;
                    int time = bytesToInt(m_payload, idx);
                    idx += 4;
                    double latitude = Double.longBitsToDouble(bytesToLong(m_payload, idx));
                    idx += 8;
                    double longitude = Double.longBitsToDouble(bytesToLong(m_payload, idx));
                    idx += 8;
                    double speed = Double.longBitsToDouble(bytesToLong(m_payload, idx));
                    idx += 8;
                    boolean gps = m_payload[idx] != 0;
                    m_listener.memberLocationChanged(db_id, time, latitude, longitude, speed, gps);
                }
            }
            m_current_query_type = QUERY_UNKNOWN;
            return true;
        }
        return false;
    }

    private boolean onQuerySetTrackingPermission()
    {
        if (processPacketHeader())
        {
            if (m_is_last_packet && m_data_length == m_payload_index)
            {
                if (m_crc32 == crc32(m_payload, m_payload_index))
                {
                    int db_id = bytesToInt(m_payload, 0);
                    byte state = m_payload[4];
                    m_listener.memberTrackingPermissionChanged(db_id, state);
                }
            }
            m_current_query_type = QUERY_UNKNOWN;
            return true;
        }
        return false;
    }

    private boolean onQuerySetRecordingPermission()
    {
        if (processPacketHeader())
        {
            if (m_is_last_packet && m_data_length == m_payload_index)
            {
                if (m_crc32 == crc32(m_payload, m_payload_index))
                {
                    int db_id = bytesToInt(m_payload, 0);
                    byte state = m_payload[4];
                    m_listener.memberRecordingPermissionChanged(db_id, state);
                }
            }
            m_current_query_type = QUERY_UNKNOWN;
            return true;
        }
        return false;
    }


    public synchronized void start()
    {
        if (!m_is_active)
        {
            m_is_active = true;
            m_thread = new Thread(this);
            m_thread.start();
        }
    }

    public synchronized void stop()
    {
        m_is_active = false;
        m_heartbeat_source.stop();
        m_qdispatcher.stop();
        if (m_socket != null && !m_socket.isClosed())
        {
            try
            {
                m_socket.close();
            }
            catch (IOException ignored){}
        }
        synchronized (m_synch)
        {
            m_synch.notify();
        }
        /*if (m_thread != null && m_thread.isAlive())
        {
            try
            {
                m_thread.join();
            }
            catch (InterruptedException ignored){}
        }*/
    }

    private boolean doConnecting()
    {
        //m_is_authorized = false;
        Socket socket = new Socket();
        try
        {
            socket.connect(new InetSocketAddress(m_host, m_port), 3000);
            m_input = socket.getInputStream();
            m_output = socket.getOutputStream();
        }
        catch (IOException e)
        {
            socket = null;
            //Log.d(LOG_TAG, "[Watcher] Connecting error: " + e.toString());
        }
        if (socket != null)
        {
            m_socket = socket;
            m_tcp_status = TCP_CLIENT_ONLINE_WORK;
            return false;
        }
        Timer timer = new Timer();
        timer.schedule(new TimerTask()
        {
            @Override
            public void run()
            {
                synchronized (m_synch)
                {
                    m_synch.notify();
                }
            }
        }, 10000);
        return true;
    }

    private void doOnlineWork()
    {
        Log.d(LOG_TAG, "[Watcher] Online.");
        m_qdispatcher.start();
        int n,rcount;
        n = BUFFER_SIZE;
        m_buffer_index = 0;
        m_current_query_type = QUERY_UNKNOWN;
        boolean cnt;
        m_last_read = System.currentTimeMillis();
        while (m_is_active)
        {
            try
            {
                rcount = m_input.read(m_buffer, m_buffer_index, n);
                //Log.d(LOG_TAG, "[Watcher] buffer_index=" + m_buffer_index + "; n=" + n);
            }
            catch (IOException e) //возникает при внезапной потере свзязи с сервером
            {
                rcount = 0;
                //Log.d(LOG_TAG, "[Watcher] Connection error: " + e.toString());
            }
            if (rcount > 0)
            {
                m_last_read = System.currentTimeMillis();
                m_buffer_index += rcount;
                cnt = true;
                do
                {
                    if (m_current_query_type == QUERY_UNKNOWN) m_current_query_type = findStartMarker();
                    switch (m_current_query_type)
                    {
                        case QUERY_UNKNOWN:
                            cnt = false;
                            break;
                        case QUERY_CLIENT_HEARTBEAT:
                            cnt = onQueryClientHeartbeat();
                            break;
                        case QUERY_SERVER_HEARTBEAT:
                            cnt = onQueryServerHeartbeat();
                            break;
                        case QUERY_AUTHORIZATION:
                            cnt = onQueryAuthorization();
                            break;
                        case QUERY_AUTHORIZATION_OK:
                            cnt = onQueryAuthorizationOk();
                            break;
                        case QUERY_AVATAR_CHANGETIME:
                            cnt = onQueryAvatarChangeTime();
                            break;
                        case QUERY_AVATAR:
                            cnt = onQueryAvatar();
                            break;
                        case QUERY_MEMBERS_LIST:
                            cnt = onQueryMembersList();
                            break;
                        case QUERY_MEMBER:
                            cnt = onQueryMember();
                            break;
                        case QUERY_MEMBERS_ONLINE:
                            cnt = onQueryMembersOnline();
                            break;
                        case QUERY_MEMBER_CONNECTED:
                            cnt = onQueryMemberConnected();
                            break;
                        case QUERY_MEMBER_DISCONNECTED:
                            cnt = onQueryMemberDisconnected();
                            break;
                        case QUERY_BEACON_STATE:
                            cnt = onQueryBeaconState();
                            break;
                        case QUERY_LOCATION:
                            cnt = onQueryLocation();
                            break;
                        case QUERY_TRACKING_PERMISSION:
                            cnt = onQuerySetTrackingPermission();
                            break;
                        case QUERY_RECORDING_PERMISSION:
                            cnt = onQuerySetRecordingPermission();
                            break;
                    }
                } while (cnt);
                n = BUFFER_SIZE - m_buffer_index;
            }
            else
            {
                m_heartbeat_source.stop();
                m_qdispatcher.stop();
                if (m_tcp_status == TCP_CLIENT_ONLINE_WORK) m_tcp_status = TCP_CLIENT_CONNECTING;
                if (m_socket != null && !m_socket.isClosed())
                {
                    try
                    {
                        m_socket.close();
                    }
                    catch (IOException ignored){}
                }
                m_socket = null;
                m_listener.disconnected();
                break;
            }
        }
    }

    @Override
    public void run()
    {
        m_tcp_status = TCP_CLIENT_CONNECTING;
        m_socket = null;
        while (m_is_active)
        {
            if (m_tcp_status == TCP_CLIENT_CONNECTING)
            {
                if (doConnecting() && m_is_active)
                {
                    synchronized (m_synch)
                    {
                        try
                        {
                            m_synch.wait();
                        }
                        catch (InterruptedException ignored){}
                    }
                }
            }
            else
            {
                if (m_tcp_status == TCP_CLIENT_ONLINE_WORK) doOnlineWork();
                else m_is_active = false;
            }
        }
        m_thread = null;
    }

    public void requestStartMemberBeacon(int member_id)
    {
        byte[] payload = new byte[4];
        intToBytes(member_id, payload, 0);
        m_qdispatcher.sendQuery(QUERY_START_BEACON, payload);
    }

    public void requestStopMemberBeacon(int member_id)
    {
        byte[] payload = new byte[4];
        intToBytes(member_id, payload, 0);
        m_qdispatcher.sendQuery(QUERY_STOP_BEACON, payload);
    }
}
