<?php

function islandora_relationship_editor_add_relationships(array $form, array &$form_state, $object_pid) {

$options_relationship = get_possible_relationships($object_pid);
$value_relationship = !empty($form_state['values']['relationship']) ? $form_state['values']['relationship'] : "";

  $form = array();
  $form['this'] = array(
    '#title' => t('This Object'),
    '#type' => 'textfield',
    '#value' => $object_pid,
    '#disabled' => TRUE,
  );
  $form['relationship'] = array(
    '#type' => 'select',
    '#title' => t('Relationship'),
    '#options' => $options_relationship,
    '#required' => TRUE,
    '#ajax' => array(
      	'method' => 'replace',
      	'effect' => 'fade',
        'event' => 'change',
        'callback' => 'relationship_ajax_callback',
        'wrapper' => 'textfield_description_replace',
     ),
  );
/*  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textfield',
        '#prefix' => '<div id="textfield_description_replace">',
        '#suffix' => '</div>',
    '#value' => textfield_description_value($value_relationship),
    '#disabled' => FALSE,
    '#ajax' => array(
      	'method' => 'replace',
      	'effect' => 'fade',
        'event' => 'mousedown',
        'callback' => 'relationship_ajax_callback',
        'wrapper' => 'textfield_description_replace',
     ),
  );
*/
  $form['description'] = array(
    '#markup' => textfield_description_value($value_relationship),
        '#prefix' => '<div id="textfield_description_replace">',
        '#suffix' => '</div>',
  );



  $form['target'] = array(
    '#type' => 'textfield',
    '#title' => t('Target object'),
  );
  $form['symmetric'] = array(
	  '#type' => 'fieldset',
	  '#title' => 'Symmetry',
  );
 $form['symmetric']['is-symmetric'] = array(
	  '#type' => 'checkbox',
	  '#title' => t('Add the corresponding relationship?'),
  );
  $form['symmetric']['sym-relationship'] = array(
	  '#type' => 'select',
	  '#title' => t('Choose the corresponding relationship'),
	  '#description' => t('This should be the "opposite" relationship, which will be applied to the target object, with a value of this object.'),
	  '#options' => get_possible_relationships($object_pid),
	  '#default_value' => '',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('submit'),
  );
  return $form;

}


function relationship_ajax_callback($form, $form_state) {
    return $form['description'];
}

function textfield_description_value($value_relationship) {
  if (!empty($value_relationship)) {
  $vocabulary = get_current_vocabulary();
  $term = $vocabulary['terms'][$value_relationship];
	if (isset($term['comment'])) {
	  if (isset($term['comment']['en'])) {
		$servoc = $term['comment']['en'];
	  } else {
		$servoc = reset($term['comment']);
	  }
	}
	$value_description = $servoc;
  }
  return empty($value_description) ? "No description<HR>" : $value_description."<HR>";

}

function islandora_relationship_editor_add_relationships_submit($form, &$form_state) {
  $object = islandora_object_load($form_state['values']['this']);
  if (!$object) {
    drupal_set_message("Fedora object doesn't exist.");
  }
  $target = islandora_object_load($form_state['values']['target']);
  if (!$target) {
    drupal_set_message("Fedora object doesn't exist.");
  }
  $object->relationships->add(MYNS_URI, $form_state['values']['relationship'], $form_state['values']['target']);

  if ($form_state['values']['is-symmetric'] == 1) {
	  $target->relationships->add(MYNS_URI, $form_state['values']['sym-relationship'], $form_state['values']['this']);
  }
  $form_state['redirect'] = "islandora/object/{$object->id}/manage/relationships";

}
