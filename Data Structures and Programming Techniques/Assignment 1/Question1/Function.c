

NodeType *FindNode(char *A, NodeType *L)
{
while ((L != NULL)&&(strcmp(L->Airport,A) != 0) ){	//changed the order of the parameters
L=L->Link;
}
return L;
}
