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
		<title>Семья рядом. Регистрация.</title>
		<meta name="description" content="GPS tracker: Семья рядом" />
		<meta name="author" content="Вячеслав Блинников" />
		<meta name="keywords" content="Семья рядом, Family Near, GPS, GPS tracker, карты, транспорт, мобильные устройства, слежение" />
		<link rel="stylesheet" href="style.css" />
		<script type="text/javascript" src="utils.js"></script>
		<script type="text/javascript" src="profile_validate.js"></script>
	</head>
	<body>
		<div align="center">
			<a href="index.php"><img src="images/fnlogo.png"/></a>
			<form id="register_form" action="tryreg.php" method="post" onsubmit="return checkForm()">
				<h1>Регистрация</h1>
				<fieldset class="inputs">
					<div align="left">
						<input name="regname"  id="regname" class="text_field" type="text" placeholder="Имя" onblur="checkName(this.value)" autofocus required />
						<img class="img_hidden" id="name_check_failed" src="images/exclamation.png" title="Это имя пользователя уже занято!"/>
						<img class="img_hidden" id="name_check_success" src="images/tick.png"/>
					</div>
					<div align="left">
						<input name="regpass1" id="regpass1" class="password_field" type="password" placeholder="Пароль" onchange="checkPassword()" required />
						<img class="img_hidden" id="pass1_check_failed" src="images/exclamation.png" title="Пароль должен состоять не менее чем из трёх символов!"/>
						<img class="img_hidden" id="pass1_check_success" src="images/tick.png"/>
					</div>
					<div align="left">
						<input name="regpass2" id="regpass2" class="password_field" type="password" placeholder="Подтвердите пароль" onchange="checkPassword()" required />
						<img class="img_hidden" id="pass2_check_failed" src="images/exclamation.png" title="Пароли не совпадают!"/>
						<img class="img_hidden" id="pass2_check_success" src="images/tick.png"/>
					</div>
				</fieldset>
				<fieldset class="actions">
					<input type="submit" class="submit_button" value="ОТПРАВИТЬ" />
					<span id="error_message" class="msg_hidden">ошибка</span>
				</fieldset>
			</form>
		</div>
	</body>
</html>';
}
else
{
	header("Location:index.php");
}