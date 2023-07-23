var regname_ok;
regname_ok = true;

function checkPassword()
{
	pass1 = document.getElementById('regpass1');
	input1 = pass1.value;
	img1_ok = document.getElementById('pass1_check_success');
	img1_fail = document.getElementById('pass1_check_failed');
	img2_ok = document.getElementById('pass2_check_success');
	img2_fail = document.getElementById('pass2_check_failed');
	if (input1 == '')
	{
		img1_ok.className = 'img_hidden';
		img1_fail.className = 'img_hidden';
		img2_ok.className = 'img_hidden';
		img2_fail.className = 'img_hidden';
	}
	else
	{
		msg_span = document.getElementById('error_message');
		if (input1.length < 3)
		{
			img1_ok.className = 'img_hidden';
			img1_fail.className = 'img_displayed';
			img2_ok.className = 'img_hidden';
			img2_fail.className = 'img_hidden';

			msg_span.innerHTML = 'Пароль должен состоять не менее чем из трёх символов!';
			msg_span.className = 'msg_displayed';
		}
		else
		{
			img1_ok.className = 'img_displayed';
			img1_fail.className = 'img_hidden';
			pass2 = document.getElementById('regpass2');
			input2 = pass2.value;
			if (input1 == input2)
			{
				img2_ok.className = 'img_displayed';
				img2_fail.className = 'img_hidden';
				msg_span.className = 'msg_hidden';
				return true;
			}
			else
			{
				img2_ok.className = 'img_hidden';
				img2_fail.className = 'img_displayed';
				msg_span.innerHTML = 'Пароли не совпадают!';
				msg_span.className = 'msg_displayed';
			}
		}
	}
	return false;
}

function checkName(input)
{
	if (input != '')
	{
		var xmlhttp = getXmlHttp();
		if (xmlhttp)
		{
			xmlhttp.open('POST', 'username_validate.php', true);
			xmlhttp.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
			xmlhttp.send('q=' + encodeURIComponent(input));
			xmlhttp.onreadystatechange = function ()
			{
				if (xmlhttp.readyState == 4)
				{
					if (xmlhttp.status == 200)
					{
						img_ok = document.getElementById('name_check_success');
						img_fail = document.getElementById('name_check_failed');
						msg_span = document.getElementById('error_message');

						response = xmlhttp.responseXML.documentElement;
						res_tag = response.getElementsByTagName('result')[0];
						regname_ok = res_tag.firstChild.data == 'f';
						if (regname_ok)
						{
							img_ok.className = 'img_displayed';
							img_fail.className = 'img_hidden';
							msg_span.className = 'msg_hidden';
						}
						else
						{
							img_ok.className = 'img_hidden';
							img_fail.className = 'img_displayed';
							msg_span.innerHTML = 'Имя "'+ input + '" уже занято!';
							msg_span.className = 'msg_displayed';
						}
					}
				}
			};
		}
	}
	else
	{
		img_ok = document.getElementById('name_check_success');
		img_fail = document.getElementById('name_check_failed');
		msg_span = document.getElementById('error_message');
		img_ok.className = 'img_hidden';
		img_fail.className = 'img_hidden';
		msg_span.className = 'msg_hidden';
		regname_ok = false;
	}
}

function checkNewName(input, old_name)
{
	if (input != '')
	{
		if (input == old_name)
		{
			img_ok = document.getElementById('name_check_success');
			img_fail = document.getElementById('name_check_failed');
			msg_span = document.getElementById('error_message');
			img_ok.className = 'img_hidden';
			img_fail.className = 'img_hidden';
			msg_span.className = 'msg_hidden';
			regname_ok = true;
		}
		else
		{
			var xmlhttp = getXmlHttp();
			if (xmlhttp)
			{
				xmlhttp.open('POST', 'username_validate.php', true);
				xmlhttp.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
				xmlhttp.send('q=' + encodeURIComponent(input));
				xmlhttp.onreadystatechange = function ()
				{
					if (xmlhttp.readyState == 4)
					{
						if (xmlhttp.status == 200)
						{
							img_ok = document.getElementById('name_check_success');
							img_fail = document.getElementById('name_check_failed');
							msg_span = document.getElementById('error_message');

							response = xmlhttp.responseXML.documentElement;
							res_tag = response.getElementsByTagName('result')[0];
							regname_ok = res_tag.firstChild.data == 'f';
							if (regname_ok)
							{
								img_ok.className = 'img_displayed';
								img_fail.className = 'img_hidden';
								msg_span.className = 'msg_hidden';
							}
							else
							{
								img_ok.className = 'img_hidden';
								img_fail.className = 'img_displayed';
								msg_span.innerHTML = 'Имя "' + input + '" уже занято!';
								msg_span.className = 'msg_displayed';
							}
						}
					}
				};
			}
		}
	}
	else
	{
		img_ok = document.getElementById('name_check_success');
		img_fail = document.getElementById('name_check_failed');
		msg_span = document.getElementById('error_message');
		img_ok.className = 'img_hidden';
		img_fail.className = 'img_hidden';
		msg_span.className = 'msg_hidden';
		regname_ok = false;
	}
}

