<?php

/**
 * Settings form
 * */
function email_subscribe_settings_form($form=array()){
    $form['email_subscribe_from_email']= array(
		'#type' => 'textfield',
		'#title' => t('Mailer'),
		'#weight' => 0,
		'#required' => TRUE,
		'#default_value' => variable_get('email_subscribe_from_email', ''),
    );
    $form['email_subscribe_send_notification_to_admin'] = array(
        '#type' => 'checkbox',
        '#title' => t('Notify the site administrator of any changes in the subscription'),
        '#default_value' => variable_get('email_subscribe_send_notification_to_admin', 0),
    );
    $form['email_subscribe_interval']= array(
		'#type' => 'select',
		'#title' => t('Interval'),
		'#weight' => 1,
        '#options' => array(
            '3600' => format_plural(1, 'Every 1 hour', 'Every @count hours'),
            '7200' => format_plural(2, 'Every 1 hour', 'Every @count hours'),
            '10800' => format_plural(3, 'Every 1 hour', 'Every @count hours'),
            '14400' => format_plural(4, 'Every 1 hour', 'Every @count hours'),
            '18000' => format_plural(5, 'Every 1 hour', 'Every @count hours'),
            '21600' => format_plural(6, 'Every 1 hour', 'Every @count hours'),
            '25200' => format_plural(7, 'Every 1 hour', 'Every @count hours'),
            '28800' => format_plural(8, 'Every 1 hour', 'Every @count hours'),
            '32400' => format_plural(9, 'Every 1 hour', 'Every @count hours'),
            '43200' => format_plural(12, 'Every 1 hour', 'Every @count hours'),
            '57600' => format_plural(16, 'Every 1 hour', 'Every @count hours'),
            '64800' => format_plural(18, 'Every 1 hour', 'Every @count hours'),
            '72000' => format_plural(20, 'Every 1 hour', 'Every @count hours'),
            '79200' => format_plural(22, 'Every 1 hour', 'Every @count hours'),
            '86400' => format_plural(24, 'Every 1 hour', 'Every @count hours'),
        ),
		'#required' => TRUE,
		'#default_value' => variable_get('email_subscribe_interval', 3600),
    );
    $form['email_subscribe_limit']= array(
		'#type' => 'select',
		'#title' => t('Limit for emails per one crons run'),
		'#weight' => 2,
        '#options' => array(
            '15' => '15',
            '20' => '20',
            '25' => '25',
            '30' => '30',
            '35' => '35',
            '40' => '40',
            '45' => '45',
            '50' => '50',
        ),
		'#required' => TRUE,
		'#default_value' => variable_get('email_subscribe_limit', 15),
    );
    
    $default_values = variable_get('email_subscribe_allowed_node_types', array('page'));
    $form['email_subscribe_allowed_node_types'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Node types are available for subscription'),
        '#options' => node_type_get_names(),
        '#default_value' => $default_values,
        '#required' => TRUE,
    );
    $form['group_settings'] = array(
		'#type' => 'vertical_tabs',
        '#weight' => 5,
	);
 	
    //~ Confirm subscribe email
 	$form['new_content_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Message when a new content'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#group' => 'group_settings'
    );
    
    $value=variable_get('email_subscribe_new_content_body');
    
    if(is_array($value)){
		$value=$value['value'];
	}
	
    $form['new_content_settings']['item1'] = array(
        '#markup'=>t('You may include placeholders like').':[url]('.t('Unsubscribe').'),[node_url],[title],[teaser]',
        '#weight' => 0,
    );
    
	$form['new_content_settings']['email_subscribe_new_content_body'] = array(
		'#type' => 'text_format',
		'#title' => t('Message body when sending a notification of new content'),
		'#default_value' => $value,
        '#required' => TRUE,
    );
    
    //~ Confirm subscribe email
 	$form['confirm_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Confirm subscribe email'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#group' => 'group_settings'
    );
    
    $value=variable_get('email_subscribe_confirm_body');
    
    if(is_array($value)){
		$value=$value['value'];
	}
	
    $form['confirm_settings']['item2'] = array(
        '#markup'=>t('You may include placeholders like').':[url]',
        '#weight' => 0,
    );
    
	$form['confirm_settings']['email_subscribe_confirm_body'] = array(
		'#type' => 'text_format',
		'#title' => t('Message body when sent to confirm email'),
		'#default_value' => $value,
        '#required' => TRUE,
    );
    
    //~ Confirm unsubscribe email
 	$form['unsubscribe_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Confirm unsubscribe email'),
      '#weight' => 2,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#group' => 'group_settings'
    );
    
    $value=variable_get('email_unsubscribe_confirm_body');
    
    if(is_array($value)){
		$value=$value['value'];
	}
	
    $form['unsubscribe_settings']['item3'] = array(
        '#markup'=>t('You may include placeholders like').':[url]',
        '#weight' => 0,
    );
    
	$form['unsubscribe_settings']['email_unsubscribe_confirm_body'] = array(
		'#type' => 'text_format',
		'#title' => t('Message body when sent to unsubscribe email'),
		'#default_value' => $value,
        '#required' => TRUE,
    );
    
    //~ Confirm re-subscribe email
 	$form['resubscribe_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Confirm re-subscribe email'),
      '#weight' => 3,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#group' => 'group_settings'
    );
    
    $value=variable_get('email_resubscribe_confirm_body');
    
    if(is_array($value)){
		$value=$value['value'];
	}
	
    $form['resubscribe_settings']['item4'] = array(
        '#markup'=>t('You may include placeholders like').':[url]',
        '#weight' => 0,
    );
    
	$form['resubscribe_settings']['email_resubscribe_confirm_body'] = array(
		'#type' => 'text_format',
		'#title' => t('Message body when sent to re-subscribe email'),
		'#default_value' => $value,
        '#required' => TRUE,
    );
    
    return system_settings_form($form);
}

