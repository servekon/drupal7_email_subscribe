<?php

/**
 * Main subscribe form
 * */
function email_subscribe_subscribe_form($form=array(), &$form_state){
  global $user;
  $email = !empty($form_state['values']['email']) ? $form_state['values']['email'] : isset($user->mail) ? $user->mail : '';
  
  $form['email']= array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#weight' => 0,
    '#default_value' => $email,
    '#size' => 15,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Perform subscription'),
    '#weight' => 10,
  );
  $form['#action'] = url('email-subscribe');
  return $form;
}

/**
 * Form validation logic for the contact form.
 */
function email_subscribe_subscribe_form_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t('That e-mail address is not valid.'));
  }
}

function email_subscribe_subscribe_form_submit($form, &$form_state){
  global $user;
  $form_state['rebuild'] = FALSE;
  $form_state['redirect'] = false;
  $email = !empty($form_state['values']['email']) ? $form_state['values']['email'] : '';
  try{
    $checkMail = db_select('email_subscribe_users', 'e')
    ->fields('e', array('status'))
    ->condition('email', check_plain($email))
    ->condition('status', 0, '>')
    ->range(0,1)
    ->execute()
    ->fetchObject();
    if($checkMail){
      switch($checkMail->status){
        case 1:
          $form_state['redirect'] = 'email-subscribe-confirm-email/'.check_plain($email);
        break;
        case 2:
          $form_state['redirect'] = 'email-unsubscribe/'.check_plain($email);
        break;
        case 3:
          $form_state['redirect'] = 'email-resubscribe/'.check_plain($email);
        break;
      }
    }
    else{
      $new = db_insert('email_subscribe_users')->fields(array(
        'uid'=>$user->uid,
        'email'=>check_plain($email),
        'status'=>1,
        'datechange'=>REQUEST_TIME,
      ))->execute();
      if($new){
        //~ Send email
        email_subcribe_send_email_letter($email);
      }
    }
  }
  catch(Exception $e){
    watchdog_exception('email-subscribe-error', $e);
    drupal_set_message(t('Error'), 'error'); return false;
  }
  return true;
}

/**
 * When the user confirms the e-mail address or renew a subscription, 
 * we need to fill in the table 'email_subscribe_readed_nodes'.
 * */
function _email_subscribe_fill_readed_nodes($token){
  try{
    //~ Fill table for unpublished nodes
    $sid = db_select('email_subscribe_users', 'u')
    ->fields('u', array('sid'))
    ->condition('subs_token', check_plain($token))
    ->execute()
    ->fetchObject();
    
    $nids = db_select('node', 'n')
    ->fields('n', array('nid', 'status'))
    ->condition('type', _email_subcribe_get_list_node_types(), 'IN')
    ->condition('status', NODE_PUBLISHED, '<>')
    ->execute();
    if($nids->rowCount() > 0){
      foreach($nids as $val){
        $values[] = array(
          'rn_nid' => $val->nid,
          'rn_sid' => (int)$sid->sid,
          'rn_status' => (int)$val->status,
          'datechange' => REQUEST_TIME,
        );
      }
      unset($val);
      $query = db_insert('email_subscribe_readed_nodes')->fields(array('rn_nid', 'rn_sid', 'rn_status', 'datechange'));
      foreach ($values as $record) {
        $query->values($record);
      }
      $query->execute();
    }
  }
  catch(Exception $e){
    watchdog_exception('email-subscribe-error', $e);
    drupal_set_message(t('Error'), 'error'); return false;
  }
  return true;
}
/**
 * Form for re-sending messages with confirmation
 * */
function email_subscribe_confirmemail_form($form, &$form_state){
  $email = !empty($form_state['values']['email']) ? $form_state['values']['email'] : check_plain(arg(1));
  
  $form['email']= array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#weight' => 0,
    '#size' => 30,
    '#default_value' => $email,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm email'),
    '#weight' => 10,
  );
  $form['#validate'][] = 'email_subscribe_subscribe_form_validate';
  return $form;
}

function email_subscribe_confirmemail_form_submit($form, &$form_state){
  $email = !empty($form_state['values']['email']) ? $form_state['values']['email'] : '';
  return email_subcribe_send_email_letter($email);
}

/**
 * Form for to unsubscribe
 * */
