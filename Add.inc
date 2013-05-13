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

  $form['description'] = array(
    '#markup' => textfield_description_value($value_relationship),
        '#prefix' => '<div id="textfield_description_replace">',
        '#suffix' => '</div>',
  );

  $form['target'] = array(
    '#type' => 'textfield',
    '#title' => t('Target object'),
    '#required' => TRUE,
  );

  $form['symmetric'] = array(
	  '#type' => 'fieldset',
	  '#title' => 'Symmetry',
  );

  $form['symmetric']['inv-relationship'] = array(
    	'#type' => 'textfield',
    	'#title' => t('Symmetric relationship'),
	'#disabled' => TRUE,
        '#prefix' => '<div id="textfield_description_replace3">',
        '#suffix' => '</div>',
	'#value' => textfield_inverseof_value($value_relationship),
  );

  $form['symmetric']['sym-relationship'] = array(
    	'#markup' => description_inverseof_value($value_relationship),
        '#prefix' => '<div id="textfield_description_replace1">',
        '#suffix' => '</div>',
  );

 $form['symmetric']['is-symmetric'] = array(
	  '#type' => 'checkbox',
	  '#disabled' => has_inverseof_value($value_relationship),
	  '#title' => t('Add the corresponding relationship?'),
         '#prefix' => '<div id="textfield_description_replace2">',
         '#suffix' => '</div>',
  );

$form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('submit'),
  );
  return $form;

}


function relationship_ajax_callback(&$form, $form_state) {
//    return $form['description'];
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace("#textfield_description_replace", render($form['description'])),
      ajax_command_replace("#textfield_description_replace1", render($form['symmetric']['sym-relationship'])),
      ajax_command_replace("#textfield_description_replace2", render($form['symmetric']['is-symmetric'])),
      ajax_command_replace("#textfield_description_replace3", render($form['symmetric']['inv-relationship']))
    )
  );
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
function textfield_inverseof_value($value_relationship) {
  if (!empty($value_relationship)) {
  	$vocabulary = get_current_vocabulary();
  	$term = $vocabulary['terms'][$value_relationship];
	if (isset($term['inverse'][0])) {
//		list($term_ns, $value_inverse) = rdfx_split_uri($term['inverse'][0]);
//		$value_inverse = $value_inverse;
		$value_inverse = $term['inverse'][0];
	}
  }

  return empty($value_inverse) ? "No inverse" : $value_inverse;
}
function description_inverseof_value($value_relationship) {
  if (!empty($value_relationship)) {
  	$vocabulary = get_current_vocabulary();
  	$term = $vocabulary['terms'][$value_relationship];
	if (isset($term['inverse'][0])) {
		$term1 = $vocabulary['terms'][$term['inverse'][0]];
		if (isset($term1['label'])) {
	  		if (isset($term1['label']['en'])) {
				$servoc = $term1['label']['en'];
	  		} else {
				$servoc = reset($term1['label']);
	  		}
		}
		$value_inverse = "<B>".$servoc."</B>";
	}
  }

  return empty($value_inverse) ? "No inverse<HR>" : $value_inverse."<HR>";
}

function has_inverseof_value($value_relationship) {
  if (!empty($value_relationship)) {
  	$vocabulary = get_current_vocabulary();
  	$term = $vocabulary['terms'][$value_relationship];
	if (isset($term['inverse'][0])) {
		return FALSE;
	}
  }

  return TRUE;
}

function islandora_relationship_editor_add_relationships_submit($form, &$form_state) {
  $subject = islandora_object_load($form_state['values']['this']);
  if (!$subject) {
    drupal_set_message("Subject error: Fedora object doesn't exist.");
    return ;
  }


  $object = islandora_object_load($form_state['values']['target']);
  if (!$object) {
    drupal_set_message("Object error: Fedora object doesn't exist.");
    return ;
  }
  list($rel_ns, $rel_name) = rdfx_split_uri($form_state['values']['relationship']);  
  $subject->relationships->add($rel_ns, $rel_name, $form_state['values']['target']);

  if ($form_state['values']['is-symmetric'] == 1) {
  	list($relinv_ns, $relinv_name) = rdfx_split_uri($form_state['values']['inv-relationship']);  
	$object->relationships->add($relinv_ns, $relinv_name, $form_state['values']['this']);
  }

  $form_state['redirect'] = "islandora/object/{$subject->id}/manage/relationships";

}




