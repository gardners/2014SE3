Test Cases for Existing Open-Source Project
===========================================

This is the competency gate for obtaining Distinction in this topic.

To pass this gate you will need to write one, or ideally several
test cases for some existing software.  The tests must be actionable,
and of a quality that would be acceptable for inclusion in open-source
software.  The tests must be appropriately packaged and documented to allow
submission to the open-source software under consideration, and then actually
submitted.  

Your tests do not however have to be accepted, but we do encourage you to follow
through with the submission process as far as the project will allow.  The experience
and acknowledgement of contributing to a real software project will be invaluable to
you in many ways.

To obtain the checkpoint you must submit an issue that refers to sufficient evidence of the work you have done, and where it has been submitted.

Using Succinct Data as the target project
-----------------------------------------

Assistance will be provided to write tests for the Serval Succinct Data project
which consists of the https://github.com/servalproject/smac and https://github.com/servalproject/survey-acquisition-management
repositories.  Example form and recipe files exist in https://github.com/servalproject/succinct-data-example-config-bundles/tree/master/examples/kiwiex-2013

See also the contents of succinct.tgz for example input and output data.

Example invocations of smac to do various things:

1. Extract all received succinct data compressed messages in succinct_rxspool:

	./smac recipe decompress succinct_recipes succinct_rxspool succinct_output

2. Generate HTML maps from received and decompressed succinct data messagse:

	./smac recipe map succinct_recipes succinct_output

	(note that (1) must be run before (2) can be run, as it uses the XML forms obtained during (1).)

3. To strip the data out of an ODK form instance:

	./smac recipe strip nz_redcross_1a_people foo.xml foo.strip

4. To compress a stripped data file:

	./smac recipe compress foo.recipe foo.strip foo.sd
