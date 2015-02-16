<?php
function template_preprocess_islandora_relationship_editor_block(&$variables) {

$related = array();
$pid = arg(2);
$fedora_object = islandora_object_load($pid);

if ($fedora_object) {

  $rels = $fedora_object->relationships->get();
  foreach ($rels as $key => $rel) {
    if (($rel['predicate']['namespace'] === "http://d-nb.info/standards/elementset/gnd#")) {
       watchdog('rosie', 'on object ' . $rel['object']['value']);
       try {
         $object = $fedora_object->repository->getObject($rel['object']['value']);
       } catch (Exception $e) {
         continue;
       }
       if ($object) {
         $related[] = $object;
       } 
    }
  }
}

$variables['related'] = $related;

$options_relationship = ser_current_ontology();
$variables['list_ontology'] = $options_relationship;

}