/**
 * List subscrobed users
 * */
function email_subscribe_list_users_form($form = array(), &$form_state){
	global $user;
	$attrLinks	= array('html'=>true, 'attributes'=>array('style'=>'vertical-align:top'));
    $arrStat = array(
        1 => t('Email not confirmed'),
        2 => t('Subscribed'),
        3 => t('Unsubscribed'),
    );
    
    $arg = (string)isset($form_state['input']['filter_text']) ? $form_state['input']['filter_text'] : '';
    $collapsed = true;
    $argType = (int)isset($form_state['input']['filter_type']) ? $form_state['input']['filter_type'] : 0;
    
    if(($argType > 0 and $argType < 5) OR ($argType == 5 and !empty($arg))){
        $collapsed = false;
    }
    
    $form['filter_email'] = array(
      '#type' => 'fieldset',
      '#title' => t('Filter'),
      '#weight' => -5,
      '#collapsible' => TRUE,
      '#collapsed' =>$collapsed,
    );
    $form['filter_email']['filter_type'] = array(
        '#type' => 'select',
        '#title' => t('Filter'),
        '#options' => array(
            5 => t('Email'),
            1 => t('Only with email not confirmed'),
            2 => t('Only subscribed'),
            3 => t('Only unsubscribed'),
            4 => t('Only re-subscribed'),
        ),
        '#default_value' => $argType,
        '#weight' => 1,
    );
    $form['filter_email']['filter_text'] = array(
        '#type' => 'textfield',
        '#value' => $arg,
        '#title' => t('Search'),
        '#weight' => 2,
    );
    $form['filter_email']['actions'] = array(
        '#type' => 'actions',
        '#attributes' => array('class' => array('container-inline')),
    );
    $form['filter_email']['actions']['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Filter'),
    );
    $form['filter_email']['actions']['reset'] = array(
            '#type' => 'submit',
            '#value' => t('Cancel'),
    );
	$headArr = array(
		array('data' => t('Email'), 'field' => 'email'),
		array('data' => t('Status'), 'field' => 'status'),
		array('data' => t('Date subscribe'), 'field' => 'date_subscribe','sort' => 'desc'),
		array('data' => t('Date unsubscribe'), 'field' => 'date_unsubscribe'),
		array('data' => t('Date re-subscribe'), 'field' => 'date_resubscribe'),
		array('data' => t('User performed an action')),
        array('data' => t('Actions')),
	);
	
	foreach($headArr as $hval){
		$header[] = $hval;
	}
	unset($hval);
	
	$rows = array();
	
	try{
		$query 	= db_select('email_subscribe_users', 'u')->extend('PagerDefault')	//Pager Extender
		->limit(25)
		->extend('TableSort')
		->orderByHeader($header);
		
		$query->fields('u', array('sid', 'email', 'status', 'subs_token', 'unsubs_token', 'date_subscribe', 'date_unsubscribe', 'date_resubscribe', 'uid'));
		
        if(!empty($argType)){
			switch($argType){
				case 5: default:
					$query->condition('email', '%'.db_like($arg).'%', 'LIKE');
				break;
				
				case 1: case 2: case 3:
					$query->condition('status', $argType);
				break;
				
                case 4:
					$query->isNotNull('date_resubscribe');
				break;
			}
		}
		
		$result = $query->execute();
	}
	catch(Exception $e){
		watchdog_exception('email-subscribe-error', $e);
		drupal_set_message('Error. More information in '.l('syslog','admin/reports/dblog').'.<br />'.check_plain($e->getMessage()), 'error', FALSE);
		return $form;
	}
	$noBalans = 0;
	foreach($result as $key=>$val){
		$val->actions = '';
        _email_subscribe_format_date($val->date_subscribe);
		_email_subscribe_format_date($val->date_unsubscribe);
		_email_subscribe_format_date($val->date_resubscribe);
        switch($val->status){
            case 1: default:
                $val->actions	= l(t('Confirm'), 'admin/config/services/email_subscribe/confirm/'.$val->sid, $attrLinks);
            break;
            case 2:
                $val->actions	= l(t('To unsubscribe'), 'admin/config/services/email_subscribe/unsubscribe/'.$val->sid, $attrLinks);    
            break;
            case 3:
                $val->actions	= l(t('Re-subscribe'), 'admin/config/services/email_subscribe/re-subscribe/'.$val->sid, $attrLinks);    
            break;
        }
        
        if($val->uid === $user->uid){
            $val->uid = t('I am');
        }
        elseif($val->uid == 0){
            $val->uid = t('Anonymous');
        }
        elseif($val->uid > 0){
            $userObj = user_load($val->uid);
            $val->uid = l($userObj->name, 'user/'.$val->uid.'/edit');
        }
        
        $val->status = $arrStat[$val->status];
        unset($val->sid);
		unset($val->subs_token);
		unset($val->unsubs_token);
        $row	= (array)($val);
		$rows[] = $row;
	}
	unset($key,$val);
	
	$form['subscribed_users'] = array(
		'#theme' => 'table',
		'#header' => $header,
		'#rows' => $rows,
		'#pager' => theme('pager'),
		'#empty' => t('No data.'),
	);
	$form['pager'] = array('#markup' => theme('pager'));
    
    return $form;
}

