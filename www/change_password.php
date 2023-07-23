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
	$role_name = $_SESSION['role_name'];
	$password = stripslashes($_POST['old_password']);
	$password = htmlspecialchars($password);
	$password = trim($password);
	$usrconn = pg_connect('host=localhost port=5432 dbname=bladerunner user=' . $role_name . ' password=' . $password);
	if ($usrconn)
	{
		$new_password = stripslashes($_POST['regpass1']);
		$new_password = htmlspecialchars($new_password);
		$new_password = trim($new_password);
		$result = pg_query_params($usrconn, 'select meta.set_crew_password($1)', array($new_password));
		if ($result)
		{
			$_SESSION['password'] = $new_password;
			$message = 'Пароль успешно изменён!';
		}
		else
		{
			$message = 'Ошибка базы данных: ' . pg_last_error($usrconn);
		}
	}
	else
	{
		$message = 'Неправильно введён старый пароль!';
	}
}

echo '
<body>
	<div align="center">
		<a href="index.php"><img src="images/fnlogo.png"/></a>
		<h2>' . $message . '<br/>
			<a href="menu.php?t=password"><b>Назад</b></a>
		</h2>
	</div>
</body>';