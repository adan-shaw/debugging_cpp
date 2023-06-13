//编译:
//		gcc ./statfs.c -g3 -o x 



#include <stdio.h>
#include <sys/statfs.h>
#include <sys/statvfs.h>
#include <stdlib.h>



#define FILEPATH "./statfs.c"



void statfs_test(const char* filepath){
	struct statfs sfs;

	if(statfs(filepath, &sfs) == -1){
		perror("statfs()");
		return;
	}

	printf("File system type:              %#lx\n",(unsigned long)sfs.f_type);
	printf("Optimal I/O block size:        %lu\n",(unsigned long)sfs.f_bsize);
	printf("Total data blocks:             %lu\n",(unsigned long)sfs.f_blocks);
	printf("Free data blocks:              %lu\n",(unsigned long)sfs.f_bfree);
	printf("Free blocks for nonsuperuser:  %lu\n",(unsigned long)sfs.f_bavail);
	printf("Total i-nodes:                 %lu\n",(unsigned long)sfs.f_files);
	printf("File system ID:                %#x, %#x\n",(unsigned)sfs.f_fsid.__val[0],(unsigned)sfs.f_fsid.__val[1]);
	printf("Free i-nodes:                  %lu\n",(unsigned long)sfs.f_ffree);
	printf("Maximum file name length:      %lu\n",(unsigned long)sfs.f_namelen);

	return;
}



void statvfs_test(const char* filepath){
	struct statvfs sb;

	if(statvfs(filepath, &sb) == -1){
		perror("statvfs()");
		return;
	}

	printf("Block size                       %lu\n", sb.f_bsize);
	printf("Fundamental block size           %lu\n", sb.f_frsize);
	printf("Total blocks (in above units)    %lu\n",(unsigned long) sb.f_blocks);
	printf("Free blocks for priv. proc.      %lu\n",(unsigned long) sb.f_bfree);
	printf("Free blocks for unpriv. proc.    %lu\n",(unsigned long) sb.f_bavail);
	printf("Total number of i-nodes          %lu\n",(unsigned long) sb.f_files);
	printf("Free i-nodes for priv. proc.     %lu\n",(unsigned long) sb.f_ffree);
	printf("Free i-nodes for nonpriv. proc.  %lu\n",(unsigned long) sb.f_favail);
	printf("File system ID                   %#lx\n", sb.f_fsid);
	printf("Flags                            %#lx\n", sb.f_flag);
	printf("Maximum filename length          %lu\n", sb.f_namemax);

	return;
}



int main(void){
	printf("statfs_test() start:\n");
	statfs_test(FILEPATH);

	printf("statvfs_test() start:\n");
	statvfs_test(FILEPATH);
	return 0;
}