function email_subscribe_list_users_form_submit($form, &$form_state){
	$op = $form_state['values']['op'];
	switch ($op) {
		case t('Filter'):
			$form_state['rebuild'] = TRUE;
		break;
		case t('Reset'):
			$form_state['rebuild'] = FALSE;
		break;
	}
}

function _email_subscribe_format_date(&$val){
    if($val !== Null){
        $val	= format_date(strtotime($val), 'short');
    }
    return true;
}

function _email_subscribe_confirmemail_by_admin($sid=0){
    global $user;
    if(!empty($sid)){
        try{
            $tokenUns = email_subscribe_generate_token('',1,FALSE);
            $res = db_update('email_subscribe_users')
            ->fields(array('unsubs_token' => $tokenUns, 'status' => 2, 'uid'=>$user->uid, 'date_subscribe' => date('Y-m-d H:i:s')))
            ->condition('status', 1)
            ->condition('sid', (int)$sid)
            ->execute();
            if(!empty($res)){
                drupal_set_message('Email confirmed.');
            }
            else{
                drupal_set_message('Email already confirmed.', 'warning');
            }
            drupal_goto('admin/config/services/email_subscribe');
        }
        catch(Exception $e){
            watchdog_exception('error', $e);
            drupal_set_message(t('Error'), 'error'); return false;
        }
    }
    return '';
}

function _email_subscribe_unsubscribe_by_admin($sid=0){
    global $user;
    if(!empty($sid)){
        try{
            $res = db_update('email_subscribe_users')
            ->fields(array('status' => 3, 'uid'=>$user->uid, 'date_unsubscribe' => date('Y-m-d H:i:s')))
            ->condition('status', 2)
            ->condition('sid', (int)$sid)
            ->execute();
            if(!empty($res)){
                $del = db_delete('email_subscribe_readed_nodes')
                ->condition('rn_sid', (int)$sid)
                ->execute();
                if((bool)$del){
                    drupal_set_message(t('Email unsubscribed.'));
                }
            }
            else{
                drupal_set_message(t('Email already unsubscribed.'), 'warning');
            }
            drupal_goto('admin/config/services/email_subscribe');
        }
        catch(Exception $e){
            watchdog_exception('error', $e);
            drupal_set_message(t('Error'), 'error'); return false;
        }
    }
    return '';
}

