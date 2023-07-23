<?php
echo '
			<h1>Список подключённых устройств</h1>
			<div class="inner_content">
				<button class="submit_button" onclick="showNewMemberForm()">НОВОЕ УСТРОЙСТВО</button>
				<div id="member_form" class="member_form_hide">
					<h3 id="member_form_title">Новое устройство</h3>
					<form id="mbr_form" method="post" enctype="multipart/form-data">
						<fieldset class="inputs">
							<span id="title_avatar" class="title_hidden">Чтобы изменить аватар, укажите файл с изображением:</span>
							<input type="file" id="filename" name="filename"/>
							<div align="left">
								<input name="regname"  id="regname" class="text_field" type="text" placeholder="Имя" onblur="checkMemberName(this.value)" required/>
								<img class="img_hidden" id="name_check_failed" src="images/exclamation.png" title="Это имя устройства вами уже использовалось!"/>
								<img class="img_hidden" id="name_check_success" src="images/tick.png"/>
							</div>
							<br/>
							<span id="title_password" class="title_hidden">Для смены пароля заполните поле пароля и его подтверждение:</span>
							<div align="left">
								<input name="regpass1" id="regpass1" class="password_field" type="password" placeholder="Пароль" onchange="checkPassword()"/>
								<img class="img_hidden" id="pass1_check_failed" src="images/exclamation.png" title="Пароль должен состоять не менее чем из трёх символов!"/>
								<img class="img_hidden" id="pass1_check_success" src="images/tick.png"/>
							</div>
							<div align="left">
								<input name="regpass2" id="regpass2" class="password_field" type="password" placeholder="Подтвердите пароль" onchange="checkPassword()"/>
								<img class="img_hidden" id="pass2_check_failed" src="images/exclamation.png" title="Пароли не совпадают!"/>
								<img class="img_hidden" id="pass2_check_success" src="images/tick.png"/>
							</div>
							<br/>
							<input name="regnote"  id="regnote" class="text_field" type="text" placeholder="Описание" />
						</fieldset>
						<span id="error_message" class="msg_hidden">ошибка</span>
						<fieldset class="actions">
							<input id="applay_button" type="submit" class="submit_button" value="ДОБАВИТЬ"/>
							<button class="submit_button" style="float: right" onclick="hideForm()">ОТМЕНА</button>
						</fieldset>
						<input id="old_name" name="old_name" type="hidden" value=""/>
						<input id="member_id" name="member_id" type="hidden" value="-1"/>
					</form>
				</div>
				<div id="remove_member_form" class="member_form_hide">
					<form id="rmbr_form" method="post">
						<span class="warning_title" id="remove_title">Вы действительно желаете удалитьустройство?</span>
						<fieldset class="actions">
							<input type="submit" class="submit_button" value="УДАЛИТЬ"/>
							<button class="submit_button" style="float: right" onclick="hideRemoveForm()">ОТМЕНА</button>
						</fieldset>
						<input id="rmember_id" name="member_id" type="hidden" value="-1"/>
					</form>
				</div>
				<table class="members_table" cols="4">
					<thead align="center">
						<tr>
							<td id="col_ava">Аватар</td>
							<td id="col_name">Имя</td>
							<td id="col_note">Описание</td>
							<td id="col_actions">Действия</td>
						</tr>
					</thead>
					<tbody>';
if (!$usrconn) $usrconn = pg_connect('host=localhost port=5432 dbname=bladerunner user=' . $role_name . ' password=' . $password);
$ok = false;
if ($usrconn)
{
	$result = pg_query($usrconn, 'select member, rolname, usrname, note, avatar from meta.v_members() order by usrname');
	if ($result)
	{
		$n = pg_num_rows($result);
		$ok = true;
		if ($n >= 1)
		{
			while ($row = pg_fetch_row($result))
			{
				$member_id = $row[0];
				$member_role = $row[1];
				$path = 'users/' . $role_name . '/' . $member_role;
				echo '
						<tr id="member_' . $member_id . '">
							<td><a href="menu.php?t=map&m=' . $member_id . '">';
				$no_ava = true;
				$user_ava = $row[4];
				if ($user_ava != '')
				{
					$avainfo = json_decode($user_ava, true);
					if ($avainfo)
					{
						$ava_file = $avainfo['filename'];
						$no_ava = false;
						echo '<img class="avatar_image" src="' . $path . '/' . $ava_file . '" width="80" height="80"/>';
					}
				}
				if ($no_ava)
				{
					echo '<img class="avatar_image" src="images/no_member_avatar.png" width="80" height="80"/>';
				}
				echo '</a></td>
							<td>'. $row[2] .'</td>
							<td>'. $row[3] .'</td>
							<td align="center">
								<button class="submit_button" onclick="showEditMemberForm(' . $member_id . ')">Изменить</button>
								<button class="submit_button" onclick="removeMember(' . $member_id . ')">Удалить</button>
							</td>
						</tr>';
			}
		}
		else
		{
			echo '
						<tr>
							<td colspan="4"><h2>У Вас пока нет ни одного устройства.</h2></td>
						</tr>';
		}
	}
}

if (!$ok)
{
	echo '
						<tr>
							<td colspan="4"><h2>Не могу прочитать сведения об устройствах! Ошибка соединения с базой данных.</h2></td>
						</tr>';
}
echo '
					</tbody>
				</table>
			</div>';