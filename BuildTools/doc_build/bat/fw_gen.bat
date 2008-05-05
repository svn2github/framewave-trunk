@echo off

call ..\app\do_two fw_doc.xml fw_gen_htm_wf.xsl \fw_env\fw_dev\generated_files\aa_xit_wf.html

call ..\app\do_two fw_doc.xml fw_gen_htm_nf.xsl \fw_env\fw_dev\generated_files\aa_xit_nf.html