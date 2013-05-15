islandora_relationship_editor
=============================

Islandora extension to add/view/delete RELS-EXT relationships

With this module, you can add rels-ext relationships to an object via the "Manage" tab. (Manage -> relationships). 
You can also add the reciprocal ('symmetric') relationship in one fell swoop.
Relationships are derived from ontology file.

Dependecies
-----------
- islandora
- block
- colorbox
- arc2: download from https://github.com/semsol/arc2, untar and copy arc2 dir in islandora_relationship_editor module dir.

NOTE
----
- ontology must be in OWL RDF/XML and must be loaded into a datastream in a object of Fedora repository



- TODO: create permissions to let Drupal users add/delete relationships - currently it's FEDORA_METADATA_EDIT. 
- Further TODO: autocomplete the target object pid from typing pid or label
- Further TODO: insert this form into the ingest steps (?)
