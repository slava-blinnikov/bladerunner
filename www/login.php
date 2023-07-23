<?php
session_start();

if (isset($_POST['username']))
{
	$user_name = $_POST['username'];
	if ($user_name == '')
	{
		unset($user_name);
	}
}
if (isset($_POST['password']))
{
	$password = $_POST['password'];
	if ($password == '')
	{
		unset($password);
	}
}

if (empty($user_name) or empty($password))
{
	$errmsg = 'Укажите имя пользователя и пароль!';
}
else
{
	$user_name = stripslashes($user_name);
	$user_name = htmlspecialchars($user_name);
	$user_name = trim($user_name);
	$password = stripslashes($password);
	$password = htmlspecialchars($password);
	$password = trim($password);

	$dispconn = pg_connect('host=localhost port=5432 dbname=bladerunner user=dispatcher password=qwerty');
	if (!$dispconn)
	{
		$errmsg = 'Извините, у нас технические неполадки. Повторите попытку входа позже.';
	}
	else
	{
		$result = pg_query_params($dispconn, 'select rolname from meta.crews where usrname = $1', array($user_name));
		if (!$result)
		{
			$errmsg = 'Ошибка базы данных: ' . pg_last_error($dispconn);
		}
		else
		{
			if (pg_num_rows($result) != 1)
			{
				$errmsg = 'Неверное имя пользователя или пароль!';
			}
			else
			{
				$row = pg_fetch_row($result);
				$role_name = $row[0];
				pg_close($dispconn);
				$usrconn = pg_connect('host=localhost port=5432 dbname=bladerunner user=' . $role_name . ' password=' . $password);
				if (!$usrconn)
				{
					$errmsg = 'Неверное имя пользователя или пароль!';
				}
				else
				{
					$result = pg_query($usrconn, 'select crew, avatar from meta.v_crews()');
					if (!$result)
					{
						$errmsg = 'Ошибка базы данных: ' . pg_last_error($usrconn);
					}
					else
					{
						if (pg_num_rows($result) != 1)
						{
							$errmsg = 'Непредвиденная ошибка базы данных!';
						}
						else
						{
							$row = pg_fetch_row($result);
							$_SESSION['user_name'] = $user_name;
							$_SESSION['role_name'] = $role_name;
							$_SESSION['password']  = $password;
							$_SESSION['user_id']   = $row[0];
							$_SESSION['user_ava']  = $row[1];
							$errmsg = 'Добро пожаловать, ' . $user_name . '!';
							header("Location:menu.php?t=members");
						}
					}
				}
			}
		}
	}
}

echo '
<body>
	<div align="center">
		<a href="index.php"><img src="images/fnlogo.png"/></a>
			<h2>' . $errmsg . '<br/>
			<a href="index.php"><b>Назад</b></a>
		</h2>
	</div>
</body>';