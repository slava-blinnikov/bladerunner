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
	$user_name = $_SESSION['user_name'];
	$role_name = $_SESSION['role_name'];
	$user_ava = $_SESSION['user_ava'];
	if (!empty($user_ava))
	{
		$avainfo = json_decode($user_ava, true);
		if ($avainfo)
		{
			$ava_file = $avainfo['filename'];
		}
	}
	echo '
<html lang="ru">
	<head>
		<meta charset="utf-8" />
		<title>Семья рядом. '. $user_name .'</title>
		<meta name="description" content="GPS tracker: Семья рядом" />
		<meta name="author" content="Вячеслав Блинников" />
		<meta name="keywords" content="Семья рядом, Family Near, GPS, GPS tracker, карты, транспорт, мобильные устройства, слежение" />
		<link rel="stylesheet" href="style.css" />
	</head>
	<body>
		<div align="center">
			<a href="logout.php"><img src="images/fnlogo.png"/></a>
		</div>
		<div id="login_form">
			<img id="img_avatar" class="avatar_image" src="' . (empty($ava_file) ? 'images/no_crew_avatar.png' : ('users/'.$role_name.'/'.$ava_file)) . '" width="80" height="80"/>
			<label for="img_avatar" class="avatar_caption">'.$user_name.'</label>
			<br/>
			<span class="warning_title">Вы действительно желаете удалить эту прекрасную учётную запись со всеми привязанными к ней устройствами?</span>
			<br/>
			<span class="warning_title">Вглядитесь в глаза этой аватарки. Неужели, Вы хотите стереть её и все её данные с нашего сервера?</span>
			<br/>
			<a href="remove.php"><button class="submit_button">ДА</button></a>
			<a href="menu.php?t=members"><button class="submit_button" style="float: right;">НЕТ!</button></a>
		</div>
	</body>';
}