function checkForm()
{
	if (!regname_ok)
	{
		regname = document.getElementById('regname');
		input = regname.value;

		msg_span = document.getElementById('error_message');
		msg_span.innerHTML = 'Имя "'+ input + '" уже занято!';
		msg_span.className = 'msg_displayed';
		return false;
	}
	return checkPassword();
}

function checkProfileForm()
{
	if (!regname_ok)
	{
		regname = document.getElementById('regname');
		input = regname.value;

		msg_span = document.getElementById('error_message');
		msg_span.innerHTML = 'Имя "'+ input + '" уже занято!';
		msg_span.className = 'msg_displayed';
		return false;
	}
	return true;
}

function checkMemberName(input)
{
	if (input != '')
	{
		old_name = document.getElementById('old_name').value;
		if (old_name != '' && input == old_name)
		{
			regname_ok = true;
			return true;
		}
		else
		{
			member_id = document.getElementById('member_id').value;
			var xmlhttp = getXmlHttp();
			if (xmlhttp)
			{
				xmlhttp.open('POST', 'membername_validate.php', true);
				xmlhttp.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
				xmlhttp.send('q=' + encodeURIComponent(input) + '&m=' + encodeURIComponent(member_id));
				xmlhttp.onreadystatechange = function ()
				{
					if (xmlhttp.readyState == 4)
					{
						if (xmlhttp.status == 200)
						{
							img_ok = document.getElementById('name_check_success');
							img_fail = document.getElementById('name_check_failed');
							msg_span = document.getElementById('error_message');

							response = xmlhttp.responseXML.documentElement;
							res_tag = response.getElementsByTagName('result')[0];

							regname_ok = res_tag.firstChild.data == 'f';
							if (regname_ok)
							{
								img_ok.className = 'img_displayed';
								img_fail.className = 'img_hidden';
								msg_span.className = 'msg_hidden';
							}
							else
							{
								img_ok.className = 'img_hidden';
								img_fail.className = 'img_displayed';
								msg_span.innerHTML = 'Имя "' + input + '" уже занято!';
								msg_span.className = 'msg_displayed';
							}
						}
					}
				};
			}
		}
	}
	else
	{
		img_ok = document.getElementById('name_check_success');
		img_fail = document.getElementById('name_check_failed');
		msg_span = document.getElementById('error_message');
		img_ok.className = 'img_hidden';
		img_fail.className = 'img_hidden';
		msg_span.className = 'msg_hidden';
		regname_ok = false;
	}
}

function checkMemberForm()
{
	if (!regname_ok)
	{
		regname = document.getElementById('regname');
		input = regname.value;

		msg_span = document.getElementById('error_message');
		msg_span.innerHTML = 'Имя "'+ input + '" уже занято!';
		msg_span.className = 'msg_displayed';
		return false;
	}
	member_id = document.getElementById('member_id').value;
	regpass1 = document.getElementById('regpass1').value;
	regpass1 = regpass1.trim();
	if (member_id <= 0)
	{
		if (regpass1 == '')
		{
			msg_span = document.getElementById('error_message');
			msg_span.innerHTML = 'Необходимо задать пароль!';
			msg_span.className = 'msg_displayed';
			return false;
		}
	}
	else
	{
		regpass2 = document.getElementById('regpass2').value;
		regpass2 = regpass1.trim();
		if (regpass1 == '' && regpass2 == '') return true;
	}
	return checkPassword();
}