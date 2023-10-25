Project group number: 84
Group member names and x500s: 
	Justin Pederson: pede0831@umn.edu, nonleaf_process.c
	Chiemeka Nwakama: nwaka013@umn.edu, root_process.c
	Ibrahim Ismail-Adebiyi: ismai128@umn.edu, leaf_process.c/Makefile
CSELabs computer the code was tested on: csel-kh1250-01.cselabs.umn.edu
We modified the makefile to remove the output directory with make clean
and generated the directory directly in our code.
We didn't make any assumptions

Planned outlining individual contributions:

Psuedocode for root_process.c
    // TODO(overview): redirect standard output to an output file in output_file_folder("output/final_submission/")
    // TODO(step1): determine the filename based on root_dir. e.g. if root_dir is "./root_directories/root1", the output file's name should be "root1.txt"
    //TODO(step2): redirect standard output to output file (output/final_submission/root*.txt)
    //TODO(step3): read the content each symbolic link in dup_list, write the path as well as the content of symbolic link to output file(as shown in expected)
    //TODO(): create symbolic link at the location of deleted duplicate file
    //TODO(): dup_list[i] will be the symbolic link for retain_list[i]
    //TODO(): delete duplicate files, each element in dup_list is the path of the duplicate file
    //TODO(overview): fork the first non_leaf process associated with root directory("./root_directories/root*")
    //TODO(step1): construct pipe
    //TODO(step2): fork() child process & read data from pipe to all_filepath_hashvalue
    //TODO(step3): malloc dup_list and retain list & use parse_hash() in utils.c to parse all_filepath_hashvalue
    //TODO(step4): implement the functions
    //TODO(step5): free any arrays that are allocated using malloc!!
Psuedocode for root_process.c
    //TODO(overview): fork the child processes(non-leaf process or leaf process) each associated with items under <directory_path>
    //TODO(step1): get <file_path> <pipe_write_end> from argv[]
    //TODO(step2): malloc buffer for gathering all data transferred from child process as in root_process.c
    //TODO(step3): open directory
    //TODO(step4): traverse directory and fork child process
    //TODO(step5): read from pipe constructed for child process and write to pipe constructed for parent process
