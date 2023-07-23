<?php
session_start();

if (empty($_SESSION['user_name']) or empty($_SESSION['user_id']) or empty($_SESSION['role_name']) or empty($_SESSION['password']))
{
	unset($_SESSION['user_name']);
	unset($_SESSION['user_id']);
	unset($_SESSION['role_name']);
	unset($_SESSION['password']);
	unset($_SESSION['user_ava']);
	echo '
<html lang="ru">
	<head>
		<meta charset="utf-8" />
		<title>Семья рядом. Не упускай родных из виду!</title>
		<meta name="description" content="GPS tracker: Семья рядом" />
		<meta name="author" content="Вячеслав Блинников" />
		<meta name="keywords" content="Семья рядом, Family Near, GPS, GPS tracker, карты, транспорт, мобильные устройства, слежение" />
		<link rel="stylesheet" href="style.css" />
	</head>
	<body>
		<div align="center">
			<a href="index.php"><img src="images/fnlogo.png"/></a>
		</div>
		<form id="login_form" action="login.php" method="post">
			<h1>Присоединяйся</h1>
			<fieldset class="inputs">
				<input name="username" class="text_field" type="text" placeholder="Имя" autofocus required />
				<input name="password" class="password_field" type="password" placeholder="Пароль" required />
			</fieldset>
			<fieldset class="actions">
				<input type="submit" class="submit_button" value="ВОЙТИ" />
				<a href="register.php" style="float: right;">Регистрация</a>
			</fieldset>
		</form>
	</body>
</html>';
}
else
{
	header('Location:menu.php?t=members');
}