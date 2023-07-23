<?php
function removeDirectory($dir)
{
	if ($objs = glob($dir . '/*'))
	{
		foreach($objs as $obj)
		{
			is_dir($obj) ? removeDirectory($obj) : unlink($obj);
		}
	}
	rmdir($dir);
}

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
	$dispconn = pg_connect('host=localhost port=5432 dbname=bladerunner user=dispatcher password=qwerty');
	if (!$dispconn)
	{
		$errmsg = 'Извините, у нас технические неполадки. Повторите попытку удаления позже.';
	}
	else
	{
		$user_id = $_SESSION['user_id'];
		$result = pg_query_params($dispconn, 'select meta.remove_crew($1)', array($user_id));
		if (!$result)
		{
			$errmsg = 'Ошибка базы данных: ' . pg_last_error($dispconn);
		}
		else
		{
			removeDirectory('users/' . $_SESSION['role_name']);
			unset($_SESSION['user_name']);
			unset($_SESSION['user_id']);
			unset($_SESSION['role_name']);
			unset($_SESSION['password']);
			unset($_SESSION['user_ava']);
			session_destroy();
			header("Location:index.php");
		}
	}
}

echo '
<body>
	<div align="center">
		<a href="index.php"><img src="images/fnlogo.png"/></a>
		<h2>' . $errmsg . '<br/>
			<a href="menu.php?t=members"><b>Назад</b></a>
		</h2>
	</div>
</body>';