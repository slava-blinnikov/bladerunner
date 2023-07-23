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
	$t = stripslashes($_GET['t']);
	$t = htmlspecialchars($t);
	$t = trim($t);
	if (empty($t)) $t = 'profile';
	$user_name = $_SESSION['user_name'];
	$user_id = $_SESSION['user_id'];
	$role_name = $_SESSION['role_name'];
	$password = $_SESSION['password'];
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
		<script type="text/javascript" src="utils.js"></script>';
	if ($t === 'map')
	{
		echo '
		<script	type="text/javascript" src="http://api-maps.yandex.ru/2.1/?load=package.standard&lang=ru_RU"></script>
		<script type="text/javascript" src="yandex_map.js"></script>';
	}
	else
	{
		echo '
		<script type="text/javascript" src="profile_validate.js"></script>';
		if ($t === 'members')
		{
			echo '
		<script type="text/javascript" src="member_form.js"></script>';
		}
	}
	echo '
	</head>
	<body>
		<div align="center">
			<a href="logout.php"><img src="images/fnlogo.png"/></a>
		</div>
		<div class="left_menu avatar_panel">
			<img id="img_avatar" class="avatar_image" src="' . (empty($ava_file) ? 'images/no_crew_avatar.png' : ('users/'.$role_name.'/'.$ava_file)) . '" width="80" height="80"/>
			<label for="img_avatar" class="avatar_caption">'.$user_name.'</label>
		</div>
		<div class="left_menu profile_menu_container">
			<ul class="profile_menu">
				<a href="menu.php?t=profile"'. ($t === 'profile' ? ' style="color:black"' : '') .'><li>Учётная запись</li></a>
				<a href="menu.php?t=password"'. ($t === 'password' ? ' style="color:black"' : '') .'><li>Пароль</li></a>
				<a href="menu.php?t=members"'. ($t === 'members' ? ' style="color:black"' : '') .'><li>Устройства</li></a>
				<a href="menu.php?t=map"'. ($t === 'map' ? ' style="color:black"' : '') .'><li>Карта</li></a>';
	if ($t === 'map')
	{
		$m = stripslashes($_GET['m']);
		$m = htmlspecialchars($m);
		$m = trim($m);
		require "map_submenu.php";
	}
	echo '
				<a href="menu.php?t=downloads"'. ($t === 'downloads' ? ' style="color:black"' : '') .'><li>Загрузки</li></a>
			</ul>
			<a href="logout.php"><button class="submit_button">ВЫХОД</button></a>
			<a href="remove_dialog.php" style="float: right;">Удалить учётную запись</a>
		</div>
		<div class="content_container">';

	if ($t === 'profile')
	{
		echo '
			<h1>Параметры учётной записи</h1>
			<div class="inner_content">
				<form action="applay_profile.php" method="post" enctype="multipart/form-data" onsubmit="return checkProfileForm()">
					<fieldset class="inputs">
						<span>Чтобы изменить аватар, укажите файл с изображением:</span><br/>
						<input type="file" name="filename"/>
						<div align="left">
							<input name="regname"  id="regname" class="text_field" type="text" placeholder="Имя" onblur="checkNewName(this.value,\'' . $user_name . '\')" value="'.$user_name.'" required/>
							<img class="img_hidden" id="name_check_failed" src="images/exclamation.png" title="Это имя пользователя уже занято!"/>
							<img class="img_hidden" id="name_check_success" src="images/tick.png"/>
							<span id="error_message" class="msg_hidden">ошибка</span>
						</div>
					</fieldset>
					<fieldset class="actions">
						<input type="submit" class="submit_button" value="ПРИМЕНИТЬ"/>
					</fieldset>
				</form>
			</div>';
	}
	elseif ($t === 'password')
	{
		echo '
			<h1>Смена пароля</h1>
			<div class="inner_content">
				<form action="change_password.php" method="post" enctype="multipart/form-data" onsubmit="return checkPassword()">
					<fieldset class="inputs">
						<input name="old_password" id="old_password" class="password_field" type="password" placeholder="Старый пароль" required />
						<div align="left">
							<input name="regpass1" id="regpass1" class="password_field" type="password" placeholder="Новый пароль" onchange="checkPassword()" required />
							<img class="img_hidden" id="pass1_check_failed" src="images/exclamation.png" title="Пароль должен состоять не менее чем из трёх символов!"/>
							<img class="img_hidden" id="pass1_check_success" src="images/tick.png"/>
						</div>
						<div align="left">
							<input name="regpass2" id="regpass2" class="password_field" type="password" placeholder="Подтвердите новый пароль" onchange="checkPassword()" required />
							<img class="img_hidden" id="pass2_check_failed" src="images/exclamation.png" title="Пароли не совпадают!"/>
							<img class="img_hidden" id="pass2_check_success" src="images/tick.png"/>
						</div>
					</fieldset>
					<fieldset class="actions">
						<input type="submit" class="submit_button" value="ПРИМЕНИТЬ" />
						<span id="error_message" class="msg_hidden">ошибка</span>
					</fieldset>
				</form>
			</div>';
	}
	elseif ($t === 'members')
	{
		require "members_list.php";
	}
	elseif ($t === 'map')
	{
		require "map.php";
	}
	elseif ($t === 'downloads')
	{
		echo '
		<ul>
			<a href="apps/android/beacon/bladerunner.apk"><li>Маячок на телефон</li></a>
			<a href="apps/windows/watcher/bladerunner.zip"><li>Десктопный клиент под Windows</li></a>
			<a href="apps/linux/watcher/bladerunner.zip"><li>Десктопный клиент под Linux</li></a>
		</ul>';
	}

	echo '
		</div>
	</body>
</html>';
}