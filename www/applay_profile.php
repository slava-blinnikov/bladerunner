<?php
session_start();

if (empty($_SESSION['user_name']) or empty($_SESSION['user_id']) or empty($_SESSION['role_name']) or empty($_SESSION['password']))
{
	unset($_SESSION['user_name']);
	unset($_SESSION['user_id']);
	unset($_SESSION['role_name']);
	unset($_SESSION['password']);
	unset($_SESSION['user_note']);
	unset($_SESSION['user_ava']);
	header('Location:index.php');
}
else
{
	$role_name = $_SESSION['role_name'];
	$password = $_SESSION['password'];
	$usrconn = pg_connect('host=localhost port=5432 dbname=bladerunner user=' . $role_name . ' password=' . $password);
	if ($usrconn)
	{
		$user_ava = $_SESSION['user_ava'];
		if (!empty($user_ava))
		{
			$avainfo = json_decode($user_ava, true);
			if ($avainfo)
			{
				$ava_file = $avainfo['filename'];
			}
		}
		if (!empty($_FILES['filename']['name']))
		{
			if (is_uploaded_file($_FILES['filename']['tmp_name']))
			{
				$src_file = 'users/' . $role_name . '/avatar_temp.image';
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
								rename($src_file, 'users/' . $role_name . '/avatar.png');
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
							imagepng($isrc, 'users/' . $role_name . '/avatar.png');
							imagedestroy($isrc);
							unlink($src_file);
							$ava_file = 'avatar.png';
						}
					}
				}
			}
		}
		$user_name = stripslashes($_POST['regname']);
		$user_name = htmlspecialchars($user_name);
		$user_name = trim($user_name);
		if (!empty($ava_file))
		{
			$ava_file = '{"filename":"'.$ava_file.'","changetime":"'.date('Y-m-d H:i:s').'"}';
			$result = pg_query_params($usrconn, 'select meta.update_crew($1, $2)', array($user_name, $ava_file));
		}
		else
		{
			$result = pg_query_params($usrconn, 'select meta.update_crew($1, null)', array($user_name));
		}
		if ($result)
		{
			$_SESSION['user_name'] = $user_name;
			$_SESSION['user_ava']  = $ava_file;
			header("Location:menu.php?t=profile");
		}

	}
}

echo '
<body>
	<div align="center">
		<a href="index.php"><img src="images/fnlogo.png"/></a>
			<h2>
				Извините, у нас технические неполадки. Повторите попытку позже.<br/>
				<a href="menu.php?t=profile"><b>Назад</b></a>
			</h2>
	</div>
</body>';