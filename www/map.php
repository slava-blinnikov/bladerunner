<?php
if (!empty($m))
{
	$member_id = $m;
	if (!$usrconn) $usrconn = pg_connect('host=localhost port=5432 dbname=bladerunner user=' . $role_name . ' password=' . $password);
	if ($usrconn)
	{
		$result = pg_query_params($usrconn, 'select member, rolname, usrname, note, avatar from meta.v_members() where member = $1', array($member_id));
		if ($result)
		{
			if (pg_num_rows($result) == 1)
			{
				$row = pg_fetch_row($result);
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
						echo '<img id="member_avatar" class="avatar_image" src="' . $path . '/' . $ava_file . '" width="40" height="40"/>';
					}
				}
				if ($no_ava)
				{
					echo '<img id="member_avatar" class="avatar_image" src="images/no_member_avatar.png" width="40" height="40"/>';
				}
				echo '
				<label for="member_avatar" class="avatar_caption" style="position: relative; top: -15px;">' . $row[2] . '</label>
				<table class="map_info_table">
					<tbody>
						<tr>
							<td>Не в сети.</td>
							<td></td>
							<td>Скорость:</td><td><span id="speed_value">0</span>км/ч</td>
						</tr>
					</tbody>
				</table>
				<div id="map" style="width: 100%; height: 94%"></div>';
			}
		}
	}
}
