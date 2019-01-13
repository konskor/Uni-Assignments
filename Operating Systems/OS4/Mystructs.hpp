
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/inotify.h>




#include "NameTree.hpp"
#include "Inode.hpp"


void copy1(char*, char*, char, bool);

int str_cmp(const void *a, const void *b);

void Removedir(char*);




