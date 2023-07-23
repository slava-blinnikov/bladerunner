<?php
session_start();

if (empty($_SESSION['user_name']) or empty($_SESSION['user_id']) or empty($_SESSION['role_name']) or empty($_SESSION['password']))
{
	unset($_SESSION['user_name']);
	unset($_SESSION['user_id']);
	unset($_SESSION['role_name']);
	unset($_SESSION['password']);
	unset($_SESSION['user_ava']);
	header('Location:index.php');
}
else
{
	header('Content-Type: text/xml');
	echo '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>';

	function nameInUse($q, $m)
	{
		$role_name = $_SESSION['role_name'];
		$password = $_SESSION['password'];
		$usrconn = pg_connect('host=localhost port=5432 dbname=bladerunner user=' . $role_name . ' password=' . $password);
		if ($usrconn)
		{
			$member_name = stripslashes($q);
			$member_name = htmlspecialchars($member_name);
			$member_name = trim($member_name);

			$member_id = stripslashes($m);
			$member_id = htmlspecialchars($member_id);
			$member_id = trim($member_id);

			$result = pg_query_params($usrconn, 'select exists(select * from meta.v_members() where usrname = $1 and member <> $2)', array($member_name, $member_id));
			if ($result)
			{
				$r = pg_fetch_result($result, 0, 0);
				return $r;
			}
			return 't';
		}
		return 'f';
	}

	echo '
<response>
	<result>' . nameInUse($_POST['q'], $_POST['m']) . '</result>
</response>';
}