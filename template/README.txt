Project group number: 84
Group member names and x500s: 
	Justin Pederson: pede0831@umn.edu
	Chiemeka Nwakama: nwaka013@umn.edu
	Ibrahim Ismail-Adebiyi: ismai128@umn.edu
CSELabs computer the code was tested on: csel-kh1250-01.cselabs.umn.edu 
Planned outlining individual contributions:
	Justin: child_process.c
	Chiemeka: utils.c
	Ibrahim: merkle.c
There were no changes made to the Makefile or existing files that weren’t supposed to be changed.
There were no assumptions made that were not outlined in 7.Assumptions/Notes
Design of the program for creating the process tree:
if(child_id == leaf_process)
hash_data_block(data_from_hash, block folder and leafs)
blockfile = fopen(blockFileName)
fwrite(data_from_hash, blockfile)

else if(child_id == non-leaf process)
pid = fork();
execl(run child_process again)
pid = fork();
execl(run child_process again)

wait(NULL)
wait(NULL)

fscanf(child_buf1, read_child_data1)
fscanf(child_buf2, read_child_data2)

compute_dual_hash(hash_result, read_child_data1, read_child_data2)

fprinf(output, hash_result)

There was no code that was implemented that AI helper tools created.
