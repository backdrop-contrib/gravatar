<?php
// $Id$

/**
 * Formbuilder function for gravatar_admin_settings 
 *
 * @return (mixed)
 *    array according to form-api 
 */
function gravatar_admin_settings () {
   
   $form = array();
   
   $form['gravatar_preferences'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('Display Preferences'),
      '#description' => t('Control how gravatars are displayed'),
      '#collapsible' => true,
      '#collapsed'   => false,
   );
   
   $form['gravatar_preferences']['gravatar_toggle'] = array(
      '#type'        => 'checkbox',
      '#title'       => t('Enable gravatar integration'),
      '#description' => t('Enable or disable to integration of gravatar user images for this site.'),
      '#options'     => array(
         0 => t('Disable'),
         1 => t('Enable'), 
      ),
      '#default_value'   => variable_get('gravatar_toggle', 0),
   );
   
   $form['gravatar_display'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('Display Preferences'),
      '#description' => t('Control how gravatars are displayed'),
      '#collapsible' => true,
      '#collapsed'   => false,
   );
   
   $form['gravatar_display']['gravatar_displaysize'] = array(
      '#type'        => 'select',
      '#title'       => t('Image size'),
      '#description' => t('Select your preferred gravatar image size (max. 80x80)'),
      '#default_value' => variable_get('gravatar_displaysize', 48),
      '#options'     => drupal_map_assoc(array(16, 22, 24, 32, 48, 64, 72, 80), '_gravatar_admin_settings_map_size'),
      '#disabled'    => variable_get('gravatar_toggle', 0) ? false : true, 
   );

   $form['gravatar_display']['gravatar_imagerating'] = array(
      '#type'        => 'select',
      '#title'       => t('Maturity filter'),
      '#description' => t('An optional maturity rating parameter with a value (MPAA nomenclatura) that determines the highest rating (inclusive) that will be returned.'),
      '#default_value' => variable_get('gravatar_displayrating', 'PG'), 
      '#options'     => drupal_map_assoc(array(0, 'G', 'PG', 'R', 'X')),
      '#disabled'    => variable_get('gravatar_toggle', 0) ? false : true,
   );
   
   $form['gravatar_display']['gravatar_imagedefault'] = array(
      '#type'        => 'radios',
      '#title'       => t('Default Avatar Image'),
      '#description' => t('Specifies an image that should be returned if either the requested email address has no associated gravatar, or that gravatar has a rating higher than is allowed by the "rating" parameter.'),
      '#options'     => array(
         // 0 => t('No default image'), sound nice in theory, but there is no way of knowing 'no' gravatar is available
         1 => t('Global default user image'),
         2 => t('Gravatar integration default user image'),
         3 => t('Image provided by gravatar.com'),
      ),
      '#default_value' => variable_get('gravatar_imagedefault', 1),
      '#prefix'      => '<div id="default-gravatar-preview">' . _gravatar_admin_settings_previewimage() . '</div>',
      '#disabled'    => variable_get('gravatar_toggle', 0) ? false : true,
   );
   
   return system_settings_form($form);
}

/**
 * map pixel size to helper string
 * 
 * helper function 
 *
 * @param (int) $size
 *    pixel size
 * @return (string)
 *    helper text
 */
function _gravatar_admin_settings_map_size ($size) {
   return str_replace('@num', $size, '@numx@num px');
}

/**
 * previews the selected replacement image in the form
 *
 * @return (string)
 *    html formatted string
 */
function _gravatar_admin_settings_previewimage () {
   
   $option = variable_get('gravatar_imagedefault', 0);
   $size = variable_get('gravatar_displaysize', 48);
   
   switch ($option) {
   	// system default image
      case 1:
   	  $image = variable_get('user_picture_default', '');
   	  break;
      // module default image
      case 2:
         $image = drupal_get_path('module', 'gravatar') . '/avatar.png';
         break;
      // please try not to abuse this ...
      case 3:
         $image = GRAVATAR_SERVICEURL;
         break;
      default:
   		$image = '';
   	break;
   }
   
   if (!empty($image)) {
      return theme('image', $image, t('Default gravatar preview'), t('Default gravatar preview'), array('width' => $size, 'height' => $size), false); 
   }
   
}