function _email_subscribe_resubscribe_by_admin($sid=0){
    global $user;
    if(!empty($sid)){
        try{
            $res = db_update('email_subscribe_users')
            ->fields(array('status' => 2, 'uid'=>$user->uid, 'date_resubscribe' => date('Y-m-d H:i:s')))
            ->condition('status', 3)
            ->condition('sid', (int)$sid)
            ->execute();
            if(!empty($res)){
                $nids = db_select('node', 'n')
                ->fields('n', array('nid'))
                ->condition('status', 1)
                ->condition('type', variable_get('email_subscribe_allowed_node_types', array('page')), 'IN')
                ->execute();
                
                foreach($nids as $val){
                    $values[] = array(
                        'rn_nid' => $val->nid,
                        'rn_sid' => (int)$sid,
                        'rn_status' => 1,
                    );
                }
                unset($val);
                $query = db_insert('email_subscribe_readed_nodes')->fields(array('rn_nid', 'rn_sid', 'rn_status'));
                foreach ($values as $record) {
                    $query->values($record);
                }
                $query->execute();
                
                drupal_set_message(t('Email re-subscribed.'));
            }
            else{
                drupal_set_message(t('Email already re-subscribed.'), 'warning');
            }
            drupal_goto('admin/config/services/email_subscribe');
        }
        catch(Exception $e){
            watchdog_exception('error', $e);
            drupal_set_message(t('Error'), 'error'); return '';
        }
    }
    return '';
}

/**
 * Bulk subscribe
 * */
function email_subscribe_mass_subscribe_form($form=array(),&$form_state){
    $form['emails'] = array(
        '#type' => 'textarea',
        '#title' => t('Email addresses'),
        '#cols' => 60,
        '#rows' => 5,
        '#description' => t('Email addresses must be separated by comma, space or newline.'),
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Subscribe'),
    );

    return $form;
}

/**
 * Bulk subscribe submit. Part of the source code is taken from certain functions 
 * "simplenews_subscription_list_add_submit" module SimpleNews.
 * Thanks to the developers of this module.
 * */
function email_subscribe_mass_subscribe_form_submit($form, &$form_state){
    global $user;
    $added = array();
    $invalid = array();
    $skipped = array();
    $values = array();
    $emails = preg_split("/[\s,]+/", $form_state['values']['emails']);
    
    $nids = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->condition('status', 1)
    ->condition('type', variable_get('email_subscribe_allowed_node_types', array('page')), 'IN')
    ->execute()->fetchAllAssoc('nid');
    for($i=0; $i<sizeof($emails); $i++){
        $email = trim($emails[$i]);
        $nodes = $nids;
        if($email == ''){
          continue;
        }
        if(valid_email_address($email)){
            $checkMail = db_select('email_subscribe_users', 'e')
            ->fields('e', array('status'))
            ->condition('email', check_plain($email))
            ->condition('status', 0, '>')
            ->range(0,1)
            ->execute()
            ->fetchObject();
            
            if(!$checkMail){
                $subsToken = email_subscribe_generate_token($email, 0, FALSE);
                $unsubsToken = email_subscribe_generate_token($email, 1, FALSE);
                $ins = db_insert('email_subscribe_users')->fields(array(
                    'uid' => $user->uid,
                    'email' => check_plain($email),
                    'subs_token' => $subsToken,
                    'unsubs_token' => $unsubsToken,
                    'status' => 2,
                    'date_subscribe' => date('Y-m-d H:i:s'),
                ));
                $newid = $ins->execute();
                if($newid){
                    $query = db_insert('email_subscribe_readed_nodes')->fields(array('rn_nid', 'rn_sid', 'rn_status'));
                    $values = array();
                    foreach($nodes as $val){
                        $values[] = array(
                            'rn_nid' => $val->nid,
                            'rn_sid' => (int)$newid,
                            'rn_status' => 1,
                        );
                    }
                    unset($val);
                    foreach($values as $record){
                        $query->values($record);
                    }
                    unset($record);
                    $query->execute();
                    $added[] = $email;
                }
            }
            else{
                $skipped[] = $email;
            }
        }
        else{
          $invalid[] = $email;
        }
    }
    if($added){
        $added = implode(", ", $added);
        drupal_set_message(t('The following addresses were added: %added.', array('%added' => $added)));
    }
    else{
        drupal_set_message(t('No addresses were added.'));
    }
    
    if($invalid){
        $invalid = implode(", ", $invalid);
        drupal_set_message(t('The following addresses were invalid: %invalid.', array('%invalid' => $invalid)), 'error');
    }
    
    if($skipped){
        $skipped = implode(", ", $skipped);
        drupal_set_message(t('The following addresses were skipped: %skipped.', array('%skipped' => $skipped)), 'warning');
    }
    
}
