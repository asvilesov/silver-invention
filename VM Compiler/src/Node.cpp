#include "Node.h"

void NBlock::AddStatement(NStatement* statement)
{
	mStatements.emplace_back(statement);
}

void NData::AddDecl(NDecl* decl)
{
	mDecls.emplace_back(decl);
}
