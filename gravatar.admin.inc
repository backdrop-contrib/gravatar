<?php

/**
 * @file
 * Administrative page callbacks for the gravatar module.
 */

/**
 * Administration settings form.
 *
 * @see system_settings_form()
 */
function gravatar_admin_settings($form, $form_state) {
  $config = config('gravatar.settings');
  $form['#config'] = 'gravatar.settings';
  $form['gravatar_default'] = array(
    '#type' => 'radios',
    '#title' => t('Default image'),
    '#description' => t('Specifies an image that should be returned if either the requested e-mail address has no associated gravatar, or that gravatar has a rating higher than is allowed by the maturity filter.'),
    '#options' => array(
      GRAVATAR_DEFAULT_MYSTERY_MAN => t('Gravatar.com mystery man'),
      GRAVATAR_DEFAULT_LOGO => t('Gravatar.com logo'),
      GRAVATAR_DEFAULT_IDENTICON => t('Gravatar.com identicon (generated)'),
      GRAVATAR_DEFAULT_MONSTERID => t('Gravatar.com monsterid (generated)'),
      GRAVATAR_DEFAULT_WAVATAR => t('Gravatar.com wavatar (generated)'),
      GRAVATAR_DEFAULT_RETRO => t('Gravatar.com retro 8-bit arcade-style pixelated faces (generated)'),
    ),
    '#default_value' => $config->get('gravatar_default'),
    '#field_prefix' => '<div class="picture js-show">' . theme('image', array('path' => '', 'alt' => t('Default picture example'), 'title' => t('Default picture example'), 'attributes' => array('id' => 'gravatar-imagepreview'), 'getsize' => FALSE)) . '</div>',
    '#process' => array('form_process_radios', 'gravatar_process_default_setting'),
  );
  $form['gravatar_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Gravatar size'),
    '#description' => t('The preferred image size for Gravatars (maximum @max pixels).', array('@max' => GRAVATAR_SIZE_MAX)),
    '#default_value' => $config->get('gravatar_size'),
    '#size' => 3,
    '#maxlength' => 3,
    '#element_validate' => array('_element_validate_integer'),
  );
  $form['gravatar_rating'] = array(
    '#type' => 'select',
    '#title' => t('Image maturity filter'),
    '#description' => theme('item_list', array('items' => array(
        t('G: Suitable for display on all websites with any audience type.'),
        t('PG: May contain rude gestures, provocatively dressed individuals, the lesser swear words, or mild violence.'),
        t('R: May contain such things as harsh profanity, intense violence, nudity, or hard drug use.'),
        t('X: May contain hardcore sexual imagery or extremely disturbing violence.'),
      ))),
    '#options' => backdrop_map_assoc(array('G', 'PG', 'R', 'X')),
    '#default_value' => $config->get('gravatar_rating'),
  );

  // Add JavaScript and CSS to swap the defalut image previews.
  $form['#attached']['js'][] = backdrop_get_path('module', 'gravatar') . '/gravatar.js';
  $form['#attached']['css'][] = backdrop_get_path('module', 'gravatar') . '/gravatar.css';

  // Advanced settings.
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#description' => t('Do not modify these options unless you know what you are doing!'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced']['gravatar_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Gravatar URL'),
    '#default_value' => $config->get('gravatar_url'),
  );
  $form['advanced']['gravatar_url_ssl'] = array(
    '#type' => 'textfield',
    '#title' => t('Gravatar secure URL'),
    '#default_value' => $config->get('gravatar_url_ssl'),
  );

  return system_settings_form($form);
}

/**
 * Add previews for each default picture option.
 */
function gravatar_process_default_setting($element) {

  foreach ($element['#options'] as $key => $choice) {
    // Add an image to preview this default image option.
    $options = array(
      'path' => gravatar_get_gravatar(mt_rand(), array('default' => _gravatar_get_default_image($key), 'force default' => TRUE)),
      'alt' => $choice,
      'title' => $choice,
      'attributes' => array(
        'id' => 'gravatar-imagepreview-' . $key,
        // Hide the image if the respective option is disabled.
        'class' => !empty($choice['#disabled']) ? 'hide' : 'js-hide',
      ),
      'getsize' => FALSE,
    );
    $element[$key]['#suffix'] = theme('image', $options);
  }

  return $element;
}
