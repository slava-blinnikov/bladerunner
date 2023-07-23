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
	if (isset($_POST['member_id']))
	{
		$member_id = $_POST['member_id'];
		if ($member_id == '')
		{
			unset($member_id);
		}
		if (empty($member_id))
		{
			$errmsg = 'Укажите имя и идентификатор устройства!';
		}
		else
		{
			$member_id = stripslashes($member_id);
			$member_id = htmlspecialchars($member_id);
			$member_id = trim($member_id);

			$role_name = $_SESSION['role_name'];
			$role_pass = $_SESSION['password'];
			$usrconn = pg_connect('host=localhost port=5432 dbname=bladerunner user=' . $role_name . ' password=' . $role_pass);
			if (!$usrconn)
			{
				$errmsg = 'Извините, у нас технические неполадки. Повторите попытку удаления позже.';
			}
			else
			{
				$result = pg_query_params($usrconn, 'select rolname from meta.v_members() where member = $1', array($member_id));
				if (!$result)
				{
					$errmsg = 'Ошибка базы данных: ' . pg_last_error($usrconn);
				}
				else
				{
					if (pg_num_rows($result) <= 0)
					{
						$errmsg = 'Нельзя править чужие устройства!';
					}
					else
					{
						$role_name = pg_fetch_row($result)[0];
						$path = 'users/' . $_SESSION['role_name'] . '/' . $role_name;
						$result = pg_query_params($usrconn, 'select meta.remove_member($1)', array($member_id));
						if (!$result)
						{
							$errmsg = 'Ошибка базы данных: ' . pg_last_error($usrconn);
						}
						else
						{
							removeDirectory($path);
							header('Location:menu.php?t=members');
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
			<a href="menu.php?t=members"><b>Назад</b></a>
		</h2>
	</div>
</body>';