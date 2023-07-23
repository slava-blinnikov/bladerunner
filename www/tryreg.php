<?php
session_start();

if (isset($_POST['regname']))
{
	$user_name = $_POST['regname'];
	if ($user_name == '')
	{
		unset($user_name);
	}
}
if (isset($_POST['regpass1']))
{
	$password = $_POST['regpass1'];
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
		$errmsg = 'Извините, у нас технические неполадки. Повторите попытку регистрации позже.';
	}
	else
	{
		$result = pg_query_params($dispconn, 'select crew from meta.crews where usrname = $1', array($user_name));
		if (!$result)
		{
			$errmsg = 'Ошибка базы данных: ' . pg_last_error($dispconn);
		}
		else
		{
			if (pg_num_rows($result) >= 1)
			{
				$errmsg = 'Пользователь с таким именем уже существует!';
			}
			else
			{
				$result = pg_query_params($dispconn, 'select meta.add_crew($1, $2)', array($user_name, $password));
				if (!$result)
				{
					$errmsg = 'Ошибка базы данных: ' . pg_last_error($dispconn);
				}
				else
				{
					$user_id = pg_fetch_row($result)[0];
					$result = pg_query_params($dispconn, 'select rolname from meta.crews where crew = $1', array($user_id));
					$role_name = pg_fetch_row($result)[0];
					pg_close($dispconn);
					$usrconn = pg_connect('host=localhost port=5432 dbname=bladerunner user=' . $role_name . ' password=' . $password);
					if (!$usrconn)
					{
						$errmsg = 'Непредвиденная ошибка базы данных! user=' . $role_name . ' pass=' . $password;
					}
					else
					{
						$_SESSION['user_name'] = $user_name;
						$_SESSION['role_name'] = $role_name;
						$_SESSION['password']  = $password;
						$_SESSION['user_id']   = $user_id;
						$_SESSION['user_ava']  = '';
						if (!mkdir('users/' . $role_name, 0777))
						{
							$errmsg = 'Ошибка создания каталога пользователя!';
						}
						$errmsg = 'Добро пожаловать, ' . $user_name . '!';
						header("Location:index.php");
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