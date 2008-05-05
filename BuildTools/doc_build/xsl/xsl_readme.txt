These xsl v2.0-compliant transforms process the tags in the generated xml to produce html. They are not backward-compatible with xsl v1.0. 

fw_gen_htm_wf.xsl does the frames version of the html; fw_gen_htm_nf.xsl does the no-frames version. 

There is no DTD, and the tagset has evolved over time, so the node structure is flatter than it strictly should be. Some nodes that would logically have a parent/child relation are actually siblings. To make this less difficult to deal with, many node references are made explicitly. Approach any change to generated structures, such as the table of contents, with care -- make sure of the node relationships before making changes.

fw_gen_pdf.xsl is a draft version of an xsl-fo to process xml directly into pdf using an FO 1.1 compliant formatter. it isn't complete.