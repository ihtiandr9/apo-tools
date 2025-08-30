
#ifndef H_CODEGEN_H
#define H_CODEGEN_H

STRUCT(ASTree);
STRUCT(NodeList);

char* codegen_link(ASTree* ast);
NodeList* vars_get();

#endif