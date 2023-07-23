<?php
header('Content-Type: text/xml');
echo '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>';
function nameInUse($q)
{
	$dispconn = pg_connect('host=localhost port=5432 dbname=bladerunner user=dispatcher password=qwerty');
	if ($dispconn)
	{
		$user_name = stripslashes($q);
		$user_name = htmlspecialchars($user_name);
		$user_name = trim($user_name);
		$result = pg_query_params($dispconn, 'select exists(select usrname from meta.crews where usrname = $1)', array($user_name));
		if  ($result)
		{
			$r = pg_fetch_result($result, 0, 0);
			return $r;
		}
	}
	return 'f';
}
echo '
<response>
	<result>'.nameInUse($_POST['q']).'</result>
</response>';