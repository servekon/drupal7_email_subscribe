<?php

/**
 * Settings form
 * */
function email_subscribe_settings_form($form=array(), &$form_state) {
  $limits = array();
  for($i=50; $i<=1000; $i+=25) {
    $limits[$i] = $i;
  }
  
  $groupStates  = array(
    'visible' => array(
      'select[name="email_subscribe_group_nodes"]' => array('value' => 1),
    ),
  );
  
  if(isset($form_state['preview_html'])) {
    $form['#prefix'] = $form_state['preview_html'];
  }
  
  $form['email_subscribe_from_email']= array(
    '#type' => 'textfield',
    '#title' => t('Mailer'),
    '#weight' => 0,
    '#required' => TRUE,
    '#default_value' => variable_get('email_subscribe_from_email', ''),
  );
  $form['email_subscribe_notification_email']= array(
    '#type' => 'textfield',
    '#title' => t('Receive e-mail notifications'),
    '#weight' => 1,
    '#required' => TRUE,
    '#default_value' => variable_get('email_subscribe_notification_email', ''),
  );
  $form['email_subscribe_send_notification_to_admin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify the site administrator of any changes in the subscription'),
    '#default_value' => variable_get('email_subscribe_send_notification_to_admin', 0),
    '#weight' => 2,
  );
  
  $form['email_subscribe_dont_store_info_sent_emails'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not store the information about sent emails'),
    '#description' => t('If checked, after send emails stored data will be removed'),
    '#default_value' => variable_get('email_subscribe_dont_store_info_sent_emails', 0),
    '#weight' => 3,
  );
  
  $default_values = variable_get('email_subscribe_allowed_node_types', array('page'));
  $form['email_subscribe_allowed_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node types are available for subscription'),
    '#options' => node_type_get_names(),
    '#default_value' => $default_values,
    '#required' => TRUE,
    '#weight' => 4,
  );
  
  $form['email_subscribe_node_views_type'] = array(
    '#type' => 'select',
    '#title' => t('Type of node display for message body'),
    '#description' => t('You must configure the view type on the display control page of each type of material. Example link for "page" node type').': '.l(t('manage display'), 'admin/structure/types/manage/page/display'),
    '#options' => _email_subcribe_get_list_node_view_types(),
    '#default_value' => (int)variable_get('email_subscribe_node_views_type', 1),
    '#required' => TRUE,
    '#weight' => 5,
  );
  
  $form['email_subscribe_interval']= array(
    '#type' => 'select',
    '#title' => t('Interval'),
    '#weight' => 6,
    '#options' => array(
      '1800' => format_plural(0.5, 'Every 1 hour', 'Every @count hours'),
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
    '#weight' => 7,
    '#options' => $limits,
    '#required' => TRUE,
    '#default_value' => variable_get('email_subscribe_limit', 15),
  );
  
  $form['email_subscribe_group_nodes'] = array(
    '#type' => 'select',
    '#title' => t('Grouped nodes in one email?'),
    '#options' => array(t('No'), t('Yes')),
    '#description' => t('If you select "Yes" then all available the published nodes at the time of sending letter will be sent in the one letter.'),
    '#default_value' => variable_get('email_subscribe_group_nodes', 0),
    '#weight' => 8,
  );
  
  $form['group_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 9,
  );
  //~ Confirm subscribe email
  $form['new_content_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Message when a new content'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'group_settings'
  );
  
  $form['new_content_settings']['item1'] = array(
    '#type' => 'item',
    '#markup'=>t('You may include placeholders like').':[url]('.t('Unsubscribe').'),[node_url],[title],[teaser]',
    '#weight' => 0,
    '#states' => array(
      'visible' => array(
        'select[name="email_subscribe_group_nodes"]' => array('value' => 0),
      ),
    ),
  );
  
  $value=variable_get('email_subscribe_new_content_header');
  
  if(is_array($value)) {
    $value=$value['value'];
  }
  //~ Fix incorrect visible state for text_format. From https://www.drupal.org/node/997826#comment-9119483
  $form['new_content_settings']['container_header'] = array(
   '#type' => 'container',
   '#states' => $groupStates,
   '#attributes' => array('class'=>array('email_s_header')),
  );
  
  $form['new_content_settings']['container_header']['item11'] = array(
    '#type' => 'item',
    '#markup'=>t('For message header You may include placeholders like').':[url]('.t('Unsubscribe').')',
    '#states' => $groupStates,
  );
    
  $form['new_content_settings']['container_header']['email_subscribe_new_content_header'] = array(
    '#type' => 'text_format',
    '#title' => t('Message header when sending a notification of new content'),
    '#default_value' => $value,
    '#states' => $groupStates,
  );
  
  $form['new_content_settings']['container_body'] = array(
   '#type' => 'container',
   '#attributes' => array('class'=>array('email_s_body')),
  );
  
  $form['new_content_settings']['container_body']['item5'] = array(
    '#type' => 'item',
    '#markup'=>t('You may include placeholders like').':[url]('.t('Unsubscribe').'),[node_url],[title],[teaser]',
    '#states' => $groupStates,
  );
  
  $value=variable_get('email_subscribe_new_content_body');
  
  if(is_array($value)) {
    $value=$value['value'];
  }
    
  $form['new_content_settings']['container_body']['email_subscribe_new_content_body'] = array(
    '#type' => 'text_format',
    '#title' => t('Message body when sending a notification of new content'),
    '#default_value' => $value,
    '#required' => TRUE,
  );
  
  $value=variable_get('email_subscribe_new_content_footer');
  
  if(is_array($value)) {
    $value=$value['value'];
  }
  //~ Fix incorrect visible state for text_format. From https://www.drupal.org/node/997826#comment-9119483
  $form['new_content_settings']['container_footer'] = array(
   '#type' => 'container',
   '#states' => $groupStates,
   '#attributes' => array('class'=>array('email_s_header')),
  );
  
  $form['new_content_settings']['container_footer']['item12'] = array(
    '#type' => 'item',
    '#markup'=>t('For message footer You may include placeholders like').':[url]('.t('Unsubscribe').')',
    '#states' => $groupStates,
  );
    
  $form['new_content_settings']['container_footer']['email_subscribe_new_content_footer'] = array(
    '#type' => 'text_format',
    '#title' => t('Message footer when sending a notification of new content'),
    '#default_value' => $value,
    '#states' => $groupStates,
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
  
  if(is_array($value)) {
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
  
  if(is_array($value)) {
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
  
  if(is_array($value)) {
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

  $form['actions']['preview'] = array(
    '#type' => 'submit',
    '#value' => t('Preview'),
    '#weight' => 5,
    '#submit' => array('_email_subscribe_settings_form_preview_submit'),
  );

  return system_settings_form($form);
}

function _email_subscribe_settings_form_preview_submit($form, &$form_state) {
  $html = _email_subcribe_preview_letter($form_state['values']);
  $form_state['preview_html'] = $html;
  $form_state['rebuild'] = TRUE;
}

/**
 * Validate email of the sender
 * */
function email_subscribe_settings_form_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['email_subscribe_from_email'])) {
    form_set_error('email', t('That e-mail address is not valid.'));
  }
}

/**
 * List subscrobed users
 * */
function email_subscribe_list_users_form($form = array(), &$form_state) {
  global $user;
  $attrLinks  = array('html'=>true, 'attributes'=>array('style'=>'vertical-align:top'));
  $arrStat = array(
    1 => t('Email not confirmed'),
    2 => t('Subscribed'),
    3 => t('Unsubscribed'),
  );
  
  $arg = check_plain(arg(6));
  $collapsed = true;
  $argType = (int)arg(5);
  
  if(($argType > 0 and $argType < 5) OR ($argType == 5 and !empty($arg))) {
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
    '#states' => array(
      'enabled' => array(
        ':input[name="filter_type"]' => array('value' => 5),
      ),
    ),
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
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/services/email_subscribe',
  );
  $headArr = array(
    array('data' => '#'),
    array('data' => t('Email'), 'field' => 'email'),
    array('data' => t('Status'), 'field' => 'status'),
    array('data' => t('Date subscribe'), 'field' => 'date_subscribe','sort' => 'desc'),
    array('data' => t('Date unsubscribe'), 'field' => 'date_unsubscribe'),
    array('data' => t('Date re-subscribe'), 'field' => 'date_resubscribe'),
    array('data' => t('User performed an action')),
    array('data' => t('Actions')),
  );
  
  foreach($headArr as $hval) {
    $header[] = $hval;
  }
  unset($hval);
  
  $rows = array();
  try{
    $query  = db_select('email_subscribe_users', 'u')
    ->extend('TableSort')
    ->extend('PagerDefault');
    
    $query->fields('u', array('sid', 'email', 'status', 'subs_token', 'unsubs_token', 'date_subscribe', 'date_unsubscribe', 'date_resubscribe', 'uid'));
  
    if(!empty($argType)) {
      switch($argType) {
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
    $query->orderByHeader($header)->limit(50);
    $result = $query->execute();
  }
  catch(Exception $e) {
    watchdog_exception('email-subscribe-error', $e);
    drupal_set_message('Error. More information in '.l('syslog','admin/reports/dblog').'.<br />'.check_plain($e->getMessage()), 'error', FALSE);
    return $form;
  }
  $noBalans = 0;
  foreach($result as $key=>$val) {
    $val->actions = '';
    _email_subscribe_format_date($val->date_subscribe);
    _email_subscribe_format_date($val->date_unsubscribe);
    _email_subscribe_format_date($val->date_resubscribe);
    switch($val->status) {
      case 1: default:
        $val->actions = l(t('Confirm'), 'admin/config/services/email_subscribe/confirm/'.$val->sid, $attrLinks);
      break;
      case 2:
        $val->actions = l(t('To unsubscribe'), 'admin/config/services/email_subscribe/unsubscribe/'.$val->sid, $attrLinks);  
      break;
      case 3:
        $val->actions = l(t('Re-subscribe'), 'admin/config/services/email_subscribe/re-subscribe/'.$val->sid, $attrLinks);  
      break;
    }
    
    if(!empty($val->actions)) {
      $val->actions .= '<br />'.l(t('Remove'), 'admin/config/services/email_subscribe/remove/'.$val->sid, $attrLinks); 
    }
    
    if($val->uid === $user->uid) {
      $val->uid = t('I am');
    }
    elseif($val->uid == 0) {
      $val->uid = t('Anonymous');
    }
    elseif($val->uid > 0) {
      $userObj = user_load($val->uid);
      $val->uid = l($userObj->name, 'user/'.$val->uid.'/edit');
    }
    
    $val->status = $arrStat[$val->status];
    unset($val->sid);
    unset($val->subs_token);
    unset($val->unsubs_token);
    $row  = (array)($val);
    array_unshift($row, $key+1);
    $rows[] = $row;
  }
  unset($key,$val);
  
  $form['subscribed_users'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No data.'),
  );
  $form['pager'] = array('#markup' => theme('pager'));
  return $form;
}

function email_subscribe_list_users_form_submit($form, &$form_state) {
  $filter_type = $filter_text = '';
  unset($form_state['input']['op'], $form_state['input']['form_id']);
  unset($form_state['input']['form_build_id'],$form_state['input']['form_token']);
  
  extract($form_state['input']);
  drupal_goto('admin/config/services/email_subscribe/filter/'.$filter_type.'/'.$filter_text);
}

function _email_subscribe_format_date(&$val) {
  if($val !== Null) {
    $val  = format_date(strtotime($val), 'short');
  }
  return true;
}

function _email_subscribe_confirmemail_by_admin($sid=0) {
  global $user;
  if(!empty($sid)) {
    try{
      $tokenUns = email_subscribe_generate_token('',1,FALSE);
      $res = db_update('email_subscribe_users')
      ->fields(array('unsubs_token' => $tokenUns, 'status' => 2, 'uid'=>$user->uid, 'date_subscribe' => date('Y-m-d H:i:s')))
      ->condition('status', 1)
      ->condition('sid', (int)$sid)
      ->execute();
      if(!empty($res)) {
        drupal_set_message('Email confirmed.');
      }
      else{
        drupal_set_message('Email already confirmed.', 'warning');
      }
      drupal_goto('admin/config/services/email_subscribe');
    }
    catch(Exception $e) {
      watchdog_exception('email-subscribe-error', $e);
      drupal_set_message(t('Error'), 'error'); return false;
    }
  }
  return '';
}

function _email_subscribe_unsubscribe_by_admin($sid=0) {
  global $user;
  if(!empty($sid)) {
    try{
      $res = db_update('email_subscribe_users')
      ->fields(array('status' => 3, 'uid'=>$user->uid, 'date_unsubscribe' => date('Y-m-d H:i:s')))
      ->condition('status', 2)
      ->condition('sid', (int)$sid)
      ->execute();
      if(!empty($res)) {
        $del = db_delete('email_subscribe_readed_nodes')
        ->condition('rn_sid', (int)$sid)
        ->execute();
        if((bool)$del) {
          drupal_set_message(t('Email unsubscribed.'));
        }
      }
      else{
        drupal_set_message(t('Email already unsubscribed.'), 'warning');
      }
      drupal_goto('admin/config/services/email_subscribe');
    }
    catch(Exception $e) {
      watchdog_exception('email-subscribe-error', $e);
      drupal_set_message(t('Error'), 'error'); return false;
    }
  }
  return '';
}

function _email_subscribe_resubscribe_by_admin($sid=0) {
  global $user;
  if(!empty($sid)) {
    try{
      $res = db_update('email_subscribe_users')
      ->fields(array('status' => 2, 'uid'=>$user->uid, 'date_resubscribe' => date('Y-m-d H:i:s')))
      ->condition('status', 3)
      ->condition('sid', (int)$sid)
      ->execute();
      if(!empty($res)) {
        $nids = db_select('node', 'n')
        ->fields('n', array('nid', 'status'))
        ->condition('type', _email_subcribe_get_list_node_types(), 'IN')
        ->execute();
        
        foreach($nids as $val) {
          $values[] = array(
            'rn_nid' => $val->nid,
            'rn_sid' => (int)$sid,
            'rn_status' => (int)$val->status,
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
    catch(Exception $e) {
      watchdog_exception('email-subscribe-error', $e);
      drupal_set_message(t('Error'), 'error'); return '';
    }
  }
  return '';
}

function _email_subscribe_remove_by_admin($sid=0) {
  $form = drupal_get_form('_email_subscribe_remove_by_admin_form', (int)$sid);
  return drupal_render($form);
}

function _email_subscribe_remove_by_admin_form($form, &$form_state, $sid) {
  $form = array();
  
  $email = db_select('email_subscribe_users', 'u')
  ->fields('u', array('email'))
  ->condition('sid', (int)$sid)
  ->range(0,1)
  ->execute()->fetchObject();
  
  if(empty($email->email)) {
    drupal_set_message(t('No data.'), 'warning');
    drupal_goto('admin/config/services/email_subscribe');
  }
  $form['email_subscribe_item'] = array(
    '#type' => 'item',
    '#title' => t('Are you sure you want to remove user with email %email?', array('%email'=>$email->email)),
    '#weight' => -1,
  );
  
  $form['email'] = array(
    '#type' => 'hidden',
    '#value' => $email->email,
  );
  
  $form['email_id'] = array(
    '#type' => 'hidden',
    '#value' => $sid,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Yes'),
    '#weight' => 1,
  );
  
  $form['alcom_konsalt_cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/services/email_subscribe',
    '#weight' => 2,
  );
  return $form;
}

function _email_subscribe_remove_by_admin_form_submit($form, &$form_state) {
  $sid = (int)$form_state['values']['email_id'];
  $email = check_plain($form_state['values']['email']);
  $del = db_delete('email_subscribe_users')->condition('sid', $sid)->execute();
  if($del) {
    drupal_set_message(t('Email %email was removed.', array('%email'=>$email)));
  }
  else{
    drupal_set_message(t('Email %email was not removed.', array('%email'=>$email)), 'error');
  }
  $form_state['redirect'] = 'admin/config/services/email_subscribe';
}

/**
 * Bulk subscribe
 * */
function email_subscribe_mass_subscribe_form($form=array(),&$form_state) {
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
function email_subscribe_mass_subscribe_form_submit($form, &$form_state) {
  global $user;
  $added = array();
  $invalid = array();
  $skipped = array();
  $values = array();
  $emails = preg_split('/[\s,]+/', $form_state['values']['emails']);
  
  $nids = db_select('node', 'n')
  ->fields('n', array('nid', 'status'))
  ->condition('type', _email_subcribe_get_list_node_types(), 'IN')
  ->execute()->fetchAllAssoc('nid');
  for($i=0; $i<sizeof($emails); $i++) {
    $email = trim($emails[$i]);
    if($email == '') {
      continue;
    }
    if(valid_email_address($email)) {
      $checkMail = db_select('email_subscribe_users', 'e')
      ->fields('e', array('status'))
      ->condition('email', check_plain($email))
      ->condition('status', 0, '>')
      ->range(0,1)
      ->execute()
      ->fetchObject();
      
      if(!$checkMail) {
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
        if($newid) {
          $query = db_insert('email_subscribe_readed_nodes')->fields(array('rn_nid', 'rn_sid', 'rn_status'));
          $values = array();
          foreach($nids as $val) {
            $values[] = array(
              'rn_nid' => $val->nid,
              'rn_sid' => (int)$newid,
              'rn_status' => (int)$val->status,
            );
          }
          unset($val);
          foreach($values as $record) {
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
  if($added) {
    $added = implode(', ', $added);
    drupal_set_message(t('The following addresses were added: %added.', array('%added' => $added)));
  }
  else{
    drupal_set_message(t('No addresses were added.'));
  }
  
  if($invalid) {
    $invalid = implode(', ', $invalid);
    drupal_set_message(t('The following addresses were invalid: %invalid.', array('%invalid' => $invalid)), 'error');
  }
  
  if($skipped) {
    $skipped = implode(', ', $skipped);
    drupal_set_message(t('The following addresses were skipped: %skipped.', array('%skipped' => $skipped)), 'warning');
  }
}

/**
 * email_subscribe_clean_database_form
 * */
function email_subscribe_clean_database_form($form=array(), &$form_state) {
  
  $options = array(
    1 => format_plural(12, 'Remove data older then 1 month', 'Remove data older then @count months'),
    2 => format_plural(6, 'Remove data older then 1 month', 'Remove data older then @count months'),
    3 => format_plural(3, 'Remove data older then 1 month', 'Remove data older then @count months'),
    4 => format_plural(1, 'Remove data older then 1 month', 'Remove data older then @count months'),
    5 => t('Remove all data'),
  );
  
  $form['es_yes'] = array(
    '#type' => 'item',
    '#title' => t('Cleaning table with information about sent emails(read nodes)'),
    //~ '#markup' => t('After unlocking will be available to unload all the previously listed indications.'),
    '#weight' => -1,
  );
  
  $form['es_options'] = array(
    '#type' => 'radios',
    '#title' => t('Type of removing'),
    '#options' => $options,
    '#default_value' => 4,
  );
  
  $form['es_del_not_confimed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove users with unconfirmed emails'),
    '#default_value' => 1,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Yes'),
    '#weight' => 1,
  );
  $form['es_cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/services/email_subscribe/service',
    '#weight' => 2,
  );
  return $form;
}

function email_subscribe_clean_database_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  extract($form_state['values']);
  $changed = 0;
  
  switch($es_options) {
    case 1:
      $changed = strtotime('-12 Month', REQUEST_TIME);
    break;
    case 2:
      $changed = strtotime('-6 Month', REQUEST_TIME);
    break;
    case 3:
      $changed = strtotime('-3 Month', REQUEST_TIME);
    break;
    case 4:
      $changed = strtotime('-1 Month', REQUEST_TIME);
    break;
    case 5:
      $changed = REQUEST_TIME;
    break;
  }

  $sqlDel = "DELETE rn.* FROM {email_subscribe_readed_nodes} AS rn 
  INNER JOIN {email_subscribe_users} AS u ON rn.rn_sid=u.sid
  INNER JOIN {node} AS n ON n.nid=rn.rn_nid
  WHERE u.status > 1
  AND rn.rn_status = 1 
  AND n.type in(:type)
  AND n.status = :status
  AND n.changed <= :changed";
  
  try{
    $sql = db_query($sqlDel, array(
      ':type' => _email_subcribe_get_list_node_types(),
      ':status' => NODE_PUBLISHED,
      ':changed'=> $changed)
    )->rowCount();
    
    //~ Delete data for users with uconfirmed emails
    if($es_del_not_confimed) {
      $usdel = db_select('email_subscribe_users', 'u')
      ->fields('u', array('sid'))
      ->condition('status', 1)
      ->condition(db_or()
        ->condition('datechange', $changed, '<')
        ->condition('datechange', NULL, 'IS NULL')
      )
      ->execute()
      ->fetchAllAssoc('sid');
      
      if(sizeof($usdel) > 0) {
        $delRN = db_delete('email_subscribe_readed_nodes')
        ->condition('rn_sid', array_keys($usdel), 'IN')
        ->execute();
        
        $delUs = db_delete('email_subscribe_users')
        ->condition('sid', array_keys($usdel), 'IN')
        ->execute();
        
        drupal_set_message(format_plural($delRN, 'Was removed 1 row.', 'Was removed @count rows.').' '.t('For users with unconfirmed email, from %table table.', array('%table'=>'email_subscribe_readed_nodes')), 'warning');
        drupal_set_message(format_plural($delUs, 'Was removed 1 row.', 'Was removed @count rows.').' '.t('For users with unconfirmed email, from %table table.', array('%table'=>'email_subscribe_users')), 'warning');
      }
    }
    
    db_query('OPTIMIZE TABLE {email_subscribe_readed_nodes}');
    drupal_set_message(format_plural($sql, 'Was removed 1 row.', 'Was removed @count rows.'), 'warning');
  }
  catch(Exception $e) {
    watchdog_exception('email-subscribe-error', $e);
    drupal_set_message(t('Error'), 'error');
  }
}

/**
 * Statistics page
 * */
function email_subscribe_statistics_page() {
  $out = '';
  $rows = array();
  
  $subsUser = db_select('email_subscribe_users')
  ->condition('status', 2)
  ->countQuery()
  ->execute()
  ->fetchField();
  
  $unSubsUser = db_select('email_subscribe_users')
  ->condition('status', 3)
  ->countQuery()
  ->execute()
  ->fetchField();
  
  $reSubsUser = db_select('email_subscribe_users')
  ->condition('status', 2)
  ->isNotNull('date_resubscribe')
  ->countQuery()
  ->execute()
  ->fetchField();
  
  $fromYandex = db_select('email_subscribe_users')
  ->condition(db_or()
    ->condition('email', '%'.db_like('@ya.ru'), 'LIKE')
    ->condition('email', '%'.db_like('@yandex.ru'), 'LIKE')
  )
  ->condition('status', 2)
  ->countQuery()
  ->execute()
  ->fetchField();
  
  $fromGmail = db_select('email_subscribe_users')
  ->condition(db_or()
    ->condition('email', '%'.db_like('@gmail.com'), 'LIKE')
  )
  ->condition('status', 2)
  ->countQuery()
  ->execute()
  ->fetchField();
  
  $fromMailru = db_select('email_subscribe_users')
  ->condition(db_or()
    ->condition('email', '%'.db_like('@mail.ru'), 'LIKE')
    ->condition('email', '%'.db_like('@inbox.ru'), 'LIKE')
    ->condition('email', '%'.db_like('@list.ru'), 'LIKE')
    ->condition('email', '%'.db_like('@bk.ru'), 'LIKE')
  )
  ->condition('status', 2)
  ->countQuery()
  ->execute()
  ->fetchField();
  
  $fromRambler = db_select('email_subscribe_users')
  ->condition(db_or()
    ->condition('email', '%'.db_like('@rambler.ru'), 'LIKE')
  )
  ->condition('status', 2)
  ->countQuery()
  ->execute()
  ->fetchField();
  
  $otherMail  = (int)$subsUser-$fromGmail-$fromMailru-$fromRambler-$fromYandex;
  
  $rowsArr = array(
    t('The total number of subscribed the users') => $subsUser,
    t('The total number of unsubscribed the users') => $unSubsUser,
    t('The total number of re-subscribed and the active users') => $reSubsUser,
    t('The total number the subscribed users from :post', array(':post'=>'Yandex')) => $fromYandex.' ('.round($fromYandex*100/$subsUser, 2).'%)',
    t('The total number the subscribed users from :post', array(':post'=>'Mail.ru')) => $fromMailru.' ('.round($fromMailru*100/$subsUser, 2).'%)',
    t('The total number the subscribed users from :post', array(':post'=>'Google')) => $fromGmail.' ('.round($fromGmail*100/$subsUser, 2).'%)',
    t('The total number the subscribed users from :post', array(':post'=>'Rambler')) => $fromRambler.' ('.round($fromRambler*100/$subsUser, 2).'%)',
    t('The total number the subscribed users from other mail systems') => $otherMail.' ('.round($otherMail*100/$subsUser, 2).'%)',
  );
  
  foreach($rowsArr as $key=>$val) {
    $rows[] = array(
      'data' => array(array('data'=>$key.':', 'style' => array('width:50%; text-align:right')), array('data'=>$val, 'class'=>array('es-bold'))),  
    );
  }
  
  $out = theme('table',
    array(
      'header' => array(),
      'rows'=>$rows,
      'empty' => t('No data.'),
    )
  );
  
  return $out;
}