function email_subscribe_unsubscribe_form($form, &$form_state){
  $email = !empty($form_state['values']['email']) ? $form_state['values']['email'] : check_plain(arg(1));
  
  $form['email']= array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#weight' => 0,
    '#size' => 30,
    '#default_value' => $email,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Unsubscribe'),
    '#weight' => 10,
  );
  $form['#validate'][] = 'email_subscribe_subscribe_form_validate';
  return $form;
}

function email_subscribe_unsubscribe_form_submit($form, &$form_state){
  $email = !empty($form_state['values']['email']) ? $form_state['values']['email'] : '';
  return email_subcribe_send_email_letter($email,2);
}

/**
 * Re-subscribe form
 * */
function email_subscribe_resubscribe_form($form, &$form_state){
  $email = !empty($form_state['values']['email']) ? $form_state['values']['email'] : check_plain(arg(1));
  
  $form['email']= array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#weight' => 0,
    '#size' => 30,
    '#default_value' => $email,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Re-subscribe'),
    '#weight' => 10,
  );
  $form['#validate'][] = 'email_subscribe_subscribe_form_validate';
  return $form;
}

function email_subscribe_resubscribe_form_submit($form, &$form_state){
  $email = !empty($form_state['values']['email']) ? $form_state['values']['email'] : '';
  return email_subcribe_send_email_letter($email,3);
}

/**
 * Subscription confirmation link with the token
 * */
function email_subscribe_confirmemail_token($token=''){
  global $user;
  if(!empty($token)){
    try{
      $tokenUns = email_subscribe_generate_token('',1,FALSE);
      $res = db_update('email_subscribe_users')
      ->fields(array('unsubs_token' => $tokenUns, 'status' => 2, 'date_subscribe' => date('Y-m-d H:i:s'), 'datechange'=>REQUEST_TIME))
      ->condition('status', 1)
      ->condition('subs_token', check_plain($token))
      ->execute();
      if(!empty($res)){
        if(_email_subscribe_fill_readed_nodes($token)){
          drupal_set_message(t('Email confirmed.'));
        }
      }
      else{
        drupal_set_message(t('Email already confirmed.'), 'warning');
      }
      drupal_goto('email-subscribe');
    }
    catch(Exception $e){
      watchdog_exception('email-subscribe-error', $e);
      drupal_set_message(t('Error'), 'error'); return false;
    }
  }
  return '';
}

/**
 * Unsubscribe link with the token
 * */
function email_subscribe_unsubscribe_token($token=''){
  global $user;
  if(!empty($token)){
    try{
      $res = db_update('email_subscribe_users')
      ->fields(array('status' => 3, 'date_unsubscribe' => date('Y-m-d H:i:s'), 'datechange'=>REQUEST_TIME))
      ->condition('status', 2)
      ->condition('unsubs_token', check_plain($token))
      ->execute();
      if(!empty($res)){
        //~ Remove data to if when user will be re-subscribe, fill data will be correct
        $sid = db_select('email_subscribe_users', 'u')
        ->fields('u', array('sid'))
        ->condition('unsubs_token', check_plain($token))
        ->execute()
        ->fetchObject();
        
        $del = db_delete('email_subscribe_readed_nodes')
        ->condition('rn_sid', (int)$sid->sid)
        ->execute();
        if((bool)$del){
          drupal_set_message(t('Email unsubscribed.'));
        }
      }
      else{
        drupal_set_message(t('Email already unsubscribed.'), 'warning');
      }
      drupal_goto('email-subscribe');
    }
    catch(Exception $e){
      watchdog_exception('email-subscribe-error', $e);
      drupal_set_message(t('Error'), 'error'); return false;
    }
  }
  return '';
}

/**
 * Re-subscribe link with the token
 * */
function email_subscribe_resubscribe_token($token=''){
  global $user;
  if(!empty($token)){
    try{
      $res = db_update('email_subscribe_users')
      ->fields(array('status' => 2, 'date_resubscribe' => date('Y-m-d H:i:s'), 'datechange'=>REQUEST_TIME))
      ->condition('status', 3)
      ->condition('subs_token', check_plain($token))
      ->execute();
      if(!empty($res)){
        if(_email_subscribe_fill_readed_nodes($token)){
          drupal_set_message(t('Email re-subscribed.'));
        }
      }
      else{
        drupal_set_message(t('Email already re-subscribed.'), 'warning');
      }
      drupal_goto('email-subscribe');
    }
    catch(Exception $e){
      watchdog_exception('email-subscribe-error', $e);
      drupal_set_message(t('Error'), 'error'); return '';
    }
  }
  return '';
}
