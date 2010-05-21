<?php
// $Id$

/**
 * @file
 *   Provides admin settings for the counterfeld module.
 */

/**
 * Page callback to provide a list of available counter blocks.
 */
function counterfield_block_listing() {
  // TODO: add a theme function for this page?
  $settings = variable_get('counterfield_blocks', array());
  $rows = array();
  if (count($settings)) {
    foreach ($settings as $setting) {
      $title = ($setting['title'] ? $setting['title'] : $setting['name']);
      $rows[] = array(
        'title' => $title,
        'edit' => l('Edit', 'admin/settings/counterfield/' . $setting['name'] . '/edit'),
        'delete' => drupal_get_form('counterfield_block_' . $setting['name'] . '_delete_form'),
      );
    }
  }
  else {
    $string = 'You have not added any blocks yet.  Click !add to get started!';
    $rows[] = array(
      'title' => t($string, array('!add' => l('add', 'admin/settings/counterfield/add'))),
      'edit' => l('Add One', 'admin/settings/counterfield/add'),
      'delete' => '',
    );
  }
  $header = array('title' => t('Title'), 'edit' => t('Edit'), 'delete' => t('Delete'));
  $output = theme('table', $header, $rows);
  $output .= drupal_get_form('counterfield_add_block_form');
  return $output;
}

/**
 * Add block button form.
 */
function counterfield_add_block_form() {
  $form['add'] = array(
    '#value' => t('Add'),
    '#type' => 'submit',
  );
  $form['#redirect'] = 'admin/settings/counterfield/add';
  return $form;
}

/**
 * Provide the page to add/edit counter blocks.
 */
function counterfield_block_edit_page($arg) {
  $settings = variable_get('counterfield_blocks', array());
  if (!is_null($arg) && isset($settings[$arg])) {
    $form['counterfield_setting'] = array(
      '#type' => 'value',
      '#value' => serialize($settings[$arg]),
    );
  }
  if ($arg == 'add') {
    $form['name'] = array(
      '#title' => t('Block Name'),
      '#description' => t('The human-readable name of this content type. This text will be displayed as part of the list on the <em>create content</em> page. It is recommended that this name begin with a capital letter and contain only letters, numbers, and <strong>spaces</strong>. This name must be unique.'),
      '#default_value' => $setting['title'] ? $setting['title'] : '',
      '#type' => 'textfield',
      '#required' => TRUE,
    );
  }
  else {
    $form['name'] = array(
      '#type' => 'value',
      '#value' => $arg,
    );
  }
  return drupal_get_form('counterfield_block_form', $form);
}

/**
 *
 */
function counterfield_block_form($form_state = NULL, $form = NULL) {
  $setting = unserialize($form['counterfield_setting']['#value']);
  $form['title'] = array(
    '#title' => t('Block Title'),
    '#description' => t('The human-readable name of this content type. This text will be displayed as part of the list on the <em>create content</em> page. It is recommended that this name begin with a capital letter and contain only letters, numbers, and <strong>spaces</strong>. This name must be unique.'),
    '#default_value' => $setting['title'] ? $setting['title'] : '',
    '#type' => 'textfield',
    '#required' => FALSE,
  );
  // If we have all relevant data, calculate the current start number.
  if (isset($setting['start_number']) && isset($setting['rate']) && isset($setting['changed'])) {
    $start_number = counterfield_calculate_current_start_value($setting['changed'], $setting['start_number'], $setting['rate']);
  }
  $form['start_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Start number'),
    '#default_value' =>  $start_number ? $start_number : NULL,
  );
  $form['rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Rate to increment the counter (in dollars per hour)'),
    '#default_value' => isset($setting['rate']) ? $setting['rate'] : NULL,
  );
  $form['link'] = array(
    '#type' => 'textfield',
    '#title' => t('The destination of the \'Lear More\' link'),
    '#default_value' => isset($setting['link']) ? $setting['link'] : NULL,
  );
  $field_types = _content_field_types();
  foreach ($field_types['counterfield']['formatters'] as $name => $formatter) {
    $options[$name] = $formatter['label'];
  }
  $form['formatter'] = array(
    '#title' => t('Formatter'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => isset($setting['formatter']) ? $setting['formatter'] : NULL,
  );
  $form['submit'] = array(
    '#value' => t('Save'),
    '#type' => 'submit',
    '#submit' => array('counterfield_block_form_submit'),
  );
  $form['#redirect'] = 'admin/settings/counterfield';
  return $form;
}

/**
 * VAlidate a counterfield_block_form submission.
 */
function counterfield_block_form_validate($form, &$form_state) {
  if (preg_match('/[\s]/', $form_state['values']['name'])) {
    form_set_error('name', 'May only contain letters, numbers and underscores.');
  }
}

/**
 * Save a counterfield_block_form submission.
 */
function counterfield_block_form_submit($form, &$form_state) {
  $settings = variable_get('counterfield_blocks', array());
  foreach($form_state['values'] as $name => $value) {
    // Store only the elements we care about.
    if (in_array($name, array('name', 'title', 'start_number', 'rate', 'formatter', 'link'))) {
      $settings[$form_state['values']['name']][$name] = $value;
    }
  }
  $settings[$form_state['values']['name']]['changed'] = time();
  $settings[$form_state['values']['name']]['name'] = $form_state['values']['name'];
  variable_set('counterfield_blocks', $settings);
}

/**
 * Implementation of hook_forms().
 */
function counterfield_forms($form_id, $args) {
  if (preg_match('/counterfield_block_(.*)_delete_form/', $form_id, $matches)) {
    $name = $matches[1];
    $forms['counterfield_block_' . $name . '_delete_form'] = array(
      'callback' => 'counterfield_block_delete_form',
      'callback arguments' => array($name),
    );
    return $forms;
  }
}

/**
 * Provide the counterfield delete form.
 */
function counterfield_block_delete_form($form, $name=NULL) {
  $form['counterfield_block_name'] = array(
    '#type' => 'value',
    '#value' => $name,
  );
  $form['submit'] = array(
    '#value' => t('Delete'),
    '#type' => 'submit',
    '#submit' => array('counterfield_block_delete_form_submit'),
  );
  return $form;
}

/**
 *
 */
function counterfield_block_delete_form_submit($form, &$form_state) {
  $block_name = $form['counterfield_block_name']['#value'];
  $settings = variable_get('counterfield_blocks', array());
  if (isset($settings[$block_name])) {
    unset($settings[$block_name]);
  }
  variable_set('counterfield_blocks', $settings);
}
