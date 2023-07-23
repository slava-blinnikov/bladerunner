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
	if (isset($_POST['regnote']))
	{
		$note = $_POST['regnote'];
		if ($note == '')
		{
			unset($note);
		}
	}
	if (empty($user_name) or empty($password))
	{
		$errmsg = 'Укажите имя устройства и пароль!';
	}
	else
	{
		$user_name = stripslashes($user_name);
		$user_name = htmlspecialchars($user_name);
		$user_name = trim($user_name);
		$password = stripslashes($password);
		$password = htmlspecialchars($password);
		$password = trim($password);
		$note = stripslashes($note);
		$note = htmlspecialchars($note);
		$note = trim($note);

		$role_name = $_SESSION['role_name'];
		$role_pass = $_SESSION['password'];
		$usrconn = pg_connect('host=localhost port=5432 dbname=bladerunner user=' . $role_name . ' password=' . $role_pass);
		if (!$usrconn)
		{
			$errmsg = 'Извините, у нас технические неполадки. Повторите попытку регистрации позже.';
		}
		else
		{
			$result = pg_query_params($usrconn, 'select member from meta.v_members() where usrname = $1', array($user_name));
			if (!$result)
			{
				$errmsg = 'Ошибка базы данных: ' . pg_last_error($usrconn);
			}
			else
			{
				if (pg_num_rows($result) >= 1)
				{
					$errmsg = 'Устройство с таким именем у Вас уже есть!';
				}
				else
				{
					$result = pg_query_params($usrconn, 'select meta.add_member($1, $2)', array($user_name, $password));
					if (!$result)
					{
						$errmsg = 'Ошибка базы данных: ' . pg_last_error($usrconn);
					}
					else
					{
						$member_id = pg_fetch_row($result)[0];
						$result = pg_query_params($usrconn, 'select rolname from meta.v_members() where member = $1', array($member_id));
						$ava_ok = true;
						$role_name = pg_fetch_row($result)[0];
						$path = 'users/' . $_SESSION['role_name'] . '/' . $role_name;
						if (!mkdir($path, 0777))
						{
							$errmsg = 'Ошибка создания каталога пользователя!';
							$ava_ok = false;
						}
						if ($ava_ok)
						{
							$ava_ok = false;
							if (!empty($_FILES['filename']['name']))
							{
								if (is_uploaded_file($_FILES['filename']['tmp_name']))
								{
									$src_file = $path . '/avatar_temp.image';
									if (move_uploaded_file($_FILES['filename']['tmp_name'], $src_file))
									{
										$imginfo = getimagesize($src_file);
										if ($imginfo)
										{
											unset($isrc);
											switch (strtolower($imginfo['mime']))
											{
												case "image/png":
													$ava_file = 'avatar.png';
													rename($src_file, $path . '/avatar.png');
													$ava_ok = true;
													break;
												case "image/jpeg":
													$isrc = imagecreatefromjpeg($src_file);
													break;
												case "image/gif":
													$isrc = imagecreatefromgif($src_file);
													break;
												case "image/wbmp":
													$isrc = imagecreatefromwbmp($src_file);
													break;
											}
											if ($isrc)
											{
												imagepng($isrc, $path . '/avatar.png');
												imagedestroy($isrc);
												unlink($src_file);
												$ava_file = 'avatar.png';
												$ava_ok = true;
											}
										}
									}
								}
							}
						}
						if (!empty($ava_file))
						{
							$ava_file = '{"filename":"'.$ava_file.'","changetime":"'.date('Y-m-d H:i:s').'"}';
							$result = pg_query_params($usrconn, 'select meta.update_member($1, $2, $3)', array($member_id, $user_name, $ava_file));
						}
						else
						{
							$result = pg_query_params($usrconn, 'select meta.update_member($1, $2, null)', array($member_id, $user_name));
						}
						if (!$result)
						{
							$errmsg = 'Ошибка базы данных: ' . pg_last_error($usrconn);
						}
						else
						{
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