function showNewMemberForm()
{
	frm = document.getElementById('mbr_form');
	frm.setAttribute('action', 'reg_new_member.php');
	frm.onsubmit = checkMemberForm;
	document.getElementById('regname').value = '';
	document.getElementById('regpass1').value = '';
	document.getElementById('regpass2').value = '';
	document.getElementById('regnote').value = '';
	document.getElementById('old_name').value = '';
	document.getElementById('member_id').value = -1;
	document.getElementById('name_check_success').className = 'img_hidden';
	document.getElementById('name_check_failed').className = 'img_hidden';
	document.getElementById('error_message').className = 'msg_hidden';
	document.getElementById('pass1_check_success').className = 'img_hidden';
	document.getElementById('pass1_check_failed').className = 'img_hidden';
	document.getElementById('pass2_check_success').className = 'img_hidden';
	document.getElementById('pass2_check_failed').className = 'img_hidden';
	document.getElementById('title_avatar').className = 'title_hidden';
	document.getElementById('title_password').className = 'title_hidden';
	document.getElementById('applay_button').value = 'ДОБАВИТЬ';
	document.getElementById('member_form_title').innerHTML = 'Новое устройство';
	document.getElementById('member_form').className = 'member_form_displayed';
}

function showEditMemberForm(member_id)
{
	mrow = document.getElementById('member_' + member_id);
	mcol = mrow.firstElementChild;
	mcol = mcol.nextElementSibling;
	old_name = mcol.innerHTML;
	mcol = mcol.nextElementSibling;
	note = mcol.innerHTML;
	frm = document.getElementById('mbr_form');
	frm.setAttribute('action', 'update_member.php');
	frm.onsubmit = checkMemberForm;
	document.getElementById('regname').value = old_name;
	document.getElementById('regpass1').value = '';
	document.getElementById('regpass2').value = '';
	document.getElementById('regnote').value = note;
	document.getElementById('old_name').value = old_name;
	document.getElementById('member_id').value = member_id;
	document.getElementById('name_check_success').className = 'img_hidden';
	document.getElementById('name_check_failed').className = 'img_hidden';
	document.getElementById('error_message').className = 'msg_hidden';
	document.getElementById('pass1_check_success').className = 'img_hidden';
	document.getElementById('pass1_check_failed').className = 'img_hidden';
	document.getElementById('pass2_check_success').className = 'img_hidden';
	document.getElementById('pass2_check_failed').className = 'img_hidden';
	document.getElementById('title_avatar').className = 'title_displayed';
	document.getElementById('title_password').className = 'title_displayed';
	document.getElementById('applay_button').value = 'ПРИМЕНИТЬ';
	document.getElementById('member_form_title').innerHTML = 'Профиль устройства';
	document.getElementById('member_form').className = 'member_form_displayed';
}

function hideForm()
{
	frm = document.getElementById('mbr_form');
	frm.setAttribute('action', '');
	frm.onsubmit = function(){return false};
	document.getElementById('regname').value = '';
	document.getElementById('regpass1').value = '';
	document.getElementById('regpass2').value = '';
	document.getElementById('regnote').value = '';
	document.getElementById('name_check_success').className = 'img_hidden';
	document.getElementById('name_check_failed').className = 'img_hidden';
	document.getElementById('error_message').className = 'msg_hidden';
	document.getElementById('pass1_check_success').className = 'img_hidden';
	document.getElementById('pass1_check_failed').className = 'img_hidden';
	document.getElementById('pass2_check_success').className = 'img_hidden';
	document.getElementById('pass2_check_failed').className = 'img_hidden';
	document.getElementById('member_form').className = 'member_form_hide';
}

function removeMember(member_id)
{
	frm = document.getElementById('rmbr_form');
	frm.setAttribute('action', 'remove_member.php');
	frm.onsubmit = null;
	mrow = document.getElementById('member_' + member_id);
	mcol = mrow.firstElementChild;
	mcol = mcol.nextElementSibling;
	old_name = mcol.innerHTML;
	document.getElementById('remove_title').innerHTML = 'Вы действительно желаете удалить "' + old_name + '"?';
	document.getElementById('rmember_id').value = member_id;
	document.getElementById('remove_member_form').className = 'member_form_displayed';
}

function hideRemoveForm()
{
	frm = document.getElementById('rmbr_form');
	frm.setAttribute('action', '');
	frm.onsubmit = function(){return false};
	document.getElementById('remove_member_form').className = 'member_form_hide';
}