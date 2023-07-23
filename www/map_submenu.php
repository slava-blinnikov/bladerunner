<?php

if (!$usrconn) $usrconn = pg_connect('host=localhost port=5432 dbname=bladerunner user=' . $role_name . ' password=' . $password);
$ok = false;
if ($usrconn)
{
	$result = pg_query($usrconn, 'select member, rolname, usrname, note, avatar from meta.v_members() order by usrname');
	if ($result)
	{
		$n = pg_num_rows($result);
		if ($n >= 1)
		{
			echo '
					<ul class="map_menu">';
			while ($row = pg_fetch_row($result))
			{
				$member_id = $row[0];
				if (empty($m)) $m = $member_id;
				echo '
						<a href="menu.php?t=map&m=' . $member_id . '"'. ($m == $member_id ? ' style="color:black"' : '') .'><li>';
				$member_role = $row[1];
				$path = 'users/' . $role_name . '/' . $member_role;
				$no_ava = true;
				$user_ava = $row[4];
				if ($user_ava != '')
				{
					$avainfo = json_decode($user_ava, true);
					if ($avainfo)
					{
						$ava_file = $avainfo['filename'];
						$no_ava = false;
						echo '<img class="avatar_image" src="' . $path . '/' . $ava_file . '" width="40" height="40"/>';
					}
				}
				if ($no_ava)
				{
					echo '<img class="avatar_image" src="images/no_member_avatar.png" width="40" height="40"/>';
				}
				echo '<span>' . $row[2] . '</span></li></a>';
			}
			echo '
					</ul>';
		}
	}
}